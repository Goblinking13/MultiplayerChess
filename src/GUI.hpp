#pragma once
#include<iostream>
#include"Pieces.hpp"
#include"Bitboards.hpp"
#include<string>
#include"Move.hpp"

class GUI
{  public:
  static constexpr char White[6] = {'P','N','B','R','Q','K'};
  static constexpr char Black[6] = {'p','n','b','r','q','k'};



  static void DrawBits(uint64_t board);
  static void DrawBoard(Pieces pieces, bool AttackPattern[64] = NULL, uint8_t side = WHITE);
  static void setAttackPattern(uint64_t BB, bool *attackPattern);
  
};



