#include "Zobrist.hpp"
#include<cstdlib>
#include"Pieces.hpp"
#include<ctime>
#include<unordered_set>
#include"Bitboards.hpp"
#include"GameOverCallback.hpp"
#include<iostream>


  HashTable::HashTable(const HashTable &p)
  {
    this->size = p.size;
    this->table = new cell*[this->size];
    
    for(int i =0; i < this->size;i++)
    {
      if(p.table[i] != nullptr)
      {
        cell* prev = nullptr;
        for(cell* cur = p.table[i]; cur != nullptr;cur = cur->next)
        {
          cell* newEl = new cell;
          newEl->count = cur->count;
          newEl->value = cur->value;
          newEl->next = nullptr;
          if(prev == nullptr)
            this->table[i] = newEl;
          else
          {
            prev->next = newEl;
          }
          prev = newEl;
        }
      }
      else
        this->table[i] = nullptr;
     
    }
    
  }

  HashTable::HashTable(int size)
  {
    if(size < 0)
    {
      exit(5);
    }
    
    this->size = size;
    
    table = new cell*[size];
    
    for(int i = 0; i <size;i++)
      table[i] = nullptr;
  }

  cell* HashTable::count(uint64_t value)
  {
    int ii = value % size;
    for(cell* cur = table[ii]; cur!=nullptr;cur = cur->next )
      if(cur->value == value ) return cur;
    
    return nullptr;
  }

  int HashTable::insert(uint64_t value)
  {
    
    int repCount = 1;
    cell* temp = count(value);
    if(temp != nullptr)
    {
      temp->count ++;

      // Three move repetition callback 
      if(temp->count == 3)
      {
        return 3;
      }
      return 0;
    }
    
    
    int ii = value % size;
    
    cell* newEl = new cell;
    newEl->count = 1;
    newEl->value = value;
    newEl->next = table[ii];
    table[ii] = newEl;
    return 0;
  }

  bool HashTable::remove(uint64_t value)
  {
    int ii = value % size;
    cell* prev = nullptr;
    for(cell* cur = table[ii]; cur!=nullptr;cur = cur->next )
    {
      if(cur->value == value)
      {
        if(prev == nullptr)
        {
          table[ii] = cur->next;
          delete cur;
          return true;
        }
        
        prev->next = cur->next;
        delete cur;
        return false;
      }
      prev = cur;
    }
       return false;
  }


  void HashTable::clear()
  {
    for(int i =0; i < size;i++)
    {
      cell* temp =table[i];
      while(temp != nullptr)
      {
        cell* next = temp->next;
        delete temp;
        temp = next;
      }
      
      table[i] = nullptr;
    }
    

    
  }

  HashTable::~HashTable()
  {
    clear();
    delete []table;
  }


  uint64_t ZobristHash::RandomNumberGen(uint32_t randIdx)
  {
    srand(SEED+randIdx);
    
    return rand()^((rand() << 13) ^ (rand() << 19));
  }


  ZobristHash::ZobristHash(uint32_t SEED)
  {
    this->SEED = SEED;
    currentPosition =0;
    this->hash = new HashTable(1000);
    this->RandomNumbers = new uint64_t[RandomArrSize];
    genRandomNumbers();
    
  }
  
  ZobristHash::~ZobristHash()
  {
    delete[] RandomNumbers;
  }


  void ZobristHash::genRandomNumbers()
  {
    srand(static_cast<unsigned int>(time(NULL)));
    std::unordered_set<uint64_t> origSet;
    
    for(int i = 0; i < RandomArrSize;i++)
    {
      uint64_t genNum =RandomNumberGen(rand()^rand());
      if(origSet.count(genNum))
      {
        i--;
        continue;
      }
      origSet.insert(genNum);
      RandomNumbers[i] = genNum;
  }
  
}



uint64_t ZobristHash::initFullBoard(Pieces pieces)
{
  uint64_t position =0;
  
  for(int i = PAWN; i <=KING;i++)
  {
    for(int j =0; j < 64;j++)
    {
      if(pieces.getWhite()[i] & (1LL << j))
      {
        uint64_t temp =RandomNumbers[j+(i*64)];
        position ^=temp;
      }
    }
  }
  
  for(int i = PAWN; i <=KING;i++)
  {
    for(int j =0; j < 64;j++)
    {
      if(pieces.getBlack()[i] & (1LL << j))
      {
        position ^= RandomNumbers[j+(i*64)+6*64];
      }
    }
  }
  
  
    
  hash->insert(position);
  currentPosition = position ;
  return position;
  
}

uint64_t ZobristHash::pieceMove(uint8_t moveType, uint8_t piece1,uint8_t side1, uint32_t posA, uint32_t posB,uint8_t piece2,uint8_t side2, uint8_t pieceIfCaptureAndTransform, bool clearAllow)
{
  uint64_t position =currentPosition;
  
  switch(moveType)
  {
    case PUSH:
    {
      position = position ^ RandomNumbers[side1*6*64 + piece1 * 64 + posA];
      position = position ^ RandomNumbers[side1*6*64 + piece1 * 64 + posB];
      break;
    }
    case CAPTURE:
    {
      if(clearAllow)
      hash->clear();
      
      break;
    }
    case CASTLKINGSIDE:
    {
      if(side1 == WHITE)
      {
        position = position ^ RandomNumbers[side1*6*64 + KING * 64 + 3];
        position = position ^ RandomNumbers[side1*6*64 + KING * 64 + 1];
        
        position = position ^ RandomNumbers[side1*6*64 + ROOK * 64 + 0];
        position = position ^ RandomNumbers[side1*6*64 + ROOK * 64 + 2];
        
      }
      else
      {
        position = position ^ RandomNumbers[side1*6*64 + KING * 64 + 59];
        position = position ^ RandomNumbers[side1*6*64 + KING * 64 + 57];
        
        position = position ^ RandomNumbers[side1*6*64 + ROOK * 64 + 56];
        position = position ^ RandomNumbers[side1*6*64 + ROOK * 64 + 58];
      }
      
      break;
    }
    case CASTLQUEENSIDE:
    {
      if(side1 == WHITE)
      {
        position = position ^ RandomNumbers[side1*6*64 + KING * 64 + 3];
        position = position ^ RandomNumbers[side1*6*64 + KING * 64 + 5];
        
        position = position ^ RandomNumbers[side1*6*64 + ROOK * 64 + 7];
        position = position ^ RandomNumbers[side1*6*64 + ROOK * 64 + 4];
      }
      else
      {
        position = position ^ RandomNumbers[side1*6*64 + KING * 64 + 59];
        position = position ^ RandomNumbers[side1*6*64 + KING * 64 + 61];
        
        position = position ^ RandomNumbers[side1*6*64 + ROOK * 64 + 63];
        position = position ^ RandomNumbers[side1*6*64 + ROOK * 64 + 60];
      }
      break;
    }
    case TRANSFORM:
    {
      position = position ^ RandomNumbers[side1*6*64 + piece1 * 64 + posA];
      position = position ^ RandomNumbers[side1*6*64 + piece2 * 64 + posB];
      
      break;
    }
    case CAPTURETRANSFROM:
    {
   if(clearAllow)
      hash->clear();
      break;
    }
    case ENPASSENT:
    {
      if(clearAllow)
      hash->clear();
      break;
    }
    default:
      error(__LINE__,"Unknown move type in zobristHash!\n");
      return -1;
      
  }


  return currentPosition = position;
}


ZobristHash::ZobristHash(const ZobristHash &p)
{
  this->SEED = p.SEED;
  this->currentPosition = p.currentPosition;
  
  this->RandomNumbers = new uint64_t[RandomArrSize];
  
  for(int i =0; i < RandomArrSize;i++)
  {
    this->RandomNumbers[i] = p.RandomNumbers[i];
  }

  this->hash = new HashTable(*p.hash);
}


HashTable* ZobristHash::getTable()
{
  return hash;
}
