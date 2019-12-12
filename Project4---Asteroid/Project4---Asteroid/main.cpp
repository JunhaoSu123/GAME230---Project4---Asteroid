#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#include<iostream>
#include"Game_State.h"
#include"Main_Menu.h"

game_state coreState;
bool quitGame = false;
float DEGTORAD;

int main()
{

	sf::RenderWindow window(sf::VideoMode(1200, 900), "Asteroid!");

	coreState.SetWindow(&window);
	coreState.SetState(new main_menu());

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::Black);
		coreState.Update();
		coreState.Render();
		window.display();

		if (quitGame) {
			window.close();
		}
		Sleep(6);
	}

	return 0;
}
