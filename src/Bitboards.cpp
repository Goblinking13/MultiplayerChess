#include"Bitboards.hpp"
#include<assert.h>

uint64_t MoveBB::southOne(uint64_t b)
{return b >> 8;}

uint64_t MoveBB::northOne(uint64_t b)
{return b << 8;}

uint64_t MoveBB::eastOne(uint64_t b)
{return (b & NotFileA) << 1;}

uint64_t MoveBB::noEaOne(uint64_t b)
{return  (b & NotFileH) << 7;}

uint64_t MoveBB::soEaOne(uint64_t b)
{return  (b & NotFileH) >> 9;}


uint64_t MoveBB::westOne(uint64_t b)
{return (b & NotFileH) >> 1;}

uint64_t MoveBB::noWeOne(uint64_t b)
         {return (b << 9)& NotFileH ;}

uint64_t MoveBB::soWeOne(uint64_t b)
{return (b >> 7)& NotFileH ;}


uint64_t Bitboard::setOne(uint64_t bb ,uint8_t square)
{
  return bb | (1ULL << square);
}
uint64_t  Bitboard::setZero(uint64_t bb ,uint8_t square)
{
  return bb & ~(0ULL | (1ULL << square));
}

uint8_t  Bitboard::popCount(uint64_t bb)
{
  uint8_t count =0;
  while(bb)
  {
    count++;
    bb &= bb-1;
  }

  return count;
}

uint32_t  Bitboard::find1LSB(uint64_t bb)
{
  if(bb == 0)
    return 0;
  unsigned int f;
  bb ^= (bb -1);
  f = (int)bb ^ (bb>>32);
  return indexLSB[f*0x78291ACF  >> 26];

}

uint32_t  Bitboard::find1MSB(uint64_t bb)
{
 
    bb |= bb >> 1;
    bb |= bb >> 2;
    bb |= bb >> 4;
    bb |= bb >> 8;
    bb |= bb >> 16;
    bb |= bb >> 32;
    return indexMSB[(bb * 0x03f79d71b4cb0a89) >> 58];

}

uint64_t  Bitboard::bbVFlip(uint64_t bb)
{
  return ((bb<<56)) |
         ((bb<<40) & Rank7) |
         ((bb<<24) & Rank6) |
         ((bb<<8)  & Rank5) |
         ((bb>>8)  & Rank4) |
         ((bb>>24) & Rank3) |
         ((bb>>40) & Rank2) |
         ((bb>>56) ) ;

}

uint64_t  Bitboard::bbHFlip(uint64_t bb)
{
   constexpr static uint64_t k1 = 0x5555555555555555;
   constexpr static uint64_t k2 = 0x3333333333333333;
   constexpr static uint64_t k4 = 0x0f0f0f0f0f0f0f0f;

   bb = ((bb >> 1) & k1) | ((bb & k1) << 1);
   bb = ((bb >> 2) & k2) | ((bb & k2) << 2);
   bb = ((bb >> 4) & k4) | ((bb & k4) << 4);

   return bb;
}

void Bitboard::genVeNorth(uint64_t square[64])
{
  uint64_t nRay = 0x0101010101010100;
  for(int i =0; i <64;i++, nRay = nRay << 1)
  {
    square[i] =nRay;
  }
}

void Bitboard::genDiWeNorth(uint64_t square[64])
{
  uint64_t nRay = 0x8040201008040200;
  for(int i =0; i < 8;i++, nRay = MoveBB::eastOne(nRay))
  {
    uint64_t tempRay = nRay;
    for(int j =0; j < 8*8;j+=8, tempRay <<= 8)
    {
      square[j+i] = tempRay;
    }
  }
}

void Bitboard::genDiEaNorth(uint64_t square[64])
{
  uint64_t nRay = 0x102040810204000;
  
  for(int i =7; i>=0;i--, nRay = MoveBB::westOne(nRay))
  {
    uint64_t tempRay = nRay;
    for(int j =0; j < 8*8;j+=8, tempRay <<= 8)
    {
      square[j+i] = tempRay;
    }
  }
  
}

void Bitboard::genVeSouth(uint64_t square[64])
{
  uint64_t Ray = 0x80808080808080;
  for(int i =63;i>=0;i--, Ray>>=1)
  {
    square[i] = Ray;
  }
}

void Bitboard::genDiWeSouth(uint64_t square[64])
{
  uint64_t Ray =0x2040810204080;
  
  for(int i =0; i < 8;i++,Ray =MoveBB::eastOne(Ray))
  {
    uint64_t tempRay =Ray;
    for(int j =8*7; j >=0;j-=8,tempRay >>=8)
    {
      square[i+j] = tempRay;
    }
  }
}

void Bitboard::genDiEaSouth(uint64_t square[64])
{
  uint64_t Ray =0x40201008040201;
  
  for(int i =7; i >= 0;i--,Ray =MoveBB::westOne(Ray))
  {
    uint64_t tempRay =Ray;
    for(int j =8*7; j >=0;j-=8,tempRay >>=8)
    {
      square[i+j] = tempRay;
    }
  }
}

void Bitboard::genHoEast(uint64_t square[64])
{
  uint64_t Ray =0x7F;
  for(int i =7; i >= 0;i--,Ray = MoveBB::westOne(Ray))
  {
    uint64_t tempRay = Ray;
    for(int j =0; j < 8*8; j+= 8, tempRay <<= 8)
    {
      square[j+i] = tempRay;
    }
  }
}

void Bitboard::genHoWest(uint64_t square[64])
{
  uint64_t Ray =0xFE;
  
  for(int i =0; i <8;i++,Ray = MoveBB::eastOne(Ray))
  {
    uint64_t tempRay = Ray;
    for(int j =0; j < 8*8; j+= 8, tempRay <<= 8)
    {
      square[j+i] = tempRay;
    }
  }
}

bool Bitboard::getOne(uint64_t bb, uint8_t square)
{
  return bb & (1LL << square);
}
