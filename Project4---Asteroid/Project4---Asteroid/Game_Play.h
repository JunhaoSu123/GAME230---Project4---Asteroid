#pragma once
#include "Game_State.h"
#include"Player.h"
//#include"Brick.h"
//#include"Score.h"
//#include"Life.h"
#include<SFML/Audio.hpp>
//#include"PowerUp.h"

class game_play : public tiny_state {
public:
	void Initialize(sf::RenderWindow* window);
	void Update(sf::RenderWindow* window);
	void Render(sf::RenderWindow* window);
	void Destroy(sf::RenderWindow* window);


private:
	player* P;

	sf::Font* font;

	sf::Text* level1;
	sf::Text* level2;
	sf::Text* level3;
	sf::SoundBuffer* bufferWin;
	sf::Sound* soundWin;
	sf::SoundBuffer* bufferGo;
	sf::Sound* soundGo;
};