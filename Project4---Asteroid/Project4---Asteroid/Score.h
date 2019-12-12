#pragma once
#include <SFML/Graphics.hpp>

class score : public sf::Text {
public:
	score(sf::Font &font, unsigned int size);
	void PlusOne();
	void Update();
	//void WinCheck();
private:
	int points;
};