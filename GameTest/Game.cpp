#include "stdafx.h"
#include "Game.h"


Game::Game() {
	myTable = new CTable;

	// Add some flippers!
	flippers.push_back(Flipper(vec2(354, 90), 100, (330.0 / 180)*M_PI, (45.0 / 180)*M_PI, false, .25, 'A'));
	flippers.push_back(Flipper(vec2(552, 86), 100, (210.0 / 180)*M_PI, -(45.0 / 180)*M_PI, true, .25, 'D'));


	inputMap[VK_LBUTTON, false];
	for (auto &flipper : flippers) {
		inputMap[flipper.controlKey] = false;
	}
}

Game::~Game() {
	delete myTable;
}


void Game::loadLevel(const char* levelName) {
	myLoad(levelName, myTable);
}


void Game::processInput(float deltaTime) {
	float pre_dt = deltaTime / 1000;

	// If holding
	if (inputMap[VK_LBUTTON] && balls.size() < MAX_BALLS) {
		leftClickHoldTime += pre_dt;
		leftClickOnLastFrame = true;
	}

	// If released, spawn a ball with correct size and random color.
	// Also assign a velocity depending on mouse movement over time.
	if (!inputMap[VK_LBUTTON] && leftClickOnLastFrame) {
		leftClickOnLastFrame = false;
		balls.push_back(Ball(mpos, 10 * (1 + leftClickHoldTime), FRAND, FRAND, FRAND));
		balls.back().vel = 2 * (mpos - mpos_0);
		leftClickHoldTime = 0;
	}

	// Collect flipper input
	for (auto &flipper : flippers) {
		flipper.activated = inputMap[flipper.controlKey];

		// Animate the flipper
		if (flipper.activated)
			flipper.moveToEnd();
		else
			flipper.moveToStart();
	}

}

void Game::updateSimulation(float deltaTime) {
	float pre_dt = deltaTime / 1000;
	float dt = pre_dt*dtMod / stepsPerFrame;

	// Perform a number of physics steps
	for (int step = 0; step < stepsPerFrame; step++) {
		// "Gauss-seidel" type iterative solver to resolve collisions, with passcount dependent on number of balls.
		int passes = sqrt(balls.size());
		for (int pass = 0; pass < passes; pass++) {
			collisionResolutionPass(dt);
		}

		// Remove rogue balls.
		balls.erase(remove_if(balls.begin(), balls.end(), toRemoveBall), balls.end());

		// Advance timestep. Implicit Euler.
		// Loses energy, but doesn't blow up!!!
		for (auto &ball : balls) {
			ball.accel = gravity;
			ball.vel += dt*ball.accel;
			ball.pos += dt*ball.vel;
		}
	}
}


// Does a single iteration of collision detection and resolution
void Game::collisionResolutionPass(float dt) {
	for (std::vector<Ball>::size_type i = 0; i < balls.size(); i++) {
		// Note the indexing below. Still O(n^2), but much better constant.
		// Could reduce to O(n logn) I think with a spacial data structure.
		for (std::vector<Ball>::size_type j = i + 1; j < balls.size(); j++)
			balls[i].resolveBallCollision(balls[j]);

		for (auto &plane : myTable->m_lines)
			balls[i].resolvePlanarCollision(plane, dt);
	}

	// Flipper collisions
	for (auto &flipper : flippers) {

		// If you 'just' activated a flipper, resolve sector collisions
		if (flipper.notMoving && flipper.activated) {
			for (auto &ball : balls) {
				ball.resolveSectorCollision(flipper.getSector(1), 200, dt);
			}
			flipper.notMoving = false;
		}
		flipper.notMoving = !flipper.activated;

		// Otherwise, treat the flipper as a plane.
		for (auto &ball : balls) {
			ball.resolvePlanarCollision(flipper.getSeg(), dt);
		}
	}
}