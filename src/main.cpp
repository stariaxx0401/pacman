#include <SFML/Graphics.hpp>// SFML grafik kütüphanesini dahil etme
#include <cmath> // Açısal ve matematiksel işlemler için
#include <string> // puan gösterimi için
#include <cstdlib> // Rastgele yön seçimi için

// oyun durumlarını tanımlamak için enum
enum GameState {
    START,
    PLAYING,
    GAME_OVER
};
GameState currentState = START;// oyunun başlangıç durumunu START olarak ayarlama

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

const int initialMap[MAP_ROWS][MAP_COLS] = {
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
void resetMap() {
    for(int r=0;r<MAP_ROWS;r++) {
        for(int c=0;c<MAP_COLS;c++) {
           map[r][c] = initialMap[r][c];
        }
    }
}

class Ghost {
public:
    sf::CircleShape shape;// Hayaletin görsel temsilcisi
    float speed = 1.5f;// Hayaletin hareket hızı
    int currentDirection = 0;// 0: sağ, 1: sol, 2: yukarı, 3: aşağı
    sf::Color color;// Hayaletin rengi
    sf::Clock aiClock;// Yapay zeka hareketlerini kontrol etmek için zamanlayıcı
    float startX, startY;// Hayaletin başlangıç pozisyonu

    Ghost(float startX_in, float startY_in, sf::Color ghostColor) {
        startX = startX_in;// Başlangıç X pozisyonu
        startY = startY_in;// Başlangıç Y pozisyonu
        color = ghostColor;
        shape.setRadius(CELL_SIZE / 2 - 2);// Hayaletin yarıçapı
        shape.setFillColor(ghostColor);// Hayaletin rengi
        shape.setPosition(startX, startY);// Hayaletin başlangıç pozisyonu
    }

    void respawn() {
        shape.setPosition(startX, startY);// Hayaletin başlangıç pozisyonuna geri dönmesi
        currentDirection = 0;// Hareket yönünün sıfırlanması
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    bool canGoDirection(int mapArray[MAP_ROWS][MAP_COLS], int row, int col, int dir) {
        int nr = row + (dir == 3 ? 1 : dir == 2 ? -1 : 0);// Yeni satır hesaplama
        int nc = col + (dir == 0 ? 1 : dir == 1 ? -1 : 0);// Yeni sütun hesaplama
        if (nr < 0 || nr >= MAP_ROWS || nc < 0 || nc >= MAP_COLS) return false;// Harita sınırlarının dışına çıkmayı engelleme
        return mapArray[nr][nc] != 1;// Duvar olmayan hücrelere geçiş izni verme
    }

    void move(int mapArray[MAP_ROWS][MAP_COLS]) {
        float x = shape.getPosition().x;// Mevcut X pozisyonu
        float y = shape.getPosition().y;// Mevcut Y pozisyonu
        float radius = CELL_SIZE / 2 - 2;// Hayaletin yarıçapı
        float nextX = x; float nextY = y;// Bir sonraki pozisyonun başlangıç değeri
        if (currentDirection == 0) nextX += speed;// Sağa hareket
        else if (currentDirection == 1) nextX -= speed;// Sola hareket
        else if (currentDirection == 2) nextY -= speed;// Yukarı hareket
        else if (currentDirection == 3) nextY += speed;// Aşağı hareket

        int leftCol = (int)(nextX) / CELL_SIZE;// Bir sonraki pozisyonun sol sütunu
        int rightCol = (int)(nextX + radius * 2) / CELL_SIZE;// Bir sonraki pozisyonun sağ sütunu
        int topRow = (int)(nextY) / CELL_SIZE;// Bir sonraki pozisyonun üst satırı
        int bottomRow = (int)(nextY + radius * 2) / CELL_SIZE;// Bir sonraki pozisyonun alt satırı

        bool canMove = (topRow >= 0 && bottomRow < MAP_ROWS && leftCol >= 0 && rightCol < MAP_COLS) &&
                       (mapArray[topRow][leftCol] != 1 && mapArray[topRow][rightCol] != 1 &&mapArray[bottomRow][leftCol] != 1 && mapArray[bottomRow][rightCol] != 1);

        if (canMove) {
            shape.setPosition(nextX, nextY);// Yeni pozisyona hareket
        } 
        else {
            currentDirection = rand() % 4;// Rastgele yeni bir yön seçme
        }
    }

    void moveTowards(int mapArray[MAP_ROWS][MAP_COLS], float pacmanX, float pacmanY) {
        float radius = CELL_SIZE / 2 - 2;// Hayaletin yarıçapı
        float cx = shape.getPosition().x + radius;// Hayaletin merkez X pozisyonu
        float cy = shape.getPosition().y + radius;// Hayaletin merkez Y pozisyonu
        int col = (int)(cx / CELL_SIZE);// Hayaletin bulunduğu sütun
        int row = (int)(cy / CELL_SIZE);// Hayaletin bulunduğu satır

        if (aiClock.getElapsedTime().asSeconds() > 0.2f) {
            float diffX = pacmanX - cx;// Pacman ile hayalet arasındaki X farkı
            float diffY = pacmanY - cy;// Pacman ile hayalet arasındaki Y farkı
            int dirs[4];// Hareket yönlerini öncelik sırasına göre tutacak dizi
            if (std::abs(diffX) > std::abs(diffY)) {// X ekseninde daha büyük fark varsa önce yatay hareket dene
                dirs[0] = (diffX > 0) ? 0 : 1;// Öncelikli yön sağ veya sol
                dirs[1] = (diffY > 0) ? 3 : 2;// İkincil yön aşağı veya yukarı
                dirs[2] = (dirs[1] == 3) ? 2 : 3;// İkincil yönün tersi
                dirs[3] = (dirs[0] == 0) ? 1 : 0;// Öncelikli yönün tersi
            } 
            else {
                dirs[0] = (diffY > 0) ? 3 : 2;// Öncelikli yön aşağı veya yukarı
                dirs[1] = (diffX > 0) ? 0 : 1;// İkincil yön sağ veya sol
                dirs[2] = (dirs[1] == 0) ? 1 : 0;// İkincil yönün tersi
                dirs[3] = (dirs[0] == 3) ? 2 : 3;// Öncelikli yönün tersi
            }

            bool foundDir = false;// Geçerli bir yön bulunup bulunmadığını kontrol etmek için bayrak
            for (int i = 0; i < 4; i++) {
                if (canGoDirection(mapArray, row, col, dirs[i])) {// Geçerli yön olup olmadığını kontrol et
                    currentDirection = dirs[i];// Geçerli yön olarak ayarla
                    foundDir = true;// Geçerli bir yön bululduğunu işaretle
                    break;
                }
            }
            if (!foundDir && !canGoDirection(mapArray, row, col, currentDirection)) {// Geçerli yön geçersizse ve yeni bir yön bulunamadıysa rastgele bir yön seç
                currentDirection = rand() % 4;// Rastgele yeni bir yön seç
            }
            aiClock.restart();// Yapay zeka hareket zamanlayıcısını sıfırla
        }

        float nextX = shape.getPosition().x;// Mevcut X pozisyonu
        float nextY = shape.getPosition().y;// Mevcut Y pozisyonu
        if (currentDirection == 0) nextX += speed;// Sağa hareket
        else if (currentDirection == 1) nextX -= speed;// Sola hareket
        else if (currentDirection == 2) nextY -= speed;// Yukarı hareket
        else if (currentDirection == 3) nextY += speed;// Aşağı hareket

        int lc = (int)(nextX + 2) / CELL_SIZE;// Bir sonraki pozisyonun sol sütunu
        int rc = (int)(nextX + radius * 2 - 2) / CELL_SIZE;// Bir sonraki pozisyonun sağ sütunu
        int tr = (int)(nextY + 2) / CELL_SIZE;// Bir sonraki pozisyonun üst satırı
        int br = (int)(nextY + radius * 2 - 2) / CELL_SIZE;// Bir sonraki pozisyonun alt satırı

        if (tr >= 0 && br < MAP_ROWS && lc >= 0 && rc < MAP_COLS && mapArray[tr][lc] != 1 && mapArray[tr][rc] != 1 && mapArray[br][lc] != 1 && mapArray[br][rc] != 1) {// Yeni pozisyona hareket izni kontrolü
            shape.setPosition(nextX, nextY);// Yeni pozisyona hareket
        } 
        else {
            currentDirection = rand() % 4;// Geçerli yön geçersizse rastgele yeni bir yön seç
        }
    }
};

class Pacman {
public:
    sf::ConvexShape shape;// Pacmanin görsel temsilcisi
    float speed = 2.0f;// Pacmanin hareket hızı
    int score = 0;// Pacmanin skoru
    int lives =3;// Pacmanin can sayısı
    sf::Clock animationClock;// Pacmanin animasyonunu kontrol etmek için zamanlayıcı
    float biteAngle = 30.0f;// Pacmanin ağzının açılma açısı
    bool biting = false;// Pacmanin ağzının açılıp kapanma durumunu kontrol etmek için 
    int currentDirection = 0;// 0: sağ 1: sol 2: yukarı 3: aşağı
    float startX, startY; // Başlangıç pozisyonu için

    Pacman() {
        shape.setFillColor(sf::Color::Yellow);// Pacmanin rengi
        float radius = CELL_SIZE / 2 - 2;// Pacmanin yarıçapı
        startX = 1 * CELL_SIZE + (CELL_SIZE / 2) - radius;// Pacmanin başlangıç X pozisyonu
        startY = 1 * CELL_SIZE + (CELL_SIZE / 2) - radius;// Pacmanin başlangıç Y pozisyonu
        shape.setPosition(startX, startY);// Pacmanin başlangıç pozisyonu
        updateShape(radius);// Pacmanin şeklinin güncellenmesi
    }

    // Pacman için Yeniden Doğma
    void respawn() {
        shape.setPosition(startX, startY);// Pacmanin başlangıç pozisyonuna geri dönmesi
        currentDirection = 0;// Hareket yönünün sıfırlanması
    }

    void updateShape(float radius) {
        int pointCount = 20;// Pacmanin ağzının açılıp kapanma animasyonu için nokta sayısı
        shape.setPointCount(pointCount + 2);// Pacmanin şeklinin nokta sayısını ayarlama
        shape.setPoint(0, sf::Vector2f(radius, radius));// Pacmanin merkez noktası
        float radBite = (biteAngle / 2.0f) * 3.14159f / 180.0f;// Pacmanin ağzının açılma açısını radyan cinsine çevirme
        for (int i = 0; i <= pointCount; i++) {// Pacmanin ağzının açılıp kapanma animasyonu için noktaların konumunu hesaplama
            float angle = radBite + i * (2 * 3.14159f - 2 * radBite) / pointCount;// Her nokta için açı hesaplama
            float x = radius + radius * std::cos(angle);// Noktanın X koordinatını hesaplama
            float y = radius + radius * std::sin(angle);// Noktanın Y koordinatını hesaplama
            shape.setPoint(i + 1, sf::Vector2f(x, y));// Noktanın konumunu şekle ekleme
        }
    }

    void handleInput(int mapArray[MAP_ROWS][MAP_COLS]) {// Klavye girişlerini işleme ve Pacmanin hareketini kontrol etme
        float nextX = shape.getPosition().x;// Mevcut X pozisyonu
        float nextY = shape.getPosition().y;// Mevcut Y pozisyonu
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {// Yukarı hareket tuşu kontrolü
            currentDirection = 2;// Hareket yönünü yukarı olarak ayarlama
            int col = (shape.getPosition().x + (CELL_SIZE / 2 - 2)) / CELL_SIZE;// Mevcut sütunu hesaplama
            nextX = col * CELL_SIZE + (CELL_SIZE / 2) - (CELL_SIZE / 2 - 2);// X pozisyonunu hücre merkezine hizalama
            nextY -= speed;// Y pozisyonunu yukarı hareket ettirme
        }
         else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {// Aşağı hareket tuşu kontrolü
            currentDirection = 3;// Hareket yönünü aşağı olarak ayarlama
            int col = (shape.getPosition().x + (CELL_SIZE / 2 - 2)) / CELL_SIZE;// Mevcut sütunu hesaplama
            nextX = col * CELL_SIZE + (CELL_SIZE / 2) - (CELL_SIZE / 2 - 2);// X pozisyonunu hücre merkezine hizalama
            nextY += speed;// Y pozisyonunu aşağı hareket ettirme
        }
         else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {// Sol hareket tuşu kontrolü
            currentDirection = 1;// Hareket yönünü sol olarak ayarlama
            int row = (shape.getPosition().y + (CELL_SIZE / 2 - 2)) / CELL_SIZE;// Mevcut satırı hesaplama
            nextY = row * CELL_SIZE + (CELL_SIZE / 2) - (CELL_SIZE / 2 - 2);// Y pozisyonunu hücre merkezine hizalama
            nextX -= speed;// X pozisyonunu sola hareket ettirme
        } 
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {// Sağ hareket tuşu kontrolü
            currentDirection = 0;// Hareket yönünü sağ olarak ayarlama
            int row = (shape.getPosition().y + (CELL_SIZE / 2 - 2)) / CELL_SIZE;// Mevcut satırı hesaplama
            nextY = row * CELL_SIZE + (CELL_SIZE / 2) - (CELL_SIZE / 2 - 2);// Y pozisyonunu hücre merkezine hizalama
            nextX += speed;// X pozisyonunu sağa hareket ettirme
        }

        float radius = CELL_SIZE / 2 - 2;// Pacmanin yarıçapı
        int leftCol = (nextX) / CELL_SIZE;// Bir sonraki pozisyonun sol sütunu
        int rightCol = (nextX + (radius * 2)) / CELL_SIZE;// Bir sonraki pozisyonun sağ sütunu
        int topRow = (nextY) / CELL_SIZE;// Bir sonraki pozisyonun üst satırı
        int bottomRow = (nextY + (radius * 2)) / CELL_SIZE;// Bir sonraki pozisyonun alt satırı

        if (topRow >= 0 && bottomRow < MAP_ROWS && leftCol >= 0 && rightCol < MAP_COLS) {// Yeni pozisyona hareket izni kontrolü
            if (mapArray[topRow][leftCol] != 1 && mapArray[topRow][rightCol] != 1 && mapArray[bottomRow][leftCol] != 1 && mapArray[bottomRow][rightCol] != 1) {// Yeni pozisyona hareket izni kontrolü
                shape.setPosition(nextX, nextY);// Yeni pozisyona hareket
                int centerGridX = (nextX + radius) / CELL_SIZE;// Bir sonraki pozisyonun merkez sütunu
                int centerGridY = (nextY + radius) / CELL_SIZE;// Bir sonraki pozisyonun merkez satırı
                if (centerGridX >= 0 && centerGridX < MAP_COLS && centerGridY >= 0 && centerGridY < MAP_ROWS) {// Merkez hücre koordinatlarının geçerli olup olmadığını kontrol etme
                    if (mapArray[centerGridY][centerGridX] == 0) {// Eğer merkez hücrede bir nokta varsa
                        mapArray[centerGridY][centerGridX] = 2;// Noktayı yeme işlemi
                        score += 10;// Skoru artırma
                    }
                }
            }
        }
        updateAnimation();// Pacmanin ağzının açılıp kapanma animasyonunu güncelleme
    }

    void updateAnimation() {// Pacmanin ağzının açılıp kapanma animasyonunu kontrol etme
        if (animationClock.getElapsedTime().asSeconds() > 0.015f) {// Animasyon güncelleme süresi kontrolü
            if (biting) {// Eğer Pacman ağzını açıyorsa
                biteAngle += 8.0f;// Ağzın açılma açısını artırma
                if (biteAngle >= 55.0f) biting = false;// Ağzın tamamen açıldığında kapanmaya başlama
            } 
            else {// Eğer Pacman ağzını kapıyorsa
                biteAngle -= 8.0f;// Ağzın açılma açısını azaltma
                if (biteAngle <= 5.0f) biting = true;// Ağzın tamamen kapandığında açmaya başlama
            }
            float radius = CELL_SIZE / 2 - 2;// Pacmanin yarıçapı
            updateShape(radius);// Pacmanin şeklinin güncellenmesi
            animationClock.restart();// Animasyon zamanlayıcısını sıfırlama
        }
    }

    void draw(sf::RenderWindow& window) {// Pacmanin çizimi
        float rotation = 0.0f;// Pacmanin yönüne göre döndürme açısını belirleme
        if (currentDirection == 1) rotation = 180.0f;// Sol hareket yönünde 180 derece döndürme
        else if (currentDirection == 2) rotation = 270.0f;// Yukarı hareket yönünde 270 derece döndürme
        else if (currentDirection == 3) rotation = 90.0f;// Aşağı hareket yönünde 90 derece döndürme
        float radius = CELL_SIZE / 2 - 2;// Pacmanin yarıçapı
        shape.setOrigin(radius, radius);// Pacmanin merkezini döndürme noktası olarak ayarlama
        sf::Vector2f pos = shape.getPosition();// Pacmanin mevcut pozisyonu
        shape.setPosition(pos.x + radius, pos.y + radius);// Pacmanin pozisyonunu döndürme noktasına göre ayarlama
        shape.setRotation(rotation);// Pacmanin yönüne göre döndürme
        window.draw(shape);// Pacmanin çizimi
        shape.setPosition(pos.x, pos.y);// Pacmanin pozisyonunu orijinal haline döndürme
        shape.setOrigin(0, 0);// Pacmanin döndürme noktasını orijinal haline döndürme
    }
};

bool checkCollision(sf::Vector2f pos1, sf::Vector2f pos2, float radius) {// Pacman ile hayalet arasındaki çarpışmayı kontrol etme
    float distance = std::sqrt(std::pow(pos1.x - pos2.x, 2) + std::pow(pos1.y - pos2.y, 2));// İki nokta arasındaki mesafeyi hesaplama
    return distance < (radius * 1.5f);// Eğer mesafe belirli bir eşikten küçükse çarpışma olduğunu kabul etme
}

int main() {
    sf::RenderWindow window(sf::VideoMode(MAP_COLS * CELL_SIZE, MAP_ROWS * CELL_SIZE + 30), "Pacman Test");// Oyun penceresini oluşturma
    window.setFramerateLimit(60);// Oyun penceresinin kare hızını sınırlama
    sf::Font font;// Yazı tipini yükleme
    font.loadFromFile("arial.ttf");// Arial yazı tipini yükleme 
    sf::Text scoreText;// Skor metnini oluşturma
    scoreText.setFont(font);// Skor metninin yazı tipini ayarlama
    scoreText.setCharacterSize(16);// Skor metninin karakter boyutunu ayarlama
    scoreText.setFillColor(sf::Color::Red);// Skor metninin rengini ayarlama
    scoreText.setPosition(5, MAP_ROWS * CELL_SIZE + 5);// Skor metninin konumunu ayarlama
    

    sf::Text livesText;// Can sayısı metnini oluşturma
    livesText.setFont(font);// Can sayısı metninin yazı tipini ayarlama
    livesText.setCharacterSize(16);// Can sayısı metninin karakter boyutunu ayarlama
    livesText.setFillColor(sf::Color::Yellow);// Can sayısı metninin rengini ayarlama
    livesText.setPosition(MAP_ROWS*CELL_SIZE-95,MAP_ROWS*CELL_SIZE+5);// Can sayısı metninin konumunu ayarlama
    Pacman player;
    float radius = CELL_SIZE / 2 - 2;// Karakterlerin yarıçapı
    float offsetX = (CELL_SIZE / 2) - radius;// Hayaletlerin hücre merkezine hizalanması için X ekseninde offset değeri
    float offsetY = (CELL_SIZE / 2) - radius;// Hayaletlerin hücre merkezine hizalanması için Y ekseninde offset değeri

    Ghost blinky(9 * CELL_SIZE + offsetX, 7 * CELL_SIZE + offsetY, sf::Color::Red);// Blinky hayaletini oluşturma ve başlangıç pozisyonunu ayarlama
    Ghost pinky(10 * CELL_SIZE + offsetX, 7 * CELL_SIZE + offsetY, sf::Color::Magenta);// Pinky hayaletini oluşturma ve başlangıç pozisyonunu ayarlama
    Ghost inky(8 * CELL_SIZE + offsetX, 7 * CELL_SIZE + offsetY, sf::Color::Cyan);// Inky hayaletini oluşturma ve başlangıç pozisyonunu ayarlama
    Ghost clyde(11 * CELL_SIZE + offsetX, 7 * CELL_SIZE + offsetY, sf::Color(255, 127, 0));// Clyde hayaletini oluşturma ve başlangıç pozisyonunu ayarlama

    while (window.isOpen()) {// Oyun döngüsü
        sf::Event event;// Olayları işleme
        while (window.pollEvent(event)) {// Olayları kontrol etme
            if (event.type == sf::Event::Closed)// Pencere kapatma olayı kontrolü
                window.close();// Pencereyi kapatma
            // Başlangıç ekranında Enter tuşuna basıldığında oyunu başlatma
            if (currentState == START) {  
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                    currentState = PLAYING;
                }
            }
        }

        window.clear(sf::Color::Black);// Ekranı temizleme

        if (currentState == START) {
            // Başlangıç ekranı metnini oluşturma ve konumlandırma
            sf::Text startText;// Başlangıç ekranı metnini oluşturma
            startText.setFont(font);// Başlangıç ekranı metninin yazı tipini ayarlama
            startText.setString("PACMAN\n\nBaslamak icin ENTER'a bas");// Başlangıç ekranı metnini ayarlama    
            startText.setCharacterSize(25);// Başlangıç ekranı metninin karakter boyutunu ayarlama
            startText.setFillColor(sf::Color::Yellow);// Başlangıç ekranı metninin rengini ayarlama

            sf::FloatRect textRect = startText.getLocalBounds();// Başlangıç ekranı metninin boyutlarını alma
            startText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);// Başlangıç ekranı metninin merkezini hesaplama
            startText.setPosition(sf::Vector2f((MAP_COLS * CELL_SIZE) / 2.0f, (MAP_ROWS * CELL_SIZE) / 2.0f));// Başlangıç ekranı metninin konumunu ayarlama

            window.draw(startText);// Başlangıç ekranı metnini çizme
        }
        else if (currentState == PLAYING) {// Oyun oynanırken karakterlerin hareketi ve çizimi
            blinky.moveTowards(map, player.shape.getPosition().x, player.shape.getPosition().y);// Blinky hayaletinin Pacmanin konumuna doğru hareket etmesi
            pinky.move(map);// Pinky hayaletinin rastgele hareket etmesi
            inky.move(map);// Inky hayaletinin rastgele hareket etmesi
            clyde.move(map);// Clyde hayaletinin rastgele hareket etmesi
            player.handleInput(map);// Pacmanin klavye girişlerini işleme ve hareketini kontrol etme

            float pacRadius = CELL_SIZE / 2 - 2;//  Pacmanin yarıçapı
            sf::Vector2f pacPos = player.shape.getPosition();   // Pacmanin mevcut pozisyonu

            // Çarpışma kontrolü eğer Pacman herhangi bir hayaletle çarpışırsa oyuncunun puanını ekrana yazdır ve tüm karakterleri başlangıç pozisyonlarına ışınla
            if (checkCollision(pacPos, blinky.shape.getPosition(), pacRadius) || checkCollision(pacPos, pinky.shape.getPosition(), pacRadius) || checkCollision(pacPos, inky.shape.getPosition(), pacRadius) ||checkCollision(pacPos, clyde.shape.getPosition(), pacRadius)) {

                player.lives--;// Can sayısını azaltma

                if(player.lives <= 0) {
                    currentState = GAME_OVER;// Can sayısı sıfır veya daha az olduğunda oyun durumunu GAME_OVER olarak değiştirme
                }
                else { 
                // karakterlerin başlangıç pozisyonlarına geri dönmesi
                player.respawn();
                blinky.respawn();
                pinky.respawn();
                inky.respawn();
                clyde.respawn();
                }
            }
            sf::RectangleShape wall(sf::Vector2f(CELL_SIZE - 1.0f, CELL_SIZE - 1.0f));// Duvarların görsel temsilcisi
            wall.setFillColor(sf::Color::Blue);// Duvarların rengi
            sf::CircleShape food(2);// Noktaların görsel temsilcisi
            food.setFillColor(sf::Color::White);// Noktaların rengi

            for (int row = 0; row < MAP_ROWS; row++) {// Harita satırlarını döngü ile gezme
                for (int col = 0; col < MAP_COLS; col++) {// Harita sütunlarını döngü ile gezme
                    if (map[row][col] == 1) {// Eğer hücrede duvar varsa
                        wall.setPosition(col * CELL_SIZE, row * CELL_SIZE);// Duvarın konumunu ayarlama
                        window.draw(wall);// Duvarı çizme
                    } 
                    else if (map[row][col] == 0) {// Eğer hücrede nokta varsa
                        food.setPosition(col * CELL_SIZE + CELL_SIZE / 2 - 2, row * CELL_SIZE + CELL_SIZE / 2 - 2);// Noktanın konumunu ayarlama
                        window.draw(food);// Noktayı çizme
                    }//else if kapanış
                }//ic for kapanış
            }//for kapanış

            player.draw(window);
            blinky.draw(window);
            pinky.draw(window);
            inky.draw(window);
            clyde.draw(window);
            scoreText.setString("Puan: " + std::to_string(player.score));// Skor metnini güncelleme
            livesText.setString("Can: " + std::to_string(player.lives));// Can sayısı metnini güncelleme
            window.draw(livesText);// Can sayısı metnini çizme
            window.draw(scoreText);// Skor metnini çizme
        } //while kapanış
        
         else if(currentState ==GAME_OVER)
            { 
                sf::Text gameOverText;// Oyun bitti metnini oluşturma
                gameOverText.setFont(font);// Oyun bitti metninin yazı tipini ayarlama
                gameOverText.setString("------------Oyun Bitti------------\nSkorun: " + std::to_string(player.score) + "\nTekrar oynamak icin ENTER'a bas");// Oyun bitti metnini ayarlama
                gameOverText.setCharacterSize(25);// Oyun bitti metninin karakter boyutunu ayarlama
                gameOverText.setFillColor(sf::Color::Red);// Oyun bitti metninin rengini ayarlama
                
                sf::FloatRect textRect = gameOverText.getLocalBounds();// Oyun bitti metninin boyutlarını alma
                gameOverText.setOrigin(textRect.left + textRect.width/2.0f,textRect.top + textRect.height / 2.0f);// Oyun bitti metninin merkezini hesaplama
                gameOverText.setPosition(sf::Vector2f((MAP_COLS * CELL_SIZE) / 2.0f, (MAP_ROWS * CELL_SIZE) / 2.0f));// Oyun bitti metninin konumunu ayarlama
                window.draw(gameOverText);// Oyun bitti metnini çizme
                
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    window.close();// Escape tuşuna basıldığında pencereyi kapatma
                }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                // Oyun bitti ekranında Enter tuşuna basıldığında oyunu yeniden başlatmae
                player.score = 0;// Skoru sıfırlama
                player.lives = 3;// Can sayısını sıfırlama
                // Karakterleri başlangıç pozisyonlarına geri döndürme
                player.respawn();
                currentState = PLAYING;
                blinky.respawn();
                pinky.respawn();
                inky.respawn();
                clyde.respawn();

                resetMap();// Haritayı başlangıç durumuna sıfırlama
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();// Escape tuşuna basıldığında pencereyi kapatma
            }
        window.display();
    }
    return 0;
}