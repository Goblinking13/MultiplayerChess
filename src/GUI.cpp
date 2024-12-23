#include"GUI.hpp"
#include<ctype.h>

void GUI::DrawBits(uint64_t board)
{
  uint64_t temp = board;
  std::string word="";

  for(int i =63; i >= 0;i--)
  {
    std::cout << (((temp>>i)&1ULL) ? '1' : '.');
    if(i % 8 ==0 )
      std::cout << "\n";
  }
  std::cout << "==============================\n";
}

char getPiece(Pieces piece, uint32_t position)
{
  char cellPiece = ' ';
  int side = WHITE;
  uint64_t *PieceSide = piece.getWhite();
  
  if(Bitboard::getOne(piece.getSideArr()[WHITE], position) == 0)
  {
    PieceSide = piece.getBlack();
    side = BLACK;
    
    if(Bitboard::getOne(piece.getSideArr()[BLACK], position) ==0)
      return cellPiece;
  }
   
  

  
  
  
  for(int i = PAWN; i <= KING;i++)
  {
    if(Bitboard::getOne(PieceSide[i],position) == 1)
    {
      if(side == WHITE)
      {
        cellPiece = GUI::White[i];
        break;
      }
      cellPiece = GUI::Black[i];
      break;
      
    }
  }
  
  return cellPiece;
}

bool cmpWhite(int a, int b)
{
  return a >=b;
}

bool cmpBlack(int a, int b)
{
  return a <=b;
}



void GUI::DrawBoard(Pieces pieces, bool AttackPattern[64], uint8_t side)
{
  bool isNull = false;
  if(AttackPattern == nullptr)
  {
    AttackPattern = new bool[64];
    memset(AttackPattern,0,64);
    isNull = true;
    
  }
  std::string horLine = "   +---+---+---+---+---+---+---+---+";
  std::string cellAttack = "| * ";
  std::string cellNoAttack = "|   ";
  
  std::cout<< std::endl;
  
  bool (*cmp)(int,int) = cmpWhite;
  int a = 7 ,b =0,op = -1;
  if(side == BLACK){
    a = 0; b = 7; op = 1;
    cmp = cmpBlack;
  }
  
  for(int i = a;cmp(i,b);i+=op)
  {
    std::cout << horLine << std::endl;
    for(int j =7; j >= 0;j--)
    {
      if(j == 7)
        std::cout << "   ";
      
      if(AttackPattern[i*8 + j])
        std::cout << cellAttack;
      else
        std::cout << cellNoAttack;
    }
    std::cout<< "|\n";
    
    
    for(int j =7; j >= 0;j--)
    {
      if(j == 7)
        std::cout << " "<< i +1 << " ";
      
      if(AttackPattern[i*8 + j])
      {
        std::cout << "|*" << getPiece(pieces, i*8+j) << "*";
      }
      else
        std::cout << "| " << getPiece(pieces, i*8+j) << " ";

    }
    std::cout<< "|\n";
    
    
   // for(int j =0; j < 8;j++)
    for(int j =7; j >= 0;j--)
    {
      if(j == 7)
        std::cout << "   ";
      
      if(AttackPattern[i*8 + j])
        std::cout << cellAttack;
      else
        std::cout << cellNoAttack;
    }
    std::cout<< "|\n";
    
  }
  
  std::cout << horLine << std::endl;
  std::cout << "     a   b   c   d   e   f   g   h  \n";
  
  
  if(isNull)
  {
    free(AttackPattern);
  }
  
}


void GUI::setAttackPattern(uint64_t BB, bool *attackPattern)
{
  for(int i =0; i < 64;i++)
  {
    attackPattern[i] = (BB & (1ULL << i));
  }
}


