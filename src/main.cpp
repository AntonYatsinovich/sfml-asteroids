#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>

const sf::Vector2u WINDOW_RESOLUTION(1200, 800);
const sf::Vector2f WINDOW_CENTRE(600.f, 400.f);

enum class Action {
    Thrust,   // W
    TurnLeft, // A
    TurnRight,// D
    Shoot     // Space
};

struct InputConfig {
    sf::Keyboard::Key thrust    = sf::Keyboard::Key::W;
    sf::Keyboard::Key turnLeft  = sf::Keyboard::Key::A;
    sf::Keyboard::Key turnRight = sf::Keyboard::Key::D;
    sf::Keyboard::Key shoot     = sf::Keyboard::Key::Space;
};

// TODO: Написать класс игрока, имеющий:
// 1. Поля формы
// 2. Методы движения игрока
// 3. Метод стрельбы
class Player {
	public:
		sf::ConvexShape shape;

		Player()
		{			
			shape.setPointCount(3);
			shape.setPoint(0, sf::Vector2f(0, 0));
			shape.setPoint(1, sf::Vector2f(0, 60));
			shape.setPoint(2, sf::Vector2f(60, 30));
			shape.setFillColor(sf::Color::Black);
			shape.setOutlineColor(sf::Color::White);
			shape.setOutlineThickness(2.f);
			shape.setPosition(WINDOW_CENTRE);
		}
};

int main()
{
	sf::RenderWindow window( sf::VideoMode(WINDOW_RESOLUTION), "SFML Asteroids", sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	Player player;
	
	const float SPEED = 5.f; // Скорость корабля при движении

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		// Движение игрока
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		    player.shape.rotate(-sf::degrees(3.f));
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		    player.shape.rotate(sf::degrees(3.f));
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		    player.shape.move(sf::Vector2f(SPEED, player.shape.getRotation()));
		    player.shape.rotate(-sf::degrees(3.f));
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		    player.shape.rotate(sf::degrees(3.f));
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		    player.shape.move(sf::Vector2f(SPEED, player.shape.getRotation()));
		}

		
		window.clear();
		window.draw(player.shape);
		window.display();
	}
}

