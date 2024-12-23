#include<cstdint>
#include"Pieces.hpp"
#pragma once

struct cell
{
  uint64_t value;
  uint32_t count;
  cell *next;
};

class HashTable
{

  int size;
  cell** table;
  public:
  
  HashTable(int size=500);
  HashTable() = default;
  
  ~HashTable();
  HashTable(const HashTable &p);
  
  cell* count(uint64_t value);
  int insert(uint64_t value);
  bool remove(uint64_t value);
  void clear();
  
};

class ZobristHash
{
public:
 
  HashTable *hash;
  uint32_t SEED;
  uint64_t currentPosition;
  uint32_t RandomArrSize = 781;
  uint64_t RandomNumberGen(uint32_t randIdx);

  uint64_t *RandomNumbers;
  
  ZobristHash(uint32_t SEED);
  ~ZobristHash();
  ZobristHash(const ZobristHash &p);
  
  void genRandomNumbers();
  // init first position (then just update by pieceMove)
  uint64_t initFullBoard(Pieces pieces);
  //moveType - PUSH, CAPTURE,CASTLKINGSIDE,CASTLQUEENSIDE, TRANSFORM, add new hash to hasttable
  uint64_t pieceMove(uint8_t moveType, uint8_t piece1,uint8_t side1, uint32_t posA, uint32_t posB,uint8_t piece2,uint8_t side2, uint8_t pieceIfCaptureAndTRansform, bool clearAllow = true);
  HashTable* getTable();
  
};
