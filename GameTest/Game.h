#pragma once


#include "utils.h"
#include "Ball.h"
#include "Flipper.h"
#include <vector>
#include <map>
#include <algorithm>

const vec2 gravity(0, -10);
const int MAX_BALLS = 30;
const float dtMod = 5;
const int stepsPerFrame = 2;

class Game {
public:
	Game();
	~Game();
	void updateSimulation(float deltaTime);
	void processInput(float deltaTime);
	void loadLevel(const char* levelName);

	// Note, we replace the previous mpos here too.
	void setMousePos(vec2 newMpos) { mpos_0 = mpos; mpos = newMpos; };
	vec2 getMousePos(){ return mpos; }
	float getMouseHeldTime() { return leftClickHoldTime; }

	CTable* getLineTable() { return myTable; }
	std::vector<Ball> getBalls() { return balls; };
	std::vector<Flipper> getFlippers() { return flippers; };

	std::map<int, bool> getInputMap() { return inputMap; };
	void setInput(int button, bool result) { inputMap[button] = result; };

private:
	CTable* myTable;
	std::vector<Ball> balls;
	std::vector<Flipper> flippers;
	std::map<int, bool> inputMap;

	bool leftClickOnLastFrame = false;
	float leftClickHoldTime = 0;
	vec2 mpos;
	vec2 mpos_0;

	void collisionResolutionPass(float dt);

	// Used to check if a ball should be removed from the game
	static bool Game::toRemoveBall(const Ball &ball) {
		return (ball.removeMe || ball.pos.x < 0 || ball.pos.x > APP_VIRTUAL_WIDTH || ball.pos.y < 0 || ball.pos.y > APP_VIRTUAL_HEIGHT);
	}

};
