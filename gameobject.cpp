#include "gameobject.h"
#include "math.h"
//REMEMBER:  When populatingObjectVector, take it in as a pointer (std::vector* objectVector).


void GameObject::render() {
// Okay. Check if the object is within the bounds of the camera.
// If yes, texture.render(this.x - camera.x, this.y - camera.y, this.sprite);
// ... no, the texture.render() function was overloaded in the tutorial. Hmm.
// this'll be tricky. Should I make my own texture class? or just shove all the code here?
//If (thisCollision is within bounds of camera)

SDL_RenderCopy(gRenderer,this ->getTexture(),NULL,this -> getCollision());

//RenderCopy( Renderer to render at, texture to render, source rectangle if not full texture, destination rectangle if not full renderer.)

//Why is Jack not rendering here...?

}


void GameObject::update() {
	
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	
	if (state[SDL_SCANCODE_LEFT]){
		changeXVel(-1.5);
	}
	
	if (state[SDL_SCANCODE_RIGHT]){
		changeXVel(1.5);
	}
	
	if (state[SDL_SCANCODE_UP]){
		changeYVel(-1.5);
	}
	
	if (state[SDL_SCANCODE_DOWN]){
		changeYVel(1.5);
	}
	
	if (!state[SDL_SCANCODE_RIGHT] && !state[SDL_SCANCODE_LEFT]) {
		if (getXVel() > 0.5){
			changeXVel(-.5);
		}
		if (getXVel() < -0.5) {
			changeXVel(0.5);
		}
		if (getXVel() <= 0.5 && getXVel() > 0) {
			setXVel(0);
			std::cout << "Jack's XVel is zeroed.\n";
		}
		if (getXVel() >= -0.5 && getXVel() < 0) {
			setXVel(0);
			std::cout << "Jack's XVel is zeroed.\n";
		}
		
	}
	
	if (!state[SDL_SCANCODE_UP] && !state[SDL_SCANCODE_DOWN]) {
		if (getYVel() > 0.5){
			changeYVel(-.5);
		}
		if (getYVel() < -0.5) {
			changeYVel(0.5);
		}
		if (getYVel() <= 0.5 && getYVel() > 0) {
			setYVel(0);
			std::cout << "Jack's YVel is zeroed.\n";
		}
		if (getYVel() >= -0.5 && getYVel() < 0) {
			setYVel(0);
			std::cout << "Jack's YVel is zeroed.\n";
		}
		
	}
	
	if (getXVel() != 0 || getYVel() != 0) {
	mXPosition += x_vel;
	mYPosition += y_vel;
	}
	
	collisionBox -> x = floor(mXPosition);
	collisionBox -> y = floor(mYPosition);
}

void GameObject::changeXVel(float acc){
	
	//Commenting this out real quick to prevent crashes from nonzero X position.
	//No, I don't get it either.
	
	x_vel += acc;
	
	if (x_vel > VELOCITY_MAX){
		x_vel = VELOCITY_MAX;
	}
	
	if (x_vel < -VELOCITY_MAX){
		x_vel = -VELOCITY_MAX;
	}
}

void GameObject::changeYVel(float acc){
	y_vel += acc;
	
	if (y_vel > VELOCITY_MAX){
		y_vel = VELOCITY_MAX;
	}
	
	if (y_vel < -VELOCITY_MAX){
		y_vel = -VELOCITY_MAX;
	}
}