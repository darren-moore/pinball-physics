#pragma once

#include "table.h"
#include "app\app.h"
#include <fstream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>

#include "vec2.h"

bool lineIntersection(vec2 l1_1, vec2 l1_2, vec2 l2_1, vec2 l2_2, vec2 &intersection);													// Checks if two lines intersect
void drawBall(vec2 c, float r, int subDiv, float red, float green, float blue);															// Draws a ball with lines.
void myLoad(const char* fileName, CTable* gTable);																						// Stolen from the editor class. Loads a level into a CTable
void renderLines(CTable* myTable);																										// Renders all the 
void myPrint(vec2 pos, const std::string str, float r = 1.0f, float g = 1.0f, float b = 1.0f, void *font = GLUT_BITMAP_HELVETICA_18);	// Custom print func to use my vec2 and std::string.
