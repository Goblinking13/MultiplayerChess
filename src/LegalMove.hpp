#pragma once
#include <cstdint>
#include"Pieces.hpp"
#include"AttackPatternGen.hpp"
#include"Move.hpp"
class LegalMove
{
  uint64_t  LegalMv[64];
  bool isAnyLegalMove;
  
  public:
  static bool isAttacked(uint32_t p, Pieces& curPos, PsLegalMove &atcPtrn ,uint8_t side);
  bool isAnyLegal() const;
  void genAllLegal(Pieces& curPos, PsLegalMove& PsLegMov,PieceList& curPieceList,uint8_t side);
  uint64_t CheckLegal(Pieces& curPos, PsLegalMove& PsLegMov, uint32_t initPos, uint8_t initPieceType,uint8_t side,uint64_t InitPsLegal);
  uint64_t CheckCastelingLegal(Pieces& curPos, PsLegalMove &PsLegMove,uint8_t side, uint64_t initPsLegal);
  void PrintAllLegalMove();
  
  uint64_t* getLegalMove();
  
};
