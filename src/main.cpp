#include <vector>

// SFML 3.0.2, C++20
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Clock.hpp>

// Константы окошка
constexpr sf::Vector2u WINDOW_RESOLUTION(1200, 800);
constexpr sf::Vector2f WINDOW_CENTRE(600.f, 400.f);

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
    static constexpr float Radius = 4.0f;
    static constexpr float MaxLifetime = 2.0f;
    static constexpr float Size = 8.0f; // Размер каждой пульки        
    bool m_destroyed = false;

    Bullet(sf::Vector2f position, sf::Vector2f velocity)
        : m_position(position), m_velocity(velocity), m_lifetime(MaxLifetime) {}

    const sf::Vector2f position() const {
      return m_position;
    }    

    // TODO: Написать механику поражения астероидов
    // (пулька летит в астероид -> астероид раскалывается на маленькие части)
    void update(float delta) {
        m_position += m_velocity * delta;
        m_lifetime -= delta;
    }

    // тру, когда время жизни пульки на исходе
    bool isDead() const {
        return m_lifetime <= 0.0f || m_destroyed;
    }
};

class Player {
private:
	sf::Vector2f m_position;
	sf::Angle m_rotation;
	sf::ConvexShape m_shape;
	InputConfig config;
	float m_shootCooldown = 0.f;

	static constexpr float m_fireRate = 0.2f;
  static constexpr float SPEED = 500.f;
  static constexpr float ROTATION_SPEED = 400.f;

public:
	Player() : m_position(WINDOW_CENTRE) {
	    // Рисуем треугольник
	    m_shape.setPointCount(3);
	    m_shape.setPoint(0, {-20.f, -30.f}); 
	    m_shape.setPoint(1, {-20.f, 30.f});  
	    m_shape.setPoint(2, {40.f, 0.f});    
	    
	    // Красим треугольник
	    m_shape.setFillColor(sf::Color::Black);
	    m_shape.setOutlineColor(sf::Color::White);
	    m_shape.setOutlineThickness(2.f);
	}
	

	// TODO: Написать механику получение урона от столкновения с астероидом.
	// Обработка движения и стрельбы
	void update(float &delta, std::vector<Bullet> &bullets) {
		if (m_shootCooldown > 0.f) {
			m_shootCooldown -= delta;
		}

		// Передвижение и повороты
	  if (sf::Keyboard::isKeyPressed(config.turnLeft)) {
	      m_rotation -= sf::degrees(ROTATION_SPEED * delta);
	  }
	  if (sf::Keyboard::isKeyPressed(config.turnRight)) {
	      m_rotation += sf::degrees(ROTATION_SPEED * delta);
	  }
	  if (sf::Keyboard::isKeyPressed(config.thrust)) {
	      m_position += sf::Vector2f(SPEED * delta, m_rotation); 
	  }

		// Пив-пав
	  if (sf::Keyboard::isKeyPressed(config.shoot) && m_shootCooldown <= 0.f) {
	  	sf::Vector2f bulletVelocity = sf::Vector2f(800.f, m_rotation);
	  	sf::Vector2f noseOffset = sf::Vector2f(40.f, m_rotation);
      
	  	bullets.push_back(Bullet(m_position + noseOffset, bulletVelocity));
			m_shootCooldown = m_fireRate;
	  }
	}
	
	// Рисовашка
	void draw(sf::RenderWindow& window) {
		m_shape.setPosition(m_position);
		m_shape.setRotation(m_rotation);

		window.draw(m_shape);
	}
};

class Asteroid {
private:
  sf::Vector2f m_position;
  sf::Vector2f m_velocity;

public:
  static constexpr float Radius = 20.f;
  bool m_destroyed = false;

  Asteroid(sf::Vector2f position, sf::Vector2f velocity) :
  m_position(position), m_velocity(velocity) {}

  // геттер
  const sf::Vector2f position() const {return m_position;}

  void update(float delta) {
    m_position += m_velocity * delta;
  }

  bool is_collided(const Bullet& bullet) const {
    float dx = m_position.x - bullet.position().x;
    float dy = m_position.y - bullet.position().y;

    float distanceSquared = (dx * dx) + (dy * dy);
    float radiiSquared = (Asteroid::Radius + Bullet::Radius)
                         * (Asteroid::Radius + Bullet::Radius);

    return distanceSquared <= radiiSquared;
  }
};

int main()
{
    // Окно
    sf::RenderWindow window(sf::VideoMode(WINDOW_RESOLUTION), "SFML Asteroids", sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    // Инициализация всяких штук 
    Player player;
    sf::Clock game_clock;
    sf::Clock asteroidSpawnTimer;
    asteroidSpawnTimer.start();

    // Векторы 
    std::vector<Bullet> bullets;
    std::vector<Asteroid> asteroids;

    // Формы
    sf::RectangleShape sharedBulletShape(sf::Vector2f(Bullet::Size, Bullet::Size));
    sharedBulletShape.setOrigin({4.f, 4.f});
    sf::RectangleShape sharedAsteroidShape({40.f, 40.f});
    sharedAsteroidShape.setOrigin({20.f, 20.f});
    sharedAsteroidShape.setFillColor(sf::Color::Black);
    sharedAsteroidShape.setOutlineColor(sf::Color::White);
    sharedAsteroidShape.setOutlineThickness(3.f);

    // ГЛАВНЫЙ ЦИКЛ
    while (window.isOpen())
    {
        // Прошедшее время с каждого кадра (нужно для одинакового времени на всех устройствах)
        float delta = game_clock.restart().asSeconds();
        
        // Обработка событий окна (закрытие окошка)
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // === === === === Логика === === === ===

        player.update(delta, bullets);

        for (Asteroid &asteroid : asteroids) {
          asteroid.update(delta);
        }
        for (Bullet &bullet : bullets) {
          bullet.update(delta);
        }

        for (Bullet& bullet : bullets) {
            for (Asteroid& asteroid : asteroids) {
              if (bullet.isDead() || asteroid.m_destroyed) {
                continue;
              }

              if (asteroid.is_collided(bullet)) {
                bullet.m_destroyed = true;
                asteroid.m_destroyed = true;                
              }
            }
        }

        if (asteroidSpawnTimer.getElapsedTime().asSeconds() >= 1) {
          asteroidSpawnTimer.restart();
          asteroids.push_back(Asteroid({300.f, 300.f}, {100.f, 100.f}));
        }

        std::erase_if(bullets, [](const Bullet& b) {return b.isDead();});
        std::erase_if(asteroids, [](const Asteroid& a) {return a.m_destroyed;});
        
        // === === === === Графика === === === ===

        window.clear();

        player.draw(window);
        // Отрисовка каждой пульки
        for (const Bullet& bullet : bullets) {
          sharedBulletShape.setPosition(bullet.position());
          window.draw(sharedBulletShape);
        }
        // Отрисовка каждого астероида
        for (const Asteroid& asteroid : asteroids) {
          sharedAsteroidShape.setPosition(asteroid.position());
          window.draw(sharedAsteroidShape);
        }
    
        window.display();
    }
}
