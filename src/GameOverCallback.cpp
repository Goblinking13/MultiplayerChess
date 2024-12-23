#include "GameOverCallback.hpp"
#include"Game.hpp"
#include<iostream>


void threeRepetition(std::string msg)
{
  std::cout<< msg;
  
}


void invalidMove(std::string msg){
  std::cout << msg ;
}

void error(int line,std::string type)
{
  std::cout << "Errro: "<< type<< "s:"<<line << std::endl; exit(1);
}

void draw(std::string str)
{
  std::cout << str << std::endl;
}

void gameOverWon(uint8_t side)
{
  std::string sideName[] = {"WHITE","BLACK"};
  std::cout << "Game over! \n"<< sideName[side] << " won.\n";
}
