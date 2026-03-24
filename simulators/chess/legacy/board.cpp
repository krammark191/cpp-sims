/***********************************************************************
 * Source File:
 *    BOARD
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    A collection of pieces and a small amount of game state
 ************************************************************************/

#include "board.h"
#include "uiDraw.h"
#include "position.h"
#include "piece.h"
#include "pieceSpace.h"
#include "pieceKnight.h"
#include "pieceRook.h"
#include "pieceBishop.h"
#include "pieceQueen.h"
#include "pieceKing.h"
#include "piecePawn.h"
#include <cassert>
#include <cstdlib>
using namespace std;

namespace
{
Piece * clonePiece(const Piece & piece)
{
   const Position pos = piece.getPosition();
   Piece * clone = nullptr;

   switch (piece.getType())
   {
      case KING:   clone = new King(pos, piece.isWhite());   break;
      case QUEEN:  clone = new Queen(pos, piece.isWhite());  break;
      case ROOK:   clone = new Rook(pos, piece.isWhite());   break;
      case BISHOP: clone = new Bishop(pos, piece.isWhite()); break;
      case KNIGHT: clone = new Knight(pos, piece.isWhite()); break;
      case PAWN:   clone = new Pawn(pos, piece.isWhite());   break;
      case SPACE:  clone = new Space(pos.getCol(), pos.getRow()); break;
      default:     break;
   }

   if (clone != nullptr)
   {
      *clone = piece;
      clone->setPosition(pos.getCol(), pos.getRow());
   }

   return clone;
}

bool isSlidingType(PieceType type)
{
   return type == ROOK || type == BISHOP || type == QUEEN;
}
}


/***********************************************
 * BOARD : RESET
 *         Just fill the board with the known pieces
 *   +---a-b-c-d-e-f-g-h---+
 *   |                     |
 *   8     N         N     8
 *   7                     7
 *   6                     6
 *   5                     5
 *   4                     4
 *   3                     3
 *   2                     2
 *   1     n         n     1
 *   |                     |
 *   +---a-b-c-d-e-f-g-h---+
 ***********************************************/
void Board::reset(bool fFree)
{
   // Free all pieces if requested
   if (fFree)
      free();
   
   // Set all board pointers to nullptr
   for (int c = 0; c < 8; ++c)
      for (int r = 0; r < 8; ++r)
         board[c][r] = nullptr;
   
   // Place the Rooks on the board
   board[0][0] = new Rook(0, 0, true);
   board[7][0] = new Rook(7, 0, true);
   board[0][7] = new Rook(0, 7, false);
   board[7][7] = new Rook(7, 7, false);
   
   // Place the knights on the board
   board[1][0] = new Knight(1, 0, true);
   board[6][0] = new Knight(6, 0, true);
   board[1][7] = new Knight(1, 7, false);
   board[6][7] = new Knight(6, 7, false);
   
   // Place the bishops on the board
   board[2][0] = new Bishop(2, 0, true);
   board[5][0] = new Bishop(5, 0, true);
   board[2][7] = new Bishop(2, 7, false);
   board[5][7] = new Bishop(5, 7, false);
   
   // Place the queens on the board
   board[3][0] = new Queen(3, 0, true);
   board[3][7] = new Queen(3, 7, false);
   
   // Place the kings on the board
   board[4][0] = new King(4, 0, true);
   board[4][7] = new King(4, 7, false);
   
   // Place the pawns on the board
   for (int c = 0; c < 8; ++c)
   {
      board[c][1] = new Pawn(c, 1, true);
      board[c][6] = new Pawn(c, 6, false);
   }
   
   for (int c = 0; c < 8; ++c)
      for (int r = 0; r < 8; ++r)
         if (board[c][r] == nullptr)
            board[c][r] = new Space(c, r);
   
   numMoves = 0;
   assertBoard();
}

/***********************************************
 * BOARD : GET
 *         Get a piece from a given position.
 ***********************************************/
const Piece& Board::operator[](const Position& pos) const
{
   assert(pos.getRow() >= 0 && pos.getRow() < 8);
   assert(pos.getCol() >= 0 && pos.getCol() < 8);
   return *board[pos.getCol()][pos.getRow()];
}

Piece& Board::operator[](const Position& pos)
{
   assert(pos.getRow() >= 0 && pos.getRow() < 8);
   assert(pos.getCol() >= 0 && pos.getCol() < 8);
   return *board[pos.getCol()][pos.getRow()];
}

/***********************************************
 * BOARD : DISPLAY
 *         Display the board
 ***********************************************/
void Board::display(const Position& posHover, const Position& posSelect) const
{
	// Draw the board
   pgout->drawBoard();

  
   if (posHover.isValid())
   {
      pgout->drawCoordinateHighlight(posHover);
      pgout->drawHover(posHover);
   }

   if (posSelect.isValid())
   {
      Piece* selectedPiece = getPiece(posSelect);

      if (selectedPiece && (*this)[posSelect].getType() != SPACE)
      {
         pgout->drawSelected(posSelect);
         set<Move> possibleMoves;
         selectedPiece->getMoves(possibleMoves, *this);
         for (const Move& m : possibleMoves)
            if (isMoveLegal(m))
               pgout->drawPossible(m.getDest());
      }
   }
   
   // Draw all pieces
   for (int c = 0; c < 8; ++c)
   {
      for (int r = 0; r < 8; ++r)
      {
         if (board[c][r])
            board[c][r]->display(pgout);
      }
   }
}


/************************************************
 * BOARD : CONSTRUCT
 *         Free up all the allocated memory
 ************************************************/
Board::Board(ogstream* pgout, bool noreset) : pgout(pgout), numMoves(0)
{
   // Initialize all board pointers to nullptr
   for (int c = 0; c < 8; ++c)
      for (int r = 0; r < 8; ++r)
         board[c][r] = nullptr;

   if (!noreset)
      reset(false);
}

/************************************************
 * BOARD : FREE
 *         Free up all the allocated memory
 ************************************************/
void Board::free()
{
   // Delete all pieces and set pointers to nullptr
   for (int c = 0; c < 8; ++c)
      for (int r = 0; r < 8; ++r)
         if (board[c][r] != nullptr)
         {
            delete board[c][r];
            board[c][r] = nullptr;
         }
}

/**********************************************
 * BOARD : IS EMPTY
 *         Determine if a square has no piece
 *********************************************/
bool Board::isEmpty(const Position& pos) const
{
   if (!pos.isValid())
      return false;

   const Piece * piece = board[pos.getCol()][pos.getRow()];
   return piece == nullptr || piece->getType() == SPACE;
}

/**********************************************
 * BOARD : IS EN PASSANT TARGET
 *         Determine if a pawn may be captured en passant
 *********************************************/
bool Board::isEnPassantTarget(const Position& pos) const
{
   if (!pos.isValid())
      return false;

   const Piece * piece = board[pos.getCol()][pos.getRow()];
   if (piece == nullptr || piece->getType() != PAWN)
      return false;

   if (piece->getNMoves() != 1 || !piece->justMoved(getCurrentMove()))
      return false;

   return piece->isWhite() ? pos.getRow() == 3 : pos.getRow() == 4;
}

/**********************************************
 * BOARD : FIND KING
 *********************************************/
Position Board::findKing(bool white) const
{
   for (int c = 0; c < 8; ++c)
      for (int r = 0; r < 8; ++r)
         if (board[c][r] != nullptr &&
             board[c][r]->getType() == KING &&
             board[c][r]->isWhite() == white)
            return Position(c, r);

   Position notFound;
   notFound.setInvalid();
   return notFound;
}

/**********************************************
 * BOARD : IS SQUARE ATTACKED
 *********************************************/
bool Board::isSquareAttacked(const Position& pos, bool byWhite) const
{
   if (!pos.isValid())
      return false;

   const int col = pos.getCol();
   const int row = pos.getRow();

   const int pawnRow = row + (byWhite ? -1 : 1);
   if (pawnRow >= 0 && pawnRow < 8)
   {
      for (int dCol = -1; dCol <= 1; dCol += 2)
      {
         const int pawnCol = col + dCol;
         if (pawnCol < 0 || pawnCol >= 8)
            continue;
         const Piece * piece = board[pawnCol][pawnRow];
         if (piece != nullptr && piece->getType() == PAWN && piece->isWhite() == byWhite)
            return true;
      }
   }

   static const int knightCols[] = { 1, 2, 2, 1, -1, -2, -2, -1 };
   static const int knightRows[] = { -2, -1, 1, 2, 2, 1, -1, -2 };
   for (int i = 0; i < 8; ++i)
   {
      const int c = col + knightCols[i];
      const int r = row + knightRows[i];
      if (c < 0 || c >= 8 || r < 0 || r >= 8)
         continue;
      const Piece * piece = board[c][r];
      if (piece != nullptr && piece->getType() == KNIGHT && piece->isWhite() == byWhite)
         return true;
   }

   static const int kingCols[] = { 0, 0, -1, 1, 1, 1, -1, -1 };
   static const int kingRows[] = { 1, -1, 0, 0, 1, -1, 1, -1 };
   for (int i = 0; i < 8; ++i)
   {
      const int c = col + kingCols[i];
      const int r = row + kingRows[i];
      if (c < 0 || c >= 8 || r < 0 || r >= 8)
         continue;
      const Piece * piece = board[c][r];
      if (piece != nullptr && piece->getType() == KING && piece->isWhite() == byWhite)
         return true;
   }

   static const int rookCols[] = { 0, 0, -1, 1 };
   static const int rookRows[] = { 1, -1, 0, 0 };
   for (int dir = 0; dir < 4; ++dir)
   {
      int c = col + rookCols[dir];
      int r = row + rookRows[dir];
      while (c >= 0 && c < 8 && r >= 0 && r < 8)
      {
         const Piece * piece = board[c][r];
         if (piece != nullptr && piece->getType() != SPACE)
         {
            if (piece->isWhite() == byWhite &&
                (piece->getType() == ROOK || piece->getType() == QUEEN))
               return true;
            break;
         }
         c += rookCols[dir];
         r += rookRows[dir];
      }
   }

   static const int bishopCols[] = { 1, 1, -1, -1 };
   static const int bishopRows[] = { 1, -1, 1, -1 };
   for (int dir = 0; dir < 4; ++dir)
   {
      int c = col + bishopCols[dir];
      int r = row + bishopRows[dir];
      while (c >= 0 && c < 8 && r >= 0 && r < 8)
      {
         const Piece * piece = board[c][r];
         if (piece != nullptr && piece->getType() != SPACE)
         {
            if (piece->isWhite() == byWhite &&
                (piece->getType() == BISHOP || piece->getType() == QUEEN))
               return true;
            break;
         }
         c += bishopCols[dir];
         r += bishopRows[dir];
      }
   }

   return false;
}

/**********************************************
 * BOARD : IS IN CHECK
 *********************************************/
bool Board::isInCheck(bool white) const
{
   const Position king = findKing(white);
   if (!king.isValid())
      return false;
   return isSquareAttacked(king, !white);
}

/**********************************************
 * BOARD : COPY FROM
 *********************************************/
void Board::copyFrom(const Board& rhs)
{
   free();
   for (int c = 0; c < 8; ++c)
      for (int r = 0; r < 8; ++r)
         board[c][r] = rhs.board[c][r] == nullptr ? nullptr : clonePiece(*rhs.board[c][r]);

   numMoves = rhs.numMoves;
   pgout = rhs.pgout;
}

/**********************************************
 * BOARD : MOVE UNCHECKED
 *********************************************/
void Board::moveUnchecked(const Move& move, int currentMove)
{
   const int srcCol = move.getSource().getCol();
   const int srcRow = move.getSource().getRow();
   const int dstCol = move.getDest().getCol();
   const int dstRow = move.getDest().getRow();

   Piece * moving = board[srcCol][srcRow];
   if (moving == nullptr)
      return;

   board[srcCol][srcRow] = nullptr;

   auto clearSquare = [this](int c, int r)
   {
      if (board[c][r] != nullptr)
      {
         delete board[c][r];
         board[c][r] = nullptr;
      }
   };

   if (move.getMoveType() == Move::ENPASSANT)
   {
      const int capturedRow = moving->isWhite() ? dstRow - 1 : dstRow + 1;
      clearSquare(dstCol, dstRow);
      board[dstCol][dstRow] = moving;
      moving->setPosition(dstCol, dstRow);
      moving->setLastMove(currentMove);
      clearSquare(dstCol, capturedRow);
      board[dstCol][capturedRow] = new Space(dstCol, capturedRow);
      board[srcCol][srcRow] = new Space(srcCol, srcRow);
      return;
   }

   if (move.getMoveType() == Move::CASTLE_KING || move.getMoveType() == Move::CASTLE_QUEEN)
   {
      const int rookSrcCol = move.getMoveType() == Move::CASTLE_KING ? 7 : 0;
      const int rookDstCol = move.getMoveType() == Move::CASTLE_KING ? 5 : 3;
      Piece * rook = board[rookSrcCol][srcRow];
      board[rookSrcCol][srcRow] = nullptr;

      clearSquare(dstCol, dstRow);
      board[dstCol][dstRow] = moving;
      moving->setPosition(dstCol, dstRow);
      moving->setLastMove(currentMove);

      clearSquare(rookDstCol, srcRow);
      board[rookDstCol][srcRow] = rook;
      if (rook != nullptr)
      {
         rook->setPosition(rookDstCol, srcRow);
         rook->setLastMove(currentMove);
      }

      board[srcCol][srcRow] = new Space(srcCol, srcRow);
      board[rookSrcCol][srcRow] = new Space(rookSrcCol, srcRow);
      return;
   }

   clearSquare(dstCol, dstRow);
   board[dstCol][dstRow] = moving;
   moving->setPosition(dstCol, dstRow);

   if (moving->getType() == PAWN && move.getPromote() != SPACE)
   {
      const bool white = moving->isWhite();
      delete board[dstCol][dstRow];
      switch (move.getPromote())
      {
         case ROOK:   board[dstCol][dstRow] = new Rook(dstCol, dstRow, white);   break;
         case BISHOP: board[dstCol][dstRow] = new Bishop(dstCol, dstRow, white); break;
         case KNIGHT: board[dstCol][dstRow] = new Knight(dstCol, dstRow, white); break;
         case QUEEN:
         default:     board[dstCol][dstRow] = new Queen(dstCol, dstRow, white);  break;
      }
      board[dstCol][dstRow]->setLastMove(currentMove);
   }
   else
   {
      moving->setLastMove(currentMove);
   }

   board[srcCol][srcRow] = new Space(srcCol, srcRow);
}

/**********************************************
 * BOARD : IS MOVE LEGAL
 *********************************************/
bool Board::isMoveLegal(const Move& move) const
{
   if (!move.getSource().isValid() || !move.getDest().isValid())
      return false;

   Piece * moving = board[move.getSource().getCol()][move.getSource().getRow()];
   if (moving == nullptr || moving->getType() == SPACE || moving->isWhite() != whiteTurn())
      return false;

   set<Move> legalMoves;
   moving->getMoves(legalMoves, *this);
   if (legalMoves.find(move) == legalMoves.end())
      return false;

   Board sandbox(nullptr, true);
   sandbox.copyFrom(*this);
   sandbox.moveUnchecked(move, numMoves);
   sandbox.numMoves = numMoves + 1;
   return !sandbox.isInCheck(moving->isWhite());
}


/**********************************************
 * BOARD : ASSERT BOARD
 *         Verify the board is well-formed
 *********************************************/
void Board::assertBoard()
{
   for (int c = 0; c < 8; ++c)
      for (int r = 0; r < 8; ++r)
         if (board[c][r] != nullptr)
         {
            Position pos = board[c][r]->getPosition();
            assert(pos.getRow() == r);
            assert(pos.getCol() == c);
            assert(board[c][r]->getType() == SPACE  ||
                   board[c][r]->getType() == KING   ||
                   board[c][r]->getType() == QUEEN  ||
                   board[c][r]->getType() == ROOK   ||
                   board[c][r]->getType() == BISHOP ||
                   board[c][r]->getType() == KNIGHT ||
                   board[c][r]->getType() == PAWN);
         }
}




/**********************************************
 * BOARD : MOVE
 *         Execute a move according to the contained instructions
 *   INPUT move The instructions of the move
 *********************************************/
void Board::move(const Move& move)
{
   if (!isMoveLegal(move))
      return;

   moveUnchecked(move, numMoves);
   ++numMoves;
}

/**********************************************
 * BOARD EMPTY
 * The game board that is completely empty.
 * It does not even have spaces though each non-filled
 * spot will report it has a space. This is for unit testing
 *********************************************/
BoardEmpty::BoardEmpty() : BoardDummy(), pSpace(nullptr), moveNumber(0)
{
   for (int c = 0; c < 8; ++c)
      for (int r = 0; r < 8; ++r)
         board[c][r] = nullptr;
   pSpace = new Space(0, 0);
}
BoardEmpty::~BoardEmpty()
{
   delete pSpace;
}
