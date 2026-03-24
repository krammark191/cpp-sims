/***********************************************************************
 * Source File:
 *    Pawn
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    The Pawn class
 ************************************************************************/

#include "piecePawn.h"
#include "board.h"
#include "uiDraw.h"    // for draw*()

/***************************************************
 * PIECE DRAW
 * Draw all the pieces.
 ***************************************************/
void Pawn::display(ogstream* pgout) const
{
   pgout->drawPawn(position, !isWhite());
}


/**********************************************
 * Pawn : GET POSITIONS
 *********************************************/
void Pawn::getMoves(set <Move>& moves, const Board& board) const
{
   int col = position.getCol();
   int row = position.getRow();
   
   // Determine direction: white moves up (+1), black moves down (-1)
   int dir = (fWhite ? 1 : -1);
   int nextRow = row + dir;
   
   // Forward move (one square)
   if (nextRow >= 0 && nextRow < 8)
   {
      const Position forward(col, nextRow);
      const bool isPromotion = (fWhite && nextRow == 7) || (!fWhite && nextRow == 0);
      if (board.isEmpty(forward))
      {
         if (isPromotion)
         {
            static const PieceType promotions[] = { QUEEN, ROOK, BISHOP, KNIGHT };
            for (PieceType promote : promotions)
            {
               Move m;
               m.setSource(position);
               m.setDest(forward);
               m.setMoveType(Move::MOVE);
               m.setWhiteTurn(this->fWhite);
               m.setPromote(promote);
               moves.insert(m);
            }
         }
         else
         {
            Move m;
            m.setSource(position);
            m.setDest(forward);
            m.setMoveType(Move::MOVE);
            m.setWhiteTurn(this->fWhite);
            moves.insert(m);
         }
         
         // Initial two-square advance
         int startRow = fWhite ? 1 : 6;
         int nextNextRow = row + 2 * dir;
         if (row == startRow && nextNextRow >= 0 && nextNextRow < 8)
         {
            if (board.isEmpty(Position(col, nextNextRow)))
            {
               Move m2;
               m2.setSource(position);
               m2.setDest(Position(col, nextNextRow));
               m2.setMoveType(Move::MOVE);
               m2.setWhiteTurn(this->fWhite);
               moves.insert(m2);
            }
         }
      }
      // Diagonal captures
      for (int dCol = -1; dCol <= 1; dCol += 2) // -1 (left), +1 (right)
      {
         int diagCol = col + dCol;
         if (diagCol >= 0 && diagCol < 8)
         {
            const Position diagonal(diagCol, nextRow);
            Piece* pDiag = board.getPiece(diagonal);
            const bool isPromotionCapture = (fWhite && nextRow == 7) || (!fWhite && nextRow == 0);
            if (pDiag && pDiag->getType() != SPACE && pDiag->isWhite() != this->isWhite())
            {
               if (isPromotionCapture)
               {
                  static const PieceType promotions[] = { QUEEN, ROOK, BISHOP, KNIGHT };
                  for (PieceType promote : promotions)
                  {
                     Move m;
                     m.setSource(position);
                     m.setDest(diagonal);
                     m.setMoveType(Move::MOVE);
                     m.setWhiteTurn(this->fWhite);
                     m.setPromote(promote);
                     moves.insert(m);
                  }
               }
               else
               {
                  Move m;
                  m.setSource(position);
                  m.setDest(diagonal);
                  m.setMoveType(Move::MOVE);
                  m.setWhiteTurn(this->fWhite);
                  m.setCapture(pDiag->getType());
                  moves.insert(m);
               }
            }

            const Position side(diagCol, row);
            Piece* pSide = board.getPiece(side);
            const int enPassantRow = fWhite ? 4 : 3;
            if (row == enPassantRow &&
                board.isEmpty(diagonal) &&
                pSide && pSide->getType() == PAWN &&
                pSide->isWhite() != this->isWhite() &&
                board.isEnPassantTarget(side))
            {
               Move m;
               m.setSource(position);
               m.setDest(diagonal);
               m.setMoveType(Move::ENPASSANT);
               m.setWhiteTurn(this->fWhite);
               moves.insert(m);
            }
         }
      }
   }
}
