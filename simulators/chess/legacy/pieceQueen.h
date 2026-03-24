/***********************************************************************
 * Header File:
 *    Queen
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    The Queen class
 ************************************************************************/

#pragma once

#include "piece.h"

/***************************************************
 * Queen
 * The Queen
 ***************************************************/
class Queen : public Piece
{
public:
   Queen(const Position& pos, bool isWhite) : Piece(pos, isWhite) { }
   Queen(int c, int r, bool isWhite) : Piece(c, r, isWhite) { }
   ~Queen() {  }
   PieceType getType()            const { return QUEEN; }
   void getMoves(set <Move>& moves, const Board& board) const;
   void display(ogstream* pgout)  const;
};
