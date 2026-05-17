#pragma once
#include <SFML/Graphics.hpp>// SFML grafik kütüphanesini dahil etme
#include <cmath>// Matematiksel işlemler için
#include <cstdlib>// Rastgele yön seçimi için
#include "Constants.h"

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
          // Sol taraftan çıkarsa sağdan sok
        if (shape.getPosition().x < -CELL_SIZE) {
        shape.setPosition((MAP_COLS - 1) * CELL_SIZE, shape.getPosition().y);
        }
        // Sağ taraftan çıkarsa soldan sok
       else if (shape.getPosition().x > MAP_COLS * CELL_SIZE) {
       shape.setPosition(0, shape.getPosition().y);
        }

    }
};