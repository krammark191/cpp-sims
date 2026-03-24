/***********************************************************************
 * Source File:
 *    TEST KNIGHT
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    The unit tests for the knight
 ************************************************************************/

#include "testKnight.h"
#include "pieceKnight.h"     
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
  * 3             p   .   3
  * 2           P         2
  * 1              (n)    1
  * |                     |
  * +---a-b-c-d-e-f-g-h---+
  **************************************/
void TestKnight::getMoves_end()
{
   // SETUP
   BoardEmpty board;
   Knight knight(7, 7, false /*white*/);
   knight.fWhite = true;
   knight.position.set(6, 0);
   board.board[6][0] = &knight;
   Black black(PAWN);
   board.board[4][1] = &black;
   White white(PAWN);
   board.board[5][2] = &white;
   set <Move> moves;

   // EXERCISE
   knight.getMoves(moves, board);

   // VERIFY
   assertUnit(moves.size() == 2);  // many possible moves
   assertUnit(moves.find(Move("g1e2p")) != moves.end());
   assertUnit(moves.find(Move("g1h3")) != moves.end());

   // TEARDOWN
   board.board[6][0] = nullptr; // white knight
   board.board[4][1] = nullptr; // black pawn
   board.board[5][2] = nullptr; // white pawn
}
/*************************************
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8                     8
 * 7       p   p         7
 * 6     p       p       6
 * 5        (n)          5
 * 4     p       p       4
 * 3       p   p         3
 * 2                     2
 * 1                     1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestKnight::getMoves_blocked()
{

	// SETUP
	BoardEmpty board;
	Knight knight(Position(3, 4), true /*white*/);
	board.board[3][4] = &knight; // d5

	// Place white pawns on all possible knight move destinations
	// Knight at d5 (col=3, row=4)
	White w1(PAWN); board.board[1][5] = &w1; // b6
	White w2(PAWN); board.board[2][6] = &w2; // c7
	White w3(PAWN); board.board[4][6] = &w3; // e7
	White w4(PAWN); board.board[5][5] = &w4; // f6
	White w5(PAWN); board.board[5][3] = &w5; // f4
	White w6(PAWN); board.board[4][2] = &w6; // e3
	White w7(PAWN); board.board[2][2] = &w7; // c3
	White w8(PAWN); board.board[1][3] = &w8; // b4

	set<Move> moves;

	// EXERCISE
	knight.getMoves(moves, board);

	// VERIFY
	// All moves should be blocked (no legal moves)
	assertUnit(moves.size() == 0);
	assertUnit(moves.find(Move("d5b6p")) == moves.end());
	assertUnit(moves.find(Move("d5c7p")) == moves.end());
	assertUnit(moves.find(Move("d5e7p")) == moves.end());
	assertUnit(moves.find(Move("d5f6p")) == moves.end());
	assertUnit(moves.find(Move("d5f4p")) == moves.end());
	assertUnit(moves.find(Move("d5e3p")) == moves.end());
	assertUnit(moves.find(Move("d5c3p")) == moves.end());
	assertUnit(moves.find(Move("d5b4p")) == moves.end());

	// TEARDOWN
	board.board[1][5] = nullptr;
	board.board[2][6] = nullptr;
	board.board[4][6] = nullptr;
	board.board[5][5] = nullptr;
	board.board[5][3] = nullptr;
	board.board[4][2] = nullptr;
	board.board[2][2] = nullptr;
	board.board[1][3] = nullptr;
	board.board[3][4] = nullptr;

}

/*************************************
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8                     8
 * 7       p   p         7
 * 6     p       p       6
 * 5        (n)          5
 * 4     p       p       4
 * 3       p   p         3
 * 2                     2
 * 1                     1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestKnight::getMoves_capture()
{
	// SETUP
	BoardEmpty board;
	Knight knight(Position(3, 4), true /*white*/);
	board.board[3][4] = &knight; // d5

	// Place white pawns on all possible knight move destinations
	// Knight at d5 (col=3, row=4)
	Black w1(PAWN); board.board[1][5] = &w1; // b6
	Black w2(PAWN); board.board[2][6] = &w2; // c7
	Black w3(PAWN); board.board[4][6] = &w3; // e7
	Black w4(PAWN); board.board[5][5] = &w4; // f6
	Black w5(PAWN); board.board[5][3] = &w5; // f4
	Black w6(PAWN); board.board[4][2] = &w6; // e3
	Black w7(PAWN); board.board[2][2] = &w7; // c3
	Black w8(PAWN); board.board[1][3] = &w8; // b4

	set<Move> moves;

	// EXERCISE
	knight.getMoves(moves, board);

	// VERIFY
	// All moves should be open (no illegal moves)
	assertUnit(moves.size() == 8);
	assertUnit(moves.find(Move("d5b6p")) != moves.end());
	assertUnit(moves.find(Move("d5c7p")) != moves.end());
	assertUnit(moves.find(Move("d5e7p")) != moves.end());
	assertUnit(moves.find(Move("d5f6p")) != moves.end());
	assertUnit(moves.find(Move("d5f4p")) != moves.end());
	assertUnit(moves.find(Move("d5e3p")) != moves.end());
	assertUnit(moves.find(Move("d5c3p")) != moves.end());
	assertUnit(moves.find(Move("d5b4p")) != moves.end());

	// TEARDOWN
	board.board[1][5] = nullptr;
	board.board[2][6] = nullptr;
	board.board[4][6] = nullptr;
	board.board[5][5] = nullptr;
	board.board[5][3] = nullptr;
	board.board[4][2] = nullptr;
	board.board[2][2] = nullptr;
	board.board[1][3] = nullptr;
	board.board[3][4] = nullptr;
}

/*************************************
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8                     8
 * 7       .   .         7
 * 6     .       .       6
 * 5        (n)          5
 * 4     .       .       4
 * 3       .   .         3
 * 2                     2
 * 1                     1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestKnight::getMoves_free()
{
	// SETUP
	BoardEmpty board;
	Knight knight(Position(3, 4), true /*white*/);
	board.board[3][4] = &knight; // d5
	set<Move> moves;

	// EXERCISE
	knight.getMoves(moves, board);

	// VERIFY
	// All moves should be open (no illegal moves)
	assertUnit(moves.size() == 8);
	assertUnit(moves.find(Move("d5b6")) != moves.end());
	assertUnit(moves.find(Move("d5c7")) != moves.end());
	assertUnit(moves.find(Move("d5e7")) != moves.end());
	assertUnit(moves.find(Move("d5f6")) != moves.end());
	assertUnit(moves.find(Move("d5f4")) != moves.end());
	assertUnit(moves.find(Move("d5e3")) != moves.end());
	assertUnit(moves.find(Move("d5c3")) != moves.end());
	assertUnit(moves.find(Move("d5b4")) != moves.end());

	// TEARDOWN
	board.board[3][4] = nullptr;
}



/*************************************
 * GET TYPE : knight
 * Input:
 * Output: KNIGHT
 **************************************/
void TestKnight::getType()
{
   // SETUP  
   const Knight knight(7, 7, false /*white*/);
   PieceType pt = SPACE;

   // EXERCISE
   pt = knight.getType();

   // VERIFY
   assertUnit(pt == KNIGHT);
}  // TEARDOWN
