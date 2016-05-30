#ifndef __gameobject_h__
#define __gameobject_h__

class GameObject {
	private:
	int x_pos;
	int y_pos;
	int type;
	int id;
	int health;
	int maxhealth;
	int defense;
	int attack;
	bool dead;
	
	public:
	
	int getXPos(){
		return x_pos;
	}
	int getYPos(){
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
	
	void setXPos(int x){
		x_pos = x;
	}
	void setYPos(int y) {
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
};
#endif