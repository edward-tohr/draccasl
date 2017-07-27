#include "gameobject.h"
//#include "math.h"
//REMEMBER: When populatingObjectVector, take it in as a pointer (std::vector* objectVector).

using std::cout;
using std::string;


void GameObject::render() {
// Okay. Check if the object is within the bounds of the camera.
// If yes, texture.render(this.x - camera.x, this.y - camera.y, this.sprite);
// ... no, the texture.render() function was overloaded in the tutorial. Hmm.
// this'll be tricky. Should I make my own texture class? or just shove all the code here?
//If (thisCollision is within bounds of camera)
  SDL_Rect *sprite = new SDL_Rect();
  sprite->h = collisionBox.h;
  sprite->w = collisionBox.w;
  sprite->x = collisionBox.x;
  sprite->y = collisionBox.y;
  SDL_RenderCopy(gRenderer,getTexture(),NULL,sprite);

//RenderCopy( Renderer to render at, texture to render, source rectangle if not full texture, destination rectangle if not full renderer.)


}

void GameObject::loadSprite(string name){
  name.append(".png");
SDL_Surface* tempTexture = IMG_Load(name.c_str());
if (!tempTexture){
  if (DEBUG >= ERROR){
    cout << "Couldn't load " << name << "!\n";
  }
} else {
  SDL_SetColorKey(tempTexture,SDL_TRUE,SDL_MapRGB(tempTexture->format,0xFF,0x00,0xFF));
setTexture(SDL_CreateTextureFromSurface(gRenderer, tempTexture));
collisionBox.h = tempTexture->h;
collisionBox.w = tempTexture->w;
}

}


void GameObject::beginUpdate() {

  const Uint8 *state = SDL_GetKeyboardState(NULL);

  if (state[SDL_SCANCODE_LEFT]) {
    changeXVel(-MOVE_ACCEL);
  }

  if (state[SDL_SCANCODE_RIGHT]) {
    changeXVel(MOVE_ACCEL);
  }

  //Commenting out floatmode.


  if (state[SDL_SCANCODE_UP]) {
    changeYVel(-MOVE_ACCEL);
  }

  if (state[SDL_SCANCODE_DOWN]) {
    changeYVel(MOVE_ACCEL);
  }



  if (!state[SDL_SCANCODE_RIGHT] && !state[SDL_SCANCODE_LEFT]) {
    if (getXVel() > DRAG) {
      changeXVel(-DRAG);
    }
    if (getXVel() < -DRAG) {
      changeXVel(DRAG);
    }
    if (getXVel() <= DRAG && getXVel() > 0) {
      setXVel(0);
      //if (DEBUG == ALL){std::cout << "Jack's XVel is zeroed.\n";}
    }
    if (getXVel() >= -DRAG && getXVel() < 0) {
      setXVel(0);
      //if (DEBUG == ALL){std::cout << "Jack's XVel is zeroed.\n";}
    }

  }


  //Really, there's no need to apply physics to floatmode.
  if (!state[SDL_SCANCODE_UP] && !state[SDL_SCANCODE_DOWN]) {
   if (getYVel() > DRAG){
    changeYVel(-DRAG);
   }
   if (getYVel() < -DRAG) {
    changeYVel(DRAG);
   }
   if (getYVel() <= DRAG && getYVel() > 0) {
    setYVel(0);
    //if (DEBUG == ALL){std::cout << "Jack's YVel is zeroed.\n";}
   }
   if (getYVel() >= -DRAG && getYVel() < 0 ) {
    setYVel(0);
    //if (DEBUG == ALL){std::cout << "Jack's YVel is zeroed.\n";}
   }


  }


  // Gravity. Constant downwards force.
  if (gravity) {changeYVel(GRAVITY_V * getGravMult());}
}

void GameObject::changeXVel(float acc) {

  x_vel += acc;

  if (x_vel > VELOCITY_MAX) {
    x_vel = VELOCITY_MAX;
  }

  if (x_vel < -VELOCITY_MAX) {
    x_vel = -VELOCITY_MAX;
  }
}

void GameObject::changeYVel(float acc) {
  y_vel += acc;

  if (y_vel > VELOCITY_MAX) {
    y_vel = VELOCITY_MAX;
  }

  if (y_vel < -VELOCITY_MAX) {
    y_vel = -VELOCITY_MAX;
  }
}

SDL_Rect GameObject::moveCollider(float xVel, float yVel) {
  SDL_Rect tempCollision = getCollision();
  tempCollision.x += xVel;
  tempCollision.y += yVel;
  return tempCollision;

}

void GameObject::collisionUpdate() {
  mXPosition += x_vel;
  mYPosition += y_vel;
  setCollision(mXPosition,mYPosition,this->getWidth(),this->getHeight());
}
