#pragma once
#include <iostream>; 

using namespace std; 

class User { 
	public: 
		string name;
		int health;

		User(string userName, int userHealth) {
			name = userName; 
			health = userHealth; 
		}

		string getName() const; 
		int getHealth() const; 

		void setHealth(int value);
};