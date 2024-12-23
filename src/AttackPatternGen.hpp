#pragma once
#include"Bitboards.hpp"
static constexpr uint8_t NORTH =0;
static constexpr uint8_t SOUTH = 1;
static constexpr uint8_t WEST = 2;
static constexpr uint8_t EAST = 3;

static constexpr uint8_t NORTHWEST =4;
static constexpr uint8_t NORTHEAST =5;
static constexpr uint8_t SOUTHWEST = 6;
static constexpr uint8_t SOUTHEAST = 7;

class AttackPattern
{
  
  // attack masks
  uint64_t attackPawnBB[2][64];
  uint64_t attackKnight[64];
  uint64_t attackKing[64];
  uint64_t attackRook[64];
  uint64_t attackBishop[64];
  uint64_t attackQueen[64];
  
  // rays
  uint64_t north[64];
  uint64_t south[64];
  uint64_t west[64];
  uint64_t east[64];
  
  uint64_t northWe[64];
  uint64_t northEa[64];
  uint64_t southWe[64];
  uint64_t southEa[64];
  

  
public:
  
  uint64_t getAttack(uint32_t pieceType, uint32_t square, uint8_t side=WHITE) const;
  
  AttackPattern();
  
  void GenerateDircectionRays();
  uint64_t getDirectionRay(uint8_t direction , uint32_t square);
  
  static uint64_t wPawnOnePush(uint64_t pawns, uint64_t empty);
  static uint64_t wPawnTwoPush(uint64_t pawns, uint64_t empty);
  static uint64_t bPawnOnePush(uint64_t pawns, uint64_t empty);
  static uint64_t bPawnTwoPush(uint64_t pawns, uint64_t empty);
  
  uint64_t GetPawnAttackBB(uint32_t sq,uint8_t side);
  
  void GeneratePawnAttackW();
  void GeneratePawnAttackB();
  void GenerateKnightAttack();
  void GenerateKingAttack();
  void GenerateRookAttack();
  void GenerateBishopAttack();
  void GenerateQueenAttack();
  void GeneratePawnPushW();
  void GeneratePawnPushB();
  void GeneratePawnTwoPush();
  
  void GenerateAllAttack();
  
  friend class LegalMove;
  
};


