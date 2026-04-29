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

int main() {
    // 800x600 yerine grid yapısına tam oturan pencere oluşturuldu
    sf::RenderWindow window(sf::VideoMode(MAP_COLS * CELL_SIZE, MAP_ROWS * CELL_SIZE), "Pacman Test");

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
                }
            }
        }
        window.display();
    }

    return 0; 
}