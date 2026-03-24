/***********************************************************************
 * Source File:
 *    KNIGHT
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    The knight class
 ************************************************************************/

#include "pieceKnight.h"
#include "board.h"
#include "uiDraw.h"    // for draw*()

/***************************************************
 * PIECE DRAW
 * Draw all the pieces.
 ***************************************************/
void Knight::display(ogstream* pgout) const
{
   pgout->drawKnight(position, !isWhite());
}


/**********************************************
 * KNIGHT : GET POSITIONS
 *********************************************/
void Knight::getMoves(set <Move>& moves, const Board& board) const
{
   static const int dCol[] = { 1,  2,  2,  1, -1, -2, -2, -1 };
   static const int dRow[] = { -2, -1,  1,  2,  2,  1, -1, -2 };
   
   int col = position.getCol();
   int row = position.getRow();
   
   for (int i = 0; i < 8; ++i)
   {
      int c = col + dCol[i];
      int r = row + dRow[i];
      
      // Check if the target position is on the board
      if (c < 0 || c > 7 || r < 0 || r > 7)
         continue;
      
      // Use Board's board array directly (allowed by friend)
      Piece* pDest = board.getPiece(Position(c, r));
      
      // If the destination is empty or a SPACE, it's a valid move
      if (pDest == nullptr || pDest->getType() == SPACE)
      {
         Move m;
         m.setSource(position);
         m.setDest(Position(c, r));
         m.setMoveType(Move::MOVE);
         m.setWhiteTurn(this->fWhite);
         moves.insert(m);
      }
      // If the destination is an opponent's piece, it's a capture
      else if (pDest->isWhite() != this->fWhite)
      {
         Move m;
         m.setSource(position);
         m.setDest(Position(c, r));
         m.setMoveType(Move::MOVE);
         m.setWhiteTurn(this->fWhite);
         m.setCapture(pDest->getType());
         moves.insert(m);
      }
      // If the destination is a friendly piece, do nothing
   }
}
