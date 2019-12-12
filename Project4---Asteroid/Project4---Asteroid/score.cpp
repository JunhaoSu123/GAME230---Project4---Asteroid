#include "Score.h"
#include "Game_Object.h"

score::score(sf::Font &font, unsigned int size) :sf::Text("Score: 0", font, size) {
	this->points = 0;
}
void score::PlusOne() {
	this->points += 10;
}
void score::Update() {
	this->setString("Score: " + std::to_string(this->points));
}
