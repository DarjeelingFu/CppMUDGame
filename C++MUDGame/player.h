#pragma once
#ifndef _PLAYER_H
#define _PLAYER_H

#include <string>
#include "bag.h"

using namespace std;

enum PlayerAttr {
	HEALTH,
	MAXHEALTH,
	STRENGTH,
	DEFENCE,
	SENSITIVE,
	DAMAGE,
	MONEY
};

enum NPCCareer {
	MERCHANT,
	FIGHTER,
};

class Player {
public:
	Player(
		string name,
		float health,
		float maxHealth,
		float strength,
		float defence,
		float sensitive,
		float damage,
		int money
	) :
		name(name), health(health), maxHealth(maxHealth),
		strength(strength), defence(defence), sensitive(sensitive),
		damage(damage), money(money) {}
	
	Player(const Player& player) :
		name(player.name), health(player.health), maxHealth(player.maxHealth),
		strength(player.strength), defence(player.defence), sensitive(player.sensitive),
		damage(player.damage), money(player.money) {}

	~Player() {}

public:
	string getName() { return name; }
	Bag* getBag() { return &bag; }
	float attr(int prompt, float setValue = -1); // 获取属性
	void alter(int prompt, float value); // 用变化量改变属性

private:
	string name;		// 名称
	float health;		// HP
	float maxHealth;	// 最大HP
	float strength;		// 体力
	float defence;		// 防御
	float sensitive;	// 灵敏
	float damage;		// 伤害
	int money;			// 金钱

	Bag bag;			// 背包
};

class NPC : public Player {
public:
	NPC(
		string name,
		float health,
		float maxHealth,
		float strength,
		float defence,
		float sensitive,
		float damage,
		int money,
		int sceneID,
		string itemList,
		int career
	) :
		Player(name, health, maxHealth, strength, defence, sensitive, damage, money),
		sceneID(sceneID), itemList(itemList), career(career)
	{}

	NPC(const NPC& npc) :
		Player(npc), sceneID(npc.sceneID), itemList(npc.itemList), career(npc.career)
	{}

public:
	vector<string>& getDialogues() { return dialogues; }
	int getSceneID() { return sceneID; }
	string getItemList() { return itemList; }
	int getCareer() { return career; }

private:
	vector<string> dialogues;
	int sceneID;
	string itemList;
	int career;

};

#endif // !_PLAYER_H
