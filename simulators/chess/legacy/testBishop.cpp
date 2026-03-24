/***********************************************************************
 * Source File:
 *    TEST BISHOP
 * Author:
 *    <your name here>
 * Summary:
 *    The unit tests for a bishop
 ************************************************************************/


#include "testBishop.h"
#include "pieceBishop.h"     
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
 * 3     p   p           3
 * 2      (b)            2
 * 1     p   p           1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestBishop::getMoves_blocked()
{
   // SETUP
   BoardEmpty board;
   Bishop bishop(Position(2, 1), true /*white*/);
   board.board[2][1] = &bishop; // c2

	// Place white pawns on all possible bishop move destinations
   White w1(PAWN); board.board[1][0] = &w1; // b1
   White w2(PAWN); board.board[3][0] = &w2; // d1
   White w3(PAWN); board.board[1][2] = &w3; // b3
   White w4(PAWN); board.board[3][2] = &w4; // d3
   set <Move> moves;

   // EXERCISE
   bishop.getMoves(moves, board);

   // VERIFY
   assertUnit(moves.size() == 0);
   assertUnit(moves.find(Move("c2b1p")) == moves.end());
   assertUnit(moves.find(Move("c2b3p")) == moves.end());
   assertUnit(moves.find(Move("c2d1p")) == moves.end());
   assertUnit(moves.find(Move("c2d3p")) == moves.end());

   // TEARDOWN
   board.board[2][1] = nullptr;
   board.board[1][0] = nullptr;
   board.board[3][0] = nullptr;
   board.board[1][2] = nullptr;
   board.board[3][2] = nullptr;
}

/*************************************
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8                     8
 * 7                 .   7
 * 6               .     6
 * 5             .       5
 * 4   .       .         4
 * 3     .   .           3
 * 2      (b)            2
 * 1     .   .           1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestBishop::getMoves_slideToEnd()
{
   // SETUP
   BoardEmpty board;
   Bishop bishop(Position(2, 1), true /*white*/);
   board.board[2][1] = &bishop; // c2

   set<Move> moves;

   // EXERCISE
   bishop.getMoves(moves, board);

   // VERIFY
   // All possible diagonal moves from c2 to the edge of the board
   assertUnit(moves.size() == 9);
   assertUnit(moves.find(Move("c2b1")) != moves.end());
   assertUnit(moves.find(Move("c2d1")) != moves.end());
   assertUnit(moves.find(Move("c2b3")) != moves.end());
   assertUnit(moves.find(Move("c2a4")) != moves.end());
   assertUnit(moves.find(Move("c2d3")) != moves.end());
   assertUnit(moves.find(Move("c2e4")) != moves.end());
   assertUnit(moves.find(Move("c2f5")) != moves.end());
   assertUnit(moves.find(Move("c2g6")) != moves.end());
   assertUnit(moves.find(Move("c2h7")) != moves.end());

   // TEARDOWN
   board.board[2][1] = nullptr;
}


/*************************************
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8                     8
 * 7                 p   7
 * 6               .     6
 * 5             .       5
 * 4   p       .         4
 * 3     .   .           3
 * 2      (b)            2
 * 1     p   p           1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestBishop::getMoves_slideToBlock()
{
   // SETUP
   BoardEmpty board;
   Bishop bishop(Position(2, 1), true /*white*/);
   board.board[2][1] = &bishop; // c2

   // Place white pawns to block bishop's diagonals
   White p3(PAWN); board.board[0][3] = &p3; // a4
   White p1(PAWN); board.board[1][0] = &p1; // b1
   White p2(PAWN); board.board[3][0] = &p2; // d1
   White p4(PAWN); board.board[7][6] = &p4; // h7


   set<Move> moves;

   // EXERCISE
   bishop.getMoves(moves, board);

   // VERIFY
   // Bishop can move to all empty squares up to but not including the pawns
   assertUnit(moves.size() == 5);

   // VALID moves
   assertUnit(moves.find(Move("c2b3")) != moves.end());
   assertUnit(moves.find(Move("c2d3")) != moves.end());
   assertUnit(moves.find(Move("c2e4")) != moves.end());
   assertUnit(moves.find(Move("c2f5")) != moves.end());
   assertUnit(moves.find(Move("c2g6")) != moves.end());

   // INVALID moves (blocked or past friendly pieces)
   assertUnit(moves.find(Move("c2a4")) == moves.end());
   assertUnit(moves.find(Move("c2b1")) == moves.end());
   assertUnit(moves.find(Move("c2d1")) == moves.end());
   assertUnit(moves.find(Move("c2h7")) == moves.end());


   // TEARDOWN
   board.board[2][1] = nullptr;
   board.board[0][3] = nullptr;
   board.board[1][0] = nullptr;
   board.board[3][0] = nullptr;
   board.board[7][6] = nullptr;
   
}


/*************************************
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8                     8
 * 7                 P   7
 * 6               .     6
 * 5             .       5
 * 4   P       .         4
 * 3     .   .           3
 * 2      (b)            2
 * 1     P   P           1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestBishop::getMoves_slideToCapture()
{
   // SETUP
   BoardEmpty board;
   Bishop bishop(Position(2, 1), true /*white*/);
   board.board[2][1] = &bishop; // c2

   // Place black pawns for bishop's diagonal captures
   Black p3(PAWN); board.board[0][3] = &p3; // a4
   Black p1(PAWN); board.board[1][0] = &p1; // b1
   Black p2(PAWN); board.board[3][0] = &p2; // d1
   Black p4(PAWN); board.board[7][6] = &p4; // h7


   set<Move> moves;

   // EXERCISE
   bishop.getMoves(moves, board);

   // VERIFY
   // Bishop can move to all empty squares and capture the pawns
   assertUnit(moves.size() == 9);

   // VALID moves
   assertUnit(moves.find(Move("c2b3")) != moves.end());
   assertUnit(moves.find(Move("c2d3")) != moves.end());
   assertUnit(moves.find(Move("c2e4")) != moves.end());
   assertUnit(moves.find(Move("c2f5")) != moves.end());
   assertUnit(moves.find(Move("c2g6")) != moves.end());
   assertUnit(moves.find(Move("c2a4p")) != moves.end());
   assertUnit(moves.find(Move("c2b1p")) != moves.end());
   assertUnit(moves.find(Move("c2d1p")) != moves.end());
   assertUnit(moves.find(Move("c2h7p")) != moves.end());


   // TEARDOWN
   board.board[2][1] = nullptr;
   board.board[0][3] = nullptr;
   board.board[1][0] = nullptr;
   board.board[3][0] = nullptr;
   board.board[7][6] = nullptr;
}


/*************************************
 * GET TYPE : bishop
 * Input:
 * Output: BISHOP
 **************************************/
void TestBishop::getType()
{
   // SETUP
   const Bishop bishop(7, 7, false /*white*/);
   PieceType pt = SPACE;
   
   // EXERCISE
   pt = bishop.getType();
   
   // VERIFY
   assertUnit(pt == BISHOP);
}
