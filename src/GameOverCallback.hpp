#pragma once
#include <string>

void threeRepetition(std::string msg = "Three same repetition!!!\n");

void invalidMove(std::string msg ="Invalid move!!\n");
void error(int line=-1, std::string type= "Unknown");
void draw(std::string str);
void gameOverWon(uint8_t side);
