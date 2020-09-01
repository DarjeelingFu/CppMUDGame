#pragma once
#ifndef _BAG_H
#define _BAG_H

#include "item.h"
#include <vector>
#include <stack>

using namespace std;

class Bag {
public:
	Bag() {}
	~Bag() {}

public:
	vector<short>& getSupplies() { return supplies; }
	void addSupply(short id) { supplies.emplace_back(id); }
	vector<short>& getArmors() { return armors; }
	void addArmor(short id) { armors.emplace_back(id); }
	vector<short>& getWeapons() { return weapons; }
	void addWeapon(short id) { weapons.emplace_back(id); }

private:
	vector<short> supplies;
	vector<short> armors;
	vector<short> weapons;

public:
	stack<short> weaponSlot;
	stack<short> headArmorSlot;
	stack<short> bodyArmorSlot;
	stack<short> legArmorSlot;
	stack<short> footArmorSlot;
};

#endif // !_BAG_H
