#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
const int TILE_SIZE = 48;
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 12;

char map[MAP_HEIGHT][MAP_WIDTH+1] = 
{
"####################",
"#........*.........#",
"#.#.#.#.#.#.#.#.#..#",
"#..*...........*...#",
"#.#.#.#.#.#.#.#.#..#",
"#.....*............#",
"#.#.#.#.#.#.#.#.#..#",
"#...*..........*...#",
"#.#.#.#.#.#.#.#.#..#",
"#..........*.......#",
"#...............*..#",
"####################"
};
bool isWall(int x,int y)
{
    if (x < 0 || x >= MAP_WIDTH  || y < 0 || y >=MAP_HEIGHT)
    {
        return true;
    }
    return map[y][x] == '#' ||
    map[y][x] == '*';
    
}
struct Bomb
{
    int gridX;
    int gridY;
    sf::Clock timer;
};
struct Explosion
{
    int gridX;
    int gridY;
    sf::Clock timer;
};


int main()
{
    sf::RenderWindow window(sf::VideoMode({MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE}),
"Bomberman");
window.setFramerateLimit(60);

int playerGridX = 1;
int playerGridY = 1;

sf::RectangleShape player({static_cast<float>(TILE_SIZE),static_cast<float>(TILE_SIZE)});
player.setFillColor(sf::Color::Cyan);
std::vector<Bomb> bombs;

std::vector<Explosion> explosions;
bool spacePressed = false;
sf::Clock moveClock;

while (window.isOpen())
{
    while (const std::optional event =  window.pollEvent())
 {
    if (event->is<sf::Event::Closed>())
    {
        window.close();
    }

    }
    
if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) &&
!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) &&
!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) &&
!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
{
}

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
    {
     if (!spacePressed)
     {
         Bomb bomb;

        bomb.gridX = playerGridX;
        bomb.gridY = playerGridY;

        bombs.push_back(bomb);

        spacePressed = true;
     }
    }
    else
    {
        spacePressed = false;
    }
    
    if (moveClock.getElapsedTime().asSeconds() > 0.15f)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    { 
        if (!isWall(playerGridX,playerGridY - 1))
        {
            playerGridY --;
        }
        moveClock.restart();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    { 
     if (!isWall(playerGridX,playerGridY + 1))
        {
            playerGridY ++;
        }
         moveClock.restart();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    { 
     if (!isWall(playerGridX - 1,playerGridY))
        {
            playerGridX --;
        }
         moveClock.restart();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    { 
      if (!isWall(playerGridX + 1, playerGridY))
        {
            playerGridX ++;
        }
         moveClock.restart();
    }
        
        
    }
    
    
    player.setPosition({static_cast<float>(playerGridX*TILE_SIZE),static_cast<float>(playerGridY*TILE_SIZE)});
    window.clear(sf::Color::Black);

    for (int y=0; y < MAP_HEIGHT;y++)
    {
     for (int x=0; x < MAP_WIDTH;x++)
    {
    sf::RectangleShape tile({
     static_cast<float>(TILE_SIZE),
     static_cast<float>(TILE_SIZE)});
    tile.setPosition({
         static_cast<float>(x * TILE_SIZE),
         static_cast<float>(y * TILE_SIZE)
    });
    if (map[y][x] == '#')
    {
       tile.setFillColor(sf::Color::Magenta);
    }
    else if (map[y][x] == '*')
    {
       tile.setFillColor(sf::Color::Blue);
    }
    else
    {
        tile.setFillColor(sf::Color::White);
    }
    window.draw(tile);
    }
}
   for (int i = 0; i < bombs.size(); i++)
   {
    if (bombs[i].timer.getElapsedTime().asSeconds()>2.f)
    {
        Explosion explosion;
        explosion.gridX = bombs[i].gridX;
        explosion.gridY = bombs[i].gridY;

        explosions.push_back(explosion);
    bombs.erase(bombs.begin() + i);
    i--;
    }
    
   }
   
    for (const Bomb& bomb : bombs) 
    { 
     sf::CircleShape bombShape(15.f);
     bombShape.setOrigin({15.f,15.f});
    bombShape.setFillColor(sf::Color::Black);
    bombShape.setPosition({static_cast<float>(bomb.gridX * TILE_SIZE + TILE_SIZE / 2),static_cast<float>(bomb.gridY * TILE_SIZE + TILE_SIZE / 2)});
     window.draw(bombShape); 
     }
     for (int i = 0; i < explosions.size(); i++)
     {
       if (explosions[i].timer.getElapsedTime().asSeconds() > 0.5f)
       {
        explosions.erase(explosions.begin() + i);
        i--;
       }
       
     }
     for (const Explosion& explosion : explosions)
     {
       //orta
       if (playerGridX == explosion.gridX &&
    playerGridY == explosion.gridY)
       {
        window.close();
       }
        //üst
       if (playerGridX == explosion.gridX &&
    playerGridY == explosion.gridY - 1)
       {
        window.close();
       }
           //alt
       if (playerGridX == explosion.gridX &&
    playerGridY == explosion.gridY + 1)
       {
        window.close();
       }
           //sol
       if (playerGridX == explosion.gridX - 1 &&
    playerGridY == explosion.gridY)
       {
        window.close();
       }
             //sag
       if (playerGridX == explosion.gridX + 1 &&
    playerGridY == explosion.gridY)
       {
        window.close();
       }

    }
     
     for (const Explosion& explosion : explosions)
     {
        int gridX = explosion.gridX;
        int gridY = explosion.gridY;
        //orta

        if (map[gridY][gridX] == '*')
        {
            map[gridY][gridX] = '.';
        }

         //yukarı

        if (map[gridY - 1][gridX] == '*')
        {
            map[gridY - 1][gridX] = '.';
        }
        else if (map[gridY - 1][gridX] == '#')
        {
            
        }
        
        //alt

        if (map[gridY + 1][gridX] == '*')
        {
            map[gridY + 1][gridX] = '.';
        }
        else if (map[gridY + 1][gridX] == '#')
        {
        
        }
        
        //sol

        if (map[gridY][gridX - 1] == '*')
        {
            map[gridY][gridX - 1] = '.';
        }
         else if (map[gridY][gridX - 1] == '#')
        {
        
        }

        //sag

        if (map[gridY][gridX + 1] == '*')
        {
            map[gridY][gridX + 1] = '.';
        }
         else if (map[gridY][gridX + 1] == '#')
        {
        
        }
        
     }
     
    for (const Explosion& explosion : explosions)
    {
        sf::RectangleShape center({static_cast<float>(TILE_SIZE),
    static_cast<float>(TILE_SIZE)});
        center.setFillColor(sf::Color::Yellow);
        center.setPosition({
            static_cast<float>(explosion.gridX * TILE_SIZE),static_cast<float>(explosion.gridY * TILE_SIZE)
        });
        window.draw(center);
        //yukarı
        if (map[explosion.gridY - 1][explosion.gridX] != '#'
        && map[explosion.gridY - 1][explosion.gridX] != '*')
        {
          sf::RectangleShape up({static_cast<float>(TILE_SIZE),
    static_cast<float>(TILE_SIZE)});
        up.setFillColor(sf::Color::Yellow);
        up.setPosition({
            static_cast<float>(explosion.gridX * TILE_SIZE),static_cast<float>(explosion.gridY * TILE_SIZE - TILE_SIZE)
        });
        window.draw(up);
    }
        //aşağı
         if (map[explosion.gridY + 1][explosion.gridX] != '#')
        {
            sf::RectangleShape down({static_cast<float>(TILE_SIZE),
    static_cast<float>(TILE_SIZE)});
        down.setFillColor(sf::Color::Yellow);
        down.setPosition({
            static_cast<float>(explosion.gridX * TILE_SIZE),static_cast<float>(explosion.gridY * TILE_SIZE + TILE_SIZE)
        });
        window.draw(down);
    }
         //sol
          if (map[explosion.gridY][explosion.gridX - 1] != '#')
        {
          sf::RectangleShape left({static_cast<float>(TILE_SIZE),
    static_cast<float>(TILE_SIZE)});
        left.setFillColor(sf::Color::Yellow);
        left.setPosition({
            static_cast<float>(explosion.gridX * TILE_SIZE - TILE_SIZE), 
            static_cast<float>(explosion.gridY * TILE_SIZE)
        });
        window.draw(left);
    }
             //sag
    if (map[explosion.gridY][explosion.gridX + 1] != '#')
        {  
          sf::RectangleShape right({static_cast<float>(TILE_SIZE),
    static_cast<float>(TILE_SIZE)});

        right.setFillColor(sf::Color::Yellow);

        right.setPosition({
            static_cast<float>(explosion.gridX * TILE_SIZE + TILE_SIZE),static_cast<float>(explosion.gridY * TILE_SIZE)
        });
        window.draw(right);
    }
    
   }
 window.draw(player);
    window.display();
} 

return 0; 
}