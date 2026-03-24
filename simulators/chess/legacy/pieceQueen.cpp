/***********************************************************************
 * Source File:
 *    Queen
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    The Queen class
 ************************************************************************/

#include "pieceQueen.h"
#include "board.h"
#include "uiDraw.h"    // for draw*()

/***************************************************
 * PIECE DRAW
 * Draw all the pieces.
 ***************************************************/
void Queen::display(ogstream* pgout) const
{
   pgout->drawQueen(position, !isWhite());
}


/**********************************************
 * Queen : GET POSITIONS
 *********************************************/
void Queen::getMoves(set <Move>& moves, const Board& board) const
{
   // Directions: up, down, left, right, and 4 diagonals
   static const int dCol[] = { 0,  0, -1, 1,  1,  1, -1, -1 };
   static const int dRow[] = { 1, -1,  0, 0,  1, -1,  1, -1 };
   
   int col = position.getCol();
   int row = position.getRow();
   
   for (int dir = 0; dir < 8; ++dir)
   {
      int c = col + dCol[dir];
      int r = row + dRow[dir];
      
      while (c > -1 && c < 8 && r > -1 && r < 8)
      {
         Piece* pDest = board.getPiece(Position(c, r));
         
         if (pDest == nullptr || pDest->getType() == SPACE)
         {
            Move m;
            m.setSource(position);
            m.setDest(Position(c, r));
            m.setMoveType(Move::MOVE);
            m.setWhiteTurn(this->fWhite);
            moves.insert(m);
         }
         else
         {
            if (pDest->isWhite() != this->fWhite)
            {
               Move m;
               m.setSource(position);
               m.setDest(Position(c, r));
               m.setMoveType(Move::MOVE);
               m.setWhiteTurn(this->fWhite);
               m.setCapture(pDest->getType());
               moves.insert(m);
            }
            break;
         }
         
         c += dCol[dir];
         r += dRow[dir];
      }
   }
}
