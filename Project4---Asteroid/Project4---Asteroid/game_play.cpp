#include <iostream>
#include"Main_Menu.h"
#include"Game_Play.h"
//#include"Score.h"
//#include"Life.h"
#include"Game_State.h"
//#include"WinState.h"


void game_play::Initialize(sf::RenderWindow* window) {
	this->font = new sf::Font();
	this->font->loadFromFile("font.otf");

	this->P = new player;
	this->P->setScale(0.5f, 0.5f);
	this->P->setPosition(window->getSize().x / 2 - this->P->getGlobalBounds().width / 2, window->getSize().y /2);

}
void game_play::Update(sf::RenderWindow* window) {
	this->P->Update();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
		coreState.SetState(new main_menu());
	}

}
void game_play::Render(sf::RenderWindow* window) {
	window->draw(*this->P);

}
void game_play::Destroy(sf::RenderWindow* window) {
	delete this->font;

}
