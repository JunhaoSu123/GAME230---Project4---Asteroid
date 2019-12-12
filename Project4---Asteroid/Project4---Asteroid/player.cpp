#include "Player.h"
#include"Game_State.h"


player::player()
{
	x = 300;
	y = 300;
	DEGTORAD = 0.017453f;
	this->Load("Player.png");
}

void player::Update() {

	

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) angle += 3;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) angle -=3;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) thrust = true;
	else thrust = false;

	if (thrust) {
		dx += cos(angle*DEGTORAD)*0.2;
		dy += sin(angle*DEGTORAD)*0.2;
	}
	else {
		dx *= 0.99;
		dy *= 0.99;
	}

	int maxSpeed = 15;
	float speed = sqrt(dx*dx + dy * dy);
	if (speed > maxSpeed) {
		dx *= maxSpeed / speed;
		dy *= maxSpeed / speed;
	}
	x += dx;
	y += dy;

	Game_Object::Update();


}