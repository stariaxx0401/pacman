#include <SFML/Graphics.hpp>// SFML grafik kütüphanesini dahil etme
#include <cmath> // Açısal ve matematiksel işlemler için
#include <string> // puan gösterimi için
#include <cstdlib> // Rastgele yön seçimi için
#include <SFML/Audio.hpp> // Ses kütüphanesi
#include "Ghost.h"// Ghost sınıfını dahil etme
#include "Pacman.h"// Pacman sınıfını dahil etme
#include "Constants.h"

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
bool checkWin(){
    for(int r=0;r<MAP_ROWS;r++) {
        for(int c=0;c<MAP_COLS;c++) {
           if(map[r][c] == 0) return false;// Eğer hala yenecek nokta varsa oyunu kazanamamış demektir
        }
    }
    return true;// Eğer yenecek nokta yoksa oyunu kazanmış demektir
}

bool checkCollision(sf::Vector2f pos1, sf::Vector2f pos2, float radius) {// Pacman ile hayalet arasındaki çarpışmayı kontrol etme
    float distance = std::sqrt(std::pow(pos1.x - pos2.x, 2) + std::pow(pos1.y - pos2.y, 2));// İki nokta arasındaki mesafeyi hesaplama
    return distance < (radius * 1.5f);// Eğer mesafe belirli bir eşikten küçükse çarpışma olduğunu kabul etme
}

int main() {
    sf::RenderWindow window(sf::VideoMode(MAP_COLS * CELL_SIZE, MAP_ROWS * CELL_SIZE + 30), "Pacman Test");// Oyun penceresini oluşturma
    window.setFramerateLimit(60);// Oyun penceresinin kare hızını sınırlama
    sf::Font font;// Yazı tipini yükleme
    if (!font.loadFromFile("arial.ttf")) {
        printf("Yazi tipi yuklenemedi! Klasorde arial.ttf oldugundan emin olun.\n");
    } 
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
    
    sf::Music backgroundMusic; // Arka plan müziği nesnesi
    if(!backgroundMusic.openFromFile("freesound_community-playing-pac-man-6783.mp3")) {
    printf("Muzik yuklenemedi!\n");
    }
    backgroundMusic.setLoop(true); // Döngüye alma
    backgroundMusic.setVolume(30); // Ses seviyesi
    backgroundMusic.play(); // Müziği çalma
   

    sf::SoundBuffer eatBuffer; // Nokta yeme sesi tamponu
    if(!eatBuffer.loadFromFile("nahtt-eat-323883.mp3")) {
        printf("Yeme sesi yuklenemedi!\n");
    }
    sf::Sound eatSound; // Nokta yeme sesi nesnesi
    eatSound.setBuffer(eatBuffer); // Tamponu ayarlama
    eatSound.setVolume(60); // Ses seviyesi

    sf::SoundBuffer deathBuffer;// Ölüm sesi tamponu
    if (!deathBuffer.loadFromFile("8d82b5_pacman_dies_sound_effect.mp3")) {
    printf("Olum sesi yuklenemedi! Dosya adini kontrol et.\n");
    }
    sf::Sound deathSound; // Ölüm sesi nesnesi
    deathSound.setBuffer(deathBuffer);// Tamponu ayarlama
    deathSound.setVolume(50);// Ses seviyesi

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
            player.handleInput(map, eatSound);// Pacmanin klavye girişlerini işleme ve hareketini kontrol etme

            float pacRadius = CELL_SIZE / 2 - 2;//  Pacmanin yarıçapı
            sf::Vector2f pacPos = player.shape.getPosition();   // Pacmanin mevcut pozisyonu

            // Çarpışma kontrolü eğer Pacman herhangi bir hayaletle çarpışırsa oyuncunun puanını ekrana yazdır ve tüm karakterleri başlangıç pozisyonlarına ışınla
            if (checkCollision(pacPos, blinky.shape.getPosition(), pacRadius) || checkCollision(pacPos, pinky.shape.getPosition(), pacRadius) || checkCollision(pacPos, inky.shape.getPosition(), pacRadius) ||checkCollision(pacPos, clyde.shape.getPosition(), pacRadius)) {

                deathSound.play(); // Ölüm sesini çalma

                player.lives--;// Can sayısını azaltma

                if(player.lives <= 0) {
                    currentState = GAME_OVER;// Can sayısı sıfır veya daha az olduğunda oyun durumunu GAME_OVER olarak değiştirme
                    backgroundMusic.stop(); // Arka plan müziğini durdurma
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
            if(checkWin()) {
                currentState = WIN;// Tüm noktalar yenildiğinde oyun durumunu WIN olarak değiştirme
                backgroundMusic.stop(); // Arka plan müziğini durdurma
            } 
        }//else if kapanış
        
         else if(currentState ==GAME_OVER)
            { 
                sf::Text gameOverText;// Oyun bitti metnini oluşturma
                gameOverText.setFont(font);// Oyun bitti metninin yazı tipini ayarlama
                gameOverText.setString("----------Oyun Bitti----------\nSkorun: " + std::to_string(player.score) + "\nTekrar oynamak icin ENTER'a bas");// Oyun bitti metnini ayarlama
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
            else if(currentState == WIN) {
                sf::Text winText;// Kazanma metnini oluşturma
                winText.setFont(font);// Kazanma metninin yazı tipini ayarlama
                winText.setString("----------Tebrikler Kazandiniz---------\nSkorun: " + std::to_string(player.score) + "\nTekrar oynamak icin ENTER'a bas");// Kazanma metnini ayarlama
                winText.setCharacterSize(25);// Kazanma metninin karakter boyutunu ayarlama
                winText.setFillColor(sf::Color::Green);// Kazanma metninin rengini ayarlama 

                sf::FloatRect tr = winText.getLocalBounds();// Kazanma metninin boyutlarını alma
                winText.setOrigin(tr.left + tr.width/2.0f,tr.top + tr.height / 2.0f);// Kazanma metninin merkezini hesaplama
                winText.setPosition(sf::Vector2f((MAP_COLS * CELL_SIZE) / 2.0f, (MAP_ROWS * CELL_SIZE) / 2.0f));// Kazanma metninin konumunu ayarlama
                window.draw(winText);// Kazanma metnini çizme
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                    player.lives = 3;// Can sayısını sıfırlama
                    player.score = 0;// Skoru sıfırlama
                    player.respawn();// Pacmanin başlangıç pozisyonuna geri dönmesi
                    resetMap();// Haritayı başlangıç durumuna sıfırlama
                    currentState = PLAYING;// Oyun durumunu PLAYING olarak değiştirmee
                    backgroundMusic.play();// Arka plan müziğini başlatma
                }
            }
         if(currentState == GAME_OVER || currentState == WIN) {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                // Oyun bitti ekranında Enter tuşuna basıldığında oyunu yeniden başlatmae
                player.score = 0;// Skoru sıfırlama
                player.lives = 3;// Can sayısını sıfırlama
                // Karakterleri başlangıç pozisyonlarına geri döndürme
                player.respawn();
                currentState = PLAYING;
                backgroundMusic.play(); // Müziği başlat
                blinky.respawn();
                pinky.respawn();
                inky.respawn();
                clyde.respawn();

                resetMap();// Haritayı başlangıç durumuna sıfırlama
            }
        }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();// Escape tuşuna basıldığında pencereyi kapatma
            }
        window.display();
    }
    return 0;
}