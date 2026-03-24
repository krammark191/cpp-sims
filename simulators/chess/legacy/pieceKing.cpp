/***********************************************************************
 * Source File:
 *    King
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    The King class
 ************************************************************************/

#include "pieceKing.h"
#include "board.h"
#include "uiDraw.h"    // for draw*()

/***************************************************
 * PIECE DRAW
 * Draw all the pieces.
 ***************************************************/
void King::display(ogstream* pgout) const
{
   pgout->drawKing(position, !isWhite());
}


/**********************************************
 * King : GET POSITIONS
 *********************************************/
void King::getMoves(set <Move>& moves, const Board& board) const
{
   Move m;
   // Directions: up, down, left, right, and 4 diagonals
   static const int dCol[] = { 0,  0, -1, 1,  1,  1, -1, -1 };
   static const int dRow[] = { 1, -1,  0, 0,  1, -1,  1, -1 };
   
   int col = position.getCol();
   int row = position.getRow();
   
   for (int dir = 0; dir < 8; ++dir)
   {
      int c = col + dCol[dir];
      int r = row + dRow[dir];
      
      // Check boundaries: king moves only one step
      if (c >= 0 && c < 8 && r >= 0 && r < 8)
      {
         const Position dest(c, r);
         if (board.isSquareAttacked(dest, !this->fWhite))
            continue;

         Piece* pDest = board.getPiece(dest);
         
         if (pDest == nullptr || pDest->getType() == SPACE)
         {
            m.setSource(position);
            m.setDest(dest);
            m.setMoveType(Move::MOVE);
            m.setWhiteTurn(this->fWhite);
            m.setCapture(SPACE);
            m.setPromote(SPACE);
            moves.insert(m);
         }
         else if (pDest->isWhite() != this->fWhite)
         {
            m.setSource(position);
            m.setDest(dest);
            m.setMoveType(Move::MOVE);
            m.setWhiteTurn(this->fWhite);
            m.setCapture(pDest->getType());
            m.setPromote(SPACE);
            moves.insert(m);
         }
         // No further movement, king only moves one square
      }
   }
   
   if (nMoves == 0 && col == 4 && !board.isInCheck(this->fWhite))
   {
      Piece* qSideRook = board.getPiece(Position(0, row));
      Piece* kSideRook = board.getPiece(Position(7, row));
      
      if (qSideRook &&
          qSideRook->getType() == ROOK &&
          qSideRook->isWhite() == this->fWhite &&
          qSideRook->getNMoves() == 0 &&
          board.isEmpty(Position(1, row)) &&
          board.isEmpty(Position(2, row)) &&
          board.isEmpty(Position(3, row)) &&
          !board.isSquareAttacked(Position(3, row), !this->fWhite) &&
          !board.isSquareAttacked(Position(2, row), !this->fWhite))
      {
         m.setSource(position);
         m.setDest(Position(2, row));
         m.setMoveType(Move::CASTLE_QUEEN);
         m.setWhiteTurn(this->fWhite);
         m.setCapture(SPACE);
         m.setPromote(SPACE);
         moves.insert(m);
      }
      
      if (kSideRook &&
          kSideRook->getType() == ROOK &&
          kSideRook->isWhite() == this->fWhite &&
          kSideRook->getNMoves() == 0 &&
          board.isEmpty(Position(5, row)) &&
          board.isEmpty(Position(6, row)) &&
          !board.isSquareAttacked(Position(5, row), !this->fWhite) &&
          !board.isSquareAttacked(Position(6, row), !this->fWhite))
      {
         m.setSource(position);
         m.setDest(Position(6, row));
         m.setMoveType(Move::CASTLE_KING);
         m.setWhiteTurn(this->fWhite);
         m.setCapture(SPACE);
         m.setPromote(SPACE);
         moves.insert(m);
      }
   }
}
