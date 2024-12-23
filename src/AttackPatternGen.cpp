#include "Bitboards.hpp"
#include "AttackPatternGen.hpp"


uint64_t AttackPattern::getAttack(uint32_t pieceType, uint32_t square, uint8_t side) const
{
  switch(pieceType)
  {
    case PAWN:
      return attackPawnBB[side][square];
    case KNIGHT:
      return attackKnight[square];
    case BISHOP:
      return attackBishop[square];
    case ROOK:
      return attackRook[square];
    case QUEEN:
      return attackQueen[square];
    case KING:
      return attackKing[square];
    default:
      return 37;
  }
}


void AttackPattern::GenerateQueenAttack()
{
  uint64_t WeNo[64];
  uint64_t EaNo[64];
  uint64_t WeSo[64];
  uint64_t EaSo[64];


  Bitboard::genDiWeNorth(WeNo);
  Bitboard::genDiEaNorth(EaNo);
  Bitboard::genDiWeSouth(WeSo);
  Bitboard::genDiEaSouth(EaSo);

  uint64_t VerNor[64];
  uint64_t VerSou[64];
  uint64_t HorWe[64];
  uint64_t HorEa[64];

  Bitboard::genHoEast(HorEa);
  Bitboard::genHoWest(HorWe);
  Bitboard::genVeNorth(VerNor);
  Bitboard::genVeSouth(VerSou);

  for(int i =0; i < 64 ;i++)
  {
    attackQueen[i] = WeNo[i] | EaNo[i] | WeSo[i] | EaSo[i] | VerNor[i] | VerSou[i] | HorEa[i] | HorWe[i];

  }
}


void AttackPattern::GeneratePawnAttackW()
{
  for(int i =0; i < 64;i++)
  {
      uint64_t curSqr = Bitboard::setOne(0ULL, i);
      attackPawnBB[WHITE][i] = MoveBB::noWeOne(curSqr) | MoveBB::noEaOne(curSqr);

  }

}


void AttackPattern::GeneratePawnAttackB()
{
  for(int i =0; i < 64;i++)
  {
      uint64_t curSqr = Bitboard::setOne(0ULL, i);
     attackPawnBB[BLACK][i] = MoveBB::soWeOne(curSqr) | MoveBB::soEaOne(curSqr);
  }
}

void AttackPattern::GenerateKnightAttack()
{
  for(int i =0; i < 64;i++)
  {
    uint64_t curSq = Bitboard::setOne(0ULL, i);

    attackKnight[i] = (curSq & ~FileA) << 17          |
                      (curSq & ~FileH) << 15          |
                      (curSq & ~(FileA | FileB)) << 10|
                      (curSq & ~(FileH | FileG)) << 6;

    attackKnight[i] |=(curSq & ~(FileA | FileB)) >> 6 |
                      (curSq & ~(FileH | FileG)) >> 10|
                      (curSq & ~FileA) >> 15          |
                      (curSq & ~FileH) >> 17;
  }
}


void AttackPattern::GenerateKingAttack()
{
  for(int i =0; i < 64;i++)
  {
    uint64_t curSq = Bitboard::setOne(0ULL,i);
    attackKing[i] = MoveBB::noWeOne(curSq) | MoveBB::northOne(curSq) | MoveBB::noEaOne(curSq)|
                    MoveBB::westOne(curSq) |                           MoveBB::eastOne(curSq)|
                    MoveBB::soWeOne(curSq) | MoveBB::southOne(curSq) | MoveBB::soEaOne(curSq);
  }

}


void AttackPattern::GenerateRookAttack()
{
  uint64_t VerNor[64];
  uint64_t VerSou[64];
  uint64_t HorWe[64];
  uint64_t HorEa[64];


  Bitboard::genHoEast(HorEa);
  Bitboard::genHoWest(HorWe);
  Bitboard::genVeNorth(VerNor);
  Bitboard::genVeSouth(VerSou);

  for(int i =0; i < 64 ;i++)
  {
    attackRook[i] = VerNor[i] | VerSou[i] | HorEa[i] | HorWe[i];

  }
}

void AttackPattern::GenerateBishopAttack()
{
  uint64_t WeNo[64];
  uint64_t EaNo[64];
  uint64_t WeSo[64];
  uint64_t EaSo[64];


  Bitboard::genDiWeNorth(WeNo);
  Bitboard::genDiEaNorth(EaNo);
  Bitboard::genDiWeSouth(WeSo);
  Bitboard::genDiEaSouth(EaSo);

  for(int i =0; i < 64 ;i++)
  {
    attackBishop[i] = WeNo[i] | EaNo[i] | WeSo[i] | EaSo[i];

  }
}


void AttackPattern::GenerateAllAttack()
{
  GeneratePawnAttackW();
  GeneratePawnAttackB();
  GenerateKnightAttack();
  GenerateKingAttack();
  GenerateRookAttack();
  GenerateBishopAttack();
  GenerateQueenAttack();
}


void AttackPattern::GenerateDircectionRays()
{
  Bitboard::genHoWest(west);
  Bitboard::genHoEast(east);
  Bitboard::genVeSouth(south);
  Bitboard::genVeNorth(north);
  
  Bitboard::genDiWeNorth(northWe);
  Bitboard::genDiEaNorth(northEa);
  Bitboard::genDiWeSouth(southWe);
  Bitboard::genDiEaSouth(southEa);
  
}


uint64_t AttackPattern::getDirectionRay(uint8_t direction , uint32_t square)
{
  switch(direction)
  {
    case NORTH:
      return north[square];
    case SOUTH:
      return south[square];
    case WEST:
      return west[square];
    case EAST:
      return east[square];
    case NORTHEAST:
      return northEa[square];
    case NORTHWEST:
      return northWe[square];
    case SOUTHWEST:
      return southWe[square];
    case SOUTHEAST:
      return southEa[square];
    default:
      return 0;
  }
}


uint64_t AttackPattern::wPawnOnePush(uint64_t pawns, uint64_t empty)
{
 return MoveBB::northOne(pawns) & empty;
}

uint64_t AttackPattern::wPawnTwoPush(uint64_t pawns, uint64_t empty)
{
 uint64_t ableTwoPush = pawns & Rank2;
 uint64_t onePush =wPawnOnePush(ableTwoPush, empty);
 return wPawnOnePush(onePush, empty);
 
}

uint64_t AttackPattern::bPawnOnePush(uint64_t pawns, uint64_t empty)
{
 return MoveBB::southOne(pawns) & empty;
}


uint64_t AttackPattern::bPawnTwoPush(uint64_t pawns, uint64_t empty)
{
 uint64_t ableTwoPush = pawns & Rank7;
 uint64_t onePush =bPawnOnePush(ableTwoPush, empty);
 return bPawnOnePush(onePush, empty);
}



AttackPattern::AttackPattern(){
  GenerateAllAttack();
  GenerateDircectionRays();
}

uint64_t AttackPattern::GetPawnAttackBB(uint32_t sq,uint8_t side)
{
  return attackPawnBB[side][sq];
  
}
