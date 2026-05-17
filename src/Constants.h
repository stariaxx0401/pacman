#pragma once

// Oyun durumlarını tanımlamak için enum
enum GameState {
    START,
    PLAYING,
    GAME_OVER,
    WIN
};

GameState currentState;// Oyun durumu

// Hücre boyutu
const int CELL_SIZE = 20;

// Harita boyutu
const int MAP_ROWS = 21;
const int MAP_COLS = 19;