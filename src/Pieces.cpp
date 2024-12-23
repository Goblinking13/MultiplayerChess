#include"Pieces.hpp"
#include"Bitboards.hpp"
#include<stdio.h>
#include"GUI.hpp"
#include<array>
#include<iostream>

//GUI temp;
Pieces::Pieces(std::string FEN)
{
  
  for(int i =PAWN;i<=KING;i++)
  {
    white[i] = 0ULL;black[i]=0ULL;
  }
  
  side[BLACK] = 0ULL;
  side[WHITE] = 0ULL;
  empty =0ULL;
  all = 0ULL;
  
  uint32_t pos =63;
  for(int i =0; i < FEN.size();i++)
  {
    
    char ch = FEN[i];
    if(ch == '/')
      continue;
    if(isdigit(ch))
    {
      pos-=ch-'0';
      continue;
    }
    
    
    uint64_t* curBoard = white;
    if(islower(ch))
      curBoard = black;
   ch = tolower(ch);
    
    switch(ch)
    {
      case 'p':{
        curBoard[PAWN] =  Bitboard::setOne(curBoard[PAWN],pos) | curBoard[PAWN];
        break;
      }
      case 'n':{
        curBoard[KNIGHT] |=  Bitboard::setOne(curBoard[KNIGHT],pos);
        break;
      }
      case 'b':{
        curBoard[BISHOP] |=  Bitboard::setOne(curBoard[BISHOP],pos);
        break;
      }
      case 'r':{
        curBoard[ROOK] |=  Bitboard::setOne(curBoard[ROOK],pos);
        break;
      }
      case 'q':{
        curBoard[QUEEN] |=  Bitboard::setOne(curBoard[QUEEN],pos);
        break;
      }
      case 'k':{
        curBoard[KING] |=  Bitboard::setOne(curBoard[KING],pos);
        break;
      }
        
      default:
        perror("Wrong character!!111");
        exit(-1);
    }
    
    pos--;
  }
  
  for(int i =PAWN; i <= KING;i++)
  {
    side[BLACK] |= black[i];
    side[WHITE] |= white[i];
  }
  
  uAll();
  uEmpty();
  
  isWLeftMoved =0;
  isWRightMoved =0;
  
  isBLeftMoved =0;
  isBRightMoved =0;
  
  isWKingMoved =0;
  isBKingMoved =0;
  
  isCastledW =0;
  isCastledB =0;
  
  Check = false;

  enPassent = 0;
 
}

void Pieces::setWhiteCastled(bool stat)
{
  isCastledW = stat;
}
void Pieces::setBlackCastled(bool stat)
{
  isCastledB = stat;
}

bool Pieces::isWhiteCastled() const
{
  return isCastledW;
}

bool Pieces::isBlackCastled() const
{
  return isCastledB;
}

bool Pieces::isCastled(uint8_t side) const
{
  if(side ==WHITE)
    return isWhiteCastled();
  else
    return isBlackCastled();
}


bool Pieces::isWhiteKingMoved() const
{
  return isWKingMoved;
}

bool Pieces::isBlackKingMoved() const
{
  return isBKingMoved;
}

bool Pieces::isWhiteLRookMoved() const
{
  return isWLeftMoved;
}
bool Pieces::isWhiteRRookMoved() const
{
  return isWRightMoved;
}


bool Pieces::isBlackLRookMoved() const
{
  return isBLeftMoved;
}
bool Pieces::isBlackRRookMoved() const
{
  return isBRightMoved;
}

void Pieces::setEmpty(uint64_t empty)
{
  this->empty = empty;
}

void Pieces::setAll(uint64_t all)
{
  this->all = all;
}


bool Pieces::isLRookMoved(uint8_t side) const
{
  if(side==WHITE)
    return isWhiteLRookMoved();
  else
    return isBlackLRookMoved();
}
bool Pieces::isRRookMoved(uint8_t side) const
{
  if(side==WHITE)
    return isWhiteRRookMoved();
  else
    return isBlackRRookMoved();
}

bool Pieces::isKingMoved(uint8_t side) const
{
  if(side == WHITE)
    return isWhiteKingMoved();
  else
    return isBlackKingMoved();
}

void Pieces::uSideB()
{
  side[BLACK] = (black[PAWN] | black[KNIGHT] | black[BISHOP] | black[QUEEN] | black[ROOK] | black[KING]);
}

void Pieces::uSideW()
{
  side[WHITE] = (white[PAWN] | white[KNIGHT] | white[BISHOP] | white[QUEEN] | white[ROOK] | white[KING]);
}

void Pieces::uAll()
{
  all = (side[BLACK] | side[WHITE]);
}

void Pieces::uEmpty()
{
  empty = ~all;
}

void Pieces::setWhiteKingMoved(bool stat)
{
  isWKingMoved = stat;
}

void Pieces::setBlackKingMoved(bool stat)
{
  isBKingMoved = stat;
}

void Pieces::setKingMove(uint8_t side, bool stat)
{
  if(side == WHITE)
    setWhiteKingMoved(stat);
  else
    setBlackKingMoved(stat);
}

void Pieces::setWhiteLRookMoved(bool stat)
{
  isWLeftMoved = stat;
}
void Pieces::setWhiteRRookMoved(bool stat)
{
  isWRightMoved = stat;
}

void Pieces::setBlackLRookMoved(bool stat)
{
  isBLeftMoved = stat;
}
void Pieces::setBlackRRookMoved(bool stat)
{
  isBRightMoved = stat;
}

void Pieces::setLRookMoved(uint8_t side, bool stat)
{
  if(side == WHITE)
    setWhiteLRookMoved(stat);
  else
    setBlackLRookMoved(stat);
}

void Pieces::setRRookMoved(uint8_t side, bool stat)
{
  if(side == WHITE)
    setWhiteRRookMoved(stat);
  else
    setBlackRRookMoved(stat);
}


//void Pieces::uSideBi()
//{
//  invertSide[BLACK] = ~side[BLACK];
//}
//
//void Pieces::uSideWi()
//{
//  invertSide[WHITE] = ~side[WHITE];
//}


void Pieces::QuietMove(uint32_t a, uint32_t b, uint8_t piece, uint8_t side)
{

  uint64_t fromBB = Bitboard::setOne(0ULL, a);
  uint64_t toBB = Bitboard::setOne(0ULL, b);
  uint64_t fromToBB = fromBB | toBB;
  
  uint64_t *pieces = white;
  uint64_t *PiecesSide = &this->side[WHITE];
  
  if(side == BLACK)
  {
    pieces = black;
    PiecesSide = &this->side[BLACK];
  }
  
  pieces[piece] = pieces[piece] ^ fromToBB;
  
  *PiecesSide = *PiecesSide ^ fromToBB;
  all = all ^fromToBB;
  empty = empty ^ fromToBB;
  
}


//void Pieces::CaptureMove(uint32_t a, uint32_t b ,uint8_t piece, uint8_t cPiece, uint64_t mPieces[6],uint64_t cPieces[6], uint64_t *mSide,uint64_t *cSide )
void Pieces::CaptureMove(uint32_t a, uint32_t b ,uint8_t piece, uint8_t captured, uint8_t side)
{
  
  uint64_t fromBB = Bitboard::setOne(0ULL, a);
  uint64_t toBB = Bitboard::setOne(0ULL, b);
  uint64_t fromToBB = fromBB | toBB;
  
  uint64_t *pieces1 = white;
  uint64_t *pieces2 = black;
  uint64_t *PiecesSide1 = &this->side[WHITE];
  uint64_t *PiecesSide2 = &this->side[BLACK];
  
  if(side == BLACK)
  {
    std::swap(pieces1,pieces2);
    std::swap(PiecesSide1, PiecesSide2);
  }
  
  
  pieces1[piece] = pieces1[piece] ^ fromToBB;
  *PiecesSide1 = *PiecesSide1 ^ fromToBB;
  
  pieces2[captured] = pieces2[captured] ^ toBB;
  *PiecesSide2 = *PiecesSide2^ toBB;
  
  all = all ^fromBB;
  empty = empty ^ fromBB;
  
}

void Pieces::WhiteOO()
{
  uint64_t fromKingBB = Bitboard::setOne(0ULL, 3);
  uint64_t toKingBB = Bitboard::setOne(0ULL, 1);
  uint64_t fromToKingBB = fromKingBB | toKingBB;
  
  uint64_t fromRookBB = Bitboard::setOne(0ULL, 0);
  uint64_t toRookBB = Bitboard::setOne(0ULL, 2);
  uint64_t fromToRookBB = fromRookBB | toRookBB;
  
  white[KING] = white[KING] ^ fromToKingBB;
  white[ROOK] = white[ROOK] ^ fromToRookBB;
  
  empty ^= fromToKingBB;
  all ^= fromToKingBB;
  side[WHITE]^= fromToKingBB;
  
  empty ^= fromToRookBB;
  all ^= fromToRookBB;
  side[WHITE]^= fromToRookBB;
  
}


void Pieces::WhiteOOO()
{
  uint64_t fromKingBB = Bitboard::setOne(0ULL, 3);
  uint64_t toKingBB = Bitboard::setOne(0ULL, 5);
  uint64_t fromToKingBB = fromKingBB | toKingBB;
  
  uint64_t fromRookBB = Bitboard::setOne(0ULL, 7);
  uint64_t toRookBB = Bitboard::setOne(0ULL, 4);
  uint64_t fromToRookBB = fromRookBB | toRookBB;
  
  white[KING] = white[KING] ^ fromToKingBB;
  white[ROOK] = white[ROOK] ^ fromToRookBB;
  
  empty ^= fromToKingBB;
  all ^= fromToKingBB;
  side[WHITE]^= fromToKingBB;
  
  empty ^= fromToRookBB;
  all ^= fromToRookBB;
  side[WHITE]^= fromToRookBB;
  
}


bool Pieces::getCheck() const
{
  return Check;
}

void Pieces::setCheck(bool stat)
{
  Check = stat;
}


void Pieces::BlackOO()
{
  uint64_t fromKingBB = Bitboard::setOne(0ULL, 59);
  uint64_t toKingBB = Bitboard::setOne(0ULL, 57);
  uint64_t fromToKingBB = fromKingBB | toKingBB;
  
  uint64_t fromRookBB = Bitboard::setOne(0ULL, 56);
  uint64_t toRookBB = Bitboard::setOne(0ULL, 58);
  uint64_t fromToRookBB = fromRookBB | toRookBB;
  
  black[KING] = black[KING] ^ fromToKingBB;
  black[ROOK] = black[ROOK] ^ fromToRookBB;
  
  empty ^= fromToKingBB;
  all ^= fromToKingBB;
  side[BLACK]^= fromToKingBB;
  
  empty ^= fromToRookBB;
  all ^= fromToRookBB;
  side[BLACK]^= fromToRookBB;
  
}


void Pieces::BlackOOO()
{
  uint64_t fromKingBB = Bitboard::setOne(0ULL, 59);
  uint64_t toKingBB = Bitboard::setOne(0ULL, 61);
  uint64_t fromToKingBB = fromKingBB | toKingBB;
  
  uint64_t fromRookBB = Bitboard::setOne(0ULL, 63);
  uint64_t toRookBB = Bitboard::setOne(0ULL, 60);
  uint64_t fromToRookBB = fromRookBB | toRookBB;
  
  black[KING] = black[KING] ^ fromToKingBB;
  black[ROOK] = black[ROOK] ^ fromToRookBB;
  
  empty ^= fromToKingBB;
  all ^= fromToKingBB;
  side[BLACK]^= fromToKingBB;
  
  empty ^= fromToRookBB;
  all ^= fromToRookBB;
  side[BLACK]^= fromToRookBB;
  
}


void Pieces::Transform(uint32_t a, uint32_t b , uint8_t piece1, uint8_t piece2,uint8_t side)
{
  uint64_t fromBB = Bitboard::setOne(0ULL, a);
  uint64_t toBB = Bitboard::setOne(0ULL, b);
  uint64_t fromToBB = fromBB | toBB;
  
  uint64_t *pieces = white;
  uint64_t *PieceSide = &this->side[WHITE];
  if(side == BLACK)
  {
    pieces = black;
    PieceSide = &this->side[BLACK];
  }
  
  pieces[piece1] = pieces[piece1] ^ fromBB;
  pieces[piece2] = pieces[piece2] ^ toBB;
  
  *PieceSide = *PieceSide ^ fromToBB;
  empty = empty ^ fromToBB;
  all = all ^ fromToBB;
  
}


void Pieces::CaptureTransfrom(uint32_t a, uint32_t b , uint8_t piece, uint8_t capPiece,uint8_t transPiece,uint8_t side)
{
  uint64_t fromBB = Bitboard::setOne(0ULL, a);
  uint64_t toBB = Bitboard::setOne(0ULL, b);
  uint64_t fromToBB = fromBB | toBB;
  
  uint64_t *pieces1 = white;
  uint64_t *capPieces = black;
  uint64_t *PieceSide1 = &this->side[WHITE];
  uint64_t *PieceSide2 = &this->side[BLACK];
  
  if(side == BLACK)
  {
    std::swap(piece,capPiece);
    std::swap(PieceSide1, PieceSide2);
  }
  
  pieces1[piece] = pieces1[piece] ^ fromBB;
  pieces1[transPiece] = pieces1[transPiece] ^ toBB;
  capPieces[capPiece] = capPieces[capPiece] ^ toBB;

  
  *PieceSide1 = *PieceSide1 ^ fromToBB;
  *PieceSide2 = *PieceSide2 ^ toBB;
  
  empty = empty ^ fromToBB;
  all = all ^ fromToBB;
  
}

void Pieces::EnPassent(uint32_t a, uint32_t b,uint8_t side)
{
  uint64_t fromBB = Bitboard::setOne(0ULL, a);
  uint64_t toBB = Bitboard::setOne(0ULL, b);
  uint64_t fromToBB = fromBB | toBB;
  
  uint64_t *pieces = white;
  uint64_t *capPieces = black;
  uint64_t *PieceSide1 = &this->side[WHITE];
  uint64_t *PieceSide2 = &this->side[BLACK];
  int shift = -8;

  if(side == BLACK)
  {
    shift = 8;
    std::swap(PieceSide1, PieceSide2);
  }
  uint64_t enpassentPawn = Bitboard::setOne(0ULL, b + shift);
  pieces[PAWN] = pieces[PAWN] ^ fromToBB;
  capPieces[PAWN] = capPieces[PAWN] ^ enpassentPawn;
  
  *PieceSide1 = *PieceSide1 ^ fromToBB;
  *PieceSide2 =*PieceSide2 ^ enpassentPawn;
  
  empty = empty ^ fromToBB;
  all = all ^ fromToBB;
  empty = empty ^ enpassentPawn;
  all = all ^ enpassentPawn;
}

uint64_t Pieces::getKingBB(uint8_t side) const
{
  if(side == WHITE)
    return white[KING];
  else
  return black[KING];
}

uint64_t Pieces::getQueenBB(uint8_t side) const
{
  if(side == WHITE)
    return white[QUEEN];
  else
  return black[QUEEN];
}

uint64_t Pieces::getRookBB(uint8_t side) const
{
  if(side == WHITE)
    return white[ROOK];
  else
  return black[ROOK];
}

uint64_t Pieces::getKnightBB(uint8_t side) const
{
  if(side == WHITE)
    return white[KNIGHT];
  else
  return black[KNIGHT];
}

uint64_t Pieces::getBishopBB(uint8_t side) const
{
  if(side == WHITE)
    return white[BISHOP];
  else
  return black[BISHOP];
}

uint64_t Pieces::getPawnBB(uint8_t side) const
{
  if(side == WHITE)
    return white[PAWN];
  else
  return black[PAWN];
}

uint64_t Pieces::invertSide(uint8_t side) const
{
  if(side == WHITE)
    return this->side[BLACK];
  else
    return this->side[WHITE];
}

uint64_t Pieces::getEmpty() const
{
  return empty;
}

uint64_t Pieces::getAll() const
{
  return all;
}

uint64_t* Pieces::getSideArr() 
{
  return side;
}

uint64_t* Pieces::getWhite()
{
  return white;
}

uint64_t* Pieces::getBlack()
{
  return black;
}

uint64_t Pieces::getEnPassent() const
{
  return enPassent;
}

void Pieces::setEnPassent(uint8_t stat)
{
  enPassent = stat;
}

void Pieces::setCastled(uint8_t side, bool stat)
{
  if(side ==WHITE)
    setWhiteCastled(stat);
  else
    setBlackCastled(stat);
}

uint64_t* Pieces::getSidePieceBB(uint8_t side)
{
  if(side == WHITE)
    return getWhite();
  else
    return getBlack();
}

uint8_t Pieces::getPieceType(uint32_t square, uint8_t side)
{
  uint64_t sqBB = (1ULL << square);
  uint64_t* pieces = white;
  if(side == BLACK)
    pieces = black;
  for(int i = PAWN; i <= KING;i++)
  {
    if(pieces[i] & sqBB)
      return i;
  }
  
  return EMPTY;
}

void Pieces::makeOO(uint8_t side)
{
  if(side == WHITE)
    WhiteOO();
  else
    BlackOO();
}

void Pieces::makeOOO(uint8_t side)
{
  if(side == WHITE)
    WhiteOOO();
  else
    BlackOOO();
}

PieceList::PieceList()
{
  list = new piece**[2];
  list[BLACK] = new piece*[6];
  list[WHITE] = new piece*[6];
  
  memset(list[BLACK], NULL, sizeof(piece*)*6);
  memset(list[WHITE], NULL, sizeof(piece*)*6);
}

PieceList::PieceList(Pieces &position)
{
  list = new piece**[2];
  list[BLACK] = new piece*[6];
  list[WHITE] = new piece*[6];
  
  memset(list[BLACK], NULL, sizeof(piece*)*6);
  memset(list[WHITE], NULL, sizeof(piece*)*6);
  
  initList(position);
}

void PieceList::clear()
{
  for(int i =WHITE; i <= BLACK;i++)
  {
    for(int j = PAWN; j <= KING;j++)
    {
      piece* cur = list[i][j];
      while(cur != nullptr)
      {
        piece* temp = cur->next;
        
        if(cur != nullptr)
        {
          delete cur;
          cur = NULL;
        }
        
        cur = temp;
        
      }
      list[i][j] = nullptr;
    }
  }
}

PieceList::~PieceList()
{
  for(int i =0; i < 2;i++)
  {
    for(int j = PAWN; j <= KING;j++)
    {
      piece* cur = list[i][j];
      while(cur != nullptr)
      {
        piece* temp = cur->next;
        if(cur != nullptr)
          delete cur;
        cur = temp;
      }
    }
    delete[] list[i];
  }
  delete[] list;
}

piece* PieceList::add(piece p, uint8_t side)
{
 
  piece* newEl = new piece;
  newEl->type = p.type;
  newEl->pos = p.pos;
  piece* tempNext =list[side][p.type];
  newEl->next =tempNext;
  list[side][p.type] = newEl;
  return newEl;
}

void  PieceList::remove(piece p, uint8_t side)
{
  piece* prev = nullptr;
  for(piece* cur =list[side][p.type]; cur != nullptr;cur = cur->next)
  {
    if(cur->pos == p.pos)
    {
      if(prev == nullptr)
      {
        list[side][p.type] = list[side][p.type]->next;
        delete cur;
        return;
      }
      prev->next = cur->next;
      delete cur;
      return;
      
    }
    prev = cur;
  }
}

void PieceList::initList(Pieces &pos)
{
  uint64_t *curPieces;
  curPieces = pos.white;
  for(int i =WHITE; i <= BLACK;i++)
  {
    for(int j = PAWN; j <= KING;j++)
    {
      if(curPieces[j] ==0)
        continue;
      for(int k =0; k < 64;k++)
      {
        if(curPieces[j] & (1ULL << k))
        {
          piece temp;
          temp.type = j;
          temp.pos = k;
          add(temp, i);
        }
      }
    }
    
    curPieces = pos.black;
  }
}


void PieceList::printList()
{
  std::cout << "WHITE\n";
  piece** curList = list[WHITE];
  for(int i = PAWN; i <= KING;i++)
  {
    piece* temp = curList[i];
    while(temp!= nullptr)
    {
      std::cout << "Type: " << temp->type << "  pos: "<< temp->pos  << std::endl;
      temp = temp->next;
    }
  }
  std::cout << "BLACK\n";
  curList = list[BLACK];
  for(int i = PAWN; i <= KING;i++)
  {
    piece* temp = curList[i];
    while(temp!= nullptr)
    {
      std::cout << "Type: " << temp->type << "  pos: "<< temp->pos  << std::endl;
      temp = temp->next;
    }
  }
}


piece*** PieceList::getList(){
  return list;
}


