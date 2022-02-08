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

void showMenu(RenderWindow& window, Text menu[], int n)
{
	for (size_t i = 0; i < n; i++)
	{
		window.draw(menu[i]);
	}
}

void gameEnd(RenderWindow& window, Text end, Text repeat[], int points, double distance, Font font)
{
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
	highscoreText.setPosition(window.getSize().x / 2 - 160.0f, window.getSize().y / 2 - 30.0f);
	highscoreText.setCharacterSize(30);

	distancePass.setFont(font);
	distancePass.setFillColor(Color::White);
	distancePass.setPosition(window.getSize().x / 2 - 160.0f, window.getSize().y / 2 + 10.0f);
	distancePass.setCharacterSize(30);

	endPoints.setFont(font);
	endPoints.setFillColor(Color::White);
	endPoints.setPosition(window.getSize().x / 2 - 160.0f, window.getSize().y / 2 - 70.0f);
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


void gameRestart(int& shootTimer, int& points, double& distancePassed, int& enemyTimer, float& movementSpeed, int& hp, int& k, float& dt, int& speedTime, double& currentDistance, float& bgSpeed, float& bgY, vector<RectangleShape>& enemies, RectangleShape hpShapes[], Texture& hpIcnFull, Texture& hpIcnEmpty, RectangleShape& hpShp, Vector2f& hpPos)
{
	shootTimer = 0;
	points = 0;
	distancePassed = 0.0f;
	enemyTimer = 0;
	movementSpeed = 250.0f;
	hp = 3;
	k = 2;
	dt = 0.0f;
	speedTime = 0;
	currentDistance = 0.1f;
	bgSpeed = 250.0f;
	bgY = 0.0f;
	enemies.clear();
	hpLoad(hpShapes, hpIcnFull, hpIcnEmpty, hpShp, hpPos);
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
	Texture backgroundTexture;
	backgroundTexture.loadFromFile("textures/backGroundSprite.png");
	backgroundTexture.setRepeated(true);
	Sprite bgSprite;
	bgSprite.setTexture(backgroundTexture);
	bgSprite.setPosition(0, 0);
	bgSprite.setTextureRect(IntRect(0, bgY, 600, 700));

	//MENU
	bool menuDisplayed = true;
	Text menu[2];
	menu[0].setFont(font);
	menu[1].setFont(font);
	menu[0].setFillColor(Color::White);
	menu[1].setFillColor(Color::White);
	menu[0].setPosition(window.getSize().x / 2 - 90.0f, window.getSize().y / 2 - 100.0f);
	menu[1].setPosition(window.getSize().x / 2 - 40.0f, window.getSize().y / 2 + 20.0f);
	menu[0].setCharacterSize(80);
	menu[1].setCharacterSize(40);
	menu[0].setString("PLAY");
	menu[1].setString("EXIT");

	//GAME OVER
	bool gameOver = false;
	bool repeatGame = false;
	Text endText;
	Text repeat[2];
	endText.setFont(font);
	endText.setFillColor(Color::White);
	endText.setPosition(window.getSize().x / 2 - 160.0f, window.getSize().y / 2 - 200.0f);
	endText.setCharacterSize(60);
	endText.setString("GAME OVER");
	repeat[0].setFont(font);
	repeat[1].setFont(font);
	repeat[0].setFillColor(Color::White);
	repeat[1].setFillColor(Color::White);
	repeat[0].setPosition(window.getSize().x / 2 - 160.0f, window.getSize().y / 2 + 100.0f);
	repeat[1].setPosition(window.getSize().x / 2 - 160.0f, window.getSize().y / 2 + 160.0f);
	repeat[0].setCharacterSize(40);
	repeat[1].setCharacterSize(40);
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
	player.setPosition(Vector2f(window.getSize().x / 2 - player.getSize().x / 2, window.getSize().y - player.getSize().y * 2 - 50.0f));
	Vector2f playerCenter;
	int hp = 3;
	int k = 2;

	//BULLETS
	RectangleShape bullet;
	bullet.setFillColor(Color::Red);
	bullet.setSize(Vector2f(5.0f, 20.0f));
	vector<RectangleShape> bullets;
	bullets.push_back(RectangleShape(bullet));
	int shootTimer = 0;
	int points = 0;

	//ENEMIES
	vector<RectangleShape> enemies;
	RectangleShape enemy;
	Texture enemyTexture;
	enemyTexture.loadFromFile("textures/Asteroida.png");
	enemy.setTexture(&enemyTexture);
	enemyTexture.setSmooth(true);
	enemy.setSize(Vector2f(55.0f, 55.0f));
	enemies.push_back(RectangleShape(enemy));
	int enemyTimer = 0;
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
	int speedTime = 0;
	double currentDistance = 0.1f;

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
			}
		}

		window.clear();
		if (menuDisplayed)
		{
			showMenu(window, menu, 2);
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
			}
		}

		if (gameOver)
		{
			gameEnd(window, endText, repeat, points, distancePassed, font);
			if (Mouse::isButtonPressed(Mouse::Button::Left))
			{
				auto mousePosition = Mouse::getPosition(window);
				auto translatedPosition = window.mapPixelToCoords(mousePosition);
				if (repeat[0].getGlobalBounds().contains(translatedPosition))
				{
					gameRestart(shootTimer, points, distancePassed, enemyTimer, movementSpeed, hp, k, dt, speedTime, currentDistance, bgSpeed, bgY, enemies, wsk, hpIcnFull, hpIcnEmpty, hpShp, hpPos);
					gameOver = false;
					menuDisplayed = true;
				}
				if (repeat[1].getGlobalBounds().contains(translatedPosition))
				{
					window.close();
				}
			}
		}

		if (!menuDisplayed && !gameOver)
		{
			time = dtClock.restart();
			dt = time.asSeconds();

			/*UPDATE*/
			playerCenter = Vector2f(player.getPosition().x + player.getSize().x / 2, player.getPosition().y + player.getScale().y / 2);
			if (shootTimer < 60)
			{
				shootTimer++;
			}

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
			if (Keyboard::isKeyPressed(Keyboard::Space) && shootTimer >= 60)
			{
				bullet.setPosition(playerCenter);
				bullets.push_back(RectangleShape(bullet));
				shootTimer = 0;
			}
			for (size_t i = 0; i < bullets.size(); i++)
			{
				bullets[i].move(0.0f, -1200.0f * dt);
				if (bullets[i].getPosition().y < 0)
				{
					bullets.erase(bullets.begin() + i);
				}
			}

			//ENEMIES
			if (movementSpeed < 490.0f)
			{
				if (enemyTimer < 80)
				{
					enemyTimer++;
				}
				if (enemyTimer >= 80)
				{
					enemy.setPosition((rand() % int(window.getSize().x - enemy.getSize().x)), -40.0f);
					enemies.push_back(RectangleShape(enemy));
					enemyTimer = 0;
				}
			}
			else
			{
				if (enemyTimer < 60)
				{
					enemyTimer++;
				}
				if (enemyTimer >= 60)
				{
					enemy.setPosition((rand() % int(window.getSize().x - enemy.getSize().x)), -40.0f);
					enemies.push_back(RectangleShape(enemy));
					enemyTimer = 0;
				}
			}


			if (speedTime < 1200)
			{
				speedTime++;
			}

			if (speedTime >= 1200)
			{
				if (movementSpeed < 550)
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
				speedTime = 0;
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