#pragma once
#ifndef _ITEM_H
#define _ITEM_H

#include "item.h"
#include <string>

using namespace std;

enum ItemType {
	SUPPLY,
	ARMOR,
	WEAPON
};

enum SupplyType {
	ADDHEALTH,
	ADDSTRENGTH
};

enum ArmorPosition {
	HEAD,
	BODY,
	LEG,
	FOOT
};

class Item {
public:
	Item(int id, string name, string description, int value, int soldValue) :
		id(id), name(name), description(description), value(value), soldValue(soldValue) {}

	~Item() {}

public:
	int getID() { return id; }
	string getName() { return name; }
	string getDescription() { return description; }
	int getValue() { return value; }
	int getSoldValue() { return soldValue; }

private:
	int id;				// ID
	string name;		// 名称
	string description;	// 描述
	int value;			// 价值
	int soldValue;		// 出售价值

};

class Supply : public Item {
public:
	Supply(int id, string name, string description, int value, int soldValue, int supplyType, float effectValue) :
		Item(id, name, description, value, soldValue),
		supplyType(supplyType), effectValue(effectValue) {}

public:
	int getType() { return supplyType; }
	float getEffectValue() { return effectValue; }

private:
	int supplyType;
	float effectValue;
};

class Armor : public Item {
public:
	Armor(int id, string name, string description, int value, int soldValue, int pos, float defenceEffect, float sensitiveEffect) :
		Item(id, name, description, value, soldValue), position(pos),
		defenceEffect(defenceEffect), sensitiveEffect(sensitiveEffect) {}

public:
	float getDenfence() { return defenceEffect; }
	float getSensitive() { return sensitiveEffect; }
	int getPosition() { return position; }

private:
	int position;
	float defenceEffect;
	float sensitiveEffect;

};

class Weapon : public Item {
public:
	Weapon(int id, string name, string description, int value, int soldValue, float damage) :
		Item(id, name, description, value, soldValue), damage(damage) {}

public:
	float getDamage() { return damage; }

private:
	float damage;
};

#endif // !_ITEM_H
