#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <cmath>

using namespace sf;
using namespace std;

Vector2f rotatePoint(const Vector2f& point, const Vector2f& pivot, float angle) {
    float rad = angle * (M_PI / 180.0);
    float cosA = cos(rad);
    float sinA = sin(rad);

    float xNew = (point.x - pivot.x) * cosA - (point.y - pivot.y) * sinA + pivot.x;
    float yNew = (point.x - pivot.x) * sinA + (point.y - pivot.y) * cosA + pivot.y;

    return Vector2f(xNew, yNew);
}


float distance(const Vector2f& p1, const Vector2f& p2) {
    return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}


Color getColorBasedOnDistance(const Vector2f& point, const Vector2f& centroid) {
    float dist = distance(point, centroid);
    float maxDistance = 100.0f;
    float ratio = dist / maxDistance;

    int r = 255 * ratio;
    int g = (sin(ratio * M_PI) + 1) * 127.5f;
    int b = 255 * (1 - ratio);

    return Color(r, g, b);
}

int main()
{
    srand(static_cast<unsigned>(time(0)));

    VideoMode vm(1700, 800);
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

    float angle = 1.0;
    bool rotate = false;
    int spawnSpeed = 2;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
	    if (event.type == sf::Event::MouseWheelMoved)
	    {
		if (spawnSpeed + event.mouseWheel.delta >= 0) { spawnSpeed += event.mouseWheel.delta; }
		else { spawnSpeed = 0; }
		cout << "Current spawn rate: " << spawnSpeed << endl;
	    }
            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Right)
                {
                    rotate = !rotate;
		    if (rotate) { cout << "Rotation started" << endl; }
		    else { cout << "Rotation ended" << endl; }
                }

                if (event.mouseButton.button == Mouse::Left)
                {
                    cout << "Left mouse " << "x: " << event.mouseButton.x << "y: " << event.mouseButton.y << endl;

                    if (vertices.size() < 3)
                    {
                        vertices.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));
                    }
                    else if (points.size() == 0)
                    {
                        points.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));
                    }
                }
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        if (vertices.size() == 3 and rotate) {
            float centerX = (vertices[0].x + vertices[1].x + vertices[2].x) / 3;
            float centerY = (vertices[0].y + vertices[1].y + vertices[2].y) / 3;
            Vector2f centroid(centerX, centerY);
            for (int i = 0; i < 3; i++) {
                vertices[i] = rotatePoint(vertices[i], centroid, angle);
            }

            for (unsigned long int i = 0; i < points.size(); i++) {
                points[i] = rotatePoint(points[i], centroid, angle);
            }
        }

        if (points.size() > 0 && spawnSpeed != 0)
        {
            for (int i = 0; i <= spawnSpeed; i++)
            {
                int randV = rand() % 3;
                float midPointX = (points.back().x + vertices[randV].x) / 2;
                float midPointY = (points.back().y + vertices[randV].y) / 2;
                points.push_back(Vector2f(midPointX, midPointY));
            }
        }

        if (angle >= 360.0) angle = 0.0;

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
            RectangleShape rect(Vector2f(10, 10));
            rect.setPosition(vertices[i]);
            rect.setFillColor(Color::Blue);
            window.draw(rect);
        }

        if (vertices.size() == 3) {
            float centerX = (vertices[0].x + vertices[1].x + vertices[2].x) / 3;
            float centerY = (vertices[0].y + vertices[1].y + vertices[2].y) / 3;
            Vector2f centroid(centerX, centerY);
            for (long unsigned int i = 0; i < points.size(); i++)
            {
                CircleShape circ(1);
                circ.setPosition(points[i]);
                circ.setFillColor(getColorBasedOnDistance(points[i], centroid));
                window.draw(circ);
            }
        }

        window.display();
    }

    return 0;
}
