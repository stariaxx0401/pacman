#include <SFML/Graphics.hpp>
#include <cmath> // Açısal ve matematiksel işlemler için
#include <string> // puan gösterimi için
#include <cstdlib> // Rastgele yön seçimi için

// hücre boyutu
const int CELL_SIZE = 20;

// harita boyutu
const int MAP_ROWS = 21;
const int MAP_COLS = 19;

// haritanın grid yapısını matematiksel olarak tanımlar.
// yol=0 duvar=1
int map[MAP_ROWS][MAP_COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,2,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,0,1},
    {1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1},
    {1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1},
    {1,1,1,1,0,1,0,0,0,0,0,0,0,1,0,1,1,1,1},
    {1,1,1,1,0,1,0,1,1,0,1,1,0,1,0,1,1,1,1},
    {0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0},
    {1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1},
    {1,1,1,1,0,1,0,0,0,0,0,0,0,1,0,1,1,1,1},
    {1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
    {1,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,1},
    {1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1},
    {1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

class Ghost {
public:
    sf::CircleShape shape;
    float speed = 1.5f;
    int currentDirection = 0;
    sf::Color color;
    sf::Clock aiClock;
    float startX, startY;

    Ghost(float startX_in, float startY_in, sf::Color ghostColor) {
        startX = startX_in;
        startY = startY_in;
        color = ghostColor;
        shape.setRadius(CELL_SIZE / 2 - 2);
        shape.setFillColor(ghostColor);
        shape.setPosition(startX, startY);
    }

    void respawn() {
        shape.setPosition(startX, startY);
        currentDirection = 0;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    bool canGoDirection(int mapArray[MAP_ROWS][MAP_COLS], int row, int col, int dir) {
        int nr = row + (dir == 3 ? 1 : dir == 2 ? -1 : 0);
        int nc = col + (dir == 0 ? 1 : dir == 1 ? -1 : 0);
        if (nr < 0 || nr >= MAP_ROWS || nc < 0 || nc >= MAP_COLS) return false;
        return mapArray[nr][nc] != 1;
    }

    void move(int mapArray[MAP_ROWS][MAP_COLS]) {
        float x = shape.getPosition().x;
        float y = shape.getPosition().y;
        float radius = CELL_SIZE / 2 - 2;
        float nextX = x; float nextY = y;
        if (currentDirection == 0) nextX += speed;
        else if (currentDirection == 1) nextX -= speed;
        else if (currentDirection == 2) nextY -= speed;
        else if (currentDirection == 3) nextY += speed;

        int leftCol = (int)(nextX) / CELL_SIZE;
        int rightCol = (int)(nextX + radius * 2) / CELL_SIZE;
        int topRow = (int)(nextY) / CELL_SIZE;
        int bottomRow = (int)(nextY + radius * 2) / CELL_SIZE;

        bool canMove = (topRow >= 0 && bottomRow < MAP_ROWS && leftCol >= 0 && rightCol < MAP_COLS) &&
                       (mapArray[topRow][leftCol] != 1 && mapArray[topRow][rightCol] != 1 &&
                        mapArray[bottomRow][leftCol] != 1 && mapArray[bottomRow][rightCol] != 1);

        if (canMove) {
            shape.setPosition(nextX, nextY);
        } else {
            currentDirection = rand() % 4;
        }
    }

    void moveTowards(int mapArray[MAP_ROWS][MAP_COLS], float pacmanX, float pacmanY) {
        float radius = CELL_SIZE / 2 - 2;
        float cx = shape.getPosition().x + radius;
        float cy = shape.getPosition().y + radius;
        int col = (int)(cx / CELL_SIZE);
        int row = (int)(cy / CELL_SIZE);

        if (aiClock.getElapsedTime().asSeconds() > 0.2f) {
            float diffX = pacmanX - cx;
            float diffY = pacmanY - cy;
            int dirs[4];
            if (std::abs(diffX) > std::abs(diffY)) {
                dirs[0] = (diffX > 0) ? 0 : 1;
                dirs[1] = (diffY > 0) ? 3 : 2;
                dirs[2] = (dirs[1] == 3) ? 2 : 3;
                dirs[3] = (dirs[0] == 0) ? 1 : 0;
            } else {
                dirs[0] = (diffY > 0) ? 3 : 2;
                dirs[1] = (diffX > 0) ? 0 : 1;
                dirs[2] = (dirs[1] == 0) ? 1 : 0;
                dirs[3] = (dirs[0] == 3) ? 2 : 3;
            }

            bool foundDir = false;
            for (int i = 0; i < 4; i++) {
                if (canGoDirection(mapArray, row, col, dirs[i])) {
                    currentDirection = dirs[i];
                    foundDir = true;
                    break;
                }
            }
            if (!foundDir && !canGoDirection(mapArray, row, col, currentDirection)) {
                currentDirection = rand() % 4;
            }
            aiClock.restart();
        }

        float nextX = shape.getPosition().x;
        float nextY = shape.getPosition().y;
        if (currentDirection == 0) nextX += speed;
        else if (currentDirection == 1) nextX -= speed;
        else if (currentDirection == 2) nextY -= speed;
        else if (currentDirection == 3) nextY += speed;

        int lc = (int)(nextX + 2) / CELL_SIZE;
        int rc = (int)(nextX + radius * 2 - 2) / CELL_SIZE;
        int tr = (int)(nextY + 2) / CELL_SIZE;
        int br = (int)(nextY + radius * 2 - 2) / CELL_SIZE;

        if (tr >= 0 && br < MAP_ROWS && lc >= 0 && rc < MAP_COLS &&
            mapArray[tr][lc] != 1 && mapArray[tr][rc] != 1 &&
            mapArray[br][lc] != 1 && mapArray[br][rc] != 1) {
            shape.setPosition(nextX, nextY);
        } else {
            currentDirection = rand() % 4;
        }
    }
};

class Pacman {
public:
    sf::ConvexShape shape;
    float speed = 2.0f;
    int score = 0;
    sf::Clock animationClock;
    float biteAngle = 30.0f;
    bool biting = false;
    int currentDirection = 0;
    float startX, startY; // Başlangıç pozisyonu için

    Pacman() {
        shape.setFillColor(sf::Color::Yellow);
        float radius = CELL_SIZE / 2 - 2;
        startX = 1 * CELL_SIZE + (CELL_SIZE / 2) - radius;
        startY = 1 * CELL_SIZE + (CELL_SIZE / 2) - radius;
        shape.setPosition(startX, startY);
        updateShape(radius);
    }

    // YENİ: Pacman için Yeniden Doğma
    void respawn() {
        shape.setPosition(startX, startY);
        currentDirection = 0;
    }

    void updateShape(float radius) {
        int pointCount = 20;
        shape.setPointCount(pointCount + 2);
        shape.setPoint(0, sf::Vector2f(radius, radius));
        float radBite = (biteAngle / 2.0f) * 3.14159f / 180.0f;
        for (int i = 0; i <= pointCount; i++) {
            float angle = radBite + i * (2 * 3.14159f - 2 * radBite) / pointCount;
            float x = radius + radius * std::cos(angle);
            float y = radius + radius * std::sin(angle);
            shape.setPoint(i + 1, sf::Vector2f(x, y));
        }
    }

    void handleInput(int mapArray[MAP_ROWS][MAP_COLS]) {
        float nextX = shape.getPosition().x;
        float nextY = shape.getPosition().y;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            currentDirection = 2;
            int col = (shape.getPosition().x + (CELL_SIZE / 2 - 2)) / CELL_SIZE;
            nextX = col * CELL_SIZE + (CELL_SIZE / 2) - (CELL_SIZE / 2 - 2);
            nextY -= speed;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            currentDirection = 3;
            int col = (shape.getPosition().x + (CELL_SIZE / 2 - 2)) / CELL_SIZE;
            nextX = col * CELL_SIZE + (CELL_SIZE / 2) - (CELL_SIZE / 2 - 2);
            nextY += speed;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            currentDirection = 1;
            int row = (shape.getPosition().y + (CELL_SIZE / 2 - 2)) / CELL_SIZE;
            nextY = row * CELL_SIZE + (CELL_SIZE / 2) - (CELL_SIZE / 2 - 2);
            nextX -= speed;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            currentDirection = 0;
            int row = (shape.getPosition().y + (CELL_SIZE / 2 - 2)) / CELL_SIZE;
            nextY = row * CELL_SIZE + (CELL_SIZE / 2) - (CELL_SIZE / 2 - 2);
            nextX += speed;
        }

        float radius = CELL_SIZE / 2 - 2;
        int leftCol = (nextX) / CELL_SIZE;
        int rightCol = (nextX + (radius * 2)) / CELL_SIZE;
        int topRow = (nextY) / CELL_SIZE;
        int bottomRow = (nextY + (radius * 2)) / CELL_SIZE;

        if (topRow >= 0 && bottomRow < MAP_ROWS && leftCol >= 0 && rightCol < MAP_COLS) {
            if (mapArray[topRow][leftCol] != 1 && mapArray[topRow][rightCol] != 1 &&
                mapArray[bottomRow][leftCol] != 1 && mapArray[bottomRow][rightCol] != 1) {
                shape.setPosition(nextX, nextY);
                int centerGridX = (nextX + radius) / CELL_SIZE;
                int centerGridY = (nextY + radius) / CELL_SIZE;
                if (centerGridX >= 0 && centerGridX < MAP_COLS && centerGridY >= 0 && centerGridY < MAP_ROWS) {
                    if (mapArray[centerGridY][centerGridX] == 0) {
                        mapArray[centerGridY][centerGridX] = 2;
                        score += 10;
                    }
                }
            }
        }
        updateAnimation();
    }

    void updateAnimation() {
        if (animationClock.getElapsedTime().asSeconds() > 0.015f) {
            if (biting) {
                biteAngle += 8.0f;
                if (biteAngle >= 55.0f) biting = false;
            } else {
                biteAngle -= 8.0f;
                if (biteAngle <= 5.0f) biting = true;
            }
            float radius = CELL_SIZE / 2 - 2;
            updateShape(radius);
            animationClock.restart();
        }
    }

    void draw(sf::RenderWindow& window) {
        float rotation = 0.0f;
        if (currentDirection == 1) rotation = 180.0f;
        else if (currentDirection == 2) rotation = 270.0f;
        else if (currentDirection == 3) rotation = 90.0f;
        float radius = CELL_SIZE / 2 - 2;
        shape.setOrigin(radius, radius);
        sf::Vector2f pos = shape.getPosition();
        shape.setPosition(pos.x + radius, pos.y + radius);
        shape.setRotation(rotation);
        window.draw(shape);
        shape.setPosition(pos.x, pos.y);
        shape.setOrigin(0, 0);
    }
};

bool checkCollision(sf::Vector2f pos1, sf::Vector2f pos2, float radius) {
    float distance = std::sqrt(std::pow(pos1.x - pos2.x, 2) + std::pow(pos1.y - pos2.y, 2));
    return distance < (radius * 1.5f);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(MAP_COLS * CELL_SIZE, MAP_ROWS * CELL_SIZE + 30), "Pacman Test");
    window.setFramerateLimit(60);
    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(16);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(5, MAP_ROWS * CELL_SIZE + 5);

    Pacman player;
    float radius = CELL_SIZE / 2 - 2;
    float offsetX = (CELL_SIZE / 2) - radius;
    float offsetY = (CELL_SIZE / 2) - radius;

    Ghost blinky(9 * CELL_SIZE + offsetX, 7 * CELL_SIZE + offsetY, sf::Color::Red);
    Ghost pinky(10 * CELL_SIZE + offsetX, 7 * CELL_SIZE + offsetY, sf::Color::Magenta);
    Ghost inky(8 * CELL_SIZE + offsetX, 7 * CELL_SIZE + offsetY, sf::Color::Cyan);
    Ghost clyde(11 * CELL_SIZE + offsetX, 7 * CELL_SIZE + offsetY, sf::Color(255, 127, 0));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        blinky.moveTowards(map, player.shape.getPosition().x, player.shape.getPosition().y);
        pinky.move(map);
        inky.move(map);
        clyde.move(map);
        player.handleInput(map);

        float pacRadius = CELL_SIZE / 2 - 2;
        sf::Vector2f pacPos = player.shape.getPosition();

        // ÇARPIŞMA KONTROLÜ VE YENİDEN DOĞMA
        if (checkCollision(pacPos, blinky.shape.getPosition(), pacRadius) ||
            checkCollision(pacPos, pinky.shape.getPosition(), pacRadius) ||
            checkCollision(pacPos, inky.shape.getPosition(), pacRadius) ||
            checkCollision(pacPos, clyde.shape.getPosition(), pacRadius)) {
            
            printf("EYVAH YAKALANDIN! PUANIN: %d\n", player.score);
            
            // KARAKTERLERİ IŞINLA
            player.respawn();
            blinky.respawn();
            pinky.respawn();
            inky.respawn();
            clyde.respawn();
            
            // window.close(); // BU SATIRI SİLDİK, ARTIK OYUN KAPANMAZ!
        }

        window.clear(sf::Color::Black);

        sf::RectangleShape wall(sf::Vector2f(CELL_SIZE - 1.0f, CELL_SIZE - 1.0f));
        wall.setFillColor(sf::Color::Blue);
        sf::CircleShape food(2);
        food.setFillColor(sf::Color::White);

        for (int row = 0; row < MAP_ROWS; row++) {
            for (int col = 0; col < MAP_COLS; col++) {
                if (map[row][col] == 1) {
                    wall.setPosition(col * CELL_SIZE, row * CELL_SIZE);
                    window.draw(wall);
                } else if (map[row][col] == 0) {
                    food.setPosition(col * CELL_SIZE + CELL_SIZE / 2 - 2, row * CELL_SIZE + CELL_SIZE / 2 - 2);
                    window.draw(food);
                }
            }
        }

        player.draw(window);
        blinky.draw(window);
        pinky.draw(window);
        inky.draw(window);
        clyde.draw(window);
        scoreText.setString("Puan: " + std::to_string(player.score));
        window.draw(scoreText);
        window.display();
    }
    return 0;
}