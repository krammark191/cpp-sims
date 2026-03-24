/***********************************************************************
 * Header File:
 *    SPACE
 * Author:
 *   Taden Marston & Mark Van Horn
 * Summary:
 *    The space
 ************************************************************************/

#pragma once

#include "piece.h"

class TestSpace;

/***************************************************
 * SPACES
 * A space is a trivial piece
 ***************************************************/
class Space : public Piece
{
   friend TestSpace;
public:
   Space(int c, int r) : Piece(Position(c, r), false)
   {
      nMoves = 0;
      fWhite = false;
      position = Position(c, r);
      lastMove = 0;
   }
   ~Space() {
   }
   PieceType getType() const override {
      return SPACE;
   }
   void display(ogstream* pgout) const override { /* nothing to display for a space */
   }
};
