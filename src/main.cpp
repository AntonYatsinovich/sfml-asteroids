#include <vector>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Clock.hpp>

const sf::Vector2u WINDOW_RESOLUTION(1200, 800);
const sf::Vector2f WINDOW_CENTRE(600.f, 400.f);
const float SPEED = 400.f;
const float ROTATION_SPEED = 250.f;

struct InputConfig {
    sf::Keyboard::Key thrust    = sf::Keyboard::Key::W;
    sf::Keyboard::Key turnLeft  = sf::Keyboard::Key::A;
    sf::Keyboard::Key turnRight = sf::Keyboard::Key::D;
    sf::Keyboard::Key shoot     = sf::Keyboard::Key::Space;
};

class Bullet {
private:
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    float m_lifetime; 

public:
    static constexpr float MaxLifetime = 2.0f; 
    static constexpr float Size = 4.0f;        

    Bullet(sf::Vector2f position, sf::Vector2f velocity)
        : m_position(position), m_velocity(velocity), m_lifetime(MaxLifetime) {}

    void update(float delta) {
        m_position += m_velocity * delta;
        m_lifetime -= delta;
    }

    bool isExpired() const {
        return m_lifetime <= 0.0f;
    }

    void draw(sf::RenderWindow& window, sf::RectangleShape& sharedShape) const {
        sharedShape.setPosition(m_position);
        window.draw(sharedShape);
    }
};

// TODO: Написать класс игрока, имеющий:
// 3. Метод стрельбы
class Player {
	private:
		sf::Vector2f m_position;
		sf::Angle m_rotation;
		sf::ConvexShape m_shape;
		InputConfig config;
	public:
		Player() : m_position(WINDOW_CENTRE) {
			m_shape.setPointCount(3);
			m_shape.setPoint(0, {0.f, 0.f});
			m_shape.setPoint(1, {0.f, 60.f});
			m_shape.setPoint(2, {60.f, 30.f});
			m_shape.setFillColor(sf::Color::Black);
			m_shape.setOutlineColor(sf::Color::White);
			m_shape.setOutlineThickness(2.f);
			m_shape.setOrigin({20.f, 30.f});
		}

		void update(float &delta) {
			if (sf::Keyboard::isKeyPressed(config.turnLeft)) {
				m_rotation -= sf::degrees(ROTATION_SPEED * delta);
			}
			if (sf::Keyboard::isKeyPressed(config.turnRight)) {
				m_rotation += sf::degrees(ROTATION_SPEED * delta);
			}
			if (sf::Keyboard::isKeyPressed(config.thrust)) {
				m_position += sf::Vector2f(SPEED * delta, m_rotation); 
			}
		}
		void draw(sf::RenderWindow& window) {
			m_shape.setPosition(m_position);
			m_shape.setRotation(m_rotation);

			window.draw(m_shape);
		}
};

int main()
{
	sf::RenderWindow window( sf::VideoMode(WINDOW_RESOLUTION), "SFML Asteroids", sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	sf::Clock clock;
	Player player;
	std::vector<Bullet> bullets;

	while (window.isOpen())
	{
		float delta = clock.restart().asSeconds();
		
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		// === === === === Логика === === === ===

		player.update(delta);

	
		// === === === === Графика === === === ===
		
		window.clear();
		player.draw(window);
		window.display();
	}
}

