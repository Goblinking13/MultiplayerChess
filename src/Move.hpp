#pragma once
#include<cstdint>
#include"Pieces.hpp"
#include"Bitboards.hpp"
#include"AttackPatternGen.hpp"
#include"GUI.hpp"
#include<cstdlib>

class PsLegalMove
{
  
 typedef uint64_t (PsLegalMove::*FuncPtr)(Pieces &position, uint32_t square, uint8_t side);
  
  typedef void (PsLegalMove::*tempPtr)();
  
  uint64_t PseudoMv[64];
  AttackPattern *atkPtn;
  
  // pawn push
  uint64_t wPawnOne;
  uint64_t wPawnTwo;
  uint64_t bPawnOne;
  uint64_t bPawnTwo;
  
  public:
  
  void updatePawn(Pieces &position, uint8_t side);
  
  AttackPattern* getAttackPattern();
  
  PsLegalMove(AttackPattern *atkPtn);
  uint64_t KnightPsLegal(Pieces &position, uint32_t square, uint8_t side);
  uint64_t KingPsLegal(Pieces &position, uint32_t square, uint8_t side);
  uint64_t BishopPsLegal(Pieces &position, uint32_t square, uint8_t side);
  uint64_t RookPsLegal(Pieces &position, uint32_t square, uint8_t side);
  uint64_t QueenPsLegal(Pieces &position, uint32_t square, uint8_t side);
  uint64_t PawnPsLegal(Pieces &position, uint32_t square, uint8_t side);

  FuncPtr fooArr[6];

  void GenAllPsLegalMove(Pieces &position,PieceList &list, uint8_t side);
  void PrintAllPsLegalMove();
  friend class LegalMove;
};



