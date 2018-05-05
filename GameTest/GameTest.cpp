//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app\app.h"
#include "Game.h"

using namespace App;
using namespace std;

Game* myGame;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init(){
	FRAND_RANGE(0, 1);

	// Hello, game.
	myGame = new Game();
	myGame->loadLevel("table.txt");
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime){

	// Handle input. The system can handle adding more flippers or more elements with ease.
	std::map<int, bool> inMap = myGame->getInputMap();
	for (auto &pair : inMap) {
		myGame->setInput(pair.first, IsKeyPressed(pair.first));
	}
	vec2 mpos;	GetMousePos(mpos.x, mpos.y);
	myGame->setMousePos(mpos);
	myGame->processInput(deltaTime);

	// So some simulation
	myGame->updateSimulation(deltaTime);
}


//------------------------------------------------------------------------
// Add your display calls here (DrawLine or Print) 
// See App.h 
//------------------------------------------------------------------------
void Render() {

	myPrint(vec2(700, 600), "Left click and hold to add");
	myPrint(vec2(700, 550), "A and D to flip");
	#ifdef NDEBUG
	// Yay!
	#else
		myPrint(vec2(230, 500), "Please run me in Release mode for faster simulation.");
	#endif

	// Visualize a ball if it's currently being added.
	if (myGame->getMouseHeldTime() > 0){
		drawBall(myGame->getMousePos(), 10 * (1 + myGame->getMouseHeldTime()), 20, 0, 1, 0);
	}

	// Ideally we would want to reduce the following calls to a more pure form than
	// the whole ball or flipper structre...
	vector<Ball> balls = myGame->getBalls();
	for (auto &ball : balls) {
		drawBall(ball.pos, ball.r, 20, ball.red, ball.green, ball.blue);
	}

	vector<Flipper> flippers = myGame->getFlippers();
	for (auto &flipper : flippers) {
		vec2 end = flipper.getEnd();
		DrawLine(flipper.pos.x, flipper.pos.y, end.x, end.y, 0, 1, 1);
	}
	
	renderLines(myGame->getLineTable());
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown(){

	// Goodnight.
	delete myGame;
}