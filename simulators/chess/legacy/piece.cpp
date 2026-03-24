/***********************************************************************
 * Source File:
 *    PIECE
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    The Piece base class and all the derived classes:
 *       SPACE, KING, QUEEN, ROOK, KNIGHT, BISHOP, PAWN
 ************************************************************************/

#include "position.h"  // for POSITION
#include "piece.h"     // for all the PIECE class definitions
#include "pieceSpace.h"
#include "pieceKnight.h"
#include "pieceKing.h"
#include "pieceQueen.h"
#include "pieceBishop.h"
#include "pieceRook.h"
#include "piecePawn.h"
#include "board.h"     // for BOARD
#include "uiDraw.h"    // for draw*()
#include <cassert>     // because we are paranoid
using namespace std;


/************************************************
 * PIECE : ASSIGN
 ***********************************************/
const Piece& Piece::operator = (const Piece& rhs)
{
   if (this != &rhs)
   {
      nMoves = rhs.nMoves;
      fWhite = rhs.fWhite;
      lastMove = rhs.lastMove;
   }
   return *this;
}

/************************************************
 * PIECE : GET MOVES
 * Iterate through the moves decorator to allow a piece to move
 ***********************************************/
void Piece::getMoves(set<Move>& movesSet, const Board& board) const
{
   switch (getType())
   {
      case KING:
         static_cast<const King*>(this)->King::getMoves(movesSet, board);
         break;
      case QUEEN:
         static_cast<const Queen*>(this)->Queen::getMoves(movesSet, board);
         break;
      case ROOK:
         static_cast<const Rook*>(this)->Rook::getMoves(movesSet, board);
         break;
      case BISHOP:
         static_cast<const Bishop*>(this)->Bishop::getMoves(movesSet, board);
         break;
      case KNIGHT:
         static_cast<const Knight*>(this)->Knight::getMoves(movesSet, board);
         break;
      case PAWN:
         static_cast<const Pawn*>(this)->Pawn::getMoves(movesSet, board);
         break;
      case SPACE:
      default:
         // Do nothing for SPACE or unknown types
         break;
   }
}

