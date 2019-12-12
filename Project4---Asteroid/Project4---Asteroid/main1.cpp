#include <SFML/Graphics.hpp>
#include <sstream>
#include <time.h>
#include <list>
#include<SFML/Audio.hpp>
using namespace sf;
using namespace std;

String toString(Int64 integer)
{
	ostringstream os;
	os << integer;
	return os.str();
}
const int W = 1200;
const int H = 800;
sf::Font* font;
float DEGTORAD = 0.017453f;
int PlayerLife = 3;

//Ttile Screen Selection///////////////////////
bool start = false;
bool GameOver = false;
bool lv1 = true;
bool lv2 = false;
bool lv3 = false;
//int level = 0;
int ButtonSelect = 0;
bool Upkey, Downkey;
bool quitGame = false;
int sc = 0;

class Animation
{
   public:
   float Frame, speed;
   Sprite sprite;
   std::vector<IntRect> frames;

   Animation(){}

   Animation (Texture &t, int x, int y, int w, int h, int count, float Speed)
   {
     Frame = 0;
     speed = Speed;

     for (int i=0;i<count;i++)
      frames.push_back( IntRect(x+i*w, y, w, h)  );

     sprite.setTexture(t);
     sprite.setOrigin(w/2,h/2);
     sprite.setTextureRect(frames[0]);
   }


   void update()
   {
     Frame += speed;
     int n = frames.size();
     if (Frame >= n) Frame -= n;
     if (n>0) sprite.setTextureRect( frames[int(Frame)] );
   }

   bool isEnd()
   {
     return Frame+speed>=frames.size();
   }

};


class Entity
{
   public:
   float x,y,dx,dy,R,angle;
   bool life;
   std::string name;
   Animation anim;

   Entity()
   {
     life=1;
   }

   void settings(Animation &a,int X,int Y,float Angle=0,int radius=1)
   {
     anim = a;
     x=X; y=Y;
     angle = Angle;
     R = radius;
   }

   virtual void update(){};

   void draw(RenderWindow &app)
   {
     anim.sprite.setPosition(x,y);
     anim.sprite.setRotation(angle+90);
     app.draw(anim.sprite);

     CircleShape circle(R);
     circle.setFillColor(Color(255,0,0,170));
     circle.setPosition(x,y);
     circle.setOrigin(R,R);
     //app.draw(circle);
   }

   virtual ~Entity(){};
};


class asteroid: public Entity
{
   public:
   asteroid()
   {
     dx=rand()%8-4;
     dy=rand()%8-4;
     name="asteroid";
   }

   void update()
   {
     x+=dx;
     y+=dy;

     if (x>W) x=0;  if (x<0) x=W;
     if (y>H) y=0;  if (y<0) y=H;
   }

};


class bullet: public Entity
{
   public:
   bullet()
   {
     name="bullet";
   }

   void  update()
   {
     dx=cos(angle*DEGTORAD)*6;
     dy=sin(angle*DEGTORAD)*6;
     // angle+=rand()%7-3;  /*try this*/
     x+=dx;
     y+=dy;

     if (x>W || x<0 || y>H || y<0) life=0;
   }

};


class player: public Entity
{
   public:
   bool thrust;

   player()
   {
     name="player";
   }

   void update()
   {
     if (thrust)
      { dx+=cos(angle*DEGTORAD)*0.2;
        dy+=sin(angle*DEGTORAD)*0.2; }
     else
      { dx*=0.99;
        dy*=0.99; }

    int maxSpeed=15;
    float speed = sqrt(dx*dx+dy*dy);
    if (speed>maxSpeed)
     { dx *= maxSpeed/speed;
       dy *= maxSpeed/speed; }

    x+=dx;
    y+=dy;

    if (x>W) x=0; if (x<0) x=W;
    if (y>H) y=0; if (y<0) y=H;
   }

};


bool isCollide(Entity *a,Entity *b)
{
  return (b->x - a->x)*(b->x - a->x)+
         (b->y - a->y)*(b->y - a->y)<
         (a->R + b->R)*(a->R + b->R);
}


int main()
{
    srand(time(0));

    RenderWindow app(VideoMode(W, H), "Asteroids!");
    app.setFramerateLimit(60);
	
	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("Sounds/shoot.ogg");
	Sound shoot;
	shoot.setBuffer(shootBuffer);

	SoundBuffer blowBuffer;
	blowBuffer.loadFromFile("Sounds/explosion.ogg");
	Sound blow;
	blow.setBuffer(blowBuffer);
	SoundBuffer shipEXBuffer;
	shipEXBuffer.loadFromFile("Sounds/ship_explode.wav");
	Sound shipEX;
	shipEX.setBuffer(shipEXBuffer);
	SoundBuffer thrustBuffer;
	thrustBuffer.loadFromFile("Sounds/lose.wav");
	Sound thrust;
	thrust.setBuffer(thrustBuffer);


    Texture t1,t2,t3,t4,t5,t6,t7;
    t1.loadFromFile("Sprites/ships_saucer.png");
    t2.loadFromFile("Sprites/background.jpg");
    t3.loadFromFile("Sprites/explosions/type_C.png");
    t4.loadFromFile("Sprites/rock.png");
    t5.loadFromFile("Sprites/fire_red.png");
    t6.loadFromFile("Sprites/rock_small.png");
    t7.loadFromFile("Sprites/explosions/type_B.png");

    t1.setSmooth(true);
    t2.setSmooth(true);

    Sprite background(t2);

    Animation sExplosion(t3, 0,0,256,256, 48, 0.5);
    Animation sRock(t4, 0,0,64,64, 16, 0.2);
    Animation sRock_small(t6, 0,0,64,64, 16, 0.2);
    Animation sBullet(t5, 0,0,32,64, 16, 0.8);
    Animation sPlayer(t1, 385,50, 95, 100, 1, 0);
    Animation sPlayer_go(t1, 385, 500, 95, 100, 1, 0);
    Animation sExplosion_ship(t7, 0,0,192,192, 64, 0.5);

	//Title Screen////////////////////////////////////////////////////////////////////////
	sf::Font font;
	font.loadFromFile("font.otf");
	sf::Text title;
	sf::Text play;
	sf::Text quit;
	sf::Text gameOver;
	sf::Text again;
	sf::Text score;
	sf::Text Plife;
	sf::Text lvl;

	//lvl.setFont(font);
	//lvl.setString("Level " + toString(level));
	//lvl.setCharacterSize(48);
	//lvl.setFillColor(sf::Color::White);
	//lvl.setPosition(W / 2 - lvl.getGlobalBounds().width / 2, 10);

	// select the font
	title.setFont(font); // font is a sf::Font
	gameOver.setFont(font);
	play.setFont(font);
	quit.setFont(font);
	again.setFont(font);
	score.setFont(font);
	Plife.setFont(font);

	// set the string to display
	title.setString("Asteroid");
	gameOver.setString("Game Over");
	play.setString("Play");
	quit.setString("Quit");
	again.setString("Press B to go back");
	score.setString("Score: " + toString(sc));
	Plife.setString("Life: " + toString(PlayerLife));

	// set the character size
	title.setCharacterSize(128);
	gameOver.setCharacterSize(128);
	play.setCharacterSize(64);
	quit.setCharacterSize(64);
	again.setCharacterSize(64);
	score.setCharacterSize(32);
	Plife.setCharacterSize(32);


	// set the color
	title.setFillColor(sf::Color::White);
	gameOver.setFillColor(sf::Color::White);
	play.setFillColor(sf::Color::White);
	quit.setFillColor(sf::Color::White);
	again.setFillColor(sf::Color::White);
	score.setFillColor(sf::Color::White);
	Plife.setFillColor(sf::Color::White);

	//set the text position
	title.setPosition(W/2 - title.getGlobalBounds().width /2 ,H/2 - 300);
	gameOver.setPosition(W / 2 - gameOver.getGlobalBounds().width / 2, H / 2 - 300);
	play.setPosition(W / 2 - play.getGlobalBounds().width / 2, H / 2 - play.getGlobalBounds().height/2 + 100);
	quit.setPosition(W / 2 - quit.getGlobalBounds().width / 2, H / 2 - quit.getGlobalBounds().height / 2 + 200);
	again.setPosition(W / 2 - again.getGlobalBounds().width / 2, H / 2 - again.getGlobalBounds().height / 2 + 100);
	score.setPosition(0, 10);
	Plife.setPosition(W - Plife.getGlobalBounds().width, 10);



	//////////////////////////////////////////////////////////////////////////////////////

    std::list<Entity*> entities;


		for (int i = 0; i < 5; i++)//Asteroids number////
		{
			asteroid *a = new asteroid();
			a->settings(sRock, rand() % W, rand() % H, rand() % 360, 25);
			entities.push_back(a);
		}

    player *p = new player();
    p->settings(sPlayer,200,200,0,20);
    entities.push_back(p);

    /////main loop/////
    while (app.isOpen())
    {
        Event event;
        while (app.pollEvent(event))
        {
            if (event.type == Event::Closed)
                app.close();

            if (event.type == Event::KeyPressed)
             if (event.key.code == Keyboard::Space && !GameOver)
              {
				shoot.play();
                bullet *b = new bullet();
				
                b->settings(sBullet,p->x,p->y,p->angle,10);
                entities.push_back(b);
              }
        }

		if (start) {
			if (Keyboard::isKeyPressed(Keyboard::Right)) p->angle += 3;
			if (Keyboard::isKeyPressed(Keyboard::Left))  p->angle -= 3;
			if (Keyboard::isKeyPressed(Keyboard::Up)) 
			{
				p->thrust = true; 
				thrust.play();
			}
			else p->thrust = false;
			}

	///////////////Title Screen Selection//////////////////////////////////////
		if (!start) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && !Upkey) {
				ButtonSelect -= 1;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && !Downkey) {
				ButtonSelect += 1;
			}
			if (ButtonSelect > 1) {
				ButtonSelect = 0;
			}
			if (ButtonSelect < 0) {
				ButtonSelect = 1;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return)) {
				switch (ButtonSelect) {
				case 0:
					start = true;
					sc = 0;
					score.setString("Score: " + toString(sc));

					break;
				case 1:
					quitGame = true;
					break;
				}
			}
			switch (ButtonSelect) {
			case 0:
				play.setFillColor(sf::Color::Red);
				quit.setFillColor(sf::Color::White);
				break;
			case 1:
				quit.setFillColor(sf::Color::Red);
				play.setFillColor(sf::Color::White);
				break;
			}
		}

	Upkey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
	Downkey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
		start = false;
		PlayerLife = 3;
	}
	//////////////////////////////////////////////////////////////////////////////////
	/////////////Game Over Screen///////////////////////////////////////

	if (GameOver) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B)) {
			GameOver = false;
			start = false;
		}
	}

	////Player's life//////////////////////////////////////////
	if (PlayerLife < 0) {
		PlayerLife = 0;
	}
	//////////////////////////////////////////////////////

	if (start) {
		for (auto a : entities)
			for (auto b : entities)
			{
				if (a->name == "asteroid" && b->name == "bullet")
					if (isCollide(a, b))
					{
						blow.play();
						a->life = false;
						b->life = false;
						if (start && !GameOver) {
							sc += 10;
							score.setString("Score: " + toString(sc));
						}


						Entity *e = new Entity();
						e->settings(sExplosion, a->x, a->y);
						e->name = "explosion";
						entities.push_back(e);


						for (int i = 0; i < 2; i++)
						{
							if (a->R == 15) continue;
							Entity *e = new asteroid();
							e->settings(sRock_small, a->x, a->y, rand() % 360, 15);
							entities.push_back(e);
						}

					}

				if (a->name == "player" && b->name == "asteroid")
					if (isCollide(a, b))
					{
						shipEX.play();
						PlayerLife -= 1;
						Plife.setString("Life: " + toString(PlayerLife));
						b->life = false;

						Entity *e = new Entity();
						e->settings(sExplosion_ship, a->x, a->y);
						e->name = "explosion";
						entities.push_back(e);

						p->settings(sPlayer, W / 2, H / 2, 0, 20);
						p->dx = 0; p->dy = 0;
					}
			}
	}

    if (p->thrust)  p->anim = sPlayer_go;
    else   p->anim = sPlayer;


    for(auto e:entities)
     if (e->name=="explosion")
      if (e->anim.isEnd()) e->life=0;

    if (rand()%150==0)
     {
       asteroid *a = new asteroid();
       a->settings(sRock, 0,rand()%H, rand()%360, 25);
       entities.push_back(a);
     }

    for(auto i=entities.begin();i!=entities.end();)
    {
      Entity *e = *i;

      e->update();
      e->anim.update();

      if (e->life==false) {i=entities.erase(i); delete e;}
      else i++;
    }

   //////draw//////
	if (start && !GameOver) {
		app.clear();
		app.draw(background);
		for (auto i : entities) i->draw(app);
		app.draw(score);
		app.draw(Plife);
		app.draw(lvl);
		app.display();

	}
	//app.draw(background);
	//for (auto i : entities) i->draw(app);
	if (!start && !GameOver) {
		p->settings(sPlayer, W / 2, H / 2, 0, 20);
		p->dx = 0; p->dy = 0;
		app.clear();
		app.draw(title);
		app.draw(play);
		app.draw(quit);
		app.draw(score);
		app.display();
		PlayerLife = 3;
	}
	if (GameOver) {
		app.clear();
		app.draw(gameOver);
		app.draw(again);
		app.draw(score);
		app.display();
		
	}
	if (PlayerLife == 0) {
		GameOver = true;
	}
	if (quitGame) {
		app.close();

	}
    }

    return 0;
}

