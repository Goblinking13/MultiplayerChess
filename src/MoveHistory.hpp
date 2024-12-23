
#pragma once
#include<cstdint>
#include"Zobrist.hpp"
#include"Pieces.hpp"

struct Position
{
  
  Position() {};
  
  uint64_t  *white;
  uint64_t  *black;
  uint64_t  *side;
  
  uint64_t empty;
  uint64_t all;
  uint8_t enPassent;
  
  bool isWLeftMoved;
  bool isWRightMoved;
  
  bool isBLeftMoved;
  bool isBRightMoved;
  
  bool isWKingMoved;
  bool isBKingMoved;
  
  bool isCastledW;
  bool isCastledB;
  
  bool Check;
  
  int moveCount;
  int fiftyMoveCount;
  
  uint64_t ZobristVal;
  Position* next;
 
  static Position* setPosition(class Pieces &pieces);
  void setHash(uint64_t ZobristVal) ;
  void setMoveCount(int m) ;
  void setFiftyMoveCount(int f);
  void setPieces(class Pieces &pieces );
  
};

class MoveHistory
{
  Position* positionList;
public:

  bool isEmpty();
  MoveHistory();
  void add(Position* pos);
  void removeLast();
  void removeN(int n);
  Position* getLastMove();
};
