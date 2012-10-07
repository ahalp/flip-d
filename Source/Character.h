#pragma once

#include "SDL.h"
#include "Utilities.h"

class Display;
class CollisionGrid;
class Timing;

const int LEFT = 1;
const int RIGHT = 2;

enum movementStatus {
	STILL,
	WALK,
	JUMP
};

class Character
{

private:

	Display* _display;
	Timing* _timer;
	SDL_Surface* _image;

	int _imageX, _imageY;
	int _width, _height;
	float _xPos, _yPos;
	int _maxSpeedX, _maxSpeedY;
	int _speedX, _speedY;  // speedY set to gravity

	// Jump Vraiables
	int _gravity;
	int _jumpTime;			// Amount of time allocated for jump
	float _jumpDecrease;	// Factor by which jump speed decreases
	int timePassed, timePassed_midJump;
	
	
	bool _paused;
	movementStatus _movementStatus;
	int _directionStatus;

	// Animation Variables

	int _animateRightX[PLAYER_ANIMATION_FRAMES], _animateRightY[PLAYER_ANIMATION_FRAMES];
	int _animateLeftX[PLAYER_ANIMATION_FRAMES], _animateLeftY[PLAYER_ANIMATION_FRAMES];
	int _animateJumpX[PLAYER_ANIMATION_FRAMES], _animateJumpY[PLAYER_ANIMATION_FRAMES];
	int _animateFrame;
	

public:
	Character(	Display* display, Timing* timer,
				int imageX, int imageY, int width, int height, 
				const char* bitmapFileName,
				int x, int y, int maxSpeedX, int maxSpeedY,
				int gravity, int jumpTime, float jumpDecrease);

	~Character(void);
	
	// Update the character with the changes in position and animation.
	// Also handles collision detection
	void update(float deltaTime, GameToolsStruct & gameTools);

	void updateAutomated(float deltaTime, GameToolsStruct & gameTools);

	void draw();
	//void draw(int imageX, int imageY, int width, int height);
	void draw(int x, int y);

	void moveLeft(); 
	void moveRight();

	void undoMoveLeft();
	void undoMoveRight();

	void jump( GameToolsStruct &gameTools);

	void stopJump();
	void stopXMovement();
	void stopMovement();
	void stopAutomatedMovement();

	bool isMoving();
	int getDirectionStatus();

	void clipAnimationSprites();
	void animate();

	int getJumpTime();

	float getXPos();
	float getYPos();
	int getWidth();
	int getHeight();

	void setPosition(int x, int y);

	void hold();

};

