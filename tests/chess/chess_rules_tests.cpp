#include "simulators/chess/legacy/board.h"
#include "simulators/chess/legacy/move.h"
#include "simulators/chess/legacy/pieceKing.h"
#include "simulators/chess/legacy/piecePawn.h"
#include "simulators/chess/legacy/pieceRook.h"
#include <cassert>
#include <iostream>
#include <set>
#include <utility>

class TestBoardAccess : public Board
{
public:
   TestBoardAccess() : Board(nullptr, true)
   {
   }

   using Board::board;
   using Board::numMoves;
};

template <typename PieceType, typename... Args>
PieceType * place(TestBoardAccess & board, int col, int row, Args&&... args)
{
   PieceType * piece = new PieceType(col, row, std::forward<Args>(args)...);
   board.board[col][row] = piece;
   return piece;
}

static void testPromotionOffersAllPieces()
{
   TestBoardAccess board;
   Pawn * pawn = place<Pawn>(board, 1, 6, true);
   place<Rook>(board, 0, 7, false);
   place<Rook>(board, 2, 7, false);
   place<King>(board, 4, 0, true);
   place<King>(board, 4, 7, false);

   std::set<Move> moves;
   pawn->getMoves(moves, board);

   assert(moves.size() == 12);
   assert(moves.find(Move("b7a8Q")) != moves.end());
   assert(moves.find(Move("b7a8R")) != moves.end());
   assert(moves.find(Move("b7a8B")) != moves.end());
   assert(moves.find(Move("b7a8N")) != moves.end());
   assert(moves.find(Move("b7b8Q")) != moves.end());
   assert(moves.find(Move("b7b8R")) != moves.end());
   assert(moves.find(Move("b7b8B")) != moves.end());
   assert(moves.find(Move("b7b8N")) != moves.end());
   assert(moves.find(Move("b7c8Q")) != moves.end());
   assert(moves.find(Move("b7c8R")) != moves.end());
   assert(moves.find(Move("b7c8B")) != moves.end());
   assert(moves.find(Move("b7c8N")) != moves.end());
}

static void testEnPassantRequiresImmediateDoubleStep()
{
   {
      TestBoardAccess board;
      Pawn * whitePawn = place<Pawn>(board, 4, 4, true);
      Pawn * blackPawn = place<Pawn>(board, 3, 4, false);
      place<King>(board, 6, 0, true);
      place<King>(board, 6, 7, false);
      board.numMoves = 2;
      blackPawn->setLastMove(1);

      std::set<Move> moves;
      whitePawn->getMoves(moves, board);
      assert(moves.find(Move("e5d6E")) != moves.end());
   }

   {
      TestBoardAccess board;
      Pawn * whitePawn = place<Pawn>(board, 4, 4, true);
      Pawn * blackPawn = place<Pawn>(board, 3, 4, false);
      place<King>(board, 6, 0, true);
      place<King>(board, 6, 7, false);
      board.numMoves = 2;
      blackPawn->setLastMove(0);

      std::set<Move> moves;
      whitePawn->getMoves(moves, board);
      assert(moves.find(Move("e5d6E")) == moves.end());
   }
}

static void testPinnedMoveIsRejected()
{
   TestBoardAccess board;
   place<King>(board, 4, 0, true);
   place<Rook>(board, 4, 1, true);
   place<Rook>(board, 4, 7, false);
   place<King>(board, 0, 7, false);
   board.numMoves = 0;

   board.move(Move("e2f2"));

   assert(board.numMoves == 0);
   assert(board.board[4][1] != nullptr && board.board[4][1]->getType() == ROOK);
   assert(board.board[5][1] == nullptr);
}

static void testCastleThroughCheckIsRejected()
{
   TestBoardAccess board;
   King * whiteKing = place<King>(board, 4, 0, true);
   place<Rook>(board, 7, 0, true);
   place<Rook>(board, 5, 7, false);
   place<King>(board, 0, 7, false);
   board.numMoves = 0;

   std::set<Move> moves;
   whiteKing->getMoves(moves, board);
   assert(moves.find(Move("e1g1c")) == moves.end());

   board.move(Move("e1g1c"));
   assert(board.numMoves == 0);
   assert(board.board[4][0] != nullptr && board.board[4][0]->getType() == KING);
   assert(board.board[7][0] != nullptr && board.board[7][0]->getType() == ROOK);
}

static void testEnPassantExecutionRemovesCapturedPawn()
{
   TestBoardAccess board;
   place<King>(board, 6, 0, true);
   place<King>(board, 6, 7, false);
   place<Pawn>(board, 4, 4, true);
   Pawn * blackPawn = place<Pawn>(board, 3, 4, false);
   board.numMoves = 2;
   blackPawn->setLastMove(1);

   board.move(Move("e5d6E"));

   assert(board.numMoves == 3);
   assert(board.board[4][4] != nullptr && board.board[4][4]->getType() == SPACE);
   assert(board.board[3][4] != nullptr && board.board[3][4]->getType() == SPACE);
   assert(board.board[3][5] != nullptr && board.board[3][5]->getType() == PAWN);
   assert(board.board[3][5]->isWhite());
}

int main()
{
   testPromotionOffersAllPieces();
   testEnPassantRequiresImmediateDoubleStep();
   testPinnedMoveIsRejected();
   testCastleThroughCheckIsRejected();
   testEnPassantExecutionRemovesCapturedPawn();

   std::cout << "chess_rules_tests passed\n";
   return 0;
}
