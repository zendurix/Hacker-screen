#include <vector>
#include <memory>
#include <random>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>

using namespace std::chrono_literals;


const int LENGTH = 100 /1.2;
const int CHAR_SIZE = 24;
const int HEIGHT = 60/1.2;
const int WIN_LENGTH = LENGTH * CHAR_SIZE;
const int WIN_HEIGHT = HEIGHT * CHAR_SIZE;
const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"; //\{}()!@#$%^&* | +_[]";
const int charactersSIZE = sizeof(characters);

const int ACTV_MAX = 20;
const int OPACITY_DEFAULT_MAX = 255;
const int OPACITY_DEFAULT_MIN = 0;

const int RAND_LIGHTS = 5;
#define STEP_TIME 0.05s

sf::Font font;

thread_local std::mt19937 gen{ std::random_device{}() };

int random(int min, int max)
{// range is <min, max>
	return std::uniform_int_distribution<int>{min, max}(gen);
}

class Letter
{public:
	static int countX, countY;
	short x, y;
	sf::Text letter;
	int opacity;
	int actv;

	Letter()
	{
		opacity = OPACITY_DEFAULT_MIN;
		actv = 0;
		x = countX;
		y = countY;
		countX++;
		if (countX == LENGTH)
		{
			countX = 0;
			countY++;
		}

		letter.setFont(font);
		letter.setCharacterSize(CHAR_SIZE);
		letter.setString(characters[random(0, charactersSIZE - 1)]);

		int xx, yy;
		xx = this->x * CHAR_SIZE;
		yy = this->y * CHAR_SIZE;
		letter.move(xx, yy);

		set_color();
	}

	void set_color()
	{
		if (actv == 0)
			opacity = OPACITY_DEFAULT_MIN;
		else if (actv == ACTV_MAX)
			opacity = OPACITY_DEFAULT_MAX;
		else if (actv == 1)
			letter.setString(characters[random(0, charactersSIZE - 1)]);
		if (actv > 0 )
		{
			actv--;
			opacity -= OPACITY_DEFAULT_MAX / ACTV_MAX;
		}
		letter.setFillColor(sf::Color(0, 255, 0, opacity));
	}
};
int Letter::countX = 0;
int Letter::countY = 0;

sf::RenderWindow window(sf::VideoMode(WIN_LENGTH, WIN_HEIGHT), "hacker");
std::vector <std::vector <std::shared_ptr<Letter>> > letters;

void printScreen();
void init_letters();
void load_font();
void active_random();
void set_activations_states();
void set_colors();

int main()
{
	load_font();	
	init_letters();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type = sf::Event::Closed)
				window.close();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			window.close();

		printScreen();
		for (int i = 0; i < RAND_LIGHTS; i++)
			active_random();

		set_activations_states();
		set_colors();

		std::this_thread::sleep_for(STEP_TIME);
	}
	return 0;
}

void set_colors()
{
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < LENGTH; j++)
			letters[i][j]->set_color();
}

void set_activations_states()
{
	int x, y;
	for (int i = 1; i < HEIGHT; i++)
		for (int j = 0; j < LENGTH; j++)
			if (letters[i-1][j]->actv == ACTV_MAX-1)
				letters[i][j]->actv = ACTV_MAX;
}

void active_random()
{
	letters[random(0, 10)][random(1, LENGTH - 2)]->actv = ACTV_MAX;
}

void printScreen()
{
	window.clear();
	int x, y;
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < LENGTH; j++)
			window.draw(letters[i][j]->letter);
	window.display();
}

void init_letters()
{
	letters.reserve(HEIGHT);
	for (std::vector <std::shared_ptr<Letter> > x : letters)
		x.reserve(LENGTH);

	for (int i = 0; i < HEIGHT; i++)
	{
		std::vector <std::shared_ptr<Letter> > lettersRow;
		for (int j = 0; j < LENGTH; j++)
			lettersRow.push_back(std::make_shared<Letter>());
		letters.push_back(lettersRow);
	}
}

void load_font()
{
	font.loadFromFile("../SFML/Perfect DOS VGA 437.ttf");
}