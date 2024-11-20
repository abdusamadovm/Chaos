#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>

using namespace sf;
using namespace std;

Vector2f rotatePoint(Vector2f& point, Vector2f& pivot, float angle)
{
    float rad = angle * (M_PI / 180.0);
    float cosA = cos(rad);
    float sinA = sin(rad);

    float xNew = (point.x - pivot.x) * cosA - (point.y - pivot.y) * sinA + pivot.x;
    float yNew = (point.x - pivot.x) * sinA + (point.y - pivot.y) * cosA + pivot.y;

    return Vector2f(xNew, yNew);
}

float distance(const Vector2f& p1, const Vector2f& p2)
{
    return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}

Color getColor(const Vector2f& point, const Vector2f& centroid)
{
    float dist = distance(point, centroid);
    float maxDistance = 90;
    float ratio = dist / maxDistance;

    int r = 255 * ratio;
    int g = (sin(ratio * M_PI) + 1) * 100;
    int b = 255 * (1 - ratio);

    return Color(r, g, b);
}

int main()
{
    VideoMode vm(700, 700);
    RenderWindow window(vm, "Chaos Game!!", Style::Default);
    window.setFramerateLimit(60);

    vector<Vector2f> vertices;
    vector<Vector2f> points;
    float angle = 0.7;
    bool rotate = false;
    int spawnSpeed = 2;

    Font font;
    if (!font.loadFromFile("Arial.ttf"))
    {
        cerr << "Error: Font file not found!" << endl;
        return -1;
    }

    Text text, speedText;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(Color::Yellow);
    text.setPosition(10, 40);

    speedText.setFont(font);
    speedText.setCharacterSize(24);
    speedText.setFillColor(Color::White);
    speedText.setPosition(10, 10);

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
                spawnSpeed += event.mouseWheel.delta;
                spawnSpeed = max(-static_cast<int>(points.size()), spawnSpeed);
                cout << "Current spawn rate: " << spawnSpeed << endl;
            }

            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Right)
                {
                    rotate = !rotate;
                    if (rotate) { angle = -angle; }
                }

                if (event.mouseButton.button == Mouse::Left)
                {
                    cout << "Left x: " << event.mouseButton.x << ", y: " << event.mouseButton.y << endl;

                    if (vertices.size() < 3)
                    {
                        vertices.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));
                    }
                    else if (points.empty())
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

        if (vertices.size() == 3 && rotate)
        {
            Vector2f centroid((vertices[0].x + vertices[1].x + vertices[2].x) / 3, (vertices[0].y + vertices[1].y + vertices[2].y) / 3);

            for (Vector2f& vertex : vertices)
            {
                vertex = rotatePoint(vertex, centroid, angle);
            }

            for (Vector2f& point : points)
            {
                point = rotatePoint(point, centroid, angle);
            }
        }

        if (!points.empty() && spawnSpeed > 0)
        {
            for (int i = 0; i < spawnSpeed; ++i)
            {
                int randV = rand() % 3;
                Vector2f midPoint((points.back().x + vertices[randV].x) / 2, (points.back().y + vertices[randV].y) / 2);
                points.push_back(midPoint);
            }
        }
        else if (spawnSpeed < 0)
        {
            for (int i = 0; i < -spawnSpeed; ++i)
            {
                if (!points.empty()) { points.pop_back(); }
                else { break; }
            }
        }

        if (vertices.size() < 3) { text.setString("Choose 3 points for your triangle"); }
        else if (points.empty()) { text.setString("Choose a 4th point to start"); }
        else { text.setString(""); }

        speedText.setString("Speed: " + to_string(spawnSpeed));

        window.clear();

        for (const Vector2f& vertex : vertices)
        {
            RectangleShape rect(Vector2f(10, 10));
            rect.setPosition(vertex);
            rect.setFillColor(Color::Blue);
            window.draw(rect);
        }

        if (vertices.size() == 3)
        {
            Vector2f centroid(
                (vertices[0].x + vertices[1].x + vertices[2].x) / 3,
                (vertices[0].y + vertices[1].y + vertices[2].y) / 3
            );

            for (const Vector2f& point : points)
            {
                CircleShape circ(1);
                circ.setPosition(point);
                circ.setFillColor(getColor(point, centroid));
                window.draw(circ);
            }
        }

        window.draw(text);
        window.draw(speedText);

        window.display();
    }

    return 0;
}
