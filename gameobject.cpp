#include "gameobject.h"



void GameObject::render() {
// Okay. Check if the object is within the bounds of the camera.
// If yes, texture.render(this.x - camera.x, this.y - camera.y, this.sprite);
// ... no, the texture.render() function was overloaded in the tutorial. Hmm.
// this'll be tricky. Should I make my own texture class? or just shove all the code here?

SDL_Texture *thisTexture = getTexture();
SDL_Rect *thisCollision = getCollision();

//If (thisCollision is within bounds of camera)

SDL_RenderCopy(gRenderer,thisTexture,NULL,thisCollision);

//RenderCopy( Renderer to render at, texture to render, source rectangle if not full texture, destination rectangle if not full renderer.)

}