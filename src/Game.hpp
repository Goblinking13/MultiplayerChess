#pragma once
#include<string>
#include"base.h"


struct moveInfo
{
  uint32_t moveFrom;
  uint32_t moveTo;
  uint8_t side;
  uint8_t pieceType;
  uint8_t capturedType;
  uint8_t moveType;
  uint8_t transformType;
};

class Game
{
private:
  int opponentPort;
  int curPlayerSide;
  Pieces gamePosition;
  AttackPattern attackPattern;
  PsLegalMove psLegalMove;
  LegalMove legalMove;
  ZobristHash zobrist;
  MoveHistory moveHistory;
  PieceList list;
  
  uint32_t moveCount;
  uint8_t fiftyMoveCount;
  
  bool attackDrawSquare[64];
  
  piece* board[64];
  
  std::unordered_map<std::string, uint32_t> Square;
  moveInfo curMoveInfo;
  
  uint64_t lastMoveHash;
  bool multiplayerMode;
  int socketFd;
public:
  //Game();
  
  void setPlayerSide(uint8_t);
  uint8_t getPlayerSide();
  
  void setSocketFd(int sockFd);
  int getSocketFd();
  
  void setMultiplayerMode(bool stat);
  bool getMultiplayerMode();
  
  void setOpponentPort(int port);
  int getOpponentPort();
  bool isThreeRepetition = false;
  bool isEngine = false;
  
  void DrawCurPosition();
  int updateHistory();
  int postMove();
  Game(std::string FEN= beginPos);
  int MakeMove(uint32_t moveFrom, uint32_t moveTo=-1);
  uint8_t getSide();
  
  bool fastMoveCheck(uint32_t A, uint32_t B);
  uint8_t getPieceType(uint32_t square) const;
  int preLoop();
  uint32_t loop(std::vector<uint32_t> input);
  int PlayPvP();
  void PlayWithEngine(uint8_t playerSide = WHITE);
  std::vector<uint32_t> parser();
  uint8_t pickPieceToTrnasform();
  uint32_t getSquare(std::string s);
  friend class Engine;
  
};
