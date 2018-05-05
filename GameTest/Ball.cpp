#include "stdafx.h"
#include "Ball.h"

// Two-sided continuous collision detection and resolution for a plane.
float Ball::resolvePlanarCollision(CLineSegment seg, float dt) {

	// Plane endpoints.
	vec2 p1 = vec2(seg.m_start.m_x, seg.m_start.m_y);
	vec2 p2 = vec2(seg.m_end.m_x, seg.m_end.m_y);
	float dist = seg.DistanceToLine(pos.x, pos.y);

	// Find plane normal.
	vec2 tangent = (p2 - p1);
	vec2 normal = tangent.normal().normed();

	// For continuous detection. Ask if we will tunnel through on next timestep.
	bool tunneledThrough = false;
	vec2 newPos = pos + dt*vel;
	vec2 intersect(0, 0);

	// See if "tunneling vector" intersects our line.
	if(lineIntersection(p1, p2, pos, newPos, intersect) == 1)
		tunneledThrough = true;

	// Normals are oriented, so must be careful for correct two sided collisions.
	vec2 projection = (pos - p1).projOnto(tangent);
	vec2 rejection = (pos - p1) - projection;
	if(rejection.dot(normal) <= 0)
		normal *= -1;

	float planeResti = 1;
	switch (seg.m_type) {
	case LineType::eLine_Hard:
		planeResti = .9;
		break;
	case LineType::eLine_Flipper:
		planeResti = .9;
		break;
	case LineType::eLine_Soft:
		planeResti = .5;
		break;
	case LineType::eLine_Power:
		planeResti = 1.5;
		break;
	}

	// A literal edge case.
	// The normal on the edge should smoothly interpolate as if it were a capsule.
	// Without this bit, balls wiil get caught on the edge of cliffs.
	float dp1 = (p1 - pos).length();
	float dp2 = (p2 - pos).length();
	if ( dp1 < r || dp2 < r) {
		if (dp1 < dp2)
			normal = (pos - p1).normed();
		else
			normal = (pos - p2).normed();
	}

	// If collision happens...
	// Note: must check velocity dot w/ normal so our iterative "Gauss-Seidel" solver works correctly.
	if ((dist < r || tunneledThrough) && vel.dot(normal) < 0) {
		float mass = M_PI*pow(r, 2);
		float K = .5*mass*pow(vel.length(), 2);

		// Reflect velocity away
		vel -= (planeResti + 1)*vel.dot(normal)*normal;

		// And nudge ball out of plane
		if (tunneledThrough)
			pos = intersect + r*normal;
		else
			pos += (r - dist)*normal;

		if (seg.m_type == LineType::eLine_Fail)
			removeMe = true;

		return K;
	}

	return 0;
}

// Not really an impulse, but it works for our needs.
void Ball::impulse(vec2 force) {
	vel += force;
}

float Ball::resolveBallCollision(Ball &b2) {
	// Some math for elastic collisions. Return kinetic energy of collision

	Ball &b1 = *this;

	vec2 D = b1.pos - b2.pos;		// Dist vector
	float dist = D.length();	// Dist between centres of mass
	vec2 D_hat = D.normed();			// Normalized dist vector

	// Collect vel and position dot prod.
	float vdot_1 = b1.vel.dot(D_hat);
	float vdot_2 = b2.vel.dot(D_hat);

	float vdot = vdot_1 - vdot_2;

	// If a collision actually happens...
	// Note: must check vdot so our iterative "Gauss-Seidel" solver works correctly.
	if (dist < (b1.r + b2.r) && vdot < 0) {
		// Mass assuming density = 1 is just the area.
		float m_1 = M_PI*pow(b1.r, 2);
		float m_2 = M_PI*pow(b2.r, 2);

		// Momentum term
		float p = m_1*vdot_1 + m_2*vdot_2;

		// Total kinetic energy
		float K = m_1*pow(vdot_1, 2) + m_2*pow(vdot_2, 2);

		float u_1 = (p + m_2*resti*(vdot_2 - vdot_1)) / (m_1 + m_2);
		float u_2 = (p + m_1*resti*(vdot_1 - vdot_2)) / (m_1 + m_2);

		// Reflect velocities
		b1.vel -= (-u_1 + vdot_1)*D_hat;
		b2.vel -= (-u_2 + vdot_2)*D_hat;

		// And adjust positions
		vdot_1 = abs(b1.vel.dot(D_hat));
		vdot_2 = abs(b2.vel.dot(D_hat));
		b1.pos += (vdot_1 / (vdot_1 + vdot_2))*(b1.r + b2.r - dist)*D_hat;
		b2.pos -= (vdot_2 / (vdot_1 + vdot_2))*(b1.r + b2.r - dist)*D_hat;

		return K;
	}
	return 0;
}


void Ball::resolveSectorCollision(Sector sec, float force, float dt) {
	
	vec2 footToBall = pos - sec.foot;
	vec2 footToInitEnd = sec.getEnd(sec.initAngle) - sec.foot;
	vec2 footToFinalEnd = sec.getEnd(sec.endAngle) - sec.foot;

	float sectorTangentSegmentLength = (footToFinalEnd.normed() - footToInitEnd.normed()).length();
	float ballTangentSegmentLength = (footToBall.normed() - footToInitEnd.normed()).length();
	float percentOfArm = (footToBall.length() / sec.length);

	// Check if the sector actually intersects the ball
	if (percentOfArm < 1 && ballTangentSegmentLength < sectorTangentSegmentLength) {

		vec2 normal = footToBall.normal().normed();

		// Oriented normals issue again. Adjust to ensure it faces the ball.
		vec2 initEnd = sec.getEnd(sec.initAngle);
		vec2 finalEnd = sec.getEnd(sec.endAngle);
		if (normal.dot(finalEnd - initEnd) < 0) {
			normal *= -1;
		}

		// A mini solve to find approx plane-of-collision. This can be used for better visual feedback. Not currently in use.
		// Error growth is O(root(r)) with ball radius I think...
		vec2 approxPlaneVecTail = pos + r*normal;
		CLineSegment tempPlane(sec.foot.x, sec.foot.y, approxPlaneVecTail.x, approxPlaneVecTail.y, eLine_Flipper);
		
		// Impulse the ball. percentOfArm gives us a torque-ish term
		// Note this isn't energy preserving. Could try multiplying by velocity norm for beter kinetic energy preservation...
		impulse(percentOfArm*force*normal.normed());

		// and adjust pos...
		{
			vec2 adjustedFinalPos = percentOfArm*footToFinalEnd;

			// get normal...
			vec2 finalNormal = adjustedFinalPos.normal().normed();
			if (finalNormal.dot(finalEnd - initEnd) < 0) {
				finalNormal *= -1;
			}

			// Finally, put in correct position, with some wiggle room
			pos = (sec.foot + adjustedFinalPos) + (r + .1)*finalNormal;
		}
	}
}