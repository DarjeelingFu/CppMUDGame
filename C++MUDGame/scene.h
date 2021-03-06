#pragma once
#ifndef _SCENE_H
#define _SCENE_H

#include <string>
#include <vector>

using namespace std;

class Scene {
public:
	Scene(int id, string name, string description, string connected, int blocked) :
		name(name), description(description), connected(connected), id(id), blocked(blocked) {}

	Scene(const Scene& scene) : 
		name(scene.name), description(scene.description), id(scene.id),
		connected(scene.connected), neighbors(scene.neighbors), blocked(scene.blocked) {}

	~Scene() {}

public:
	vector<Scene*>& getNeighbors() { return neighbors; }
	string getName() { return name; }
	string getDescription() { return description; }
	string getConnected() { return connected; }
	vector<NPC*>& getNPCs() { return NPCs; }
	int getId() { return id; }
	// void setSceneId(int id) { this->id = id; }
	int blockedNeighbor() { return blocked; }
	void setAccess(int blocked) { this->blocked = blocked; }

private:
	int id;
	string name;
	string description;
	string connected;
	vector<Scene*> neighbors;
	vector<NPC*> NPCs;
	int blocked;
};

#endif // !_SCENE_H
