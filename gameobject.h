#ifndef __gameobject_h__
#define __gameobject_h__

#include "SDL.h"
#include "constants.h"

extern DEBUG_T DEBUG;


class GameObject {
	private:
	float x_pos;
	float y_pos;
	int type;
	int id;
	int health;
	int maxhealth;
	int defense;
	int attack;
	bool dead;
	
	SDL_Rect *collisionBox;
	SDL_Texture *objectTexture;
	
	
	public:float getXPos(){
		return x_pos;
	}
	float getYPos(){
		return y_pos;
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
	
	SDL_Rect* getCollision(){
		return collisionBox;
	}
	
	SDL_Texture* getTexture(){
		return objectTexture;
	}
	
	void setXPos(float x){
		x_pos = x;
	}
	void setYPos(float y) {
		y_pos = y;
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
	
	void setCollision(SDL_Rect *collider){
		collisionBox = collider;
	}
	
	void setTexture(SDL_Texture *newTexture){
		objectTexture = newTexture;
	}
	
	void render(SDL_Rect& camera);
};
#endif