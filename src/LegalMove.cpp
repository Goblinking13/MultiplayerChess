#include "LegalMove.hpp"
#include"Bitboards.hpp"
#include"GUI.hpp"

bool LegalMove::isAttacked(uint32_t p, Pieces& curPos, PsLegalMove &atcPtrn ,uint8_t side )
{
  // pattern for all pieces execpt pawn
  uint64_t allAttack = atcPtrn.BishopPsLegal(curPos, p, side) | atcPtrn.RookPsLegal(curPos, p, side)| atcPtrn.KnightPsLegal(curPos, p, side)| atcPtrn.KingPsLegal(curPos, p, side);
// pattern for pawn
  atcPtrn.updatePawn(curPos, side);
  allAttack |= atcPtrn.PawnPsLegal(curPos, p, side) & ~(allFile[7-(p%8)]);
  // WHITE == !BLACK
  


  uint64_t BishopPs =atcPtrn.BishopPsLegal(curPos, p, side) ;
  uint64_t RookPs =atcPtrn.RookPsLegal(curPos, p, side);
  
  uint64_t pawn = atcPtrn.getAttackPattern()->GetPawnAttackBB(p, side);
  
  if(BishopPs & curPos.getBishopBB(!side))
    return true;
  if( RookPs& curPos.getRookBB(!side))
    return true;
  if((RookPs | BishopPs) & curPos.getQueenBB(!side))
    return true;
  if(pawn & curPos.getPawnBB(!side))
    return true;
  if( atcPtrn.KingPsLegal(curPos, p, side) & curPos.getKingBB(!side))
    return true;
  if( atcPtrn.KnightPsLegal(curPos, p, side) & curPos.getKnightBB(!side))
    return true;
  
  return false;
}

uint64_t LegalMove::CheckLegal(Pieces& curPos, PsLegalMove& PsLegMov, uint32_t initPos, uint8_t initPieceType,uint8_t side,uint64_t InitPsLegal)
{
  uint64_t initPieceBB = curPos.getSidePieceBB(side)[initPieceType], initCapPieceBB , initSideBB = curPos.getSideArr()[side], initCapSideBB , initAllBB = curPos.getAll(), initEmptyBB = curPos.getEmpty();
  uint64_t Checked =0;
  uint64_t Legal = 0;
  uint64_t psLegal = InitPsLegal;
  uint32_t KingSquare = Bitboard::find1LSB( curPos.getKingBB(side));
  

  while(psLegal ^ Checked)
  {
    uint32_t square = Bitboard::find1LSB(psLegal ^ Checked);
    if(initPieceType == KING)
      KingSquare = square;
    
    if(((1ULL <<square) & curPos.invertSide(side))==0)
    {
      curPos.QuietMove(initPos, square, initPieceType, side);
     if( !isAttacked(KingSquare, curPos, PsLegMov, side))
       Legal |= (1ULL <<square);
      curPos.empty =initEmptyBB;
      curPos.all = initAllBB;
      curPos.getSidePieceBB(side)[initPieceType] = initPieceBB;
      curPos.side[side] = initSideBB;
    }
    else
    {
      uint8_t capPieceType = curPos.getPieceType(square, !side);
      if(capPieceType == EMPTY)
      {
        continue;
      }
      assert(capPieceType <= 5); // piece type 0<= t <= 5
      initCapPieceBB = curPos.getSidePieceBB(!side)[capPieceType];
      initCapSideBB = curPos.getSideArr()[!side];
      
      curPos.CaptureMove(initPos, square,initPieceType , capPieceType,side);
      if(!isAttacked(KingSquare, curPos, PsLegMov, side))
        Legal |= (1ULL <<square);
      
      curPos.getSidePieceBB(!side)[capPieceType] = initCapPieceBB;
      curPos.side[!side] = initCapSideBB;
      curPos.empty =initEmptyBB;
      curPos.all = initAllBB;
      curPos.getSidePieceBB(side)[initPieceType] = initPieceBB;
      curPos.side[side] = initSideBB;
    }
    
   
    Checked |= (1ULL << square);
  }
  if(initPieceType == KING )
  Legal |=CheckCastelingLegal(curPos, PsLegMov, side, InitPsLegal);
  return Legal  ;
}


uint64_t LegalMove::CheckCastelingLegal(Pieces& curPos, PsLegalMove &PsLegMove,uint8_t side, uint64_t initPsLegal)
{
  if(curPos.isKingMoved(side) || curPos.getCheck())
    return 0ULL;
  
  uint64_t result = 0ULL;
  uint64_t LeftCast = 0b10000000;
  uint64_t RightCast =0b00000001;
  uint32_t OOPos, OOOPos;
  OOPos = 2; OOOPos = 5;
  uint64_t LeftSpace = 0b01110000;
  uint64_t RightSpace =0b00000110;
  uint64_t allPieces = curPos.getAll();
  
  if(side == BLACK)
  {
    LeftCast <<= 56;
    RightCast <<= 56;
    LeftSpace <<= 56;
    RightSpace <<= 56;
    OOPos +=56; OOOPos += 56;
  }
  
    if((initPsLegal & RightCast) && !isAttacked(OOPos ,curPos, PsLegMove, side) && !curPos.isRRookMoved(side) && !(allPieces & RightSpace))
    {
      result |= RightCast;
    }

    if((initPsLegal & LeftCast) && !isAttacked(OOOPos, curPos, PsLegMove, side) && !curPos.isLRookMoved(side) && !(allPieces & LeftSpace))
    {
      result |= LeftCast;
    }
  
  return result;
}


void LegalMove::genAllLegal(Pieces& curPos, PsLegalMove& PsLegMov,PieceList& curPieceList,uint8_t side)
{
  
  memset(LegalMv,0,sizeof(LegalMv));
  isAnyLegalMove = false;
  for(int i = KING; i>= PAWN;i--)
  {
   
    
    piece* curPiece = (curPieceList.getList())[side][i];
    while(curPiece != nullptr)
    {
      uint64_t legal = CheckLegal(curPos, PsLegMov,curPiece->pos, i, side,  PsLegMov.PseudoMv[curPiece->pos]);
      LegalMv[curPiece->pos] = legal;
      if(legal)
        isAnyLegalMove = true;
      curPiece = curPiece->next;
    }
  }
}

bool LegalMove::isAnyLegal() const
{
  return isAnyLegalMove;
}

void LegalMove::PrintAllLegalMove()
{
  
  std::cout << "All legal move\n";
    for(int j =0; j < 64;j++)
    {
      if(LegalMv[j] != 0)
      {
        std::cout <<"Pos: "<< j <<std::endl;
        GUI::DrawBits(LegalMv[j]);
      }

    }
  
}

uint64_t* LegalMove::getLegalMove()
{
  return LegalMv;
}
