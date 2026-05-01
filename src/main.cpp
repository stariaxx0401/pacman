#include <SFML/Graphics.hpp>

// hücre boyutu
const int CELL_SIZE = 20;

// harita boyutu
const int MAP_ROWS = 21;
const int MAP_COLS = 19;

// haritanın grid yapısını matematiksel olarak tanımlar.
// yol=0 duvar=1
int map[MAP_ROWS][MAP_COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
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

class Pacman {
public:
    // Pacmani temsil eden daire şekli
    sf::CircleShape shape;
    float speed = 2.0f; // Pacmanin hareket hızı

    Pacman() {
        // Yarıçapı hücreden biraz küçük yapıyoruz yollara rahat sığsın
        shape.setRadius(CELL_SIZE / 2 - 2); 
        shape.setFillColor(sf::Color::Yellow); // Sarı Pacman
        
        // Başlangıç pozisyonu= Haritada 1.satır 1.sütun 
        // +2 hücre içinde tam ortalı durması
        shape.setPosition(1 * CELL_SIZE + 2, 1 * CELL_SIZE + 2); 
    }
    // Kullanıcıdan gelen inputa göre pacmani hareket ettirir ve duvarları kontrol eder
    void handleInput(int mapArray[MAP_ROWS][MAP_COLS]) {
        // Şimdiki pozisyonu alır ve hareket etmek istediği yönü belirler       
        float nextX = shape.getPosition().x;
       // Şimdiki pozisyonu alır ve hareket etmek istediği yönü belirler  
        float nextY = shape.getPosition().y;
        // Klavye yön tuşlarına basıldığında hareket ettirir
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            // Yukarı hareket  
            nextY -= speed; 
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { 
            // Aşağı hareket
            nextY += speed;
        } 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {  
            // Sola hareket
            nextX -= speed; 
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { 
            // Sağa hareket
            nextX += speed;
        }

        // Koordinatları grid hücresi indislerine çeviriyoruz
        int gridCol = (nextX + 2) / CELL_SIZE;
        int gridRow = (nextY + 2) / CELL_SIZE;

        // Harita sınırları dahilinde olup olmadığını ve duvar olup olmadığını denetliyoruz
        if (gridRow >= 0 && gridRow < MAP_ROWS && gridCol >= 0 && gridCol < MAP_COLS) {
            if (mapArray[gridRow][gridCol] != 1) {
                // Duvar değilse hareket etmesine izin ver
                shape.setPosition(nextX, nextY);
            }
        }
    }

    // Pacmani ekrana çizmek için yardımcı fonksiyon
    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }
};

int main() {
    // 800x600 yerine grid yapısına tam oturan pencere oluşturuldu
    sf::RenderWindow window(sf::VideoMode(MAP_COLS * CELL_SIZE, MAP_ROWS * CELL_SIZE), "Pacman Test"); 
    
    // HIZ SORUNUNU ÇÖZEN SATIR:
    window.setFramerateLimit(60); 

    Pacman player;

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

        // Çizilen her şeyi ekrana yansıtır
        window.display();
    }// while kapanır

    return 0; 
}