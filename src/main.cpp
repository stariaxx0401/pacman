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

    // Pacmani ekrana çizmek için yardımcı fonksiyon
    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }
};

int main() {
    // 800x600 yerine grid yapısına tam oturan pencere oluşturuldu
    sf::RenderWindow window(sf::VideoMode(MAP_COLS * CELL_SIZE, MAP_ROWS * CELL_SIZE), "Pacman Test"); 
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
        // Duvarlar için kare şeklinde bir görsel şablon oluşturuldu(-1.0f duvarlar arasında ince siyah çizgi yapısı sağlar cellsize olsaydı birbirine girerdi )
        sf::RectangleShape wall(sf::Vector2f(CELL_SIZE - 1.0f, CELL_SIZE - 1.0f));
        // Duvarları mavi renge boyar
        wall.setFillColor(sf::Color::Blue);
        // Yollar için yem şablonu
        sf::CircleShape food(2); 
        // Yemi beyaz renge boyar
        food.setFillColor(sf::Color::White);

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
                {   // col*CELL_SIZE = Sütunu bulur
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