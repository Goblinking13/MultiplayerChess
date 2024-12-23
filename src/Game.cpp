#include<iostream>
#include<stdlib.h>
#include<vector>
#include"Multiplayer.hpp"
#include"GameOverCallback.hpp"
#include "Game.hpp"


Game::Game(std::string FEN ) : gamePosition(FEN), psLegalMove(&attackPattern), zobrist(912427321)
{
  

    
  moveCount = 0;
 memset(attackDrawSquare, 0, sizeof(attackDrawSquare));
  list.initList(gamePosition);
  lastMoveHash =  zobrist.initFullBoard(gamePosition);
  
  
  char alpha[] = {'a','b','c','d','e','f','g','h'};
  char digit[] = {'1','2','3','4','5','6','7','8'};
  
  for(int i =0; i < 8;i++)
  {
    for(int j = 0; j < 8;j++)
    {
      std::string temp;
      temp.push_back(alpha[i]);
      temp.push_back(digit[j]);
      
      Square[temp] = (7-i) + j*8;
      
    }
  }
  
  // casteling
  Square["O-O"] = 64;
  Square["O-O-O"] = 65;
  
  memset(board, NULL, sizeof(board));
  
  for(int i = PAWN; i <= KING;i++)
  {
    piece* white = list.getList()[WHITE][i];
    piece* black= list.getList()[BLACK][i];
    while(white != nullptr)
    {
      board[white->pos] = white;
      white = white->next;
    }
    while(black != nullptr)
    {
      board[black->pos] = black;
      black = black->next;
    }
  }
  
};


uint8_t Game::getSide()
{
  if(moveCount % 2 == 1)
    return BLACK;
  else
  return WHITE;
}

bool Game::fastMoveCheck(uint32_t A, uint32_t B)
{
  if(gamePosition.getSideArr()[getSide()] & (1ULL  << B))
    return false;
  else
  return true;
}


uint8_t Game::getPieceType(uint32_t square) const
{
  uint64_t sBB = 1ULL << square;
  for(int i = PAWN; i <= KING;i++)
  {
    if((gamePosition.white[i] & sBB )|| (gamePosition.black[i] & sBB))
      return i;
  }
  
  return EMPTY;
}

int Game::MakeMove(uint32_t moveFrom, uint32_t moveTo)
{
  uint8_t side = getSide();
  if(moveTo == 111)
  {
    switch(moveFrom)
    {//O-O
      case 64:{
        if(!gamePosition.isRRookMoved(side))
        {
          gamePosition.makeOO(side);
          curMoveInfo.moveType = CASTLKINGSIDE;
          curMoveInfo.pieceType = KING;
          return 1;
        }
        return 0;
      }//O-O-O
      case 65:{
        if(!gamePosition.isLRookMoved(side))
        {
          gamePosition.makeOOO(side);
          curMoveInfo.pieceType = KING;
          curMoveInfo.moveType = CASTLQUEENSIDE;
          return 1;
        }
        return 0;
      }
      default:
      {
        GUI::setAttackPattern(legalMove.getLegalMove()[moveFrom],attackDrawSquare);
        return 0;
      }
    }
  }

  
  uint8_t moveType = PUSH;
  uint64_t moveToBB = 1ULL << moveTo;
  uint8_t pieceType = getPieceType(moveFrom);
  uint8_t captureType =getPieceType(moveTo);
  uint64_t curLegalMove =legalMove.getLegalMove()[moveFrom] ;
  if(!fastMoveCheck(moveFrom, moveTo))
  {
    invalidMove(); return 0;
  }
  if(pieceType == EMPTY)
  {
    invalidMove("The square is empty\n"); return 0;
  }
  if(board[moveFrom] == nullptr)
  {
    error(__LINE__);
  }
  
  if(board[moveFrom]->type == PAWN)
  {
   
    if(side == WHITE && (moveToBB & Rank8))
      moveType = TRANSFORM;
    else if(side == BLACK && (moveToBB & Rank1))
      moveType = TRANSFORM;
    if(gamePosition.getEnPassent()&& !(allFile[7 - (moveFrom % 8)] & moveToBB) &&( gamePosition.getEmpty() & moveToBB))
      moveType = ENPASSENT;
  }
  
  
  if(!(curLegalMove& (1ULL << moveTo)))
  {
    invalidMove(); return 0;
  }
  
  if((moveToBB & gamePosition.getSideArr()[!side] ))
  {
    if(moveType == TRANSFORM)
    moveType = CAPTURETRANSFROM;
    else
    moveType = CAPTURE;
  }

  


  
  switch(moveType)
  {
    case PUSH:
    {
      gamePosition.QuietMove(moveFrom, moveTo, pieceType, side);
      if(pieceType == PAWN && ((getSide() == WHITE && moveFrom + 2*8 == moveTo)  || (getSide() == BLACK && moveFrom -2*8 == moveTo)))
      {
          gamePosition.setEnPassent(allEnPassent[7 - (moveFrom % 8)]);
      }else
        gamePosition.setEnPassent(0);
      break;
    }
    case CAPTURE:
    {  gamePosition.CaptureMove(moveFrom, moveTo, pieceType, captureType, side);
     
      break;
    }
    case TRANSFORM:
    {

        uint8_t TransType= pickPieceToTrnasform();
      
      
      gamePosition.Transform(moveFrom, moveTo, pieceType, TransType, side);
      curMoveInfo.transformType = TransType;
      break;
    }
    case ENPASSENT:
    {
      gamePosition.EnPassent(moveFrom, moveTo, side);
      captureType = PAWN;
      break;
    }
    case CAPTURETRANSFROM:
    {
      uint8_t TransType = pickPieceToTrnasform();
      gamePosition.CaptureTransfrom(moveFrom, moveTo, pieceType, captureType, TransType, side);
      curMoveInfo.transformType = TransType;
      break;
    }
    default:
    {
      error(__LINE__,"Unknown move type"); return 0;
    }
  }

  curMoveInfo.moveFrom =moveFrom;
  curMoveInfo.moveTo = moveTo;
  curMoveInfo.pieceType = pieceType;
  curMoveInfo.capturedType =captureType;
  curMoveInfo.moveType = moveType;
  
  return true;
}


uint32_t Game::getSquare(std::string s)
{
  if(!Square.count(s))
    return 111;
  
  return Square[s];
}

uint8_t Game::pickPieceToTrnasform()
{
  if(isEngine)
    return QUEEN;
  
  std::cout << "Pick piece: ";
  char type;
  while(1)
  {
    std::cin >> type;
    type =tolower(type);
    switch(type)
    {
      case 'q':
        return QUEEN;
      case 'r':
        return ROOK;
      case 'b':
        return BISHOP;
      case 'n':
        return KNIGHT;
      default:
      {
        std::cout << "Wrong type!!\n";
        continue;
      }
    }
  }
  return 0;
}

std::vector<uint32_t> Game::parser()
{
  std::vector<std::string> input(2);
  std::vector<uint32_t> fromTo(2);
  char ch;
  int inputCount =0;
  std::string temp;
  while(std::cin.get(ch))
  {
    if(ch == '\n' )
    {
      if(temp != ""){
        input[inputCount] = temp;inputCount++;
      }
      
      temp= "";
      break;
    }
     
    if(isspace(ch))
    {
      if(temp != "")
      {
        input[inputCount] = temp; inputCount++;
      }
      temp= "";
      
      continue;
    }
    
    temp+=ch;
  }
  fromTo[0] = getSquare(input[0]);
  fromTo[1] = getSquare(input[1]);
  
  
  return fromTo;
}

int Game::preLoop()
{
  uint8_t side = getSide();
  psLegalMove.updatePawn(gamePosition, side);
  psLegalMove.GenAllPsLegalMove(gamePosition, list, side);
  legalMove.genAllLegal(gamePosition, psLegalMove, list, side);
  
  if(!legalMove.isAnyLegal() )
  {
    if(gamePosition.getCheck())
      return 1;
    else
      return 3;
  }
  return 0;
}

uint32_t Game::loop( std::vector<uint32_t> input)
{
  if(input[0]== 111)
  {
    invalidMove(); return 0;
  }
    
    int moveDone = MakeMove(input[0],input[1]);
    if(!moveDone)
      return 0;
  
  uint8_t side = getSide();
  if(curMoveInfo.moveType == CAPTURE)
  {
    fiftyMoveCount =0;
    list.remove(*board[input[1]], !getSide());
    board[input[1]] = nullptr;
  }
  
  if(curMoveInfo.moveType == ENPASSENT)
  {
    fiftyMoveCount =0;
    int shift = (getSide() == WHITE) ? -8 : 8;
    list.remove(*board[input[1]+shift], !getSide());
    board[input[1]+shift] = nullptr;
  }
  if(curMoveInfo.pieceType == PAWN)
  {
    fiftyMoveCount =0;zobrist.hash->clear();
  }

  if(curMoveInfo.moveType == CASTLKINGSIDE)
  {
    gamePosition.setKingMove(getSide(), true);
    gamePosition.setRRookMoved(getSide(), true);
    uint32_t k=3,r=0, nk =1, nr = 2;
    if(getSide() == BLACK)
    { k += 56; r += 56; nk += 56; nr += 56;}
    board[nk] = board[k]; board[nr] = board[r];
    board[nk]->pos = nk; board[nr]->pos = nr;
    board[k] = nullptr; board[r] = nullptr;
  }
  
  if(curMoveInfo.moveType == CASTLQUEENSIDE)
  {
    gamePosition.setKingMove(getSide(), true);
    gamePosition.setLRookMoved(getSide(), true);
    uint32_t k=3,r=7, nk =5, nr = 4;
    if(getSide() == BLACK)
    { k += 56; r += 56; nk += 56; nr += 56;}
    board[nk] = board[k]; board[nr] = board[r];
    board[nk]->pos = nk; board[nr]->pos = nr;
    board[k] = nullptr; board[r] = nullptr;
  }
  
  if(curMoveInfo.moveType == CAPTURETRANSFROM)
  {
    fiftyMoveCount =0;
    list.remove(*board[input[1]], !getSide());
    board[input[1]] = nullptr;
    curMoveInfo.moveType = TRANSFORM;
  }
  
  if(curMoveInfo.moveType != CASTLKINGSIDE && curMoveInfo.moveType != CASTLQUEENSIDE)
  {
    board[input[1]] = board[input[0]];
    board[input[0]] = nullptr;
    board[input[1]]->pos =  curMoveInfo.moveTo;
  }
  
  if(curMoveInfo.moveType == TRANSFORM)
  {
    fiftyMoveCount =0;
    list.remove(*board[input[1]], getSide());
    board[input[1]] = nullptr;
    piece newPiece;
    newPiece.type = curMoveInfo.transformType;
    newPiece.pos = input[1];
    board[input[1]] = list.add(newPiece, getSide());
  }
  
  if(curMoveInfo.pieceType == KING)
  {
    gamePosition.setKingMove(getSide(), true);
  }
  if(curMoveInfo.pieceType == ROOK)
  {

     
    switch(curMoveInfo.moveFrom)
    {
      case 0:
        gamePosition.setWhiteRRookMoved(true);
        break;
      case 7:
        gamePosition.setWhiteLRookMoved(true);
        break;
      case 56:
        gamePosition.setBlackRRookMoved(true);
        break;
      case 63:
        gamePosition.setBlackLRookMoved(true);
        break;
      default:break;
    }
  }
  
 lastMoveHash = zobrist.pieceMove(curMoveInfo.moveType, curMoveInfo.pieceType, getSide(), curMoveInfo.moveFrom, curMoveInfo.moveTo, curMoveInfo.capturedType, !getSide(), curMoveInfo.transformType);
  int t = zobrist.hash->insert(lastMoveHash) ;
 if(t== 3)
 {
   isThreeRepetition = true;
 }
  
  piece* tmpPiece =list.getList()[!side][KING];

  uint32_t kingPos = tmpPiece->pos;
  if(legalMove.isAttacked(kingPos, gamePosition, psLegalMove, !side))
    gamePosition.setCheck(true);
  else
    gamePosition.setCheck(false);
  
  return 1;
}

int Game::updateHistory()
{
  Position *curPos = Position::setPosition(gamePosition);
  curPos->setMoveCount(moveCount);
  curPos->setFiftyMoveCount(fiftyMoveCount);
  curPos->setHash(lastMoveHash);
  moveHistory.add(curPos);
  
  return 0;

}

int Game::postMove()
{
  moveCount ++;
  fiftyMoveCount++;
  if(fiftyMoveCount == 50)
    return 3;
  return 0;
}

int Game::PlayPvP()
{
  std::string sideName[] = {"WHITE","BLACK"};
  GUI::DrawBoard(gamePosition, attackDrawSquare,getPlayerSide());
  std::vector<uint32_t> input(2);
  
  while(1)
  {
    
    Position *curPos = Position::setPosition(gamePosition);
    curPos->setMoveCount(moveCount);
    curPos->setFiftyMoveCount(fiftyMoveCount);
    curPos->setHash(lastMoveHash);
    moveHistory.add(curPos);
    
   int r =  preLoop();
    
    if(r == 1)
    {
      gameOverWon(!getSide());
      return !getSide();
    }
    if(r== 3)
    {
      draw("Stalemate");
      return 2;
    }
    int moveDone = 0;
    while(!moveDone){
      if(isThreeRepetition)
      {
        threeRepetition();
        return 2;
      }
      
      
      if(getPlayerSide() == getSide() ){
        if(gamePosition.getCheck())
          std::cout << "The king under check!!\n";
        std::cout << "Move to " << sideName[getSide()] << std::endl;
        std::cout << "Enter move from-to:\n";
        input = parser();
       
      }
      else if(getMultiplayerMode())
      input = getOpMove(getSocketFd());
      
      moveDone = loop(input);
       GUI::DrawBoard(gamePosition, attackDrawSquare,getPlayerSide());
      memset(attackDrawSquare,0,sizeof(attackDrawSquare));
      
      if(getMultiplayerMode() && getPlayerSide() == getSide() ){
        int tmpMove[2] = {(const int)(input[0]),(const int )(input[1])};
        if(tmpMove[1] != 111 ||tmpMove[0] == 65 || tmpMove[0] == 64)
        sendMove("127.0.0.1", getOpponentPort(), tmpMove);
      }
      
      if(!getMultiplayerMode())
        curPlayerSide = !curPlayerSide;
      
    }
  
    moveCount ++;
    fiftyMoveCount++;
    if(fiftyMoveCount == 50)
    {
      draw("50 move!\n");
      return 2;
    }
    
  }
  
  return 3;
}

void Game::DrawCurPosition()
{
  GUI::DrawBoard(gamePosition);
}


void Game::setOpponentPort(int port)
{
  opponentPort = port;
}
int Game::getOpponentPort()
{
  return opponentPort;
}


void Game::setMultiplayerMode(bool stat)
{
  multiplayerMode = stat;
}

bool  Game::getMultiplayerMode()
{
  return multiplayerMode;
}


void Game::setSocketFd(int sockFd)
{
  socketFd = sockFd;
}
int Game::getSocketFd()
{
  return socketFd;
}


void Game::setPlayerSide(uint8_t side)
{
  curPlayerSide = side;
}

uint8_t Game::getPlayerSide(){
  return curPlayerSide;
}
