#include "stdafx.h"
#include "utils.h"

using namespace App;

// Implementation adapted from Gavin's at https://stackoverflow.com/a/1968345
bool lineIntersection(vec2 l1_1, vec2 l1_2, vec2 l2_1, vec2 l2_2, vec2 &intersection) {
	vec2 s1 = l1_2 - l1_1;
	vec2 s2 = l2_2 - l2_1;

	float s, t;
	s = (-s1.y * (l1_1.x - l2_1.x) + s1.x * (l1_1.y - l2_1.y)) / (-s2.x * s1.y + s1.x * s2.y);
	t = (s2.x * (l1_1.y - l2_1.y) - s2.y * (l1_1.x - l2_1.x)) / (-s2.x * s1.y + s1.x * s2.y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
		// Collision detected
		intersection.x = l1_1.x + (t * s1.x);
		intersection.y = l1_1.y + (t * s1.y);
		return 1;
	}

	return 0; // No collision
}

void drawBall(vec2 c, float r, int subDiv, float red, float green, float blue){
	// A polygon with many sides is a ball... right?
	for (int i = 0; i < subDiv; i++) {
		vec2 p1 = vec2(c.x + r*cos((2 * M_PI) * ((float)i/subDiv)), c.y + r*sin((2 * M_PI) * ((float)i / subDiv)));
		vec2 p2 = vec2(c.x + r*cos((2 * M_PI) * ((float)(i+1) / subDiv)), c.y + r*sin((2 * M_PI) * ((float)(i+1) / subDiv)));
		DrawLine(p1.x, p1.y, p2.x, p2.y, red, green, blue);
	}
}

// Stolen from editor class
void myLoad(const char* fileName, CTable* gTable)
{
	std::fstream file;
	file.open(fileName, std::fstream::in);
	if (file.is_open())
	{
		int version = 0;
		file >> version;
		if (version <= 1)
		{
			gTable->m_lines.clear();
			while (!file.eof())
			{
				CLineSegment line;
				int type;
				file >> type;
				line.m_type = static_cast<LineType>(type);
				file >> line.m_start.m_x;
				file >> line.m_start.m_y;
				file >> line.m_end.m_x;
				file >> line.m_end.m_y;
				gTable->m_lines.push_back(line);
			}

			//gStatusMessage = "LOADED OK";
		}
		else
		{
			//gStatusMessage = "LOAD FAILED: UNHANDLED VERSION";
		}

		file.close();
	}
	else
	{
		// error message!
		//gStatusMessage = "LOAD FAILED";
	}

}


void renderLines(CTable* myTable) {
	for (auto& line : myTable->m_lines) {
		CLineDefinition& def = myTable->m_lineDefs[line.m_type];
		App::DrawLine(line.m_start.m_x, line.m_start.m_y, line.m_end.m_x, line.m_end.m_y, def.m_Red, def.m_Green, def.m_Blue);
	}
}


void myPrint(vec2 pos, const std::string str, float r, float g, float b, void *font) {
	Print(pos.x, pos.y, str.c_str(), r, g, b, font);
}