#pragma once
#include<SFML/Graphics.hpp>
#include<string>

class Game_Object : public sf::Sprite {
public:
	Game_Object() {
		this->texture = new sf::Texture();
	}

	void Load(std::string filename) {
		this->texture->loadFromFile("Sprites/" + filename);
		this->setTexture(*this->texture);
	}

	virtual void Update() {
		this->move(this->velocity);
	}
	bool CollisionCheck(Game_Object* gameobject) {
		return this->getGlobalBounds().intersects(gameobject->getGlobalBounds());
	}
	~Game_Object() {
		delete this->texture;
	}

protected:
	sf::Vector2f velocity;

private:
	sf::Texture* texture;

};