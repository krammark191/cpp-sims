/***********************************************************************
 * Source File:
 *    TEST PAWN
 * Author:
 *    Mark Van Horn & Taden Marston
 * Summary:
 *    The unit tests for the pawn
 ************************************************************************/

#include "testPawn.h"
#include "piecePawn.h"
#include "pieceRook.h"
#include "board.h"
#include "uiDraw.h"
#include <cassert>      



 /*************************************
  * GET MOVES TEST Simple
  * White pawn in the middle of the board move forward one space: b4 --> b5
  *
  * +---a-b-c-d-e-f-g-h---+
  * |                     |
  * 8                     8
  * 7                     7
  * 6                     6
  * 5     .               5
  * 4    (p)              4
  * 3                     3
  * 2                     2
  * 1                     1
  * |                     |
  * +---a-b-c-d-e-f-g-h---+
  **************************************/
void TestPawn::getMoves_simpleWhite()
{
   // SETUP
   BoardEmpty board;
   Pawn pawn(1, 3, true); // b4, white
   board.board[1][3] = &pawn;
   pawn.fWhite = true;
   set<Move> moves;
   
   // EXERCISE
   pawn.getMoves(moves, board);
   
   // VERIFY
   assertUnit(moves.size() == 1);
   assertUnit(moves.find(Move("b4b5")) != moves.end());
   
   // TEARDOWN
   board.board[1][3] = nullptr; // white pawn
}

/*************************************
 * GET MOVES TEST Simple
 * Black pawn in the middle of the board move forward one space: b4 --> b3
 *
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8                     8
 * 7                     7
 * 6                     6
 * 5                     5
 * 4    (P)              4
 * 3     .               3
 * 2                     2
 * 1                     1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestPawn::getMoves_simpleBlack()
{
   // SETUP
   BoardEmpty board;
   Pawn pawn(1, 3, false); // b4, black
   board.board[1][3] = &pawn;
   pawn.fWhite = false;
   set<Move> moves;
   
   // EXERCISE
   pawn.getMoves(moves, board);
   
   // VERIFY
   assertUnit(moves.size() == 1);
   assertUnit(moves.find(Move("b4b3")) != moves.end());
   
   // TEARDOWN
   board.board[1][3] = nullptr; // black pawn
}


/*************************************
 * GET MOVES TEST InitialAdvance
 * White pawn initial advance: b2 --> b4 as well as b2 --> b3
 *
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8                     8
 * 7                     7
 * 6                     6
 * 5                     5
 * 4     .               4
 * 3     .               3
 * 2    (p)              2
 * 1                     1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestPawn::getMoves_initialAdvanceWhite()
{
   // SETUP
   BoardEmpty board;
   Pawn pawn(1, 1, true); // b2, white
   board.board[1][1] = &pawn;
   pawn.fWhite = true;
   set<Move> moves;
   
   // EXERCISE
   pawn.getMoves(moves, board);
   
   // VERIFY
   assertUnit(moves.size() == 2);
   assertUnit(moves.find(Move("b2b3")) != moves.end());
   assertUnit(moves.find(Move("b2b4")) != moves.end());
   
   // TEARDOWN
   board.board[1][1] = nullptr; // white pawn
}

/*************************************
 * GET MOVES TEST InitialAdvance
 * Black pawn initial advance: c7 --> c6 as well as c7 --> c5
 *
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8                     8
 * 7      (P)            7
 * 6       .             6
 * 5       .             5
 * 4                     4
 * 3                     3
 * 2                     2
 * 1                     1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestPawn::getMoves_initialAdvanceBlack()
{
   // SETUP
   BoardEmpty board;
   Pawn pawn(2, 6, false); // c7, black
   board.board[2][6] = &pawn;
   pawn.fWhite = false;
   set<Move> moves;
   
   // EXERCISE
   pawn.getMoves(moves, board);
   
   // VERIFY
   assertUnit(moves.size() == 2);
   assertUnit(moves.find(Move("c7c6")) != moves.end());
   assertUnit(moves.find(Move("c7c5")) != moves.end());
   
   // TEARDOWN
   board.board[2][6] = nullptr; // black pawn
}


/*************************************
 * GET MOVES TEST Capture
 * Double capture: move white pawn b6 and capture two: a7 and c7. b7 is blocked
 *
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8                     8
 * 7   P P P             7
 * 6    (p)              6
 * 5                     5
 * 4                     4
 * 3                     3
 * 2                     2
 * 1                     1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestPawn::getMoves_captureWhite()
{
   // SETUP
   BoardEmpty board;
   Pawn pawn(1, 5, true); // b6, white
   Pawn blackA(0, 6, false); // a7, black
   Pawn blackB(1, 6, false); // b7, black (blocker)
   Pawn blackC(2, 6, false); // c7, black
   board.board[1][5] = &pawn;
   board.board[0][6] = &blackA;
   board.board[1][6] = &blackB;
   board.board[2][6] = &blackC;
   pawn.fWhite = true;
   set<Move> moves;
   
   // EXERCISE
   pawn.getMoves(moves, board);
   
   // VERIFY
   assertUnit(moves.size() == 2);
   assertUnit(moves.find(Move("b6a7")) != moves.end());
   assertUnit(moves.find(Move("b6c7")) != moves.end());
   
   // TEARDOWN
   board.board[1][5] = nullptr;
   board.board[0][6] = nullptr;
   board.board[1][6] = nullptr;
   board.board[2][6] = nullptr;
}


/*************************************
 * GET MOVES TEST Capture
 * Double capture: move black pawn b6 and capture two: a5 and c5. b5 is blocked
 *
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8                     8
 * 7                     7
 * 6    (P)              6
 * 5   p p p             5
 * 4                     4
 * 3                     3
 * 2                     2
 * 1                     1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestPawn::getMoves_captureBlack()
{
   // SETUP
   BoardEmpty board;
   Pawn pawn(1, 5, false); // b6, black
   Pawn whiteA(0, 4, true); // a5, white
   Pawn whiteB(1, 4, true); // b5, white (blocker)
   Pawn whiteC(2, 4, true); // c5, white
   board.board[1][5] = &pawn;
   board.board[0][4] = &whiteA;
   board.board[1][4] = &whiteB;
   board.board[2][4] = &whiteC;
   pawn.fWhite = false;
   set<Move> moves;
   
   // EXERCISE
   pawn.getMoves(moves, board);
   
   // VERIFY
   assertUnit(moves.size() == 2);
   assertUnit(moves.find(Move("b6a5")) != moves.end());
   assertUnit(moves.find(Move("b6c5")) != moves.end());
   
   // TEARDOWN
   board.board[1][5] = nullptr;
   board.board[0][4] = nullptr;
   board.board[1][4] = nullptr;
   board.board[2][4] = nullptr;
}

/*************************************
 * GET MOVES TEST Enpassant
 * Enpassant: white b5 can capture a5 and c5. Note that b6 is blocked
 *
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8                     8
 * 7                     7
 * 6   . P .             6
 * 5   P(p)P             5
 * 4                     4
 * 3                     3
 * 2                     2
 * 1                     1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestPawn::getMoves_enpassantWhite()
{
   // SETUP
   BoardEmpty board;
   Pawn pawn(1, 4, true);    // b5, white
   Pawn blackA(0, 4, false); // a5, black (en passant target)
   Pawn blackB(1, 5, false); // b6, black (blocker)
   Pawn blackC(2, 4, false); // c5, black (en passant target)
   board.board[1][4] = &pawn;
   board.board[0][4] = &blackA;
   board.board[1][5] = &blackB;
   board.board[2][4] = &blackC;
   pawn.fWhite = true;
   
//   // Set up en passant flags (assuming isEnPassant is a property on Pawn; adjust as needed)
//   blackA.enPassant = true;
//   blackC.enPassant = true;
   
   set<Move> moves;
   
   // EXERCISE
   pawn.getMoves(moves, board);
   
   // VERIFY
   assertUnit(moves.size() == 2);
   assertUnit(moves.find(Move("b5a6E")) != moves.end());
   assertUnit(moves.find(Move("b5c6E")) != moves.end());
   
   // TEARDOWN
   board.board[1][4] = nullptr;
   board.board[0][4] = nullptr;
   board.board[1][5] = nullptr;
   board.board[2][4] = nullptr;
}


/*************************************
 * GET MOVES TEST Enpassant
 * Enpassant black f4 can capture e4 and g4. Note that f3 is blocked
 *
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8                     8
 * 7                     7
 * 6                     6
 * 5                     5
 * 4           p(P)p     4
 * 3           . p .     3
 * 2                     2
 * 1                     1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestPawn::getMoves_enpassantBlack()
{
   // SETUP
   BoardEmpty board;
   Pawn pawn(5, 3, false);    // f4, black
   Pawn whiteE(4, 3, true);   // e4, white (en passant target)
   Pawn blackF(5, 2, false);  // f3, black (blocker)
   Pawn whiteG(6, 3, true);   // g4, white (en passant target)
   board.board[5][3] = &pawn;
   board.board[4][3] = &whiteE;
   board.board[5][2] = &blackF;
   board.board[6][3] = &whiteG;
   pawn.fWhite = false;
   
//   // Set up en passant flags (assuming isEnPassant is a property on Pawn; adjust as needed)
//   whiteE.enPassant = true;
//   whiteG.enPassant = true;
   
   set<Move> moves;
   
   // EXERCISE
   pawn.getMoves(moves, board);
   
   // VERIFY
   assertUnit(moves.size() == 2);
   assertUnit(moves.find(Move("f4e3E")) != moves.end());
   assertUnit(moves.find(Move("f4g3E")) != moves.end());
   
   // TEARDOWN
   board.board[5][3] = nullptr;
   board.board[4][3] = nullptr;
   board.board[5][2] = nullptr;
   board.board[6][3] = nullptr;
}

/*************************************
 * GET MOVES TEST Promotion
 * Promotion: white pawn b7 can capture two pawns: a8 and c8. 
 *            It can also move into b8. All three are promoted to queen
 *
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8   P . P             8
 * 7    (p)              7
 * 6                     6
 * 5                     5
 * 4                     4
 * 3                     3
 * 2                     2
 * 1                     1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestPawn::getMoves_promotionWhite()
{
   // SETUP
   BoardEmpty board;
   Pawn pawn(1, 6, true);    // b7, white
   Pawn blackA(0, 7, false); // a8, black
   Pawn blackC(2, 7, false); // c8, black
   board.board[1][6] = &pawn;
   board.board[0][7] = &blackA;
   board.board[2][7] = &blackC;
   pawn.fWhite = true;
   set<Move> moves;
   
   // EXERCISE
   pawn.getMoves(moves, board);
   
   // VERIFY
   assertUnit(moves.size() == 3);
   assertUnit(moves.find(Move("b7a8Q")) != moves.end()); // promote via capture
   assertUnit(moves.find(Move("b7b8Q")) != moves.end()); // promote straight
   assertUnit(moves.find(Move("b7c8Q")) != moves.end()); // promote via capture
   
   // TEARDOWN
   board.board[1][6] = nullptr;
   board.board[0][7] = nullptr;
   board.board[2][7] = nullptr;
}


/*************************************
 * GET MOVES TEST Promotion
 * Promotion: black pawn e2 can capture two rooks: d1 and f1.
 *            It can also move into e1. All three are promoted to queen
 *
 * +---a-b-c-d-e-f-g-h---+
 * |                     |
 * 8                     8
 * 7                     7
 * 6                     6
 * 5                     5
 * 4                     4
 * 3                     3
 * 2          (P)        2
 * 1         r   r       1
 * |                     |
 * +---a-b-c-d-e-f-g-h---+
 **************************************/
void TestPawn::getMoves_promotionBlack()
{
   // SETUP
   BoardEmpty board;
   Pawn pawn(4, 1, false);    // e2, black
   Rook whiteD(3, 0, true);   // d1, white
   Rook whiteF(5, 0, true);   // f1, white
   board.board[4][1] = &pawn;
   board.board[3][0] = &whiteD;
   board.board[5][0] = &whiteF;
   pawn.fWhite = false;
   set<Move> moves;
   
   // EXERCISE
   pawn.getMoves(moves, board);
   
   // VERIFY
   assertUnit(moves.size() == 3);
   assertUnit(moves.find(Move("e2d1Q")) != moves.end()); // promote via capture
   assertUnit(moves.find(Move("e2e1Q")) != moves.end()); // promote straight
   assertUnit(moves.find(Move("e2f1Q")) != moves.end()); // promote via capture
   
   // TEARDOWN
   board.board[4][1] = nullptr;
   board.board[3][0] = nullptr;
   board.board[5][0] = nullptr;
}


/*************************************
 * GET TYPE : pawn
 * Input:
 * Output: PAWN
 **************************************/
void TestPawn::getType()
{
   // SETUP
   const Pawn pawn(7, 7, false /*white*/);
   PieceType pt = SPACE;
   
   // EXERCISE
   pt = pawn.getType();
   
   // VERIFY
   assertUnit(pt == PAWN);
}

