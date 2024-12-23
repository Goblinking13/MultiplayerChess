#pragma once

#include<string>
#include<array>
#include<array>
#include<unordered_map>
//low - black
//upper - white
static std::string beginPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

static constexpr uint8_t LwROOK = 1;
static constexpr uint8_t RwROOK = 2;
static constexpr uint8_t LbROOK = 4;
static constexpr uint8_t RbROOK = 8;
static constexpr uint8_t wKING=16;
static constexpr uint8_t bKING =32;




class Pieces;


struct piece
{
  uint32_t pos;
  uint32_t type;
  piece* next;
};

class PieceList
{
  piece*** list;
  public:
  PieceList();
  PieceList(Pieces &position);
  ~PieceList();
  piece*** getList();
  void initList(Pieces &pos);
  piece* add(piece p, uint8_t side);
  void remove(piece p, uint8_t side);
  void printList();
  void clear();
  
};

class Pieces
{
private:
  uint64_t  white[6];
  uint64_t  black[6];
  uint64_t  side[2];
  //
  uint64_t  empty;
  uint64_t all;
  


  uint8_t enPassent;
  bool Check;
  
  bool isWLeftMoved;
  bool isWRightMoved;
  
  bool isBLeftMoved;
  bool isBRightMoved;
  
  bool isWKingMoved;
  bool isBKingMoved;
  
  bool isCastledW;
  bool isCastledB;

public:
  
  void setWhiteKingMoved(bool stat);
  void setBlackKingMoved(bool stat);
  
  void setKingMove(uint8_t side, bool stat);
  
  void setWhiteLRookMoved(bool stat);
  void setWhiteRRookMoved(bool stat);
  
  void setBlackLRookMoved(bool stat);
  void setBlackRRookMoved(bool stat);
  
  void setLRookMoved(uint8_t side, bool stat);
  void setRRookMoved(uint8_t side, bool stat);
  
  void setWhiteCastled(bool stat);
  void setBlackCastled(bool stat);
  
  bool isWhiteCastled() const;
  bool isBlackCastled() const;
  
  bool isCastled(uint8_t side) const;
  void setCastled(uint8_t side, bool stat);
  
  bool isWhiteKingMoved() const;
  bool isBlackKingMoved() const;
  
  bool isWhiteLRookMoved() const;
  bool isWhiteRRookMoved() const;
  
  bool isBlackLRookMoved() const;
  bool isBlackRRookMoved() const;
  
  bool getCheck() const;
  void setCheck(bool stat);
  
  bool isLRookMoved(uint8_t side) const;
  bool isRRookMoved(uint8_t side) const;
  bool isKingMoved(uint8_t side) const;
  
  void setEmpty(uint64_t empty);
  void setAll(uint64_t all);
  
  void uSideW();
  void uSideB();
  void uAll();
  void uEmpty();
  
 Pieces(std::string FEN);
  
  uint8_t getCastlingExtraInfo();
  
  bool isCastlingAvailable(uint8_t side);

  void QuietMove(uint32_t a, uint32_t b, uint8_t piece, uint8_t side);
  
  void CaptureMove(uint32_t a, uint32_t b ,uint8_t piece, uint8_t capture, uint8_t side);
  
  void EnPassent(uint32_t a, uint32_t b,uint8_t side);
  
  void WhiteOO(); // KingSide
  void WhiteOOO(); // QueenSide
  
  void BlackOO(); // KingSide
  void BlackOOO(); // QueenSide
  
  void makeOO(uint8_t side);
  void makeOOO(uint8_t side);
  
  void Transform(uint32_t a, uint32_t b , uint8_t piece1, uint8_t piece2,uint8_t side);
  

  void CaptureTransfrom(uint32_t a, uint32_t b , uint8_t piece, uint8_t capPiece,uint8_t transPiece,uint8_t side);

  
  uint64_t getKingBB(uint8_t side) const;
  uint64_t getQueenBB(uint8_t side) const;
  uint64_t getRookBB(uint8_t side) const;
  uint64_t getKnightBB(uint8_t side) const;
  uint64_t getBishopBB(uint8_t side) const;
  uint64_t getPawnBB(uint8_t side) const;
  
  uint64_t invertSide(uint8_t) const;
  
  uint64_t getEmpty() const;
  uint64_t getAll() const;
  uint64_t* getSideArr();
  uint64_t getEnPassent() const;
  void setEnPassent(uint8_t stat);
  uint64_t* getWhite();
  uint64_t* getBlack();
  uint64_t* getSidePieceBB(uint8_t side);
  uint8_t getPieceType(uint32_t square, uint8_t side);
  friend void PieceList::initList(Pieces &pos);
  friend class LegalMove;
  friend class Game;
  
};




