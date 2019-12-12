#pragma once
#include"Game_Object.h"

class player : public Game_Object {
public:
	//player(int playerNum);
	player();
	void Update();
private:
	float DEGTORAD;
	float x, y;
	float dx, dy, angle = 0;
	bool thrust;
};