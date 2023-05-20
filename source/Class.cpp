#include <iostream>; 
#include "User.h";
#include "Enemy.h"; 

using namespace std;

string User::getName() const {
	return name; 
}

int User::getHealth() const {
	return health; 
}

void User::setHealth(int value) {
	health = value; 
}

int Enemy::getSlot() {
	return slot;
}