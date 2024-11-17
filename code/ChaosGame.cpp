// Include important C++ libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>

//Make the code easier to type with "using namespace"
using namespace sf;
using namespace std;

int main()
{
	srand(static_cast<unsigned>(time(0)));
	// Create a video mode object
	VideoMode vm(1920, 1080);
	// Create and open a window for the game
	RenderWindow window(vm, "Chaos Game!!", Style::Default);

	vector<Vector2f> vertices;
	vector<Vector2f> points;

	Font font;
	font.loadFromFile("Arial.ttf");
	if (!font.loadFromFile("Arial.ttf"))
	{
		cout << "file not found" << endl;
	}

	Text text;
	text.setCharacterSize(24);
	text.setFillColor(Color::Yellow);
	text.setFont(font);

	while (window.isOpen())
	{
		/*
		****************************************
		Handle the players input
		****************************************
		*/
		Event event;
		while (window.pollEvent(event))
		{
		    if (event.type == Event::Closed)
		    {
					// Quit the game when the window is closed
					window.close();
		    }
		    if (event.type == sf::Event::MouseButtonPressed)
		    {
			if (event.mouseButton.button == sf::Mouse::Left)
			{
			    std::cout << "the left button was pressed" << std::endl;
			    std::cout << "mouse x: " << event.mouseButton.x << std::endl;
			    std::cout << "mouse y: " << event.mouseButton.y << std::endl;

			    if(vertices.size() < 3)
			    {
				vertices.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));
			    }
			    else if(points.size() == 0)
			    {
				points.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));
				cout << "4th click pushed to vector!" << endl;
				///fourth click
				///push back to points vector
			    }
			}
		    }
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		/*
		****************************************
		Update
		****************************************
		*/
		if (points.size() > 0)
		{
			for (int i = 0; i < 200; i++)
			{
				///generate more point(s)
				///select random vertex
				int randV = rand() % 3;
				///calculate midpoint between random vertex and the last point in the vector
				float midPointX = (points.back().x + vertices[randV].x) / 2;
				float midPointY = (points.back().y + vertices[randV].y) / 2;
				///push back the newly generated coord.
				points.push_back(Vector2f(midPointX, midPointY));
			}
		}

		/*
		****************************************
		Draw
		****************************************
		*/
		window.clear();
		if (vertices.size() < 3)
		{
			text.setString("Choose 3 points for your triangle");
			window.draw(text);
		}
		else if (points.size() == 0)
		{
			text.setString("Choose 4th point to start");
			window.draw(text);
		}

		for (long unsigned int i = 0; i < vertices.size(); i++)
		{
		    RectangleShape rect(Vector2f(10,10));
		    rect.setPosition(Vector2f(vertices[i].x, vertices[i].y));
		    rect.setFillColor(Color::Blue);
		    window.draw(rect);
		}
		for (long unsigned int i = 0; i < points.size(); i++)
		{
			CircleShape circ(1);
			circ.setPosition(Vector2f(points[i].x, points[i].y));
			circ.setFillColor(Color::White);
			window.draw(circ);
		}
		window.display();
	}
}
