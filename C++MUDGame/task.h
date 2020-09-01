#pragma once
#ifndef _TASK_H
#define _TASK_H

#include "player.h"
#include <string>
#include <map>

using namespace std;

enum TaskProgress {
	DISMISS,
	UNCOMPLISHED,
	COMPLISHED
};

class Task {
public:
	Task(NPC* npc, Item* reward) :
		npc(npc), reward(reward) {
		state = false;
	}

public:
	NPC* getNPC() { return npc; }
	string getDialogueWithProgress(int progress) { return dialogues[progress]; }
	bool getState() { return state; }

private:
	NPC* npc;
	bool state;
	map<int, string> dialogues;
	virtual int checkProgress(Player* player) = 0;
	Item* reward;

};

class ItemCollectingTask : public Task {
public:
	ItemCollectingTask(NPC* npc, Item* reward, int itemType, short id) :
		Task(npc, reward), itemType(itemType), id(id) {}

public:


private:
	int itemType;
	short id;
	virtual int checkProgress(Player* player);
};

#endif // !_TASK_H
