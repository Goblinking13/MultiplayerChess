#pragma once
#include<cstdint>

// Pieces
static constexpr uint8_t PAWN = 0;
static constexpr uint8_t KNIGHT = 1;
static constexpr uint8_t BISHOP = 2;
static constexpr uint8_t ROOK = 3;
static constexpr uint8_t QUEEN = 4;
static constexpr uint8_t KING = 5;
static constexpr uint8_t EMPTY =6;

static constexpr uint8_t WHITE = 0;
static constexpr uint8_t BLACK = 1;

static constexpr uint8_t PUSH = 1;
static constexpr uint8_t CAPTURE = 2;
static constexpr uint8_t CASTLKINGSIDE = 4;
static constexpr uint8_t CASTLQUEENSIDE = 8;
static constexpr uint8_t TRANSFORM = 16;
static constexpr uint8_t CAPTURETRANSFROM = 32;
static constexpr uint8_t ENPASSENT = 64;

// Board 
//8
//7
//6
//5
//4
//3
//2
//1 7 6 5 4 3 2 1 0
//  a b c d e f g h

enum square
{
  a1 =7, b1 = 15, c1= 23, d1 =31, e1 = 39, f1 = 47, g1 =55, h1 =63,
  a2 =6, b2 = 14, c2= 22, d2 =30, e2 = 38, f2 = 46, g2 =54, h2 =62,
  a3 =5, b3 = 13, c3= 21, d3 =29, e3 = 37, f3 = 45, g3 =53, h3 =61,
  a4 =4, b4 = 12, c4= 20, d4 =28, e4 = 36, f4 = 44, g4 =52, h4 =60,
  a5 =3, b5 = 11, c5= 19, d5 =27, e5 = 35, f5 = 43, g5 =51, h5 =59,
  a6 =2, b6 = 10, c6= 18, d6 =26, e6 = 34, f6 = 42, g6 =50, h6 =58,
  a7 =1, b7 = 9, c7= 17, d7 =25, e7 = 33, f7 = 41, g7 =49, h7 =57,
  a8 =0, b8 = 8, c8= 16, d8 =24, e8 = 32, f8 = 40, g8 =48, h8 =56,
};



static constexpr uint64_t FileH = 0x0101010101010101ULL;
static constexpr uint64_t FileG = (FileH << 1);
static constexpr uint64_t FileF=  (FileH << 2);
static constexpr uint64_t FileE = (FileH << 3);
static constexpr uint64_t FileD = (FileH << 4);
static constexpr uint64_t FileC = (FileH << 5);
static constexpr uint64_t FileB = (FileH << 6);
static constexpr uint64_t FileA = (FileH << 7);
static constexpr uint64_t allFile[] = {FileA, FileB, FileC,FileD,FileE, FileF, FileG, FileH};

static constexpr uint64_t Rank1 = 0xFFULL;
static constexpr uint64_t Rank2 = Rank1 << 8;
static constexpr uint64_t Rank3 = Rank1 << 16;
static constexpr uint64_t Rank4 = Rank1 << 24;
static constexpr uint64_t Rank5 = Rank1 << 32;
static constexpr uint64_t Rank6 = Rank1 << 40;
static constexpr uint64_t Rank7 = Rank1 << 48;
static constexpr uint64_t Rank8 = Rank1 << 56;
static constexpr uint64_t allRank[] = {Rank1, Rank2, Rank3, Rank4, Rank5, Rank6, Rank7, Rank8};

static constexpr uint64_t NotFileA = ~FileA;
static constexpr uint64_t NotFileH = ~FileH;

static constexpr uint8_t enPassentA = 0b10000000;
static constexpr uint8_t enPassentB = 0b01000000;
static constexpr uint8_t enPassentC = 0b00100000;
static constexpr uint8_t enPassentD = 0b00010000;
static constexpr uint8_t enPassentE = 0b00001000;
static constexpr uint8_t enPassentF = 0b00000100;
static constexpr uint8_t enPassentG = 0b00000010;
static constexpr uint8_t enPassentH = 0b00000001;

static constexpr uint8_t allEnPassent[] = {enPassentA,enPassentB,enPassentC,enPassentD,enPassentE,enPassentF,enPassentG,enPassentH};

// One step movment

namespace MoveBB
{
uint64_t southOne(uint64_t b);
uint64_t northOne(uint64_t b);

uint64_t eastOne(uint64_t b);
uint64_t noEaOne(uint64_t b);
uint64_t soEaOne(uint64_t b);

uint64_t westOne(uint64_t b);
uint64_t soWeOne (uint64_t b);
uint64_t noWeOne(uint64_t b);
}

constexpr static int indexLSB[64] = {
  63, 30,  3, 32, 59, 14, 11, 33,
  60, 24, 50,  9, 55, 19, 21, 34,
  61, 29,  2, 53, 51, 23, 41, 18,
  56, 28,  1, 43, 46, 27,  0, 35,
  62, 31, 58,  4,  5, 49, 54,  6,
  15, 52, 12, 40,  7, 42, 45, 16,
  25, 57, 48, 13, 10, 39,  8, 44,
  20, 47, 38, 22, 17, 37, 36, 26
};

constexpr static int indexMSB[64] = {
    0, 47,  1, 56, 48, 27,  2, 60,
   57, 49, 41, 37, 28, 16,  3, 61,
   54, 58, 35, 52, 50, 42, 21, 44,
   38, 32, 29, 23, 17, 11,  4, 62,
   46, 55, 26, 59, 40, 36, 15, 53,
   34, 51, 20, 43, 31, 22, 10, 45,
   25, 39, 14, 33, 19, 30,  9, 24,
   13, 18,  8, 12,  7,  6,  5, 63
};


// big endian
namespace Bitboard
{
  uint64_t setOne(uint64_t bb ,uint8_t square);
  bool getOne(uint64_t bb, uint8_t square);
  uint64_t setZero(uint64_t bb ,uint8_t square);
  //
  // bb rotate & mirror
  uint64_t bbVFlip(uint64_t bb);

  uint64_t bbHFlip(uint64_t bb);

  uint8_t popCount(uint64_t x);

  uint32_t find1LSB(uint64_t bb);
  uint32_t find1MSB(uint64_t bb);

  void genVeNorth(uint64_t square[64]);
  void genDiWeNorth(uint64_t square[64]);
  void genDiEaNorth(uint64_t square[64]);
  void genHoEast(uint64_t square[64]);

  void genVeSouth(uint64_t square[64]);
  void genDiWeSouth(uint64_t square[64]);
  void genDiEaSouth(uint64_t square[64]);
  void genHoWest(uint64_t square[64]);


  }





