#include "gameobject.h"
#include "math.h"
//REMEMBER:  When populatingObjectVector, take it in as a pointer (std::vector* objectVector).


void GameObject::render() {
// Okay. Check if the object is within the bounds of the camera.
// If yes, texture.render(this.x - camera.x, this.y - camera.y, this.sprite);
// ... no, the texture.render() function was overloaded in the tutorial. Hmm.
// this'll be tricky. Should I make my own texture class? or just shove all the code here?
//If (thisCollision is within bounds of camera)

SDL_RenderCopy(gRenderer,getTexture(),NULL,getCollision());

//RenderCopy( Renderer to render at, texture to render, source rectangle if not full texture, destination rectangle if not full renderer.)

//Why is Jack not rendering here...?

}


void GameObject::beginUpdate() {
	
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
		if (getXVel() <= 0.5 && getXVel() > 0 && getXVel() != 0) {
			setXVel(0);
			if (DEBUG == ALL){std::cout << "Jack's XVel is zeroed.\n";}
		}
		if (getXVel() >= -0.5 && getXVel() < 0 && getXVel() != 0) {
			setXVel(0);
			if (DEBUG == ALL){std::cout << "Jack's XVel is zeroed.\n";}
		}
		
	}
	
	if (!state[SDL_SCANCODE_UP] && !state[SDL_SCANCODE_DOWN]) {
		if (getYVel() > 0.5){
			changeYVel(-.5);
		}
		if (getYVel() < -0.5) {
			changeYVel(0.5);
		}
		if (getYVel() <= 0.5 && getYVel() > 0 && getYVel() != 0) {
			setYVel(0);
			if (DEBUG == ALL){std::cout << "Jack's YVel is zeroed.\n";}
		}
		if (getYVel() >= -0.5 && getYVel() < 0 && getYVel() != 0) {
			setYVel(0);
			if (DEBUG == ALL){std::cout << "Jack's YVel is zeroed.\n";}
		}
		
	}
	
	// Gravity. Constant downwards force.
	//changeYVel(.45);
}

void GameObject::changeXVel(float acc){

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

SDL_Rect GameObject::moveCollider(float xVel, float yVel){
	SDL_Rect tempCollision = *getCollision();
	tempCollision.x += xVel;
	tempCollision.y += yVel;
	return tempCollision;
	
}

void GameObject::collisionUpdate(){
	mXPosition += x_vel;
	mYPosition += y_vel;
	SDL_Rect tempCollision = *getCollision();
	tempCollision.x += x_vel;
	tempCollision.y += y_vel;
	setCollision(tempCollision);
}