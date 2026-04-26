#include <SFML/Graphics.hpp> 
int main() {
    // 800x600 piksel boyutunda pencere oluşturmak için
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pacman Test");

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

        // Pacman, duvarlar, hayaletler vs. olacak

        window.display();
    }

    return 0;
}