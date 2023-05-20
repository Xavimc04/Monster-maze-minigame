#pragma once
#include <iostream>; 
#include "User.h";   

using namespace std;

class Enemy : public User {
	protected:
		bool final; 
		int slot; 

	public:
		Enemy(string name, int health, bool isLastEnemy, int slotValue) : User(name, health) {
			final = isLastEnemy; 
			slot = slotValue; 
		}

		int getSlot();
};