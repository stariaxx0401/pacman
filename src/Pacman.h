#pragma once
#include <SFML/Graphics.hpp>// SFML grafik kütüphanesini dahil etme
#include <SFML/Audio.hpp>// SFML ses kütüphanesini dahil etme
#include <cmath>// Matematiksel işlemler için
#include <string>// String işlemleri için
#include "Constants.h"

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

    void handleInput(int mapArray[MAP_ROWS][MAP_COLS],sf::Sound& eatSound) {// Klavye girişlerini işleme ve Pacmanin hareketini kontrol etme
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
                        
                        if(currentState==PLAYING&&eatSound.getStatus()!= sf::Sound::Playing) {  
                        eatSound.play();// Nokta yeme sesini çalma
                    }
                }
            }
        }
        updateAnimation();// Pacmanin ağzının açılıp kapanma animasyonunu güncelleme
    }
        if (nextX<0){
         nextX=(MAP_COLS-1)*CELL_SIZE-2;
         shape.setPosition(nextX,nextY);
        }
        else if (nextX>(MAP_COLS-1)*CELL_SIZE){
         nextX=2;
         shape.setPosition(nextX,nextY);
 } 
 
 }

    void updateAnimation(){ // Pacmanin ağzının açılıp kapanma animasyonunu kontrol etme
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

    void draw(sf::RenderWindow& window){// Pacmanin çizimi
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