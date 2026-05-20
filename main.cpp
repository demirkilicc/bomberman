#include <SFML/Graphics.hpp>
#include <optional>

int main()
{
    sf::RenderWindow window(sf::VideoMode({800,600}),
"Bomberman");
window.setFramerateLimit(60);

float playerX = 100.f;
float playerY = 100.f;
sf::RectangleShape player({530.f,500.f});
player.setFillColor(sf::Color::Cyan);

while (window.isOpen())
{
    while (const std::optional event =  window.pollEvent())
 {
    if (event->is<sf::Event::Closed>())
    {
        window.close();
    }

    } 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    { 
    playerY -= 3.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    { 
    playerY += 3.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    { 
    playerX -= 3.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    { 
    playerX += 3.f;
    }
    player.setPosition({playerX,playerY});
    window.clear(sf::Color::Black);
    window.draw(player);
    window.display();
}

return 0;

}