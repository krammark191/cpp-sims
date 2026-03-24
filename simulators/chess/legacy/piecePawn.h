/***********************************************************************
 * Header File:
 *    PAWN
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    The PAWN class
 ************************************************************************/

#pragma once

#include "piece.h"

/***************************************************
 * PAWN
 * The pawn, the most basic piece in chess
 ***************************************************/
class Pawn : public Piece
{
public:
   Pawn(const Position& pos, bool isWhite) : Piece(pos,  isWhite) { }
   Pawn(int c, int r, bool isWhite)        : Piece(c, r, isWhite) { }
   ~Pawn() {                }
   PieceType getType()            const { return PAWN; }
   void getMoves(set <Move>& moves, const Board& board) const;
   void display(ogstream* pgout)  const;
};

