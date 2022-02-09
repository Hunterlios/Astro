#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<SFML/System.hpp>
#include<iostream>
#include<vector>
#include<ctime>
#include<cstdlib>
#include<string>
#include<iomanip>
#include<sstream>
#include<fstream>
using namespace std;
using namespace sf;


//HP TEXTURES LOAD
void hpLoad(RectangleShape hpShapes[], Texture& hpIcnFull, Texture& hpIcnEmpty, RectangleShape& hpShp, Vector2f& hpPos)
{
	float j = 40;
	hpIcnFull.loadFromFile("textures/hpFull.png");
	hpIcnEmpty.loadFromFile("textures/hpEmpty.png");
	hpIcnFull.setSmooth(true);
	hpIcnEmpty.setSmooth(true);
	hpShp.setSize(Vector2f(46.0f, 46.0f));
	hpShp.setPosition(460, 620);
	for (int i = 0; i < 3; i++)
	{
		if (i == 0)
		{
			hpShp.setTexture(&hpIcnFull);
		}
		hpShapes[i] = hpShp;
		hpPos = hpShp.getPosition();
		hpPos.x += j;
		hpShp.setPosition(hpPos);
	}
}

//MENU FUNCTION
void showMenu(RenderWindow& window, Text menu[], int n, Sprite bgSprite)
{
	window.draw(bgSprite);
	for (size_t i = 0; i < n; i++)
	{
		window.draw(menu[i]);
	}
	
}

//HOW TO PLAY MENU FUNCTION
void instruction(RenderWindow& window, Text howToMove[], Sprite bgSprite)
{
	window.draw(bgSprite);
	for (size_t i = 0; i < 2; i++)
	{
		window.draw(howToMove[i]);
	}
	
}

//GAME OVER FUNCTION
void gameEnd(vector<RectangleShape>& enemies, RenderWindow& window, Text end, Text repeat[], int points, double distance, Font font, Sprite bgSprite)
{
	enemies.clear();
	window.draw(bgSprite);
	int highestPoints = 0;
	ofstream highscoreWrite;
	ifstream highscoreRead;
	stringstream stream2;
	stream2 << fixed << setprecision(1) << distance;
	string distPass = stream2.str();
	Text highscoreText;
	Text endPoints;
	Text distancePass;
	highscoreText.setFont(font);
	highscoreText.setFillColor(Color::White);
	highscoreText.setPosition(window.getSize().x / 2 - 90.0f, window.getSize().y / 2 - 30.0f);
	highscoreText.setCharacterSize(30);

	distancePass.setFont(font);
	distancePass.setFillColor(Color::White);
	distancePass.setPosition(window.getSize().x / 2 - 150.0f, window.getSize().y / 2 + 10.0f);
	distancePass.setCharacterSize(30);

	endPoints.setFont(font);
	endPoints.setFillColor(Color::White);
	endPoints.setPosition(window.getSize().x / 2 - 70.0f, window.getSize().y / 2 - 70.0f);
	endPoints.setCharacterSize(30);
	endPoints.setString("Points: " + to_string(points));

	highscoreRead.open("highscore.txt");
	highscoreRead >> highestPoints;
	highscoreRead.close();

	if (points > highestPoints)
	{
		highestPoints = points;
		highscoreWrite.open("highscore.txt");
		highscoreWrite << points << "\n";
		highscoreWrite.close();
	}
	highscoreText.setString("Highscore: " + to_string(highestPoints));
	distancePass.setString("Distance passed: " + distPass);
	window.draw(highscoreText);
	window.draw(endPoints);
	window.draw(distancePass);
	for (size_t i = 0; i < 2; i++)
	{
		window.draw(repeat[i]);
	}
	window.draw(end);

}


//GAME RESTART FUNCTION
void gameRestart(Clock& speedTime, Clock& enemyTimer, Clock& shootTimer, int& points, double& distancePassed, float& movementSpeed, int& hp, int& k, float& dt, double& currentDistance, float& bgSpeed, float& bgY, vector<RectangleShape>& enemies, RectangleShape hpShapes[], Texture& hpIcnFull, Texture& hpIcnEmpty, RectangleShape& hpShp, Vector2f& hpPos, RectangleShape &player, RenderWindow &window)
{
	points = 0;
	distancePassed = 0.0f;
	movementSpeed = 250.0f;
	hp = 3;
	k = 2;
	dt = 0.0f;
	currentDistance = 0.1f;
	bgSpeed = 250.0f;
	bgY = 0.0f;
	enemies.clear();
	player.setPosition(Vector2f(window.getSize().x / 2 - 20.0f, window.getSize().y - player.getSize().y * 2 - 50.0f));
	hpLoad(hpShapes, hpIcnFull, hpIcnEmpty, hpShp, hpPos);
	enemyTimer.restart();
	shootTimer.restart();
	speedTime.restart();
}



int main()
{
	//LOAD FONT
	Font font;
	if (!font.loadFromFile("fonts/Roboto-Medium.ttf"))
	{
		cout << "error" << endl;
	}

	//WINDOW SETTINGS
	RenderWindow window;
	window.create(VideoMode(600, 700), "Astro", Style::Titlebar | Style::Close);
	window.setKeyRepeatEnabled(true);
	window.setVerticalSyncEnabled(true);

	//BACKGROUND
	float bgSpeed = 250.0f;
	float bgY = 0.0f;
	Texture backgroundTexture, menuBackGround;
	backgroundTexture.loadFromFile("textures/backGroundSprite.png");
	menuBackGround.loadFromFile("textures/menuBackgroundSprite.png");
	backgroundTexture.setRepeated(true);
	Sprite bgSprite, menuBgSprite;
	bgSprite.setTexture(backgroundTexture);
	menuBgSprite.setTexture(menuBackGround);
	bgSprite.setPosition(0, 0);
	menuBgSprite.setPosition(0, 0);
	bgSprite.setTextureRect(IntRect(0, bgY, 600, 700));

	//MENU
	bool menuDisplayed = true;
	bool howToPlay = false;
	Text menu[4];
	for (size_t i = 0; i < 4; i++)
	{
		menu[i].setFont(font);
		menu[i].setFillColor(Color::White);
	}
	menu[0].setPosition(window.getSize().x / 2 - 90.0f, window.getSize().y / 2 - 100.0f);
	menu[1].setPosition(window.getSize().x / 2 - 40.0f, window.getSize().y / 2 + 20.0f);
	menu[2].setPosition(window.getSize().x / 2 - 125.0f, window.getSize().y / 2 + 160.0f);
	menu[3].setPosition(window.getSize().x / 2 - 155.0f, window.getSize().y / 2 + 240.0f);
	menu[0].setCharacterSize(80);
	menu[1].setCharacterSize(40);
	menu[2].setCharacterSize(40);
	menu[3].setCharacterSize(25);
	menu[0].setString("PLAY");
	menu[1].setString("EXIT");
	menu[2].setString("HOW TO PLAY");
	menu[3].setString(" PRESS UP AND DOWN KEYS\nTO CHANGE MUSIC VOLUME");

	//HOW TO PLAY
	Text howToMove[2];
	for (size_t i = 0; i < 2; i++)
	{
		howToMove[i].setFont(font);
		howToMove[i].setFillColor(Color::White);
	}
	howToMove[0].setCharacterSize(30);
	howToMove[0].setPosition(window.getSize().x / 2 - 260.0f, window.getSize().y / 2 - 200.0f);
	howToMove[0].setString("Use A to move left and D to move right.\n\t\t\t      Use SPACE to shoot.");
	howToMove[1].setCharacterSize(40);
	howToMove[1].setPosition(window.getSize().x / 2 - 110.0f, window.getSize().y / 2 - 20.0f);
	howToMove[1].setString("MAIN MENU");

	//GAME OVER
	bool gameOver = false;
	bool repeatGame = false;
	Text endText;
	Text repeat[2];
	endText.setFont(font);
	endText.setFillColor(Color::White);
	endText.setPosition(window.getSize().x / 2 - 170.0f, window.getSize().y / 2 - 200.0f);
	endText.setCharacterSize(60);
	endText.setString("GAME OVER");
	for (size_t i = 0; i < 2; i++)
	{
		repeat[i].setFont(font);
		repeat[i].setFillColor(Color::White);
		repeat[i].setCharacterSize(40);
	}
	repeat[0].setPosition(window.getSize().x / 2 - 120.0f, window.getSize().y / 2 + 100.0f);
	repeat[1].setPosition(window.getSize().x / 2 - 50.0f, window.getSize().y / 2 + 160.0f);
	repeat[0].setString("PLAY AGAIN");
	repeat[1].setString("EXIT");

	//ICON
	Image icon;
	icon.loadFromFile("textures/astroIcon.png");
	window.setIcon(256, 256, icon.getPixelsPtr());

	//PLAYER
	RectangleShape player;
	Texture playerTexture;
	playerTexture.loadFromFile("textures/rakietaSprite.png");
	player.setTexture(&playerTexture);
	playerTexture.setSmooth(true);
	player.setSize(Vector2f(40.0f, 80.0f));
	player.setPosition(Vector2f(window.getSize().x / 2 - 20.0f, window.getSize().y - player.getSize().y * 2 - 50.0f));
	Vector2f playerCenter;
	int hp = 3;
	int k = 2;

	//BULLETS
	RectangleShape bullet;
	Texture bulletTexture;
	bulletTexture.loadFromFile("textures/laser.png");
	bullet.setTexture(&bulletTexture);
	bullet.setSize(Vector2f(5.0f, 20.0f));
	vector<RectangleShape> bullets;
	Clock shootTimer;
	int points = 0;

	//ENEMIES
	vector<RectangleShape> enemies;
	RectangleShape enemy;
	Texture enemyTexture[3];
	enemyTexture[0].loadFromFile("textures/asteroida1.png");
	enemyTexture[1].loadFromFile("textures/asteroida2.png");
	enemyTexture[2].loadFromFile("textures/asteroida3.png");
	for (size_t i = 0; i < 3; i++)
	{
		enemyTexture[i].setSmooth(true);
	}
	enemy.setSize(Vector2f(55.0f, 55.0f));
	Clock enemyTimer;
	float movementSpeed = 250.0f;

	//HP ICONS
	RectangleShape hpShapes[3];
	RectangleShape* wsk = hpShapes;
	Texture hpIcnFull, hpIcnEmpty;
	RectangleShape hpShp;
	Vector2f hpPos;
	hpLoad(wsk, hpIcnFull, hpIcnEmpty, hpShp, hpPos);

	//WINDOW BOUNDS
	FloatRect windowBounds({ 0.0f,0.0f }, window.getDefaultView().getSize());
	RectangleShape bottomSide(Vector2f(600, 1));
	RectangleShape topSide(Vector2f(600, 1));
	bottomSide.setPosition(1, 700);

	//DT
	srand(time(NULL));
	Clock dtClock;
	Time time;
	float dt = 0.0f;

	//POINTS COUNTER
	Text counter("", font, 20);
	counter.setFillColor(Color::White);
	counter.setPosition(20, 20);

	//DISTANCE
	Text distance("", font, 16);
	distance.setFillColor(Color::White);
	distance.setPosition(20, 50);
	double distancePassed = 0.0f;
	Clock speedTime;
	double currentDistance = 0.1f;

	//SOUNDS
	Music music;
	music.openFromFile("sounds/backgroundMusic.wav");
	music.setVolume(50);
	music.play();
	music.setLoop(true);

	SoundBuffer buffer[3];
	buffer[0].loadFromFile("sounds/laserSound.wav");
	buffer[1].loadFromFile("sounds/enemyHit.wav");
	buffer[2].loadFromFile("sounds/playerHit.wav");
	Sound laserSound, enemyHit, playerHit;
	laserSound.setBuffer(buffer[0]);
	laserSound.setVolume(50);
	enemyHit.setBuffer(buffer[1]);
	enemyHit.setVolume(50);
	playerHit.setBuffer(buffer[2]);
	playerHit.setVolume(50);


	/*MAIN LOOP*/
	while (window.isOpen())
	{
		Event evnt;
		while (window.pollEvent(evnt))
		{

			switch (evnt.type)
			{
			case Event::Closed:
				window.close();
			case Event::EventType::KeyPressed:
				if (Keyboard::isKeyPressed(Keyboard::Up) && music.getVolume() < 100)
				{
					music.setVolume(music.getVolume() + 10);
					laserSound.setVolume(laserSound.getVolume() + 10);
					enemyHit.setVolume(enemyHit.getVolume() + 10);
					playerHit.setVolume(playerHit.getVolume() + 10);
				}
				if (Keyboard::isKeyPressed(Keyboard::Down) && music.getVolume() >= 10)
				{
					music.setVolume(music.getVolume() - 10);
					laserSound.setVolume(laserSound.getVolume() - 10);
					enemyHit.setVolume(enemyHit.getVolume() - 10);
					playerHit.setVolume(playerHit.getVolume() - 10);
				}
			}
			

		}
		window.clear();

		//MAIN MENU WINDOW
		if (menuDisplayed)
		{
			enemyTimer.restart();
			shootTimer.restart();
			speedTime.restart();
			showMenu(window, menu, 4, menuBgSprite);
			if (Mouse::isButtonPressed(Mouse::Button::Left))
			{
				auto mousePosition = Mouse::getPosition(window);
				auto translatedPosition = window.mapPixelToCoords(mousePosition);
				if (menu[0].getGlobalBounds().contains(translatedPosition))
				{
					menuDisplayed = false;
				}
				if (menu[1].getGlobalBounds().contains(translatedPosition))
				{
					window.close();
				}
				if (menu[2].getGlobalBounds().contains(translatedPosition))
				{
					howToPlay = true;
					menuDisplayed = false;
				}
			}
		}

		//GAME OVER WINDOW
		if (gameOver)
		{
			
			gameEnd(enemies, window, endText, repeat, points, distancePassed, font, bgSprite);
			if (Mouse::isButtonPressed(Mouse::Button::Left))
			{
				auto mousePosition = Mouse::getPosition(window);
				auto translatedPosition = window.mapPixelToCoords(mousePosition);
				if (repeat[0].getGlobalBounds().contains(translatedPosition))
				{
					gameRestart(speedTime, enemyTimer, shootTimer, points, distancePassed, movementSpeed, hp, k, dt, currentDistance, bgSpeed, bgY, enemies, wsk, hpIcnFull, hpIcnEmpty, hpShp, hpPos, player, window);
					gameOver = false;
					menuDisplayed = true;
				}
				if (repeat[1].getGlobalBounds().contains(translatedPosition))
				{
					window.close();
				}
			}
		}

		//HOW TO PLAY WINDOW
		if (howToPlay)
		{
			instruction(window, howToMove, bgSprite);
			if (Mouse::isButtonPressed(Mouse::Button::Left))
			{
				auto mousePosition = Mouse::getPosition(window);
				auto translatedPosition = window.mapPixelToCoords(mousePosition);
				if (howToMove[1].getGlobalBounds().contains(translatedPosition))
				{
					howToPlay = false;
					menuDisplayed = true;
				}
			}
		}

		//MAIN GAME
		if (!menuDisplayed && !gameOver && !howToPlay)
		{
			time = dtClock.restart();
			dt = time.asSeconds();

			/*UPDATE*/
			playerCenter = Vector2f(player.getPosition().x + player.getSize().x / 2, player.getPosition().y + player.getScale().y / 2);
			

			//PLAYER MOVEMENT
			float newPos = player.getPosition().x;
			Vector2f position(newPos, 0.0f);
			position.x = max(position.x, windowBounds.left + 15.0f);
			position.x = min(position.x, windowBounds.left - 15.0f + windowBounds.width - player.getSize().x);
			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.setPosition(Vector2f(position.x -= 500.0f * dt, player.getPosition().y));
			}
			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.setPosition(Vector2f(position.x += 500.0f * dt, player.getPosition().y));
			}

			//SHOOTING
			Time elapsedShootTimer = shootTimer.getElapsedTime();
			if (Keyboard::isKeyPressed(Keyboard::Space) && elapsedShootTimer.asSeconds() >= 0.35f)
			{
				laserSound.play();
				bullet.setPosition(playerCenter);
				bullets.push_back(RectangleShape(bullet));
				shootTimer.restart();
			}
			for (size_t i = 0; i < bullets.size(); i++)
			{
				bullets[i].move(0.0f, -1250.0f * dt);
				if (bullets[i].getPosition().y < 0)
				{
					bullets.erase(bullets.begin() + i);
				}
			}			

			//ENEMIES
			Time elapsedEnemyTimer = enemyTimer.getElapsedTime();

			if (movementSpeed < 460.0f)
			{
				if (elapsedEnemyTimer.asSeconds() >= 0.45f)
				{
					enemy.setPosition((rand() % int(window.getSize().x - enemy.getSize().x)), -40.0f);
					enemy.setTexture(&enemyTexture[rand() % 3]);
					enemies.push_back(RectangleShape(enemy));
					enemyTimer.restart();
				}
			}
			else
			{
				if (elapsedEnemyTimer.asSeconds() >= 0.3f)
				{
					enemy.setPosition((rand() % int(window.getSize().x - enemy.getSize().x)), -40.0f);
					enemy.setTexture(&enemyTexture[rand() % 3]);
					enemies.push_back(RectangleShape(enemy));
					enemyTimer.restart();
				}
			}

			Time elapsedSpeedTimer = speedTime.getElapsedTime();

			if (elapsedSpeedTimer.asSeconds() >= 8.0f)
			{
				if (movementSpeed < 550.0f)
				{
					movementSpeed += 30.0f;
					bgSpeed += 50.0f;
					currentDistance += 0.05f;

				}
				else
				{
					movementSpeed = 550.0f;
					bgSpeed = 750.0f;
					currentDistance = 0.6f;
				}
				speedTime.restart();
			}


			for (size_t i = 0; i < enemies.size(); i++)
			{
				enemies[i].move(0.0f, movementSpeed * dt);
				if (enemies[i].getPosition().y > window.getSize().y)
				{
					enemies.erase(enemies.begin() + i);
				}
			}

			//COLLISION
			for (size_t i = 0; i < bullets.size(); i++)
			{
				for (size_t j = 0; j < enemies.size(); j++)
				{
					if (bullets[i].getGlobalBounds().intersects(enemies[j].getGlobalBounds()))
					{
						enemyHit.play();
						bullets.erase(bullets.begin() + i);
						enemies.erase(enemies.begin() + j);
						points++;
						break;
					}
				}
			}

			for (size_t i = 0; i < enemies.size(); i++)
			{
				if (enemies[i].getGlobalBounds().intersects(player.getGlobalBounds()))
				{
					playerHit.play();
					hp--;
					hpShapes[k].setTexture(&hpIcnEmpty);
					k--;
					enemies.erase(enemies.begin() + i);
				}
			}

			//COUNTER
			counter.setString("Points: " + to_string(points));
			if (hp == 0)
			{
				enemies.clear();
				gameOver = true;
			}

			//DISTANCE
			stringstream stream;
			stream << fixed << setprecision(1) << distancePassed;
			string dist = stream.str();
			distancePassed += currentDistance;
			distance.setString("Distance: " + dist);


			//MOVING BACKGROUND
			if (bgY < 700)
			{
				bgY -= bgSpeed * dt;
			}
			else
			{
				bgY = 0;
			}
			bgSprite.setTextureRect(IntRect(0, bgY, 600, 700));

			/*DRAW*/

			window.draw(bgSprite);
			window.draw(player);
			for (size_t i = 0; i < enemies.size(); i++)
			{
				window.draw(enemies[i]);
			}
			for (size_t i = 0; i < bullets.size(); i++)
			{
				window.draw(bullets[i]);
			}
			window.draw(counter);
			window.draw(distance);
			for (int i = 0; i < 3; i++)
			{
				window.draw(hpShapes[i]);
			}

		}

		window.display();
	}
	return 0;
}