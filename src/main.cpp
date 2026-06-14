#include <SFML/Graphics.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <cmath> // Не забудьте подключить для std::cos и std::sin

const sf::Vector2u WINDOW_RESOLUTION(1200, 800);
const sf::Vector2f WINDOW_CENTRE(600.f, 400.f);

int main()
{
	sf::RenderWindow window( sf::VideoMode(WINDOW_RESOLUTION), "SFML Asteroids", sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	sf::Vector2f playerPos(WINDOW_CENTRE);
	sf::ConvexShape player;
	player.setPointCount(3);
	player.setPoint(0, sf::Vector2f(0, 0));
	player.setPoint(1, sf::Vector2f(0, 20));
	player.setPoint(2, sf::Vector2f(20, 10));
	player.setOutlineColor(sf::Color::Red);
	player.setOutlineThickness(30.f);
	player.setPosition(WINDOW_CENTRE);

	player.setOrigin({(20.f / 3.f), 10.f});
	
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
		    player.rotate(-sf::degrees(3.f));
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		    player.rotate(sf::degrees(3.f));
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		    player.move(sf::Vector2f(SPEED, player.getRotation()));
		}

		
		window.clear();
		window.draw(player);
		window.display();
	}
}

