#ifndef __gameobject_h__
#define __gameobject_h__

#include "SDL.h"
#include "constants.h"
#include "iostream"

extern DEBUG_T DEBUG;
extern SDL_Renderer *gRenderer;
extern SDL_Rect gCamera;
extern const int VELOCITY_MAX;


class GameObject {
	private:
	float mXPosition;
	float mYPosition;
	int type;
	int id;
	int health;
	int maxhealth;
	int defense;
	int attack;
	bool dead;
	float x_vel;
	float y_vel;
	bool jump;

	SDL_Rect collisionBox;
	SDL_Texture *objectTexture;


	public:

	float getXPos(){
		return mXPosition;
	}

	float getYPos(){
		return mYPosition;
	}

	int getWidth(){
	    return collisionBox.w;
	}

	int getHeight(){
	return collisionBox.h;
	}

	float getXVel(){
		return x_vel;
	}

	float getYVel(){
		return y_vel;
	}

	bool canJump(){
		return jump;
	}

	int getType(){
		return type;
	}

	int getID(){
		return id;
	}

	int getHealth(){
		return health;
	}

	int getMaxHealth(){
		return maxhealth;
	}

	int getDefense(){
		return defense;
	}

	int getAttack(){
		return attack;
	}

	bool isDead(){
		return dead;
	}

	SDL_Rect getCollision(){
		return collisionBox;
	}

	SDL_Texture* getTexture(){
		return objectTexture;
	}

	void setXPos(float x){
		mXPosition = x;
	}

	void setYPos(float y) {
		mYPosition = y;
	}

	void setXVel(float x){
		x_vel = x;
	}

	void setYVel(float y){
		y_vel = y;
	}

	void setJump(bool j) {
		jump = j;
	}

	void setType(int t) {
		type = t;
	}

	void setID(int i) {
		id = i;
	}

	void setHealth(int h){
		health = h;
		if (health >= maxhealth){
			health = maxhealth;
		} else if (health <= 0){
			health = 0;
			dead = true;
		}
	}

	void setMaxHealth(int m){
		maxhealth = m;
		if (health > maxhealth){
			health = maxhealth;
		}
		if (maxhealth <= 0){
			health = 0;
			maxhealth = 0;
			dead = true;
		}
	}

	void setDefense(int d) {
		defense = d;
	}

	void setAttack(int a) {
		attack = a;
	}

	void kill(){
		dead = true;
	}

	void unkill(){
		dead = false;
	}

	/*void setCollision(SDL_Rect *collider){
		collisionBox ->h = collider->h;
		collisionBox ->w = collider->w;
		collisionBox ->x = collider->x;
		collisionBox ->y = collider->y;
	}

	void setCollision(SDL_Rect collider){
		collisionBox ->h = collider.h;
		collisionBox ->w = collider.w;
		collisionBox ->x = collider.x;
		collisionBox ->y = collider.y;
	}*/

	void setCollision(float x, float y, int w, int h){
        collisionBox.h = h;
		collisionBox.w = w;
		collisionBox.x = (int)x;
		collisionBox.y = (int)y;
	}

	void setTexture(SDL_Texture *newTexture){
		objectTexture = newTexture;
	}

	void render();

	void init();

	void beginUpdate();

	void changeXVel(float acc);
	void changeYVel(float acc);

	SDL_Rect moveCollider(float xVel, float yVel);
	void collisionUpdate();
};
#endif
