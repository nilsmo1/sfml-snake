#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>

struct Point { int x; int y;};

typedef std::vector<Point> Snake;
typedef std::vector<Point> Fruit;

const int ROWS      = 600;
const int COLS      = 600;
const int GRID_ROWS =  20;
const int GRID_COLS =  20;
const int FRAMERATE =  15;

const float BOX_SIZE    = (float)ROWS/GRID_ROWS;

void render_snake(Snake& snake, sf::RenderWindow& window) {
    sf::RectangleShape rect;
    int size = snake.size();
    for (int i = size - 1; i >= 0; --i) {
        Point s = snake[i];
        rect.setSize(sf::Vector2f(BOX_SIZE, BOX_SIZE));
        rect.setPosition(s.x * BOX_SIZE, s.y * BOX_SIZE);
        rect.setFillColor(sf::Color(255, 0, 255 * (1 - i/(float)size)));
        window.draw(rect);
    }
}
void render_fruits(Fruit& fruits, sf::RenderWindow& window) {
    sf::RectangleShape rect;
    for (auto f : fruits) {
        rect.setSize(sf::Vector2f(BOX_SIZE, BOX_SIZE));
        rect.setPosition(f.x * BOX_SIZE, f.y * BOX_SIZE);
        rect.setFillColor(sf::Color(255, 0, 255));
        window.draw(rect);
    }
}
Point gen_fruit() {
    int x = rand() % GRID_ROWS;
    int y = rand() % GRID_ROWS;
    return { x, y };
}
void display_points() {}
void game_over() {}
bool in_bounds(Point p, char dx, char dy) { return (p.x + dx >= 0 && p.y + dy >= 0 && p.x + dx < GRID_ROWS && p.y + dy < GRID_COLS); }

int main() {
    sf::RenderWindow window(sf::VideoMode(ROWS, COLS), "Snake");
    window.setFramerateLimit(FRAMERATE);

    char dx{1}, dy{0};
    Snake snake = {{0,0}};
    Fruit fruits = {};
    int iteration{0};

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Q) { window.close(); }
                if (event.key.code == sf::Keyboard::Left)  { dx = -1; dy =  0; }
                if (event.key.code == sf::Keyboard::Right) { dx =  1; dy =  0; }
                if (event.key.code == sf::Keyboard::Up)    { dx =  0; dy = -1; }
                if (event.key.code == sf::Keyboard::Down)  { dx =  0; dy =  1; }
            }
        }
        snake.insert(snake.begin(), { snake.front().x, snake.front().y });
        snake.front().x += dx;
        snake.front().x %= GRID_COLS;
        if (snake.front().x < 0) snake.front().x = GRID_COLS - 1;
        snake.front().y += dy;
        snake.front().y %= GRID_ROWS;
        if (snake.front().y < 0) snake.front().y = GRID_ROWS - 1;

        window.clear(sf::Color::Black);
        bool ate_fruit = false;
        if (!fruits.size() || iteration % 50 == 0) { fruits.push_back(gen_fruit()); }
        render_fruits(fruits, window);
        for (long unsigned int i=0; i<fruits.size(); ++i) {
            Point f = fruits[i];
            if (f.x == snake.front().x && f.y == snake.front().y) {
                fruits.erase(fruits.begin() + i);
                ate_fruit = true;
            }
        }
        if (!ate_fruit) snake.pop_back();
        Point head = snake.front();
        int snake_size = snake.size();
        bool crash = false;
        for (int i = 1; i < snake_size; i++) {
            Point part = snake[i];
            if (head.x == part.x && head.y == part.y) {
                crash = true;
                break;
            }
        }
        if (crash) {
            while (snake_size-- > 1)
                snake.pop_back();
        }
        render_snake(snake, window);
        window.display();
        iteration++;
    }
    return 0;
}
