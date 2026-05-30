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

bool isWall(int x, int y, char (*currentMap)[MAP_WIDTH+1])
{
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        return true;
    return currentMap[y][x] == '#' || currentMap[y][x] == '*';
}

struct Bomb
{
    int gridX;
    int gridY;
    int owner;
    sf::Clock timer;
    float scale = 1.0f;
};
struct Explosion
{
    int gridX;
    int gridY;
    int owner;
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

     int currentFrame = 0;
    sf::Clock animationClock;
};

bool isEnemyThere(int x, int y, const std::vector<Enemy>& enemies, const Enemy* self = nullptr)
{
    for (const Enemy& enemy : enemies)
    {
        if (&enemy == self)
        {
            continue;
        }

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

enum GameState
{
    MENU,
    SINGLEPLAYER,
    MULTIPLAYER
};

int player2GridX = 1;
int player2GridY = 1;
int player2Health = 3;
int player2CurrentFrame = 1;
int player2CurrentDirection = 0;
sf::Clock player2AnimationClock;
bool player2Moving = false;

GameState gameState = MENU;

char multiplayerMap[MAP_HEIGHT][MAP_WIDTH+1] = 
{
"#####################",
"#...................#",
"#...................#",
"#...................#",
"#...................#",
"#...................#",
"#...................#",
"#...................#",
"#...................#",
"#...................#",
"#...................#",
"#...................#",
"#####################"
};

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


sf::Texture player2Texture;
if (!player2Texture.loadFromFile("assets/chicken.png"))
{
    player2Texture = playerTexture;
}
player2Texture.setSmooth(false);
sf::Sprite player2Sprite(player2Texture);



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


sf::Texture meatTexture, bombTexture, explosionTexture, cornTexture, wormTexture;

if (!meatTexture.loadFromFile("assets/meat.png"))
{
    return -1;
}
meatTexture.setSmooth(false);

if (!cornTexture.loadFromFile("assets/corn.png"))
{
    return -1;
}
cornTexture.setSmooth(false);

if (!wormTexture.loadFromFile("assets/worm.png"))
{
    return -1;
}
wormTexture.setSmooth(false);

if (!bombTexture.loadFromFile("assets/egg.png"))
{
    return -1;
}
bombTexture.setSmooth(false);

if (!explosionTexture.loadFromFile("assets/cegg.png"))
{
    return -1;
}

sf::Sprite meatSprite(meatTexture);
sf::Sprite cornSprite(cornTexture);
sf::Sprite wormSprite(wormTexture);
sf::Sprite bombSprite(bombTexture);
sf::Sprite explosionSprite(explosionTexture);

meatSprite.setScale({
    static_cast<float>(TILE_SIZE) / meatTexture.getSize().x,
    static_cast<float>(TILE_SIZE) / meatTexture.getSize().y
});

cornSprite.setScale({
    static_cast<float>(TILE_SIZE) / cornTexture.getSize().x,
    static_cast<float>(TILE_SIZE) / cornTexture.getSize().y
});

wormSprite.setScale({
    static_cast<float>(TILE_SIZE) / wormTexture.getSize().x,
    static_cast<float>(TILE_SIZE) / wormTexture.getSize().y
});

bombSprite.setScale({
    static_cast<float>(TILE_SIZE) / bombTexture.getSize().x,
    static_cast<float>(TILE_SIZE) / bombTexture.getSize().y
});

explosionTexture.setSmooth(false);
explosionSprite.setTexture(explosionTexture);
explosionSprite.setScale({
    static_cast<float>(TILE_SIZE) / explosionTexture.getSize().x,
    static_cast<float>(TILE_SIZE) / explosionTexture.getSize().y
});

sf::Texture foxTexture, wolfTexture, butcherTexture;

if (!foxTexture.loadFromFile("assets/fox.png"))
{
    return -1;
}
foxTexture.setSmooth(false);

if (!wolfTexture.loadFromFile("assets/wolf.png"))
{
    return -1;
}
wolfTexture.setSmooth(false);

if (!butcherTexture.loadFromFile("assets/butcher.png"))
{
    return -1;
}
butcherTexture.setSmooth(false);

sf::Sprite foxSprite(foxTexture);
sf::Sprite wolfSprite(wolfTexture);
sf::Sprite butcherSprite(butcherTexture);

sf::Font font;

if (!font.openFromFile("assets/arial.ttf"))
{
    return -1;
}


sf::Text title(font);
title.setString("BOMBER CHICKEN");
title.setCharacterSize(48);
title.setPosition({250.f, 100.f});

sf::Text singleText(font);
singleText.setString("1 - Single Player");
singleText.setCharacterSize(32);
singleText.setPosition({300.f, 250.f});

sf::Text multiText(font);
multiText.setString("2 - Multiplayer");
multiText.setCharacterSize(32);
multiText.setPosition({300.f, 320.f});

sf::Text exitText(font);
exitText.setString("3 - Exit");
exitText.setCharacterSize(32);
exitText.setPosition({300.f, 390.f});

int playerGridX = 1;
int playerGridY = 1;

int player2GridX = MAP_WIDTH - 2;
int player2GridY = MAP_HEIGHT - 2;
int player2Health = 3;
int player2CurrentFrame = 1;
int player2CurrentDirection = 0;
sf::Clock player2AnimationClock;
bool player2Moving = false;

const int FRAME_WIDTH = 48;
const int FRAME_HEIGHT = 48;

int currentFrame = 1;
int currentDirection = 0;

sf::Clock animationClock;

int playerHealth = 3;
int bombRange = 1;
int maxBombs = 1;

int player2BombRange = 1;
int player2MaxBombs = 1;
float player2MoveDelay = 0.15f;

float moveDelay = 0.15f;


std::vector<Bomb> bombs;

std::vector<Explosion> explosions;
std::vector<PowerUp> powerUps;
std::vector<Enemy> enemies;
sf::Clock moveClock;
sf::Clock player2MoveClock;
sf::Clock damageClock;
sf::Clock player2DamageClock;
bool spacePressed = false;
bool player2SpacePressed = false;

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

     char (*currentMap)[MAP_WIDTH+1];
    if (gameState == MULTIPLAYER)
    {
        currentMap = multiplayerMap;
    }
    else
    {
        currentMap = map;
    }

    if (gameState == MENU)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))
    {
        gameState = SINGLEPLAYER;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2))
    {
        gameState = MULTIPLAYER;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3))
{
    window.close();
}

    window.clear();

    window.draw(title);
    window.draw(singleText);
    window.draw(multiText);

    window.display();

    continue;
}

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

    // 1. OYUNCU BOMBASI
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
{
    if (!spacePressed && bombs.size() < maxBombs)  // 1. oyuncunun maxBombs'u
    {
        Bomb bomb;
        bomb.gridX = playerGridX;
        bomb.gridY = playerGridY;
        bomb.owner = 1;
        bombs.push_back(bomb);
        spacePressed = true;
    }
}
else
{
    spacePressed = false;
}

// 2. OYUNCU BOMBASI
if (gameState == MULTIPLAYER)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift))
    {
        if (!player2SpacePressed && bombs.size() < player2MaxBombs)  // 2. oyuncunun maxBombs'u
        {
            Bomb bomb;
            bomb.gridX = player2GridX;
            bomb.gridY = player2GridY;
            bomb.owner = 2;
            bombs.push_back(bomb);
            player2SpacePressed = true;
        }
    }
    else
    {
        player2SpacePressed = false;
    }
}
  
        // ========== 1. OYUNCU HAREKETİ (WASD) ==========
if (moveClock.getElapsedTime().asSeconds() > moveDelay)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    { 
        if (!isWall(playerGridX, playerGridY - 1, currentMap) && !isBombThere(playerGridX, playerGridY - 1, bombs))
        {
            currentDirection = 3;
            moving = true;
            playerGridY--;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    { 
        if (!isWall(playerGridX, playerGridY + 1, currentMap) && !isBombThere(playerGridX, playerGridY + 1, bombs))
        {
            currentDirection = 0;
            moving = true;
            playerGridY++;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    { 
        if (!isWall(playerGridX - 1, playerGridY, currentMap) && !isBombThere(playerGridX - 1, playerGridY, bombs))
        {
            currentDirection = 1;
            moving = true;
            playerGridX--;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    { 
        if (!isWall(playerGridX + 1, playerGridY, currentMap) && !isBombThere(playerGridX + 1, playerGridY, bombs))
        {
            currentDirection = 2;
            moving = true;
            playerGridX++;
        }
    }
    
    moveClock.restart();
}

// ========== 2. OYUNCU HAREKETİ (OK TUŞLARI) - SADECE MULTIPLAYER ==========
if (gameState == MULTIPLAYER)
{
    // Önce hareket etmediğini varsay
    player2Moving = false;
    
    if (player2MoveClock.getElapsedTime().asSeconds() > player2MoveDelay)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            if (!isWall(player2GridX, player2GridY - 1, currentMap) && !isBombThere(player2GridX, player2GridY - 1, bombs))
            {
                player2CurrentDirection = 3;
                player2Moving = true;
                player2GridY--;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            if (!isWall(player2GridX, player2GridY + 1, currentMap) && !isBombThere(player2GridX, player2GridY + 1, bombs))
            {
                player2CurrentDirection = 0;
                player2Moving = true;
                player2GridY++;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            if (!isWall(player2GridX - 1, player2GridY, currentMap) && !isBombThere(player2GridX - 1, player2GridY, bombs))
            {
                player2CurrentDirection = 1;
                player2Moving = true;
                player2GridX--;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            if (!isWall(player2GridX + 1, player2GridY, currentMap) && !isBombThere(player2GridX + 1, player2GridY, bombs))
            {
                player2CurrentDirection = 2;
                player2Moving = true;
                player2GridX++;
            }
        }
        
        player2MoveClock.restart();
    }
}

    //Düşman ai
    if (gameState == SINGLEPLAYER)
    {
    
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
            if (playerGridX == enemy.gridX && playerGridY == enemy.gridY - 1)
            {
                if (damageClock.getElapsedTime().asSeconds() > 1.0f)
                {
                    playerHealth--;
                    damageClock.restart();
                    if (playerHealth <= 0) window.close();
                }
            }
            else if (!isWall(enemy.gridX, enemy.gridY - 1, currentMap) && 
                     !isBombThere(enemy.gridX, enemy.gridY - 1, bombs) &&  
                     !isEnemyThere(enemy.gridX, enemy.gridY - 1, enemies, &enemy))
            {
                enemy.gridY--;
                moved = true;
            }
        }
        if (enemy.direction == 1)  
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
            else if (!isWall(enemy.gridX, enemy.gridY + 1, currentMap) && 
                     !isBombThere(enemy.gridX, enemy.gridY + 1, bombs) && 
                     !isEnemyThere(enemy.gridX, enemy.gridY + 1, enemies, &enemy))
            {
                enemy.gridY++;
                moved = true;
            }
        }
        if (enemy.direction == 2)
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
            else if (!isWall(enemy.gridX - 1, enemy.gridY, currentMap) && 
                     !isBombThere(enemy.gridX - 1, enemy.gridY, bombs) &&  
                     !isEnemyThere(enemy.gridX - 1, enemy.gridY, enemies, &enemy))
            {
                enemy.gridX--;
                moved = true;
            }
        }
        if (enemy.direction == 3)
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
            else if (!isWall(enemy.gridX + 1, enemy.gridY, currentMap) && 
                     !isBombThere(enemy.gridX + 1, enemy.gridY, bombs) && 
                     !isEnemyThere(enemy.gridX + 1, enemy.gridY, enemies, &enemy))
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
                else if (!isWall(enemy.gridX - 1, enemy.gridY, currentMap) && 
                         !isBombThere(enemy.gridX - 1, enemy.gridY, bombs) && 
                         !isEnemyThere(enemy.gridX - 1, enemy.gridY, enemies, &enemy))
                {
                    enemy.gridX--;
                    moved = true;
                    enemy.direction = 2;
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
                else if (!isWall(enemy.gridX + 1, enemy.gridY, currentMap) && 
                         !isBombThere(enemy.gridX + 1, enemy.gridY, bombs) && 
                         !isEnemyThere(enemy.gridX + 1, enemy.gridY, enemies, &enemy))
                {
                    enemy.gridX++;
                    moved = true;
                    enemy.direction = 3;
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
                    else if (!isWall(enemy.gridX, enemy.gridY - 1, currentMap) && 
                             !isBombThere(enemy.gridX, enemy.gridY - 1, bombs) && 
                             !isEnemyThere(enemy.gridX, enemy.gridY - 1, enemies, &enemy))
                    {
                        enemy.gridY--;
                        enemy.direction = 0;
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
                    else if (!isWall(enemy.gridX, enemy.gridY + 1, currentMap) && 
                             !isBombThere(enemy.gridX, enemy.gridY + 1, bombs) &&
                             !isEnemyThere(enemy.gridX, enemy.gridY + 1, enemies, &enemy))
                    {
                        enemy.gridY++;
                        enemy.direction = 1;
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
            else if (!isWall(nextX, nextY, currentMap) && 
                     !isBombThere(nextX, nextY, bombs) &&
                     !isEnemyThere(nextX, nextY, enemies, &enemy))
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
            currentMap[enemy.breakY][enemy.breakX] = '.';
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
            else if (!isWall(enemy.gridX - 1, enemy.gridY, currentMap) && !isEnemyThere(enemy.gridX - 1, enemy.gridY, enemies, &enemy))
            {
                enemy.gridX--;
                moved = true;
                enemy.direction = 2;  
            }
            else if (currentMap[enemy.gridY][enemy.gridX - 1] == '*')
            {
                enemy.breakingWall = true;
                enemy.breakX = enemy.gridX - 1;
                enemy.breakY = enemy.gridY;
                enemy.breakTimer.restart();
                enemy.direction = 2;  
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
            else if (!isWall(enemy.gridX + 1, enemy.gridY, currentMap) && !isEnemyThere(enemy.gridX + 1, enemy.gridY, enemies, &enemy))
            {
                enemy.gridX++;
                moved = true;
                enemy.direction = 3; 
            }
            else if (currentMap[enemy.gridY][enemy.gridX + 1] == '*')
            {
                enemy.breakingWall = true;
                enemy.breakX = enemy.gridX + 1;
                enemy.breakY = enemy.gridY;
                enemy.breakTimer.restart();
                enemy.direction = 3;  
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
                else if (!isWall(enemy.gridX, enemy.gridY - 1, currentMap) && !isEnemyThere(enemy.gridX, enemy.gridY - 1, enemies, &enemy))
                {
                    enemy.gridY--;
                    moved = true;
                    enemy.direction = 0; 
                }
                else if (currentMap[enemy.gridY - 1][enemy.gridX] == '*')
                {
                    enemy.breakingWall = true;
                    enemy.breakX = enemy.gridX;
                    enemy.breakY = enemy.gridY - 1;
                    enemy.breakTimer.restart();
                    enemy.direction = 0; 
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
                else if (!isWall(enemy.gridX, enemy.gridY + 1, currentMap) && !isEnemyThere(enemy.gridX, enemy.gridY + 1, enemies, &enemy))
                {
                    enemy.gridY++;
                    moved = true;
                    enemy.direction = 1;
                }
                else if (currentMap[enemy.gridY + 1][enemy.gridX] == '*')
                {
                    enemy.breakingWall = true;
                    enemy.breakX = enemy.gridX;
                    enemy.breakY = enemy.gridY + 1;
                    enemy.breakTimer.restart();
                    enemy.direction = 1; 
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
            else if (!isWall(nextX, nextY, currentMap) && !isEnemyThere(nextX, nextY, enemies, &enemy))
            {
                enemy.gridX = nextX;
                enemy.gridY = nextY;
                moved = true;
               
            }
            else if (currentMap[nextY][nextX] == '*')
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
  }

for (int i = 0; i < powerUps.size(); i++)
{
    // 1. OYUNCU POWERUP'ı
    if (playerGridX == powerUps[i].gridX && playerGridY == powerUps[i].gridY)
    {
        if (powerUps[i].type == 0) bombRange++;
        else if (powerUps[i].type == 1) maxBombs++;
        else if (powerUps[i].type == 2)
        {
            moveDelay -= 0.02f;
            if (moveDelay < 0.05f) moveDelay = 0.05f;
        }
        powerUps.erase(powerUps.begin() + i);
        i--;
    }
    // 2. OYUNCU POWERUP'ı (SADECE MULTIPLAYER)
    else if (gameState == MULTIPLAYER && player2GridX == powerUps[i].gridX && player2GridY == powerUps[i].gridY)
    {
        if (powerUps[i].type == 0) player2BombRange++;
        else if (powerUps[i].type == 1) player2MaxBombs++;
        else if (powerUps[i].type == 2)
        {
            player2MoveDelay -= 0.02f;
            if (player2MoveDelay < 0.05f) player2MoveDelay = 0.05f;
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

if (gameState == MULTIPLAYER)
{
    if (player2Moving)
    {
        if (player2AnimationClock.getElapsedTime().asSeconds() > 0.15f)
        {
            player2CurrentFrame++;
            if (player2CurrentFrame > 2)
            {
                player2CurrentFrame = 0;
            }
            player2AnimationClock.restart();
        }
    }
    else
    {
        player2CurrentFrame = 1;
    }
    
    player2Sprite.setTextureRect(sf::IntRect(
        {player2CurrentFrame * FRAME_WIDTH,
        player2CurrentDirection * FRAME_HEIGHT},
        {FRAME_WIDTH, FRAME_HEIGHT}
    ));

    player2Sprite.setPosition({
        static_cast<float>(player2GridX * TILE_SIZE),
        static_cast<float>(player2GridY * TILE_SIZE)
    });
}
    
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
    if (currentMap[y][x] == '#')
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
    else if (currentMap[y][x] == '*')
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
        explosion.owner = bombs[i].owner;

        explosions.push_back(explosion);
    bombs.erase(bombs.begin() + i);
    i--;
    }
    
   }
   

for (Bomb& bomb : bombs)
{
    float elapsed = bomb.timer.getElapsedTime().asSeconds();
    
   
    bomb.scale = 0.7f + (elapsed / 2.0f) * 1.0f;  
    
    if (bomb.scale > 1.7f) bomb.scale = 1.7f;
    
    float bombSize = 48.0f * bomb.scale;
    float offset = (TILE_SIZE - bombSize) / 2.0f;
    
    bombSprite.setScale({
        bombSize / bombTexture.getSize().x,
        bombSize / bombTexture.getSize().y
    });
    
    bombSprite.setPosition({
        static_cast<float>(bomb.gridX * TILE_SIZE + offset),
        static_cast<float>(bomb.gridY * TILE_SIZE + offset)
    });
    
    window.draw(bombSprite);
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
    // Patlamanın menzilini owner'a göre belirle
    int range = (explosion.owner == 1) ? bombRange : player2BombRange;
    
    bool hitPlayer = false;

    // ========== MERKEZ HASAR ==========
    if (playerGridX == explosion.gridX && playerGridY == explosion.gridY)
    {
        hitPlayer = true;
    }

    // 2. oyuncu merkez hasarı
    if (gameState == MULTIPLAYER)
    {
        if (player2GridX == explosion.gridX && player2GridY == explosion.gridY)
        {
            if (player2DamageClock.getElapsedTime().asSeconds() > 1.0f)
            {
                player2Health--;
                player2DamageClock.restart();
                if (player2Health <= 0)
                {
                    gameState = SINGLEPLAYER;
                }
            }
        }
    }

    // ========== ÜST YÖN ==========
    for (int i = 1; i <= range; i++)
    {
        int y = explosion.gridY - i;
        if (y < 0) break;
        if (currentMap[y][explosion.gridX] == '#') break;

        // 1. oyuncu hasarı
        if (playerGridX == explosion.gridX && playerGridY == y)
        {
            hitPlayer = true;
        }

        // 2. oyuncu hasarı
        if (gameState == MULTIPLAYER)
        {
            if (player2GridX == explosion.gridX && player2GridY == y)
            {
                if (player2DamageClock.getElapsedTime().asSeconds() > 1.0f)
                {
                    player2Health--;
                    player2DamageClock.restart();
                    if (player2Health <= 0) gameState = SINGLEPLAYER;
                }
            }
        }

        if (currentMap[y][explosion.gridX] == '*') break;
    }

    // ========== ALT YÖN ==========
    for (int i = 1; i <= range; i++)
    {
        int y = explosion.gridY + i;
        if (y >= MAP_HEIGHT) break;
        if (currentMap[y][explosion.gridX] == '#') break;

        // 1. oyuncu hasarı
        if (playerGridX == explosion.gridX && playerGridY == y)
        {
            hitPlayer = true;
        }

        // 2. oyuncu hasarı
        if (gameState == MULTIPLAYER)
        {
            if (player2GridX == explosion.gridX && player2GridY == y)
            {
                if (player2DamageClock.getElapsedTime().asSeconds() > 1.0f)
                {
                    player2Health--;
                    player2DamageClock.restart();
                    if (player2Health <= 0) gameState = SINGLEPLAYER;
                }
            }
        }

        if (currentMap[y][explosion.gridX] == '*') break;
    }

    // ========== SOL YÖN ==========
    for (int i = 1; i <= range; i++)
    {
        int x = explosion.gridX - i;
        if (x < 0) break;
        if (currentMap[explosion.gridY][x] == '#') break;

        // 1. oyuncu hasarı
        if (playerGridX == x && playerGridY == explosion.gridY)
        {
            hitPlayer = true;
        }

        // 2. oyuncu hasarı
        if (gameState == MULTIPLAYER)
        {
            if (player2GridX == x && player2GridY == explosion.gridY)
            {
                if (player2DamageClock.getElapsedTime().asSeconds() > 1.0f)
                {
                    player2Health--;
                    player2DamageClock.restart();
                    if (player2Health <= 0) gameState = SINGLEPLAYER;
                }
            }
        }

        if (currentMap[explosion.gridY][x] == '*') break;
    }

    // ========== SAĞ YÖN ==========
    for (int i = 1; i <= range; i++)
    {
        int x = explosion.gridX + i;
        if (x >= MAP_WIDTH) break;
        if (currentMap[explosion.gridY][x] == '#') break;

        // 1. oyuncu hasarı
        if (playerGridX == x && playerGridY == explosion.gridY)
        {
            hitPlayer = true;
        }

        // 2. oyuncu hasarı
        if (gameState == MULTIPLAYER)
        {
            if (player2GridX == x && player2GridY == explosion.gridY)
            {
                if (player2DamageClock.getElapsedTime().asSeconds() > 1.0f)
                {
                    player2Health--;
                    player2DamageClock.restart();
                    if (player2Health <= 0) gameState = SINGLEPLAYER;
                }
            }
        }

        if (currentMap[explosion.gridY][x] == '*') break;
    }

    // ========== OYUNCU HASARINI UYGULA ==========
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

    if (currentMap[gridY][gridX] == '*')
    {
        currentMap[gridY][gridX] = '.';
    }

    // üst
    for (int i = 1; i <= bombRange; i++)
    {
        int y = gridY - i;

        if (currentMap[y][gridX] == '#')
            break;

        if (currentMap[y][gridX] == '*')
        {
            currentMap[y][gridX] = '.';
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

    // alt
    for (int i = 1; i <= bombRange; i++)
    {
        int y = gridY + i;

        if (currentMap[y][gridX] == '#')
            break;

        if (currentMap[y][gridX] == '*')
        {
            currentMap[y][gridX] = '.';
             if (rand() % 100 < 35)
    {
        PowerUp p;

        p.gridX = gridX;
        p.gridY = y;

        p.type = rand() % 3;

        powerUps.push_back(p);
    }
            break;
        }
    }

    // sol
    for (int i = 1; i <= bombRange; i++)
    {
        int x = gridX - i;

        if (currentMap[gridY][x] == '#')
            break;

        if (currentMap[gridY][x] == '*')
        {
            currentMap[gridY][x] = '.';

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

    // sag
    for (int i = 1; i <= bombRange; i++)
    {
        int x = gridX + i;

        if (currentMap[gridY][x] == '#')
            break;

        if (currentMap[gridY][x] == '*')
        {
            currentMap[gridY][x] = '.';

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

    for (const Explosion& explosion : explosions)
{
    for (Enemy& enemy : enemies)
    {
        if (enemy.damaged)
            continue;

        bool hit = false;

        if (enemy.gridX == explosion.gridX &&
            enemy.gridY == explosion.gridY)
        {
            hit = true;
        }

        // üst
        for (int i = 1; i <= bombRange; i++)
        {
            int y = explosion.gridY - i;

            if (currentMap[y][explosion.gridX] == '#')
                break;

            if (enemy.gridX == explosion.gridX &&
                enemy.gridY == y)
            {
                hit = true;
            }

            if (currentMap[y][explosion.gridX] == '*')
                break;
        }

        // alt
        for (int i = 1; i <= bombRange; i++)
        {
            int y = explosion.gridY + i;

            if (currentMap[y][explosion.gridX] == '#')
                break;

            if (enemy.gridX == explosion.gridX &&
                enemy.gridY == y)
            {
                hit = true;
            }

            if (currentMap[y][explosion.gridX] == '*')
                break;
        }

        // sol
        for (int i = 1; i <= bombRange; i++)
        {
            int x = explosion.gridX - i;

            if (currentMap[explosion.gridY][x] == '#')
                break;

            if (enemy.gridX == x &&
                enemy.gridY == explosion.gridY)
            {
                hit = true;
            }

            if (currentMap[explosion.gridY][x] == '*')
                break;
        }

        // sag
        for (int i = 1; i <= bombRange; i++)
        {
            int x = explosion.gridX + i;

            if (currentMap[explosion.gridY][x] == '#')
                break;

            if (enemy.gridX == x &&
                enemy.gridY == explosion.gridY)
            {
                hit = true;
            }

            if (currentMap[explosion.gridY][x] == '*')
                break;
        }

        if (hit)
        {
            enemy.health--;
            enemy.damaged = true;
        }
    }
}
     
  for (const Explosion& explosion : explosions)
{
    // Patlamanın menzilini owner'a göre belirle
    int range = (explosion.owner == 1) ? bombRange : player2BombRange;
    
    // ========== MERKEZ ÇİZİM ==========
    explosionSprite.setPosition({
        static_cast<float>(explosion.gridX * TILE_SIZE),
        static_cast<float>(explosion.gridY * TILE_SIZE)
    });
    window.draw(explosionSprite);
    
    // ========== ÜST YÖN ÇİZİM ==========
    for (int i = 1; i <= range; i++)
    {
        int y = explosion.gridY - i;
        if (y < 0) break;
        if (currentMap[y][explosion.gridX] == '#') break;
        
        explosionSprite.setPosition({
            static_cast<float>(explosion.gridX * TILE_SIZE),
            static_cast<float>(y * TILE_SIZE)
        });
        window.draw(explosionSprite);
        
        if (currentMap[y][explosion.gridX] == '*') break;
    }
    
    // ========== ALT YÖN ÇİZİM ==========
    for (int i = 1; i <= range; i++)
    {
        int y = explosion.gridY + i;
        if (y >= MAP_HEIGHT) break;
        if (currentMap[y][explosion.gridX] == '#') break;
        
        explosionSprite.setPosition({
            static_cast<float>(explosion.gridX * TILE_SIZE),
            static_cast<float>(y * TILE_SIZE)
        });
        window.draw(explosionSprite);
        
        if (currentMap[y][explosion.gridX] == '*') break;
    }
    
    // ========== SOL YÖN ÇİZİM ==========
    for (int i = 1; i <= range; i++)
    {
        int x = explosion.gridX - i;
        if (x < 0) break;
        if (currentMap[explosion.gridY][x] == '#') break;
        
        explosionSprite.setPosition({
            static_cast<float>(x * TILE_SIZE),
            static_cast<float>(explosion.gridY * TILE_SIZE)
        });
        window.draw(explosionSprite);
        
        if (currentMap[explosion.gridY][x] == '*') break;
    }
    
    // ========== SAĞ YÖN ÇİZİM ==========
    for (int i = 1; i <= range; i++)
    {
        int x = explosion.gridX + i;
        if (x >= MAP_WIDTH) break;
        if (currentMap[explosion.gridY][x] == '#') break;
        
        explosionSprite.setPosition({
            static_cast<float>(x * TILE_SIZE),
            static_cast<float>(explosion.gridY * TILE_SIZE)
        });
        window.draw(explosionSprite);
        
        if (currentMap[explosion.gridY][x] == '*') break;
    }
}
    
    
   }

for (int i = 0; i < enemies.size(); i++)
{
    if (enemies[i].health <= 0)
    {
        enemies.erase(enemies.begin() + i);
        i--;
    }
}

if (gameState == SINGLEPLAYER)
{
for (Enemy& enemy : enemies)
{
    if (enemy.animationClock.getElapsedTime().asSeconds() > 0.15f)
    {
        enemy.currentFrame++;
        if (enemy.currentFrame > 2)
        {
            enemy.currentFrame = 0;
        }
        enemy.animationClock.restart();
    }
    
    int directionRow = 0;
    switch(enemy.direction)
    {
        case 0: directionRow = 3; break; 
        case 1: directionRow = 0; break; 
        case 2: directionRow = 1; break; 
        case 3: directionRow = 2; break; 
    }
    
    sf::IntRect textureRect(
        {enemy.currentFrame * 48, directionRow * 48},
        {48, 48}
    );
    
    if (enemy.type == 0)
    {
        foxSprite.setTextureRect(textureRect);
        foxSprite.setPosition({
            static_cast<float>(enemy.gridX * TILE_SIZE),
            static_cast<float>(enemy.gridY * TILE_SIZE)
        });
        window.draw(foxSprite);
    }
    else if (enemy.type == 1)
    {
        wolfSprite.setTextureRect(textureRect);
        wolfSprite.setPosition({
            static_cast<float>(enemy.gridX * TILE_SIZE),
            static_cast<float>(enemy.gridY * TILE_SIZE)
        });
        window.draw(wolfSprite);
    }
    else if (enemy.type == 2)
    {
        butcherSprite.setTextureRect(textureRect);
        butcherSprite.setPosition({
            static_cast<float>(enemy.gridX * TILE_SIZE),
            static_cast<float>(enemy.gridY * TILE_SIZE)
        });
        window.draw(butcherSprite);
    }
}
}

for (const PowerUp& p : powerUps)
{
    if (p.type == 0)
    {
        meatSprite.setPosition({
            static_cast<float>(p.gridX * TILE_SIZE),
            static_cast<float>(p.gridY * TILE_SIZE)
        });
        window.draw(meatSprite);
    }
    else if (p.type == 1)
    {
        cornSprite.setPosition({
            static_cast<float>(p.gridX * TILE_SIZE),
            static_cast<float>(p.gridY * TILE_SIZE)
        });
        window.draw(cornSprite);
    }
    else if (p.type == 2)
    {
        wormSprite.setPosition({
            static_cast<float>(p.gridX * TILE_SIZE),
            static_cast<float>(p.gridY * TILE_SIZE)
        });
        window.draw(wormSprite);
    }
}
if (gameState == MULTIPLAYER)
{
    window.draw(player2Sprite);
}
 window.draw(playerSprite);
    window.display();
} 

return 0; 
}