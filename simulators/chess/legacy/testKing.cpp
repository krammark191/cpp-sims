/***********************************************************************
 * Source File:
 *    TEST KING
 * Author:
 *    Mark Van Horn & Taden Marston
 * Summary:
 *    The unit tests for the King
 ************************************************************************/

#include "testKing.h"
#include "pieceRook.h"
#include "pieceKing.h"     
#include "board.h"
#include "uiDraw.h"
#include <cassert>      


/*************************************
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8                     8
 * 7                     7
 * 6       p p p         6
 * 5       p(k)p         5
 * 4       p p p         4
 * 3                     3
 * 2                     2
 * 1                     1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestKing::getMoves_blocked()
{
   // SETUP
   BoardEmpty board;
   King king(Position(3, 4), true /*white*/); // d5
   board.board[3][4] = &king;

   White p1(PAWN); board.board[2][3] = &p1; // c4
   White p2(PAWN); board.board[2][4] = &p2; // c5
   White p3(PAWN); board.board[2][5] = &p3; // c6
   White p4(PAWN); board.board[3][3] = &p4; // d4
   White p5(PAWN); board.board[3][5] = &p5; // d6
   White p6(PAWN); board.board[4][3] = &p6; // e4
   White p7(PAWN); board.board[4][4] = &p7; // e5
   White p8(PAWN); board.board[4][5] = &p8; // e6

   set<Move> moves;

   // EXERCISE
   king.getMoves(moves, board);

   // VERIFY
   assertUnit(moves.size() == 0);
   assertUnit(moves.find(Move("d5c4p")) == moves.end()); // c4 blocks d5
   assertUnit(moves.find(Move("d5c5p")) == moves.end()); // c5 blocks d5
   assertUnit(moves.find(Move("d5c6p")) == moves.end()); // c6 blocks d5
   assertUnit(moves.find(Move("d5d4p")) == moves.end()); // d4 blocks d5
   assertUnit(moves.find(Move("d5d6p")) == moves.end()); // d6 blocks d5
   assertUnit(moves.find(Move("d5e4p")) == moves.end()); // e4 blocks d5
   assertUnit(moves.find(Move("d5e5p")) == moves.end()); // e5 blocks d5
   assertUnit(moves.find(Move("d5e6p")) == moves.end()); // e6 blocks d5

   // TEARDOWN
   board.board[3][4] = nullptr;
   board.board[2][3] = nullptr;
   board.board[2][4] = nullptr;
   board.board[2][5] = nullptr;
   board.board[3][3] = nullptr;
   board.board[3][5] = nullptr;
   board.board[4][3] = nullptr;
   board.board[4][4] = nullptr;
   board.board[4][5] = nullptr;
}

/*************************************
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8                     8
 * 7                     7
 * 6       P P P         6
 * 5       P(k)P         5
 * 4       P P P         4
 * 3                     3
 * 2                     2
 * 1                     1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestKing::getMoves_capture()
{
   // SETUP
   BoardEmpty board;
   King king(Position(3, 4), true /*white*/); // d5
   board.board[3][4] = &king;

   Black p1(PAWN); board.board[2][3] = &p1; // c4
   Black p2(PAWN); board.board[2][4] = &p2; // c5
   Black p3(PAWN); board.board[2][5] = &p3; // c6
   Black p4(PAWN); board.board[3][3] = &p4; // d4
   Black p5(PAWN); board.board[3][5] = &p5; // d6
   Black p6(PAWN); board.board[4][3] = &p6; // e4
   Black p7(PAWN); board.board[4][4] = &p7; // e5
   Black p8(PAWN); board.board[4][5] = &p8; // e6

   set<Move> moves;

   // EXERCISE
   king.getMoves(moves, board);

   // VERIFY
   assertUnit(moves.size() == 8);
   assertUnit(moves.find(Move("d5c4p")) != moves.end()); // c4 blocks d5
   assertUnit(moves.find(Move("d5c5p")) != moves.end()); // c5 blocks d5
   assertUnit(moves.find(Move("d5c6p")) != moves.end()); // c6 blocks d5
   assertUnit(moves.find(Move("d5d4p")) != moves.end()); // d4 blocks d5
   assertUnit(moves.find(Move("d5d6p")) != moves.end()); // d6 blocks d5
   assertUnit(moves.find(Move("d5e4p")) != moves.end()); // e4 blocks d5
   assertUnit(moves.find(Move("d5e5p")) != moves.end()); // e5 blocks d5
   assertUnit(moves.find(Move("d5e6p")) != moves.end()); // e6 blocks d5

   // TEARDOWN
   board.board[3][4] = nullptr;
   board.board[2][3] = nullptr;
   board.board[2][4] = nullptr;
   board.board[2][5] = nullptr;
   board.board[3][3] = nullptr;
   board.board[3][5] = nullptr;
   board.board[4][3] = nullptr;
   board.board[4][4] = nullptr;
   board.board[4][5] = nullptr;
}


/*************************************
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8                     8
 * 7                     7
 * 6       . . .         6
 * 5       .(k).         5
 * 4       . . .         4
 * 3                     3
 * 2                     2
 * 1                     1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestKing::getMoves_free()
{
   // SETUP
   BoardEmpty board;
   King king(Position(3, 4), true /*white*/); // d5
   board.board[3][4] = &king;

   set<Move> moves;

   // EXERCISE
   king.getMoves(moves, board);

   // VERIFY
   assertUnit(moves.size() == 8);
   assertUnit(moves.find(Move("d5c4")) != moves.end()); // c4
   assertUnit(moves.find(Move("d5c5")) != moves.end()); // c5
   assertUnit(moves.find(Move("d5c6")) != moves.end()); // c6
   assertUnit(moves.find(Move("d5d4")) != moves.end()); // d4
   assertUnit(moves.find(Move("d5d6")) != moves.end()); // d6
   assertUnit(moves.find(Move("d5e4")) != moves.end()); // e4
   assertUnit(moves.find(Move("d5e5")) != moves.end()); // e5
   assertUnit(moves.find(Move("d5e6")) != moves.end()); // e6

   // TEARDOWN
   board.board[3][4] = nullptr;
}


/*************************************
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8                     8
 * 7                     7
 * 6                     6
 * 5                     5
 * 4                     4
 * 3                     3
 * 2   . .               2
 * 1  (k).               1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestKing::getMoves_end()
{
   // SETUP
   BoardEmpty board;
   King king(Position(0, 0), true /*white*/); // a1
   board.board[0][0] = &king;
   set<Move> moves;

   // EXERCISE
   king.getMoves(moves, board);

	// VERIFY
   assertUnit(moves.size() == 3);
   assertUnit(moves.find(Move("a1a2")) != moves.end()); // a2
   assertUnit(moves.find(Move("a1b1")) != moves.end()); // b1
   assertUnit(moves.find(Move("a1b2")) != moves.end()); // b2

   // TEARDOWN
   board.board[0][0] = nullptr;
}


/*************************************
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8                     8
 * 7                     7
 * 6                     6
 * 5                     5
 * 4                     4
 * 3                     3
 * 2         p p p       2
 * 1   r   . .(k). . r   1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestKing::getMoves_whiteCastle()
{
   // SETUP
   BoardEmpty board;
   King king(Position(4, 0), true /*white*/); // e1
   board.board[4][0] = &king;
	Rook qRook(Position(0, 0), true); board.board[0][0] = &qRook; // a1
	Rook kRook(Position(7, 0), true); board.board[7][0] = &qRook; // h1

   White p2(PAWN); board.board[3][1] = &p2; // d2
   White p3(PAWN); board.board[4][1] = &p3; // e2
   White p4(PAWN); board.board[5][1] = &p4; // f2

   set<Move> moves;

   // EXERCISE
   king.getMoves(moves, board);

   // VERIFY
   assertUnit(moves.size() == 4);
	assertUnit(moves.find(Move("e1a2p")) == moves.end()); // d2 blocked by d2 pawn
	assertUnit(moves.find(Move("e1b2p")) == moves.end()); // e2 blocked by e2 pawn
	assertUnit(moves.find(Move("e1b2p")) == moves.end()); // f2 blocked by f2 pawn
	assertUnit(moves.find(Move("e1d1")) != moves.end()); // d1
	assertUnit(moves.find(Move("e1f1")) != moves.end()); // f1
	assertUnit(moves.find(Move("e1g1c")) != moves.end()); // castle king side
	assertUnit(moves.find(Move("e1c1C")) != moves.end()); // castle queen side

   // TEARDOWN
   board.board[4][0] = nullptr; // King
	board.board[0][0] = nullptr; // Rooks
	board.board[7][0] = nullptr; // Rooks
	board.board[3][1] = nullptr; // Spaces
	board.board[4][1] = nullptr; // Spaces
	board.board[5][1] = nullptr; // Spaces

}


/*************************************
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8   R   . .(K). . R   8
 * 7         P P P       7
 * 6                     6
 * 5                     5
 * 4                     4
 * 3                     3
 * 2                     2
 * 1                     1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestKing::getMoves_blackCastle()
{
   // SETUP
   BoardEmpty board;
   King king(Position(4, 7), false /*white*/); // e7
   board.board[4][7] = &king;
   Rook qRook(Position(0, 7), false); board.board[0][7] = &qRook; // a7
   Rook kRook(Position(7, 7), false); board.board[7][7] = &qRook; // h7

   Black p2(PAWN); board.board[3][6] = &p2; // d6
   Black p3(PAWN); board.board[4][6] = &p3; // e6
   Black p4(PAWN); board.board[5][6] = &p4; // f6

   set<Move> moves;

   // EXERCISE
   king.getMoves(moves, board);

   // VERIFY
   assertUnit(moves.size() == 4);
   assertUnit(moves.find(Move("e8a7p")) == moves.end()); // d6 blocked by d6 pawn
   assertUnit(moves.find(Move("e8b7p")) == moves.end()); // e6 blocked by e6 pawn
   assertUnit(moves.find(Move("e8b7p")) == moves.end()); // f6 blocked by f6 pawn
   assertUnit(moves.find(Move("e8d8")) != moves.end()); // d7
   assertUnit(moves.find(Move("e8f8")) != moves.end()); // f7
   assertUnit(moves.find(Move("e8g8c")) != moves.end()); // castle king side
   assertUnit(moves.find(Move("e8c8C")) != moves.end()); // castle queen side

   // TEARDOWN
   board.board[4][7] = nullptr; // King
   board.board[0][7] = nullptr; // Rooks
   board.board[7][7] = nullptr; // Rooks
   board.board[3][6] = nullptr; // Spaces
   board.board[4][6] = nullptr; // Spaces
   board.board[5][6] = nullptr; // Spaces
}


/*************************************
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8                     8
 * 7                     7
 * 6                     6
 * 5                     5
 * 4                     4
 * 3                     3
 * 2         p p p       2
 * 1   r     .(k).   r   1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestKing::getMoves_whiteCastleKingMoved()
{
   // SETUP
   BoardEmpty board;
   King king(Position(4, 0), true /*white*/); // e1
	king.setLastMove(1); // King has moved
   board.board[4][0] = &king;
   Rook qRook(Position(0, 0), true); board.board[0][0] = &qRook; // a1
   Rook kRook(Position(7, 0), true); board.board[7][0] = &qRook; // h1

   White p2(PAWN); board.board[3][1] = &p2; // d2
   White p3(PAWN); board.board[4][1] = &p3; // e2
   White p4(PAWN); board.board[5][1] = &p4; // f2

   set<Move> moves;

   // EXERCISE
   king.getMoves(moves, board);

   // VERIFY
   assertUnit(moves.size() == 2);
   assertUnit(moves.find(Move("e1a2p")) == moves.end()); // d2 blocked by d2 pawn
   assertUnit(moves.find(Move("e1b2p")) == moves.end()); // e2 blocked by e2 pawn
   assertUnit(moves.find(Move("e1b2p")) == moves.end()); // f2 blocked by f2 pawn
   assertUnit(moves.find(Move("e1d1")) != moves.end()); // d1
   assertUnit(moves.find(Move("e1f1")) != moves.end()); // f1
   assertUnit(moves.find(Move("e1g1c")) == moves.end()); // castle king side
   assertUnit(moves.find(Move("e1c1C")) == moves.end()); // castle queen side

   // TEARDOWN
   board.board[4][0] = nullptr; // King
   board.board[0][0] = nullptr; // Rooks
   board.board[7][0] = nullptr; // Rooks
   board.board[3][1] = nullptr; // Spaces
   board.board[4][1] = nullptr; // Spaces
   board.board[5][1] = nullptr; // Spaces
}


/*************************************
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8                     8
 * 7                     7
 * 6                     6
 * 5                     5
 * 4                     4
 * 3                     3
 * 2         p p p       2
 * 1   r     .(k).   r   1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestKing::getMoves_whiteCastleRookMoved()
{
   // SETUP
   BoardEmpty board;
   King king(Position(4, 0), true /*white*/); // e1
   board.board[4][0] = &king;
   Rook qRook(Position(0, 0), true); board.board[0][0] = &qRook; // a1
	qRook.setLastMove(1); // Rook has moved
   Rook kRook(Position(7, 0), true); board.board[7][0] = &qRook; // h1
	kRook.setLastMove(1); // Rook has moved

   White p2(PAWN); board.board[3][1] = &p2; // d2
   White p3(PAWN); board.board[4][1] = &p3; // e2
   White p4(PAWN); board.board[5][1] = &p4; // f2

   set<Move> moves;

   // EXERCISE
   king.getMoves(moves, board);

   // VERIFY
   assertUnit(moves.size() == 2);
   assertUnit(moves.find(Move("e1a2p")) == moves.end()); // d2 blocked by d2 pawn
   assertUnit(moves.find(Move("e1b2p")) == moves.end()); // e2 blocked by e2 pawn
   assertUnit(moves.find(Move("e1b2p")) == moves.end()); // f2 blocked by f2 pawn
   assertUnit(moves.find(Move("e1d1")) != moves.end()); // d1
   assertUnit(moves.find(Move("e1f1")) != moves.end()); // f1
   assertUnit(moves.find(Move("e1g1c")) == moves.end()); // castle king side
   assertUnit(moves.find(Move("e1c1C")) == moves.end()); // castle queen side

   // TEARDOWN
   board.board[4][0] = nullptr; // King
   board.board[0][0] = nullptr; // Rooks
   board.board[7][0] = nullptr; // Rooks
   board.board[3][1] = nullptr; // Spaces
   board.board[4][1] = nullptr; // Spaces
   board.board[5][1] = nullptr; // Spaces
}

/*************************************
 * GET TYPE : king
 * Input:
 * Output: KING
 **************************************/
void TestKing::getType()
{
   // SETUP
   const King king(7, 7, false /*white*/);
   PieceType pt = SPACE;
   
   // EXERCISE
   pt = king.getType();
   
   // VERIFY
   assertUnit(pt == KING);
}


