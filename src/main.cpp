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

        // bir önceki framei siler yoksa eski görüntü üstüne çizilir
        window.clear(sf::Color::Black);

        // grid sistemi tasarlandı 

        window.display();
    }

    return 0;
}