#include <iostream>
#include<string>
#include"Game.hpp"
#include"Multiplayer.hpp"



//if playing in multiplayer mode, write <your port> <opponent port> <your side>  (W/B)
int main(int argc, const char * argv[]) {
  
  float whiteScore =0.0f, blackScore = 0.0f;
  std::cout << "To move, use write <from> <to> in chess coordinates. (Ex: e2 e4)" << std::endl;
  
  char ch;
  
  uint8_t side;
  if(argc == 4){
    if( tolower(argv[3][0])=='w' )
      side= WHITE;
    else if( tolower(argv[3][0])=='b' )
      side = BLACK;
    else
    {
      std::cout<< "Side selection error\n";
      exit(-1);
    }
  }
  
  while(1){
    Game game;
    game.setPlayerSide(WHITE);
    if(argc == 4)
    {
      game.setPlayerSide(side);
      game.setOpponentPort(atoi(argv[2]));
      game.setSocketFd( initSocket(atoi(argv[1])));
      game.setMultiplayerMode(true);
    }
    
    
    switch( game.PlayPvP())
    {
      case 0:
        whiteScore++;
        break;
      case 1:
        blackScore++;
        break;
      default:
      {
        whiteScore += 0.5f; blackScore += 0.5f;
        break;
      }
    }
    std::cout << "=====<GAME SCORE>=====\n";
    std::cout << "W: "<< whiteScore << " | B: "<< blackScore << std::endl;
    std::cout << "=====<**********>=====\n";
    
    std::cout << "Play again? y/n\n";
    std::cin >> ch;
    if(ch == 'n')
      break;
  }
  
  
  return 0;
}
