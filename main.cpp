#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
const int TILE_SIZE = 48;
const int MAP_WIDTH = 21;
const int MAP_HEIGHT = 13;

char map[MAP_HEIGHT][MAP_WIDTH+1] = 
{
"#####################",
"#...................#",
"#.#*#.#.#.#*#.#.#.#.#",
"#..*..*........*.*..#",
"#.#*#.#*#.#.#.#.#.#.#",
"#.....*...*.........#",
"#.#.#.#.#.#.#*#.#.#.#",
"#...*.....*....*....#",
"#.#*#.#.#.#.#.#.#.#.#",
"#..........*..*.....#",
"#.#.#.#.#.#.#.#.#.#*#",
"#...............***.#",
"#####################"
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


struct Enemy
{
    int gridX;
    int gridY;

    int type;
    int health;
 bool damaged = false;

     int direction;

     bool breakingWall = false;

     int breakX;
     int breakY;
     
     sf::Clock moveTimer;
    
     sf::Clock breakTimer;
};

bool isEnemyThere(int x, int y, const std::vector<Enemy>& enemies)
{
    for (const Enemy& enemy : enemies)
    {
       if (enemy.gridX == x && enemy.gridY == y)
        {
            return true;
        }
    }
return false;
}

bool isBombThere(int x, int y, const std::vector<Bomb>& bombs)
{
    for (const Bomb& bomb : bombs)
    {
        if (bomb.gridX == x && bomb.gridY == y)
            return true;
    }
    return false;
}
int main()
{
    sf::RenderWindow window(sf::VideoMode({MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE}),
"Bomberman");
window.setFramerateLimit(60);

int playerGridX = 1;
int playerGridY = 1;
int playerHealth = 3;

sf::RectangleShape player({static_cast<float>(TILE_SIZE),static_cast<float>(TILE_SIZE)});
player.setFillColor(sf::Color::Cyan);
std::vector<Bomb> bombs;

std::vector<Explosion> explosions;
std::vector<Enemy> enemies;
sf::Clock moveClock;
sf::Clock damageClock;
bool spacePressed = false;
Enemy fox;
fox.gridX = 5;
fox.gridY = 5;
fox.type = 0;
fox.health = 1;
fox.direction = rand() % 4;
enemies.push_back(fox);

Enemy wolf;
wolf.gridX = 13;
wolf.gridY = 8;
wolf.type = 1;
wolf.health = 2;
wolf.direction = rand() % 4;

enemies.push_back(wolf);

Enemy butcher;
butcher.gridX = 19;
butcher.gridY = 11;
butcher.type = 2;
butcher.health = 5;
butcher.direction = rand() % 4;

enemies.push_back(butcher);


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
        if (!isWall(playerGridX,playerGridY - 1) &&
    !isBombThere(playerGridX,playerGridY - 1,bombs))
        {
            playerGridY --;
        }
        moveClock.restart();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    { 
     if (!isWall(playerGridX,playerGridY + 1) &&
    !isBombThere(playerGridX,playerGridY + 1,bombs))
        {
            playerGridY ++;
        }
         moveClock.restart();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    { 
     if (!isWall(playerGridX - 1,playerGridY) &&
    !isBombThere(playerGridX - 1,playerGridY,bombs))
        {
            playerGridX --;
        }
         moveClock.restart();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    { 
      if (!isWall(playerGridX + 1, playerGridY) &&
    !isBombThere(playerGridX + 1,playerGridY,bombs))
        {
            playerGridX ++;
        }
         moveClock.restart();
    }
        
        
    }
    //Düşman ai
    for (Enemy& enemy : enemies)
    {
       //tilki
       if (enemy.type == 0)
       {
        if (enemy.moveTimer.getElapsedTime().asSeconds() > 0.2f)
        {
            bool moved = false;
            if (enemy.direction == 0)
            {
               if (playerGridX == enemy.gridX &&
    playerGridY == enemy.gridY - 1)
{
    if (damageClock.getElapsedTime().asSeconds() > 1.0f)
    {
        playerHealth--;
        damageClock.restart();

        if (playerHealth <= 0)
        {
            window.close();
        }
    }
}
else if (!isWall(enemy.gridX, enemy.gridY - 1) &&
!isBombThere(enemy.gridX,enemy.gridY - 1,bombs) &&
         !isEnemyThere(enemy.gridX, enemy.gridY - 1, enemies))
{
    enemy.gridY--;
    moved = true;
}
                
            }
             if (enemy.direction == 1)
            {
               if (playerGridX == enemy.gridX &&
    playerGridY == enemy.gridY + 1)
{
    if (damageClock.getElapsedTime().asSeconds() > 1.0f)
    {
        playerHealth--;
        damageClock.restart();

        if (playerHealth <= 0)
        {
            window.close();
        }
    }
}
else if (!isWall(enemy.gridX, enemy.gridY + 1) &&
!isBombThere(enemy.gridX,enemy.gridY + 1,bombs) &&
         !isEnemyThere(enemy.gridX, enemy.gridY + 1, enemies))
{
    enemy.gridY++;
    moved = true;
}
                
            }
            if (enemy.direction == 2)
            {
               if (playerGridX == enemy.gridX - 1 &&
    playerGridY == enemy.gridY)
{
    if (damageClock.getElapsedTime().asSeconds() > 1.0f)
    {
        playerHealth--;
        damageClock.restart();

        if (playerHealth <= 0)
        {
            window.close();
        }
    }
}
else if (!isWall(enemy.gridX - 1, enemy.gridY) &&
!isBombThere(enemy.gridX - 1,enemy.gridY,bombs) &&
         !isEnemyThere(enemy.gridX - 1, enemy.gridY, enemies))
{
    enemy.gridX--;
    moved = true;
}
                
            }
             if (enemy.direction == 3)
            {
                 if (playerGridX == enemy.gridX + 1 &&
    playerGridY == enemy.gridY)
{
    if (damageClock.getElapsedTime().asSeconds() > 1.0f)
    {
        playerHealth--;
        damageClock.restart();

        if (playerHealth <= 0)
        {
            window.close();
        }
    }
}
else if (!isWall(enemy.gridX + 1, enemy.gridY) &&
!isBombThere(enemy.gridX + 1,enemy.gridY,bombs) &&
         !isEnemyThere(enemy.gridX + 1, enemy.gridY, enemies))
{
    enemy.gridX++;
    moved = true;
}
                
            }
            if (!moved)
            {
                enemy.direction = rand() % 4;
            }
            if(rand() % 10 == 0)
            {
                enemy.direction = rand() % 4;
            }
            enemy.moveTimer.restart();
        }
        
       }
       //kurt
        if (enemy.type == 1)
{
    if (enemy.moveTimer.getElapsedTime().asSeconds() > 0.4f)
    {
        int distanceX = abs(playerGridX - enemy.gridX);
        int distanceY = abs(playerGridY - enemy.gridY);

        if (distanceX <= 4 && distanceY <= 4)
        {
            bool moved = false;

            if (playerGridX < enemy.gridX)
            {
            if (playerGridX == enemy.gridX - 1 && playerGridY == enemy.gridY)
                {
                if (damageClock.getElapsedTime().asSeconds() > 1.0f)
                    {
                        playerHealth--;
                        damageClock.restart();
                        if (playerHealth <= 0) window.close();
                    }
                }
                else if (!isWall(enemy.gridX - 1, enemy.gridY) &&
!isBombThere(enemy.gridX - 1, enemy.gridY, bombs) && !isEnemyThere(enemy.gridX - 1, enemy.gridY, enemies))
                {
                    enemy.gridX--;
                    moved = true;
                }
            }
            else if (playerGridX > enemy.gridX)
            {
                if (playerGridX == enemy.gridX + 1 && playerGridY == enemy.gridY)
                {
                    if (damageClock.getElapsedTime().asSeconds() > 1.0f)
                    {
                        playerHealth--;
                        damageClock.restart();
                        if (playerHealth <= 0) window.close();
                    }
                }
                else if (!isWall(enemy.gridX + 1, enemy.gridY) &&
!isBombThere(enemy.gridX + 1, enemy.gridY, bombs) && !isEnemyThere(enemy.gridX + 1, enemy.gridY, enemies))
                {
                    enemy.gridX++;
                    moved = true;
                }
            }

        
            if (!moved)
            {

             if (playerGridY < enemy.gridY)
            {
                 if (playerGridX == enemy.gridX && playerGridY == enemy.gridY - 1)
                    {
                    if (damageClock.getElapsedTime().asSeconds() > 1.0f)
                        {
                            playerHealth--;
                            damageClock.restart();
                            if (playerHealth <= 0) window.close();
                        }
                    }
                    else if (!isWall(enemy.gridX, enemy.gridY - 1) &&
!isBombThere(enemy.gridX, enemy.gridY - 1, bombs) && !isEnemyThere(enemy.gridX, enemy.gridY - 1, enemies))
                    {
                        enemy.gridY--;
                    }
                }
                else if (playerGridY > enemy.gridY)
                {
                if (playerGridX == enemy.gridX && playerGridY == enemy.gridY + 1)
                    {
                    if (damageClock.getElapsedTime().asSeconds() > 1.0f)
                        {
                            playerHealth--;
                            damageClock.restart();
                            if (playerHealth <= 0) window.close();
                        }
                    }
                    else if (!isWall(enemy.gridX, enemy.gridY + 1) &&
!isBombThere(enemy.gridX, enemy.gridY + 1, bombs) && !isEnemyThere(enemy.gridX, enemy.gridY + 1, enemies))
                    {
                        enemy.gridY++;
                    }
             }
         }
        }
        else 
        {
            bool moved = false;
            int nextX = enemy.gridX;
            int nextY = enemy.gridY;

            if (enemy.direction == 0) nextY--;
            else if (enemy.direction == 1) nextY++;
            else if (enemy.direction == 2) nextX--;
            else if (enemy.direction == 3) nextX++;

    
            if (playerGridX == nextX && playerGridY == nextY)
            {
                if (damageClock.getElapsedTime().asSeconds() > 1.0f)
                {
                    playerHealth--;
                    damageClock.restart();
                    if (playerHealth <= 0) window.close();
                }
            }
            else if (!isWall(nextX, nextY) && !isEnemyThere(nextX, nextY, enemies))
            {
                enemy.gridX = nextX;
                enemy.gridY = nextY;
                moved = true;
            }

            if (!moved || rand() % 10 == 0)
            {
                enemy.direction = rand() % 4;
            }
        }
        enemy.moveTimer.restart();
    }
}
     //kasap
     if (enemy.type == 2)
{
    if (enemy.breakingWall)
    {
        if (enemy.breakTimer.getElapsedTime().asSeconds() > 1.0f)
        {
            map[enemy.breakY][enemy.breakX] = '.';
            enemy.breakingWall = false;
        }
    }
    else if (enemy.moveTimer.getElapsedTime().asSeconds() > 0.7f)
    {
        bool moved = false;

        if (playerGridX < enemy.gridX)
        {
            if (playerGridX == enemy.gridX - 1 && playerGridY == enemy.gridY)
            {
            if (damageClock.getElapsedTime().asSeconds() > 1.0f)
                {
                    playerHealth--;
                    damageClock.restart();
                    if (playerHealth <= 0) window.close();
                }
             }
            else if (!isWall(enemy.gridX - 1, enemy.gridY) && !isEnemyThere(enemy.gridX - 1, enemy.gridY, enemies))
            {
                enemy.gridX--;
                moved = true;
             } 
            else if (map[enemy.gridY][enemy.gridX - 1] == '*')
            {
             enemy.breakingWall = true;
            enemy.breakX = enemy.gridX - 1;
            enemy.breakY = enemy.gridY;
             enemy.breakTimer.restart();
              }
        }
        else if (playerGridX > enemy.gridX)
        {
            if (playerGridX == enemy.gridX + 1 && playerGridY == enemy.gridY)
           {
                if (damageClock.getElapsedTime().asSeconds() > 1.0f)
                {
                playerHealth--;
                damageClock.restart();
                if (playerHealth <= 0) window.close();
                }
             }
            else if (!isWall(enemy.gridX + 1, enemy.gridY) && !isEnemyThere(enemy.gridX + 1, enemy.gridY, enemies))
             {
            enemy.gridX++;
             moved = true;
              } 
            else if (map[enemy.gridY][enemy.gridX + 1] == '*')
            {
              enemy.breakingWall = true;
             enemy.breakX = enemy.gridX + 1;
            enemy.breakY = enemy.gridY;
            enemy.breakTimer.restart();
            }  
        }
        if (!moved && !enemy.breakingWall)
        {
            if (playerGridY < enemy.gridY)
            {
            if (playerGridX == enemy.gridX && playerGridY == enemy.gridY - 1)
               {
                if (damageClock.getElapsedTime().asSeconds() > 1.0f)
                    {
                        playerHealth--;
                        damageClock.restart();
                        if (playerHealth <= 0) window.close();
                    }
                }
                else if (!isWall(enemy.gridX, enemy.gridY - 1) && !isEnemyThere(enemy.gridX, enemy.gridY - 1, enemies))
                {
                    enemy.gridY--;
                    moved = true;
                } 
                else if (map[enemy.gridY - 1][enemy.gridX] == '*')
                {
                enemy.breakingWall = true;
                enemy.breakX = enemy.gridX;
                enemy.breakY = enemy.gridY - 1;
                enemy.breakTimer.restart();
                }
            }
            else if (playerGridY > enemy.gridY)
            {
                if (playerGridX == enemy.gridX && playerGridY == enemy.gridY + 1) 
                {
                    if (damageClock.getElapsedTime().asSeconds() > 1.0f)
                    {
                        playerHealth--;
                        damageClock.restart();
                        if (playerHealth <= 0) window.close();
                    }
                }
                else if (!isWall(enemy.gridX, enemy.gridY + 1) && !isEnemyThere(enemy.gridX, enemy.gridY + 1, enemies))
                {
                    enemy.gridY++;
                    moved = true;
                } 
                else if (map[enemy.gridY + 1][enemy.gridX] == '*')
                {
                enemy.breakingWall = true;
                enemy.breakX = enemy.gridX;
                enemy.breakY = enemy.gridY + 1;
                enemy.breakTimer.restart();
             }  
          }
        }
        if (!moved && !enemy.breakingWall) 
        {
            int nextX = enemy.gridX;
            int nextY = enemy.gridY;

            if (enemy.direction == 0) nextY--;
            else if (enemy.direction == 1) nextY++;
            else if (enemy.direction == 2) nextX--;
            else if (enemy.direction == 3) nextX++;

            if (playerGridX == nextX && playerGridY == nextY)
            {
                if (damageClock.getElapsedTime().asSeconds() > 1.0f)
                {
                    playerHealth--;
                    damageClock.restart();
                    if (playerHealth <= 0) window.close();
                }
            }
            else if (!isWall(nextX, nextY) && !isEnemyThere(nextX, nextY, enemies))
            {
                enemy.gridX = nextX;
                enemy.gridY = nextY;
                moved = true;
            }
            else if (map[nextY][nextX] == '*')
            {
                enemy.breakingWall = true;
                enemy.breakX = nextX;
                enemy.breakY = nextY;
                enemy.breakTimer.restart();
            }
            else
            {
                enemy.direction = rand() % 4;
            }
        }
        enemy.moveTimer.restart();
    }
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
        for (Enemy& enemy : enemies)
    {
        enemy.damaged = false;
    }
        explosions.erase(explosions.begin() + i);
        i--;
       }
       
     }
     for (const Explosion& explosion : explosions)
     {
       //orta
       if (playerGridX == explosion.gridX &&
    playerGridY == explosion.gridY)
       { if (damageClock.getElapsedTime().asSeconds() > 1.0f)
       {
        playerHealth--;
        damageClock.restart();

        if (playerHealth <= 0)
       {
        window.close();
       }
       }
       
        if (playerHealth <= 0)
       {
        window.close();
       }
       }
        //üst
       if (playerGridX == explosion.gridX &&
    playerGridY == explosion.gridY - 1)
       {
        if (damageClock.getElapsedTime().asSeconds() > 1.0f)
       {
        playerHealth--;
        damageClock.restart();

        if (playerHealth <= 0)
       {
        window.close();
       }
       }
       
        if (playerHealth <= 0)
       {
        window.close();
       }
       }
           //alt
       if (playerGridX == explosion.gridX &&
    playerGridY == explosion.gridY + 1)
       {
        if (damageClock.getElapsedTime().asSeconds() > 1.0f)
       {
        playerHealth--;
        damageClock.restart();

        if (playerHealth <= 0)
       {
        window.close();
       }
       }
       
        if (playerHealth <= 0)
       {
        window.close();
       }
       }
           //sol
       if (playerGridX == explosion.gridX - 1 &&
    playerGridY == explosion.gridY)
       {
        if (damageClock.getElapsedTime().asSeconds() > 1.0f)
       {
        playerHealth--;
        damageClock.restart();

        if (playerHealth <= 0)
       {
        window.close();
       }
       }
       
        if (playerHealth <= 0)
       {
        window.close();
       }
       }
             //sag
       if (playerGridX == explosion.gridX + 1 &&
    playerGridY == explosion.gridY)
       {
       if (damageClock.getElapsedTime().asSeconds() > 1.0f)
       {
        playerHealth--;
        damageClock.restart();

        if (playerHealth <= 0)
       {
        window.close();
       }
       }
       
        if (playerHealth <= 0)
       {
        window.close();
       }
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
        for (Enemy& enemy : enemies)
{
    if (!enemy.damaged)
    {
        bool hit = false;

        if (enemy.gridX == explosion.gridX && enemy.gridY == explosion.gridY)
        {
            hit = true;
        }
        // üst
        if (enemy.gridX == explosion.gridX && enemy.gridY == explosion.gridY - 1)
        {
            hit = true;
        }
        // alt
        if (enemy.gridX == explosion.gridX &&
            enemy.gridY == explosion.gridY + 1)
        {
            hit = true;
        }
        // sol
        if (enemy.gridX == explosion.gridX - 1 && enemy.gridY == explosion.gridY)
        {
            hit = true;
        }
        // sag
        if (enemy.gridX == explosion.gridX + 1 && enemy.gridY == explosion.gridY)
        {
           hit = true;
        }
  if (hit)
     {
    enemy.health--;
     enemy.damaged = true;
     }
    }
}
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
   for (const Enemy& enemy : enemies)
   {
  
    sf::RectangleShape enemyShape({
        static_cast<float>(TILE_SIZE),
        static_cast<float>(TILE_SIZE)});

        if (enemy.type == 0)
        {
            enemyShape.setFillColor(sf::Color(255,140,0));
        }

        if (enemy.type == 1)
        {
            enemyShape.setFillColor(sf::Color(120,120,120));
        }

        if (enemy.type == 2)
        {
            enemyShape.setFillColor(sf::Color(120,0,0));
        }
        
        enemyShape.setPosition({ static_cast<float>(enemy.gridX * TILE_SIZE),
        static_cast<float>(enemy.gridY * TILE_SIZE)
    });
window.draw(enemyShape);
    }
    for (int i = 0; i < enemies.size(); i++)
{
    if (enemies[i].health <= 0)
    {
        enemies.erase(enemies.begin() + i);
        i--;
    }
}
 window.draw(player);
    window.display();
} 

return 0; 
}