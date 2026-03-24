/***********************************************************************
 * Source File:
 *    TEST ROOK
 * Author:
 *    Mark Van Horn & Taden Marston
 * Summary:
 *    The unit tests for the rook
 ************************************************************************/

#include "testRook.h"
#include "pieceRook.h"     
#include "board.h"
#include "uiDraw.h"
#include <cassert>      


/*************************************
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8                     8
 * 7                     7
 * 6                     6
 * 5                     5
 * 4                     4
 * 3       p             3
 * 2     p(r)p           2
 * 1       p             1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestRook::getMoves_blocked()
{
   // SETUP
   BoardEmpty board;
   Rook rook(Position(2, 1), true /*white*/);
   board.board[2][1] = &rook; // c2

   // Place white pawns on all possible Rook move destinations
   White w1(PAWN); board.board[1][1] = &w1; // b2
   White w2(PAWN); board.board[2][0] = &w2; // c1
   White w3(PAWN); board.board[2][2] = &w3; // c3
   White w4(PAWN); board.board[3][1] = &w4; // d2
   set <Move> moves;

   // EXERCISE
   rook.getMoves(moves, board);

   // VERIFY
   assertUnit(moves.size() == 0);
   assertUnit(moves.find(Move("c2b2p")) == moves.end());
   assertUnit(moves.find(Move("c2c1p")) == moves.end());
   assertUnit(moves.find(Move("c2c3p")) == moves.end());
   assertUnit(moves.find(Move("c2d2p")) == moves.end());

   // TEARDOWN
   board.board[2][1] = nullptr;
   board.board[1][1] = nullptr;
   board.board[2][0] = nullptr;
   board.board[2][2] = nullptr;
   board.board[3][1] = nullptr;
}


/*************************************
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8       .             8
 * 7       .             7
 * 6       .             6
 * 5       .             5
 * 4       .             4
 * 3       .             3
 * 2   . .(r). . . . .   2
 * 1       .             1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestRook::getMoves_slideToEnd()
{
   // SETUP
   BoardEmpty board;
   Rook rook(Position(2, 1), true /*white*/);
   board.board[2][1] = &rook; // c2
   set <Move> moves;

   // EXERCISE
   rook.getMoves(moves, board);

   // VERIFY

   assertUnit(moves.size() == 14);
   assertUnit(moves.find(Move("c2a2")) != moves.end());
   assertUnit(moves.find(Move("c2b2")) != moves.end());
   assertUnit(moves.find(Move("c2c1")) != moves.end());
   assertUnit(moves.find(Move("c2c3")) != moves.end());
   assertUnit(moves.find(Move("c2c4")) != moves.end());
   assertUnit(moves.find(Move("c2c5")) != moves.end());
   assertUnit(moves.find(Move("c2c6")) != moves.end());
   assertUnit(moves.find(Move("c2c7")) != moves.end());
   assertUnit(moves.find(Move("c2c8")) != moves.end());
   assertUnit(moves.find(Move("c2d2")) != moves.end());
   assertUnit(moves.find(Move("c2e2")) != moves.end());
   assertUnit(moves.find(Move("c2f2")) != moves.end());
   assertUnit(moves.find(Move("c2g2")) != moves.end());
   assertUnit(moves.find(Move("c2h2")) != moves.end());


   // TEARDOWN
   board.board[2][1] = nullptr;
}


/*************************************
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8       p             8
 * 7       .             7
 * 6       .             6
 * 5       .             5
 * 4       .             4
 * 3       .             3
 * 2   p .(r). . . . p   2
 * 1       p             1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestRook::getMoves_slideToBlock()
{
   // SETUP
   BoardEmpty board;
   Rook rook(Position(2, 1), true /*white*/);
   board.board[2][1] = &rook; // c2

   // Place black pawns on all possible edge Rook move destinations
   White w1(PAWN); board.board[0][1] = &w1; // b2
   White w2(PAWN); board.board[2][0] = &w2; // c1
   White w3(PAWN); board.board[2][7] = &w3; // c8
   White w4(PAWN); board.board[7][1] = &w4; // h2
   set <Move> moves;

   // EXERCISE
   rook.getMoves(moves, board);

   // VERIFY

   assertUnit(moves.size() == 10);
   assertUnit(moves.find(Move("c2a2p")) == moves.end());
   assertUnit(moves.find(Move("c2b2")) != moves.end());
   assertUnit(moves.find(Move("c2c1p")) == moves.end());
   assertUnit(moves.find(Move("c2c3")) != moves.end());
   assertUnit(moves.find(Move("c2c4")) != moves.end());
   assertUnit(moves.find(Move("c2c5")) != moves.end());
   assertUnit(moves.find(Move("c2c6")) != moves.end());
   assertUnit(moves.find(Move("c2c7")) != moves.end());
   assertUnit(moves.find(Move("c2c8p")) == moves.end());
   assertUnit(moves.find(Move("c2d2")) != moves.end());
   assertUnit(moves.find(Move("c2e2")) != moves.end());
   assertUnit(moves.find(Move("c2f2")) != moves.end());
   assertUnit(moves.find(Move("c2g2")) != moves.end());
   assertUnit(moves.find(Move("c2h2p")) == moves.end());

   // TEARDOWN
   board.board[2][1] = nullptr;
   board.board[0][1] = nullptr;
   board.board[2][0] = nullptr;
   board.board[2][7] = nullptr;
   board.board[7][1] = nullptr;

}

/*************************************
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8       P             8
 * 7       .             7
 * 6       .             6
 * 5       .             5
 * 4       .             4
 * 3       .             3
 * 2   P .(r). . . . P   2
 * 1       P             1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestRook::getMoves_slideToCapture()
{
   // SETUP
   BoardEmpty board;
   Rook rook(Position(2, 1), true /*white*/);
   board.board[2][1] = &rook; // c2

   // Place black pawns on all possible edge Rook move destinations
   Black w1(PAWN); board.board[0][1] = &w1; // b2
   Black w2(PAWN); board.board[2][0] = &w2; // c1
   Black w3(PAWN); board.board[2][7] = &w3; // c8
   Black w4(PAWN); board.board[7][1] = &w4; // h2
   set <Move> moves;

   // EXERCISE
   rook.getMoves(moves, board);

   // VERIFY

   assertUnit(moves.size() == 14);
   assertUnit(moves.find(Move("c2a2p")) != moves.end());
   assertUnit(moves.find(Move("c2b2")) != moves.end());
   assertUnit(moves.find(Move("c2c1p")) != moves.end());
   assertUnit(moves.find(Move("c2c3")) != moves.end());
   assertUnit(moves.find(Move("c2c4")) != moves.end());
   assertUnit(moves.find(Move("c2c5")) != moves.end());
   assertUnit(moves.find(Move("c2c6")) != moves.end());
   assertUnit(moves.find(Move("c2c7")) != moves.end());
   assertUnit(moves.find(Move("c2c8p")) != moves.end());
   assertUnit(moves.find(Move("c2d2")) != moves.end());
   assertUnit(moves.find(Move("c2e2")) != moves.end());
   assertUnit(moves.find(Move("c2f2")) != moves.end());
   assertUnit(moves.find(Move("c2g2")) != moves.end());
   assertUnit(moves.find(Move("c2h2p")) != moves.end());

   // TEARDOWN
   board.board[2][1] = nullptr;
   board.board[0][1] = nullptr;
   board.board[2][0] = nullptr;
   board.board[2][7] = nullptr;
   board.board[7][1] = nullptr;
}


/*************************************
 * GET TYPE : rook
 * Input:
 * Output: ROOK
 **************************************/
void TestRook::getType()
{
   // SETUP
   const Rook rook(7, 7, false /*white*/);
   PieceType pt = SPACE;
   
   // EXERCISE
   pt = rook.getType();
   
   // VERIFY
   assertUnit(pt == ROOK);
}
