#include "Character.h"

#include "Display.h"
#include "CollisionGrid.h"
#include "Timing.h"
#include "Utilities.h"

#include <iostream>

	Character::Character(	Display* display, Timing* timer,
			int imageX, int imageY, int width, int height, 
			const char* imageFileName,
			int x, int y, int maxSpeedX, int maxSpeedY,
			int gravity, int jumpTime, float jumpDecrease):

			_display(display), _timer(timer),
			_imageX(imageX), _imageY(imageY), _width(width), _height(height),
			_xPos(x), _yPos(y), _maxSpeedX(maxSpeedX), _maxSpeedY(maxSpeedY),
			_speedX(0), _speedY(gravity), _gravity(gravity), _jumpTime(jumpTime), _jumpDecrease(jumpDecrease)
	{

		_image = _display->loadImage(imageFileName);

		_paused = false;
		_movementStatus = STILL;
		_directionStatus = RIGHT;

		_animateFrame = -1;
		// Animation Clipping
		clipAnimationSprites();

		#ifndef CONSOLE
		std::cout << "Character created.\n";
		#endif		
	}

	Character::~Character()
	{
		_display->closeImage(_image);
	}

	
	void Character::update(float deltaTime, GameToolsStruct & gameTools )
	{

		 if ( _movementStatus == STILL && ( _speedX != 0 || _speedY != 0 ) ) {
			_movementStatus = WALK;
			if (_speedX > 0 ) {
				_directionStatus = RIGHT;
			} else {
				_directionStatus = LEFT;
			}
		}


		_xPos += (_speedX * deltaTime);
	
		// Check if the change in xPos results in a collision
		if ( gameTools.CollisionGrid->getCollisionTile(_xPos, _yPos, _width, _height) ){
			// Revert the change in position
			_xPos -= (_speedX * deltaTime);
		}

		_yPos += (_speedY * deltaTime);

		// Check if the change in yPos results in a collision
		if ( gameTools.CollisionGrid->getCollisionTile(_xPos, _yPos,  _width, _height) ){
			// Revert the change in position
			_yPos -= (_speedY * deltaTime);
		}

		gameTools.CollisionGrid->collectCoins(_xPos, _yPos,  _width, _height);

	}

	
	void Character::updateAutomated(float deltaTime, GameToolsStruct & gameTools )
	{
		
	//	static int xBeforeJump;

	//	if ( _directionStatus == RIGHT ) {
	//		if ( !gameTools.CollisionGrid->rightCollision(_xPos, _yPos, _width, _height) && 
	//			gameTools.CollisionGrid->onGround(_xPos, _yPos, _width, _height) ) {
	//			a_moveRight();
	//		} else  {
	//			if (_movementStatus == WALK || _movementStatus == JUMP) {
	//				jump( gameTools );
	//			} else {
	//				_directionStatus = LEFT;
	//			}
	//		}
	//	}
	//	else if ( _directionStatus == LEFT ) {
	//		if ( !gameTools.CollisionGrid->leftCollision(_xPos, _yPos, _width, _height) && 
	//			gameTools.CollisionGrid->onGround(_xPos, _yPos, _width, _height) ) {
	//			a_moveLeft();
	//			_aThresholdTime = gameTools.Timer->timeSinceCreation();
	//		} else {
	//			if (_movementStatus == WALK || _movementStatus == JUMP) {
	//				jump( gameTools );
	//			} else {
	//				_directionStatus = RIGHT;
	//			}
	//		}
	//	}





		// Set the status for player being still. Other statuses are set
		// in their respective methods. STILL is set on update.
		
		_xPos += (_speedX * deltaTime);
	
		// Check if the change in xPos results in a collision
		if ( gameTools.CollisionGrid->getCollisionTile(_xPos, _yPos, _width, _height) ){
			// Revert the change in position
			_xPos -= (_speedX * deltaTime);
		}

		_yPos += (_speedY * deltaTime);

		// Check if the change in yPos results in a collision
		if ( gameTools.CollisionGrid->getCollisionTile(_xPos, _yPos,  _width, _height) ){
			// Revert the change in position
			_yPos -= (_speedY * deltaTime);
		}

	}

	void Character::draw()
	{
		_display->drawImage(_image, _imageX,  _imageX, (int)_xPos, (int)_yPos, _width,  _height);
	}

	void Character::draw(int x, int y)
	{
		_display->drawImage(_image, x,  y, (int)_xPos, (int)_yPos, _width,  _height);
	}


	void Character::moveLeft()
	{
		_directionStatus = LEFT;
		_movementStatus = WALK;
		_speedX -= _maxSpeedX;

	}
	void Character::moveRight()
	{
		_directionStatus = RIGHT;
		_movementStatus = WALK;
		_speedX += _maxSpeedX;
	}

	void Character::undoMoveLeft()
	{
		if (_movementStatus!=STILL) _speedX += _maxSpeedX;
		
	}
	void Character::undoMoveRight()
	{
		if (_movementStatus!=STILL) _speedX -= _maxSpeedX;
	}

	void Character::jump( GameToolsStruct &gameTools ) {
			
			// If player is on the ground, initiate jump.
			if ( gameTools.CollisionGrid->onGround(_xPos, _yPos, _width, _height) && 
				_movementStatus != JUMP ) {
				timePassed = gameTools.Timer->timeSinceCreation();
				_movementStatus = JUMP;
				_speedY = -(_gravity + _maxSpeedY);
				timePassed_midJump = gameTools.Timer->timeSinceCreation();
			} 
			// During the jump interval, slow down the jump to create the effect of gravity
			else if ( gameTools.Timer->timeSinceCreation() < timePassed + _jumpTime ){
				if ( gameTools.Timer->timeSinceCreation() - timePassed_midJump > 1000/FPS_CAP ) {
					_speedY = _speedY * _jumpDecrease;		
					timePassed_midJump = gameTools.Timer->timeSinceCreation();
				}
			} 
			// Stop the jump
			else {
				stopJump();
			}

	}

	void Character::stopJump()
	{
		_movementStatus = STILL;
		_speedY = _gravity;
	}

	void Character::stopXMovement()
	{
		_movementStatus = STILL;
		_speedX = 0;
	}

	void Character::stopMovement()
	{
		_movementStatus = STILL;
		stopXMovement();
		stopJump();
	}

	void Character::stopAutomatedMovement()
	{
		_speedX = 0;
	}

	bool Character::isMoving(){
		if ( _speedX !=0 || _speedX != 0 ) {
			return true;
		} else {
			return false;
		}
	}

	int Character::getDirectionStatus(){
		return _directionStatus;
	}

	int Character::getJumpTime(){
		return _jumpTime;
	}

	float Character::getXPos()
	{
		return _xPos;
	}
	float Character::getYPos()
	{
		return _yPos;
	}

	int Character::getWidth()
	{
		return _width;
	}
	int Character::getHeight()
	{
		return _height;
	}

	void Character:: clipAnimationSprites(){

		int i;
	
		// Walk: Right 
		for ( i = 0 ;  i <= 3; i++ ) {
			_animateRightX[i] = i * _width;
			_animateRightY[i] = 0;
		}
		// Walk: Left
		for ( i = 0 ;  i <= 3; i++ ) {
			_animateLeftX[i] = i * _width;
			_animateLeftY[i] = _height;
		}
		// Jump
		for ( i = 0 ;  i <= 3; i++ ) {
			_animateJumpX[i] = i * _width;
			_animateJumpY[i] = _height * 2;
		}


	}
	void Character::animate(){

		static int timePassed;
	
		if (_animateFrame == -1 ) {
			draw(_animateLeftX[0] ,  _animateLeftY[0]);
			timePassed = _timer->timeSinceCreation();
			_animateFrame = 0;
		}

		if ( _directionStatus == LEFT && _movementStatus == STILL ) {
			draw(_animateLeftX[3] ,  _animateLeftY[3]);
		}
		else if ( _directionStatus == RIGHT && _movementStatus == STILL ) {
			draw( _animateRightX[3], _animateRightY[3] );
		} 
		else if ( _directionStatus == RIGHT && _movementStatus == WALK ) {
			draw( _animateRightX[_animateFrame], _animateRightY[_animateFrame] );
		} 
		else if ( _directionStatus == LEFT && _movementStatus == WALK ) {
			draw(_animateLeftX[_animateFrame] ,  _animateLeftY[_animateFrame]);
		} else if (_movementStatus == JUMP ) {
			draw(_animateJumpX[_animateFrame] ,  _animateJumpY[_animateFrame]);
		}
		

		if (  _timer->timeSinceCreation() - timePassed > PLAYER_ANIMATION_DELAY ) {
			_animateFrame++;
			if ( _animateFrame >= 4 ) {
				_animateFrame = 0;
			}
			timePassed = _timer->timeSinceCreation();
		}

	}

	void Character::setPosition(int x, int y){
		_xPos = x;
		_yPos = y;
	}

	void Character::hold(){
		_paused = true;
	}