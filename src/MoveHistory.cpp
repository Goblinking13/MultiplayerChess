#include "MoveHistory.hpp"
#include "Bitboards.hpp"
#include<cstdint>


MoveHistory::MoveHistory()
{
  positionList = nullptr;
}

void MoveHistory::add(Position *pos)
{
  
  pos->next = positionList;
  positionList = pos;
  
}

void MoveHistory::removeLast()
{
  Position* cur = positionList;
  positionList = positionList->next;
  
  delete[] cur->white;
  delete[] cur->black;
  delete[] cur->side;
  
  delete cur;
  cur = nullptr;
  
}

void MoveHistory::removeN(int n)
{
  Position* prev = nullptr;
  for(Position* cur = positionList; cur != nullptr; cur = cur->next)
  {
    if(cur->moveCount < n)
      return;
    
    if(cur->moveCount == n)
    {
      if(prev == nullptr)
      {
        positionList = cur->next;
      }
      else{
        prev->next = cur->next;}
      
      delete[] cur->white;
      delete[] cur->black;
      delete[] cur->side;
   //   delete cur->hash;
      
      delete cur;
      cur = nullptr;
      return;
    }
  }
  
}


Position* Position::setPosition(Pieces &pieces)
{
  Position *pos = new Position;
  
  pos->white = new uint64_t[6];
  pos->black = new uint64_t[6];
  pos->side = new uint64_t[2];
  
  for(int i = PAWN; i <= KING;i++)
  {
    pos->white[i] = pieces.getWhite()[i];
    pos->black[i] = pieces.getBlack()[i];
  }
  pos->side[WHITE] = pieces.getSideArr()[WHITE];
  pos->side[BLACK] = pieces.getSideArr()[BLACK];
  
  pos->empty = pieces.getEmpty();
  pos->all = pieces.getAll();
  pos->Check = pieces.getCheck();
  
  pos->enPassent = pieces.getEnPassent();
  
  
  pos->isCastledB =pieces.isBlackCastled();
  pos->isCastledW = pieces.isWhiteCastled();
  
  pos->isWKingMoved =pieces.isWhiteKingMoved();
  pos->isWLeftMoved =pieces.isWhiteLRookMoved();
  pos->isWRightMoved = pieces.isWhiteRRookMoved();
  
  pos->isBKingMoved =pieces.isBlackKingMoved();
  pos->isBLeftMoved = pieces.isBlackLRookMoved();
  pos->isBRightMoved  = pieces.isBlackRRookMoved();
  
  return pos;
}

Position* MoveHistory::getLastMove()
{
  return positionList;
}

void Position::setHash(uint64_t ZobristVal)
{
  this->ZobristVal = ZobristVal;
}

void Position::setMoveCount(int m)
{
  this->moveCount = m;
}

void Position::setFiftyMoveCount(int f)
{
  this->fiftyMoveCount = f;
}


bool MoveHistory::isEmpty()
{
    return positionList == nullptr;
}


void Position::setPieces(Pieces &pieces )
{
  for(int i = PAWN; i <= KING;i++)
  {
    pieces.getBlack()[i] = black[i];
    pieces.getWhite()[i] = white[i];
  }
  
  pieces.getSideArr()[WHITE] = side[WHITE];
  pieces.getSideArr()[BLACK] = side[BLACK];
  
  pieces.setAll(all);
  pieces.setEmpty(empty);
  pieces.setCheck(Check);
  
  pieces.setCastled(WHITE, isCastledW);
  pieces.setCastled(BLACK, isCastledB);
  
  pieces.setKingMove(WHITE, isWKingMoved);
  pieces.setKingMove(BLACK, isBKingMoved);
  
  pieces.setLRookMoved(WHITE, isWLeftMoved);
  pieces.setLRookMoved(BLACK, isBLeftMoved);
  
  pieces.setRRookMoved(WHITE, isWRightMoved);
  pieces.setRRookMoved(BLACK, isBRightMoved);
}


