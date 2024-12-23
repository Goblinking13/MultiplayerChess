#include "Move.hpp"
#include"GUI.hpp"

PsLegalMove::PsLegalMove(AttackPattern *atkPtn) 
{

  fooArr[0] =  &PsLegalMove::PawnPsLegal;
  fooArr[1] =  &PsLegalMove::KnightPsLegal;
  fooArr[2] =  &PsLegalMove::BishopPsLegal;
  fooArr[3] =  &PsLegalMove::RookPsLegal;
  fooArr[4] =  &PsLegalMove::QueenPsLegal;
  fooArr[5] =  &PsLegalMove::KingPsLegal;
  
  memset(PseudoMv,0, sizeof(PseudoMv));
  
  this->atkPtn = atkPtn;
}

uint64_t PsLegalMove::KnightPsLegal(Pieces &position, uint32_t square, uint8_t side)
{
  return  atkPtn->getAttack(KNIGHT, square) & (position.getEmpty() | position.invertSide(side));
}

uint64_t PsLegalMove::KingPsLegal(Pieces &position, uint32_t square, uint8_t side)
{
  uint64_t castling=0ULL;
  if(!position.isKingMoved(side))
  {
    if(side == WHITE)
    {
      if(!position.isRRookMoved(side)&&(( position.getEmpty() & 0b00000110) == 0b00000110 ) &&( position.getWhite()[ROOK] & 0b00000001 ))
      {
        castling |= 1ULL;
      }
       
      if(!position.isLRookMoved(side)&&(( position.getEmpty() & 0b01110000) ==0b01110000) &&( position.getWhite()[ROOK] & 0b10000000 ))
      { 
        castling |= 128;
      }
      
    }
    else
    {
      if(!position.isRRookMoved(side)&&(( position.getEmpty() & (0b00000110ULL << 56)) == (0b00000110ULL << 56)) &&( position.getBlack()[ROOK] & (0b00000001ULL  << 56)))
      {
        castling |= (1ULL << 56);
      }

      if(!position.isLRookMoved(side)&&((position.getEmpty() & (0b01110000ULL << 56)) == (0b01110000ULL << 56))&& (position.getBlack()[ROOK] & (0b10000000ULL  << 56)))
      {
        castling |= (128ULL << 56);
      }
    }
  }
  return castling | (atkPtn->getAttack(KING, square) & (position.getEmpty() | position.invertSide(side)));
}



uint64_t PsLegalMove::BishopPsLegal(Pieces &position, uint32_t square, uint8_t side)
{
  uint64_t WeNo = position.getAll() & atkPtn->getDirectionRay(NORTHWEST, square);
  if(WeNo){
    WeNo =atkPtn->getDirectionRay(NORTHWEST, Bitboard::find1LSB(WeNo));}
    WeNo ^=atkPtn->getDirectionRay(NORTHWEST, square);

  uint64_t EaNo = position.getAll() & atkPtn->getDirectionRay(NORTHEAST, square);
  if(EaNo){
    EaNo = atkPtn->getDirectionRay(NORTHEAST, Bitboard::find1LSB(EaNo));}
    EaNo ^= atkPtn->getDirectionRay(NORTHEAST, square);

  
  uint64_t WeSo = position.getAll() & atkPtn->getDirectionRay(SOUTHWEST, square);
  if(WeSo){
    WeSo = atkPtn->getDirectionRay(SOUTHWEST, Bitboard::find1MSB(WeSo));}
  WeSo^=atkPtn->getDirectionRay(SOUTHWEST, square);
  
  uint64_t EaSo = position.getAll() & atkPtn->getDirectionRay(SOUTHEAST, square);
  if(EaSo){
  EaSo  = atkPtn->getDirectionRay(SOUTHEAST, Bitboard::find1MSB(EaSo));}
  EaSo^=atkPtn->getDirectionRay(SOUTHEAST, square);
  
  
  uint64_t result = WeNo | EaNo | WeSo | EaSo;
  result &= ~position.getSideArr()[side];
  
  return result;
}


uint64_t PsLegalMove::RookPsLegal(Pieces &position, uint32_t square, uint8_t side)
{
  
  uint64_t North = position.getAll() & atkPtn->getDirectionRay(NORTH, square);
  if(North){
    North = atkPtn->getDirectionRay(NORTH, Bitboard::find1LSB(North));}
  North ^= atkPtn->getDirectionRay(NORTH, square);
  
  uint64_t South = position.getAll() & atkPtn->getDirectionRay(SOUTH, square);
  if(South){
    South = atkPtn->getDirectionRay(SOUTH, Bitboard::find1MSB(South));}
  South ^= atkPtn->getDirectionRay(SOUTH, square);
  
  uint64_t West= position.getAll() & atkPtn->getDirectionRay(WEST, square);
  if(West){
    West = atkPtn->getDirectionRay(WEST, Bitboard::find1LSB(West));}
  West ^= atkPtn->getDirectionRay(WEST, square);
  
  uint64_t East = position.getAll() & atkPtn->getDirectionRay(EAST, square);
  if(East){
    East= atkPtn->getDirectionRay(EAST, Bitboard::find1MSB(East));}
    East^= atkPtn->getDirectionRay(EAST, square);
  
  uint64_t result =  North | South | West | East;
  result &= ~position.getSideArr()[side];
  
  return result;
}

uint64_t PsLegalMove::QueenPsLegal(Pieces &position, uint32_t square, uint8_t side)
{
  return RookPsLegal(position, square, side) | BishopPsLegal(position, square, side);
}

void PsLegalMove::updatePawn(Pieces &position, uint8_t side)
{
 if(side == WHITE)
 {
   wPawnOne= AttackPattern::wPawnOnePush(position.getPawnBB(side), position.getEmpty());
   wPawnTwo= AttackPattern::wPawnTwoPush(position.getPawnBB(side), position.getEmpty());
 }
 else
 {
   bPawnOne= AttackPattern::bPawnOnePush(position.getPawnBB(side), position.getEmpty());
   bPawnTwo= AttackPattern::bPawnTwoPush(position.getPawnBB(side), position.getEmpty());
 }
}

uint64_t PsLegalMove::PawnPsLegal(Pieces &position, uint32_t square, uint8_t side)
{
  uint64_t attack = position.invertSide(side) & atkPtn->getAttack(PAWN, square,side);
 
  if(side == WHITE)
  {
    attack = attack | ((wPawnOne | wPawnTwo) & allFile[7 - (square % 8)]);
    
    if(position.getEnPassent() && (position.getPawnBB(WHITE)&Rank5))
      attack |= ((uint64_t)(position.getEnPassent())<<40 & atkPtn->getAttack(PAWN, square,side));
    
  }
  else
  {
    attack = attack | ((bPawnOne | bPawnTwo)& allFile[7 - (square % 8)]);
    
    if(position.getEnPassent() && (position.getPawnBB(BLACK)&Rank4))
      attack |= ((uint64_t)(position.getEnPassent())<<16 & atkPtn->getAttack(PAWN, square,side));
  }

  return attack;
}


void PsLegalMove::GenAllPsLegalMove(Pieces &position,PieceList &list, uint8_t side)
{
  uint64_t *curPieceArr= PseudoMv;
    for(int i = KING; i >= PAWN;i--)
    {
      piece* curPiece = (list.getList())[side][i];
      
      while(curPiece != nullptr)
      {
        uint64_t temp =(this->*fooArr[i])(position,curPiece->pos,side);
        curPieceArr[curPiece->pos] =temp;
        curPiece = curPiece->next;
      }
    }
}

void PsLegalMove::PrintAllPsLegalMove()
{
    for(int j =0; j < 64;j++)
    {
      if(PseudoMv[j] != 0)
      {
        std::cout <<"Pos: "<< j <<std::endl;
        GUI::DrawBits(PseudoMv[j]);
      }

    }
 
}



AttackPattern* PsLegalMove::getAttackPattern()
{
  return atkPtn;
}
