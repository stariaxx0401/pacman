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
class Ghost{
    public:
        sf::CircleShape shape; // Hayaletin görsel temsili için dairesel şekil kullanıldı
        float speed = 1.5f; // Hayaletin hareket hızı
        int currentDirection = 0; // 0: Sağ, 1: Sol, 2: Yukarı, 3: Aşağı
        sf::Color color; // Hayaletin rengi
        sf::Clock aiClock; // Yapay zeka yön değiştirme zamanlayıcısı
        
        Ghost(float startX, float startY, sf::Color ghostColor) {
            color = ghostColor;
            shape.setRadius(CELL_SIZE / 2 - 2); // Hayaletin yarıçapı
            shape.setFillColor(ghostColor); // Hayaletin rengi
            shape.setPosition(startX, startY); // Başlangıç pozisyonu
        }

        void draw(sf::RenderWindow& window) {
            window.draw(shape); // Hayaleti ekrana çizer
        }

        // Belirtilen yönde duvar var mı kontrol eder
        bool canGoDirection(int mapArray[MAP_ROWS][MAP_COLS], int row, int col, int dir) {
            int nr = row + (dir == 3 ? 1 : dir == 2 ? -1 : 0); // Hedef satır
            int nc = col + (dir == 0 ? 1 : dir == 1 ? -1 : 0); // Hedef sütun
            if (nr < 0 || nr >= MAP_ROWS || nc < 0 || nc >= MAP_COLS) return false;
            return mapArray[nr][nc] != 1;
        }

        void move(int mapArray[MAP_ROWS][MAP_COLS]) {
            float x = shape.getPosition().x; // Hayaletin mevcut x koordinatı
            float y = shape.getPosition().y; // Hayaletin mevcut y koordinatı
            float radius = CELL_SIZE / 2 - 2; // Hayaletin yarıçapı

            float nextX = x; float nextY = y;
            if(currentDirection == 0) nextX += speed; // Sağa hareket planla
            else if(currentDirection == 1) nextX -= speed; // Sola hareket planla
            else if(currentDirection == 2) nextY -= speed; // Yukarı hareket planla
            else if(currentDirection == 3) nextY += speed; // Aşağı hareket planla

            int leftCol = (int)(nextX) / CELL_SIZE; // Hayaletin sol köşesinin grid sütunu
            int rightCol = (int)(nextX + radius * 2) / CELL_SIZE; // Hayaletin sağ köşesinin grid sütunu
            int topRow = (int)(nextY) / CELL_SIZE; // Hayaletin üst köşesinin grid satırı
            int bottomRow = (int)(nextY + radius * 2) / CELL_SIZE; // Hayaletin alt köşesinin grid satırı

            bool canMove = (topRow >= 0 && bottomRow < MAP_ROWS && leftCol >= 0 && rightCol < MAP_COLS) &&
                           (mapArray[topRow][leftCol] != 1 && mapArray[topRow][rightCol] != 1 && 
                            mapArray[bottomRow][leftCol] != 1 && mapArray[bottomRow][rightCol] != 1);

            if(canMove) {
                shape.setPosition(nextX, nextY); // Hayaleti hareket ettir
            } else {
                currentDirection = rand() % 4; // Rastgele yeni bir yön seç
            }
        }
    void moveTowards(int mapArray[MAP_ROWS][MAP_COLS], float pacmanX, float pacmanY) {
    float radius = CELL_SIZE / 2 - 2;
    float cx = shape.getPosition().x + radius;
    float cy = shape.getPosition().y + radius;
    int col = (int)(cx / CELL_SIZE);
    int row = (int)(cy / CELL_SIZE);

    // Yön güncelleme sıklığını biraz azaltmak akıcılığı artırabilir (0.3f -> 0.2f gibi)
    if (aiClock.getElapsedTime().asSeconds() > 0.2f) {
        float diffX = pacmanX - cx;
        float diffY = pacmanY - cy;

        int dirs[4];
        // Pacman'e olan mesafeye göre yön önceliklerini belirle
        if (std::abs(diffX) > std::abs(diffY)) {
            dirs[0] = (diffX > 0) ? 0 : 1; // Yatay
            dirs[1] = (diffY > 0) ? 3 : 2; // Dikey
            dirs[2] = (dirs[1] == 3) ? 2 : 3;
            dirs[3] = (dirs[0] == 0) ? 1 : 0;
        } else {
            dirs[0] = (diffY > 0) ? 3 : 2; // Dikey
            dirs[1] = (diffX > 0) ? 0 : 1; // Yatay
            dirs[2] = (dirs[1] == 0) ? 1 : 0;
            dirs[3] = (dirs[0] == 3) ? 2 : 3;
        }

        bool foundDir = false;
        for (int i = 0; i < 4; i++) {
            // Sadece duvar olmayan DEĞİL, aynı zamanda geri dönmesini engelleyen bir kontrol de eklenebilir
            if (canGoDirection(mapArray, row, col, dirs[i])) {
                currentDirection = dirs[i];
                foundDir = true;
                break;
            }
        }
        
        // Eğer hiçbir yere gidemiyorsa mevcut yönünü koru veya rastgele seç
        if(!foundDir && !canGoDirection(mapArray, row, col, currentDirection)) {
            currentDirection = rand() % 4;
        }

        aiClock.restart();
    }

    // Hareket etmek istediği yönü belirle
    float nextX = shape.getPosition().x;
    float nextY = shape.getPosition().y;
    if (currentDirection == 0) nextX += speed;
    else if (currentDirection == 1) nextX -= speed;
    else if (currentDirection == 2) nextY -= speed;
    else if (currentDirection == 3) nextY += speed;

    // Duvar çarpışma kontrolünü daha esnek hale getirelim
    int lc = (int)(nextX + 2) / CELL_SIZE; // +2/-2 tolerans payları
    int rc = (int)(nextX + radius * 2 - 2) / CELL_SIZE;
    int tr = (int)(nextY + 2) / CELL_SIZE;
    int br = (int)(nextY + radius * 2 - 2) / CELL_SIZE;

    if (tr >= 0 && br < MAP_ROWS && lc >= 0 && rc < MAP_COLS &&
        mapArray[tr][lc] != 1 && mapArray[tr][rc] != 1 &&
        mapArray[br][lc] != 1 && mapArray[br][rc] != 1) {
        shape.setPosition(nextX, nextY);
    } else {
        // Eğer duvara çarptıysa hemen yeni bir yön seç ki "titreme" yapmasın
        currentDirection = rand() % 4;
    }
 }     
};

class Pacman {
public:
    // Dairesel şekil yerine ağız açma/kapama kontrolü için çokgen kullanıldı
    sf::ConvexShape shape;
    float speed = 2.0f; // Pacmanin hareket hızı
    int score = 0; // Pacmanin yediği noktaların sayısı 
    
    // Ağız animasyonu için değişkenler
    sf::Clock animationClock; // Animasyon zamanlayıcısı
    float biteAngle = 30.0f;  // Ağzın açısını kontrol eder
    bool biting = false;      // Ağzın açıldığını veya kapandığını belirler
    int currentDirection = 0; // 0: Sağ, 1: Sol, 2: Yukarı, 3: Aşağı

    Pacman() {
        shape.setFillColor(sf::Color::Yellow); // Sarı Pacman
        
        // Başlangıç pozisyonu= Haritada 1.satır 1.sütun 
        // +2 hücre içinde tam ortalı durması
        float radius = CELL_SIZE / 2 - 2;
        shape.setPosition(1 * CELL_SIZE + (CELL_SIZE / 2) - radius, 1 * CELL_SIZE + (CELL_SIZE / 2) - radius); 
        
        updateShape(radius);
    }
    
    // Şekli ve ağız açıklığını hesaplayan fonksiyon
    void updateShape(float radius) {
        int pointCount = 20; // Daire benzeri görünüm için nokta sayısı
        shape.setPointCount(pointCount + 2); // Merkez + yay noktaları + ağız kapama noktası
        
        shape.setPoint(0, sf::Vector2f(radius, radius)); // Merkez nokta
        
        // Ağzın açısını radyan cinsine çevir
        float radBite = (biteAngle / 2.0f) * 3.14159f / 180.0f;
        
        for (int i = 0; i <= pointCount; i++) {
            float angle = radBite + i * (2 * 3.14159f - 2 * radBite) / pointCount;
            float x = radius + radius * std::cos(angle);
            float y = radius + radius * std::sin(angle);
            shape.setPoint(i + 1, sf::Vector2f(x, y));
        }
    }

    // Kullanıcıdan gelen inputa göre pacmani hareket ettirir ve duvarları kontrol eder
    void handleInput(int mapArray[MAP_ROWS][MAP_COLS]) {
        // Şimdiki pozisyonu alır ve hareket etmek istediği yönü belirler          
        float nextX = shape.getPosition().x;
        float nextY = shape.getPosition().y;
        
        // Klavye yön tuşlarına basıldığında hareket ettirir
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            currentDirection = 2; // Yönü yukarı ayarla
            // Dikey hareket ederken yatayda mükemmel hizala (tam sütun ortası)
            int col = (shape.getPosition().x + (CELL_SIZE / 2 - 2)) / CELL_SIZE;
            nextX = col * CELL_SIZE + (CELL_SIZE / 2) - (CELL_SIZE / 2 - 2);
            nextY -= speed; // Yukarı hareket planla
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { 
            currentDirection = 3; // Yönü aşağı ayarla
            // Dikey hareket ederken yatayda mükemmel hizala (tam sütun ortası)
            int col = (shape.getPosition().x + (CELL_SIZE / 2 - 2)) / CELL_SIZE;
            nextX = col * CELL_SIZE + (CELL_SIZE / 2) - (CELL_SIZE / 2 - 2);
            nextY += speed; // Aşağı hareket planla
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {   
            currentDirection = 1; // Yönü sola ayarla
            // Yatay hareket ederken dikeyde mükemmel hizala (tam satır ortası)
            int row = (shape.getPosition().y + (CELL_SIZE / 2 - 2)) / CELL_SIZE;
            nextY = row * CELL_SIZE + (CELL_SIZE / 2) - (CELL_SIZE / 2 - 2);
            nextX -= speed; // Sola hareket planla
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { 
            currentDirection = 0; // Yönü sağa ayarla
            // Yatay hareket ederken dikeyde mükemmel hizala (tam satır ortası)
            int row = (shape.getPosition().y + (CELL_SIZE / 2 - 2)) / CELL_SIZE;
            nextY = row * CELL_SIZE + (CELL_SIZE / 2) - (CELL_SIZE / 2 - 2);
            nextX += speed; // Sağa hareket planla
        }

        // Karakterin köşe ve merkez hesaplamasını tam dairenin kapladığı alanı baz alarak yapıyoruz.
        float radius = CELL_SIZE / 2 - 2;

        // Karakterin etrafındaki 4 noktanın (köşelerin) grid koordinatlarını kontrol et
        int leftCol = (nextX) / CELL_SIZE;
        int rightCol = (nextX + (radius * 2)) / CELL_SIZE;
        int topRow = (nextY) / CELL_SIZE;
        int bottomRow = (nextY + (radius * 2)) / CELL_SIZE;

        // Harita sınırları dahilinde olup olmadığını ve duvar olup olmadığını denetliyoruz
        if (topRow >= 0 && bottomRow < MAP_ROWS && leftCol >= 0 && rightCol < MAP_COLS) {
            // Hiçbir köşe duvara çarpmıyorsa hareket etmesine izin ver
            if (mapArray[topRow][leftCol] != 1 && 
                mapArray[topRow][rightCol] != 1 && 
                mapArray[bottomRow][leftCol] != 1 && 
                mapArray[bottomRow][rightCol] != 1) {
                
                shape.setPosition(nextX, nextY);

                // Nokta Yeme Sistemi: Karakterin merkez koordinatlarını al
                int centerGridX = (nextX + radius) / CELL_SIZE;
                int centerGridY = (nextY + radius) / CELL_SIZE;

                if (centerGridX >= 0 && centerGridX < MAP_COLS && centerGridY >= 0 && centerGridY < MAP_ROWS) {
                    // Eğer yol hücresindeyse (değer 0) yenilmiş olarak işaretle (değer 2)
                    if (mapArray[centerGridY][centerGridX] == 0) {
                        mapArray[centerGridY][centerGridX] = 2;
                        score+=10; // Her yenen nokta için 10 puan ekle
                    }
                }
            }
        }

        // Animasyon zamanlayıcısını güncelle
        updateAnimation();
    }
    
    // Ağız animasyonunu kontrol eden fonksiyon
    void updateAnimation() {
        // Hızı en yüksek seviyeye çıkarmak için süre 0.015f olarak ayarlandı ve artış miktarı 8.0f'e yükseltildi
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

    // Pacmani ekrana çizmek için yardımcı fonksiyon
    void draw(sf::RenderWindow& window) {
        float rotation = 0.0f;
        
        // Yöne göre çokgenin açısını belirler
        if (currentDirection == 1) rotation = 180.0f; // Sola dön
        else if (currentDirection == 2) rotation = 270.0f; // Yukarı dön
        else if (currentDirection == 3) rotation = 90.0f; // Aşağı dön
        
        float radius = CELL_SIZE / 2 - 2;
        
        // Koordinat kaymasını önlemek için koordinat ve merkez hesaplamasını birleştiriyoruz
        shape.setOrigin(radius, radius);
        
        // Şeklin pozisyonunu merkezine alarak doğru hizalanmasını sağlıyoruz
        sf::Vector2f pos = shape.getPosition();
        shape.setPosition(pos.x + radius, pos.y + radius);
        
        shape.setRotation(rotation);
        
        // Çizimi gerçekleştir
        window.draw(shape);
        
        // Pozisyon ve orijin sıfırlaması
        shape.setPosition(pos.x, pos.y);
        shape.setOrigin(0, 0);
    }
};
bool checkCollision(sf::Vector2f pos1, sf::Vector2f pos2, float radius) {
    float distance = std::sqrt(std::pow(pos1.x - pos2.x, 2) + std::pow(pos1.y - pos2.y, 2));// İki nesne arasındaki mesafeyi hesaplar
    return distance < (radius * 1.5f); // Çarpışma algısı için mesafe kontrolü 
}

int main() {
    // 800x600 yerine grid yapısına tam oturan pencere oluşturuldu
    sf::RenderWindow window(sf::VideoMode(MAP_COLS * CELL_SIZE, MAP_ROWS * CELL_SIZE+30), "Pacman Test"); 
    
    // HIZ SORUNUNU ÇÖZEN SATIR:
    window.setFramerateLimit(60); 
    sf::Font font; // Puan göstermek için font yükleniyor
    font.loadFromFile("arial.ttf"); // Puan göstermek için font yükleniyor
    
    sf::Text scoreText; // Puan göstermek için text objesi oluşturuluyor
    scoreText.setFont(font); // Puan göstermek için font atanıyor
    scoreText.setCharacterSize(16); // Puan göstermek için karakter boyutu ayarlanıyor
    scoreText.setFillColor(sf::Color::White); // Puan göstermek için renk beyaz olarak ayarlanıyor
    scoreText.setPosition(5, MAP_ROWS * CELL_SIZE +5); // Puan göstermek için konum ayarlanıyor
    
    Pacman player;
    

    float radius = CELL_SIZE / 2 - 2;
    float offsetX= (CELL_SIZE / 2) - radius;
    float offsetY= (CELL_SIZE / 2) - radius;

    Ghost blinky(9*CELL_SIZE+offsetX,7*CELL_SIZE+offsetY,sf::Color::Red); // Blinky hayaletini oluşturuyoruz
    Ghost pinky(10*CELL_SIZE+offsetX,7*CELL_SIZE+offsetY,sf::Color::Magenta); // Pinky hayaletini oluşturuyoruz
    Ghost inky(8*CELL_SIZE+offsetX,7*CELL_SIZE+offsetY,sf::Color::Cyan); // Inky hayaletini oluşturuyoruz
    Ghost clyde(11*CELL_SIZE+offsetX,7*CELL_SIZE+offsetY,sf::Color(255, 127, 0)); // Clyde hayaletini oluşturuyoruz (turuncu renk)

    // pencere açık olduğu sürece çalışır
    while (window.isOpen()) {

        //  klavye  mouse pencere kapatma gibi olayları yakalar
        sf::Event event;
        while (window.pollEvent(event)) {
            // Kullanıcı çarpı ya basarsa pencereyi kapat
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Bir önceki framei siler yoksa eski görüntü üstüne çizilir
        window.clear(sf::Color::Black);

        blinky.moveTowards(map,player.shape.getPosition().x,player.shape.getPosition().y); // Blinky hayaletini hareket ettir
        pinky.move(map); // Pinky hayaletini hareket ettir
        inky.move(map); // Inky hayaletini hareket ettir
        clyde.move(map); // Clyde hayaletini hareket ettir

        float pacRadius = CELL_SIZE / 2 - 2;
        sf::Vector2f pacPos = player.shape.getPosition();// Pacmanin merkez pozisyonu

        if(checkCollision(pacPos,blinky.shape.getPosition(), pacRadius) || 
           checkCollision(pacPos,pinky.shape.getPosition(), pacRadius) || 
           checkCollision(pacPos,inky.shape.getPosition(), pacRadius) || 
           checkCollision(pacPos,clyde.shape.getPosition(), pacRadius)) {
           printf("GAME OVER! PUANIN: %d\n", player.score);// Oyun bittiğinde puanı konsola yazdırır
        window.close(); // Oyun bittiğinde pencereyi kapatır
        }
        
        // Duvarlar için kare şeklinde bir görsel şablon oluşturuldu
        sf::RectangleShape wall(sf::Vector2f(CELL_SIZE - 1.0f, CELL_SIZE - 1.0f));
        // Duvarları mavi renge boyar
        wall.setFillColor(sf::Color::Blue);
        
        // Yollar için yem şablonu
        sf::CircleShape food(2); 
        // Yemi beyaz renge boyar
        food.setFillColor(sf::Color::White);

        // Kullanıcıdan gelen inputa göre pacmani hareket ettirir ve duvarları kontrol eder
        player.handleInput(map);

        // Her satır ve sütunu dolaşarak haritayı ekrana çizer
        for (int row = 0; row < MAP_ROWS; row++)
        {
            for (int col = 0; col < MAP_COLS; col++)
            {
                // Artık (0,0) koordinatını atlamıyoruz ve mavi renkli duvar olarak çiziyoruz.
                
                // map te 1 varsa o duvar
                if (map[row][col] == 1)
                {
                    // col*CELL_SIZE = yatay konum
                    // row*CELL_SIZE = dikey konum
                    wall.setPosition(col * CELL_SIZE, row * CELL_SIZE);
                    window.draw(wall);
                }
                // map te 0 varsa o yol
                else if (map[row][col] == 0)
                {  
                    // col*CELL_SIZE = Sütunu bulur
                    // row*CELL_SIZE = Satır bulur
                    // + CELL_SIZE / 2 = Merkeze gider
                    // -2 = Obje payını düşer
                    food.setPosition(col * CELL_SIZE + CELL_SIZE / 2 - 2, row * CELL_SIZE + CELL_SIZE / 2 - 2);
                    window.draw(food);
                } // else if kapanır
            }// Sütun döngüsü kapanır
        } // Satır döngüsü kapanır

        // Pacmani ekrana çizer
        player.draw(window);
        blinky.draw(window);// Blinky hayaletini ekrana çizer
        pinky.draw(window);//  Pinky hayaletini ekrana çizer
        inky.draw(window);//   Inky hayaletini ekrana çizer
        clyde.draw(window);//  Clyde hayaletini ekrana çizer
        scoreText.setString("Puan: " + std::to_string(player.score)); // Puanı günceller
        window.draw(scoreText); // Puanı ekrana çizer    
        window.display(); // Çizilen her şeyi ekrana yansıtır
    }// while kapanır

    return 0; 
 
}