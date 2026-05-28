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

struct PowerUp
{
    int gridX;
    int gridY;

    int type;
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

sf::Texture playerTexture;

if (!playerTexture.loadFromFile("assets/chicken.png"))
{
    return -1;
}

playerTexture.setSmooth(false);

sf::Sprite playerSprite(playerTexture);

 sf::Texture grassTexture;

if (!grassTexture.loadFromFile("assets/grass.jpg"))
{
    return -1;
}

sf::Sprite grassSprite(grassTexture);

sf::Texture hayTexture;

if (!hayTexture.loadFromFile("assets/hay.png"))
{
    return -1;
}

sf::Sprite haySprite(hayTexture);

haySprite.setScale({
    TILE_SIZE / static_cast<float>(hayTexture.getSize().x),
    TILE_SIZE / static_cast<float>(hayTexture.getSize().y)
});

sf::Texture logTexture;

if (!logTexture.loadFromFile("assets/log.png"))
{
    return -1;
}

sf::Sprite logSprite(logTexture);


int playerGridX = 1;
int playerGridY = 1;
const int FRAME_WIDTH = 48;
const int FRAME_HEIGHT = 48;

int currentFrame = 1;
int currentDirection = 0;

sf::Clock animationClock;

int playerHealth = 3;
int bombRange = 1;
int maxBombs = 1;

float moveDelay = 0.15f;


std::vector<Bomb> bombs;

std::vector<Explosion> explosions;
std::vector<PowerUp> powerUps;
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
    bool moving = false;
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
     if (!spacePressed && bombs.size() < maxBombs)
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
    
    if (moveClock.getElapsedTime().asSeconds() > moveDelay)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    { 
        if (!isWall(playerGridX,playerGridY - 1) &&
    !isBombThere(playerGridX,playerGridY - 1,bombs))
        {
            currentDirection = 3;
              moving = true;
            playerGridY --;
        }
        moveClock.restart();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    { 
     if (!isWall(playerGridX,playerGridY + 1) &&
    !isBombThere(playerGridX,playerGridY + 1,bombs))
        {
            currentDirection = 0;
            moving = true;
            playerGridY ++;
        }
         moveClock.restart();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    { 
     if (!isWall(playerGridX - 1,playerGridY) &&
    !isBombThere(playerGridX - 1,playerGridY,bombs))
        {
            currentDirection = 1;
            moving = true;
            playerGridX --;
        }
         moveClock.restart();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    { 
      if (!isWall(playerGridX + 1, playerGridY) &&
    !isBombThere(playerGridX + 1,playerGridY,bombs))
        {
            currentDirection = 2;
             moving = true;
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


     // Oyuncu düşmana çarptı mı?
for (Enemy& enemy : enemies)
{
    if (playerGridX == enemy.gridX && playerGridY == enemy.gridY)
    {
        if (damageClock.getElapsedTime().asSeconds() > 1.0f)
        {
            playerHealth--;
            damageClock.restart();

            if (playerHealth <= 0)
                window.close();
        }
    }
}




     
     for (int i = 0; i < powerUps.size(); i++)
{
    if (playerGridX == powerUps[i].gridX &&
        playerGridY == powerUps[i].gridY)
    {
        if (powerUps[i].type == 0)
        {
            bombRange++;
        }

        if (powerUps[i].type == 1)
        {
            maxBombs++;
        }

        if (powerUps[i].type == 2)
{
    moveDelay -= 0.02f;

    if (moveDelay < 0.05f)
    {
        moveDelay = 0.05f;
    }
}

        powerUps.erase(powerUps.begin() + i);

        i--;
    }
}
    
if (moving)
{
    if (animationClock.getElapsedTime().asSeconds() > 0.15f)
    {
        currentFrame++;

        if (currentFrame > 2)
        {
            currentFrame = 0;
        }

        animationClock.restart();
    }
}
else
{
    currentFrame = 1;
}

playerSprite.setTextureRect(sf::IntRect(
    {currentFrame * FRAME_WIDTH,
    currentDirection * FRAME_HEIGHT},
    {FRAME_WIDTH, FRAME_HEIGHT}
));

playerSprite.setPosition({
    static_cast<float>(playerGridX * TILE_SIZE),
    static_cast<float>(playerGridY * TILE_SIZE)
});
    
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
        logSprite.setPosition({
        static_cast<float>(x * TILE_SIZE),
        static_cast<float>(y * TILE_SIZE)
    });

    logSprite.setScale({
        static_cast<float>(TILE_SIZE) / logTexture.getSize().x,
        static_cast<float>(TILE_SIZE) / logTexture.getSize().y
    });

    window.draw(logSprite);

    }
    else if (map[y][x] == '*')
    {
        haySprite.setPosition({
        static_cast<float>(x * TILE_SIZE),
        static_cast<float>(y * TILE_SIZE)
    });

    window.draw(haySprite);
    }
    else
    {
        grassSprite.setPosition({
        static_cast<float>(x * TILE_SIZE),
        static_cast<float>(y * TILE_SIZE)
    });

    window.draw(grassSprite);
    }
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
    bool hitPlayer = false;

    if (playerGridX == explosion.gridX &&
        playerGridY == explosion.gridY)
    {
        hitPlayer = true;
    }

    // yukarı
    for (int i = 1; i <= bombRange; i++)
    {
        int y = explosion.gridY - i;

        if (map[y][explosion.gridX] == '#')
            break;

        if (playerGridX == explosion.gridX &&
            playerGridY == y)
        {
            hitPlayer = true;
        }

        if (map[y][explosion.gridX] == '*')
            break;
    }

    // aşağı
    for (int i = 1; i <= bombRange; i++)
    {
        int y = explosion.gridY + i;

        if (map[y][explosion.gridX] == '#')
            break;

        if (playerGridX == explosion.gridX &&
            playerGridY == y)
        {
            hitPlayer = true;
        }

        if (map[y][explosion.gridX] == '*')
            break;
    }

    // sol
    for (int i = 1; i <= bombRange; i++)
    {
        int x = explosion.gridX - i;

        if (map[explosion.gridY][x] == '#')
            break;

        if (playerGridX == x &&
            playerGridY == explosion.gridY)
        {
            hitPlayer = true;
        }

        if (map[explosion.gridY][x] == '*')
            break;
    }

    // sağ
    for (int i = 1; i <= bombRange; i++)
    {
        int x = explosion.gridX + i;

        if (map[explosion.gridY][x] == '#')
            break;

        if (playerGridX == x &&
            playerGridY == explosion.gridY)
        {
            hitPlayer = true;
        }

        if (map[explosion.gridY][x] == '*')
            break;
    }

    if (hitPlayer)
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
}
     
    for (const Explosion& explosion : explosions)
{
    int gridX = explosion.gridX;
    int gridY = explosion.gridY;

    if (map[gridY][gridX] == '*')
    {
        map[gridY][gridX] = '.';
    }

    // yukarı
    for (int i = 1; i <= bombRange; i++)
    {
        int y = gridY - i;

        if (map[y][gridX] == '#')
            break;

        if (map[y][gridX] == '*')
        {
            map[y][gridX] = '.';
             if (rand() % 100 < 35)
    {
        PowerUp p;

        p.gridX = gridX;
        p.gridY = y;

        int chance = rand() % 100;
        if (chance < 25)
       {
          p.type = 0;
       }
       else if (chance < 60)
     {
        p.type = 1;
        }
     else
      {
        p.type = 2;
       }

        powerUps.push_back(p);
    }
            break;
        }
    }

    // aşağı
    for (int i = 1; i <= bombRange; i++)
    {
        int y = gridY + i;

        if (map[y][gridX] == '#')
            break;

        if (map[y][gridX] == '*')
        {
            map[y][gridX] = '.';
             if (rand() % 100 < 35)
    {
        PowerUp p;

        p.gridX = gridX;
        p.gridY = y;

        p.type = rand() % 2;

        powerUps.push_back(p);
    }
            break;
        }
    }

    // sol
    for (int i = 1; i <= bombRange; i++)
    {
        int x = gridX - i;

        if (map[gridY][x] == '#')
            break;

        if (map[gridY][x] == '*')
        {
            map[gridY][x] = '.';

            if (rand() % 100 < 35)
    {
        PowerUp p;

        p.gridX = x;
        p.gridY = gridY;

        p.type = rand() % 2;

        powerUps.push_back(p);
    }

            
            break;
        }
    }

    // sağ
    for (int i = 1; i <= bombRange; i++)
    {
        int x = gridX + i;

        if (map[gridY][x] == '#')
            break;

        if (map[gridY][x] == '*')
        {
            map[gridY][x] = '.';

            if (rand() % 100 < 35)
    {
        PowerUp p;

        p.gridX = x;
        p.gridY = gridY;

        p.type = rand() % 2;

        powerUps.push_back(p);
    }

            break;
        }
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
        for (int i = 1; i <= bombRange; i++)
{
    int y = explosion.gridY - i;

    if (map[y][explosion.gridX] == '#')
    {
        break;
    }

    sf::RectangleShape up({
        static_cast<float>(TILE_SIZE),
        static_cast<float>(TILE_SIZE)
    });

    up.setFillColor(sf::Color::Yellow);

    up.setPosition({
        static_cast<float>(explosion.gridX * TILE_SIZE),
        static_cast<float>(y * TILE_SIZE)
    });

    window.draw(up);

    if (map[y][explosion.gridX] == '*')
    {
        break;
    }
}
        //aşağı
        for (int i = 1; i <= bombRange; i++)
{
    int y = explosion.gridY + i;

    if (map[y][explosion.gridX] == '#')
    {
        break;
    }

    sf::RectangleShape down({
        static_cast<float>(TILE_SIZE),
        static_cast<float>(TILE_SIZE)
    });

    down.setFillColor(sf::Color::Yellow);

    down.setPosition({
        static_cast<float>(explosion.gridX * TILE_SIZE),
        static_cast<float>(y * TILE_SIZE)
    });

    window.draw(down);

    if (map[y][explosion.gridX] == '*')
    {
        break;
    }
}
        
         //sol
         for (int i = 1; i <= bombRange; i++)
{
    int x = explosion.gridX - i;

    if (map[explosion.gridY][x] == '#')
    {
        break;
    }

    sf::RectangleShape left({
        static_cast<float>(TILE_SIZE),
        static_cast<float>(TILE_SIZE)
    });

    left.setFillColor(sf::Color::Yellow);

    left.setPosition({
        static_cast<float>(x * TILE_SIZE),
        static_cast<float>(explosion.gridY * TILE_SIZE)
    });

    window.draw(left);

    if (map[explosion.gridY][x] == '*')
    {
        break;
    }
}
       
             //sag
             for (int i = 1; i <= bombRange; i++)
{
    int x = explosion.gridX + i;

    if (map[explosion.gridY][x] == '#')
    {
        break;
    }

    sf::RectangleShape right({
        static_cast<float>(TILE_SIZE),
        static_cast<float>(TILE_SIZE)
    });

    right.setFillColor(sf::Color::Yellow);

    right.setPosition({
        static_cast<float>(x * TILE_SIZE),
        static_cast<float>(explosion.gridY * TILE_SIZE)
    });

    window.draw(right);

    if (map[explosion.gridY][x] == '*')
    {
        break;
    }
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

for (const PowerUp& p : powerUps)
{
    sf::CircleShape shape(12.f);

    if (p.type == 0)
    {
        shape.setFillColor(sf::Color::Red);
    }

    if (p.type == 1)
    {
        shape.setFillColor(sf::Color::Green);
    }
    
    if (p.type == 2)
{
    shape.setFillColor(sf::Color::Blue);
}

    shape.setPosition({
        static_cast<float>(p.gridX * TILE_SIZE + 12),
        static_cast<float>(p.gridY * TILE_SIZE + 12)
    });

    window.draw(shape);
}

 window.draw(playerSprite);
    window.display();
} 

return 0; 
}