/***********************************************************************
 * Source File:
 *    TEST BOARD
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    The unit tests for board
 ************************************************************************/


#include "testBoard.h"
#include "position.h"
#include "piece.h"
#include "board.h"
#include <cassert>




/********************************************************
 *    e5c6
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 * |                     |       |                     |
 * 8                     8       8                     8
 * 7                     7       7                     7
 * 6       .             6       6       n             6
 * 5          (n)        5       5           .         5
 * 4                     4  -->  4                     4
 * 3                     3       3                     3
 * 2                     2       2                     2
 * 1                     1       1                     1
 * |                     |       |                     |
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 ********************************************************/
void TestBoard::move_knightMove()
{  // SETUP
   Move move;
   move.source.set(4, 4);
   move.dest.set(2, 5);
   move.capture = SPACE;
   move.promote = SPACE;
   move.isWhite = true;
   move.moveType = Move::MOVE;
   Board board(nullptr, true /*noreset*/);
   board.numMoves = 10;
   board.board[4][4] = new PieceSpy(4, 4, true  /*isWhite*/, KNIGHT);
   board.board[2][5] = new PieceSpy(2, 5, false /*isWhite*/, SPACE);
   board.board[4][4]->nMoves = 0;
   PieceSpy::reset();
   
   // EXERCISE
   board.move(move);
   
   // VERIFY
   assertUnit(11 == board.numMoves);
   assertUnit(SPACE == (board.board[4][4])->getType());
   assertUnit(KNIGHT == (board.board[2][5])->getType());
   assertUnit(PieceSpy::numConstruct == 0);
   assertUnit(PieceSpy::numCopy == 0);
   assertUnit(PieceSpy::numDelete == 0);
   assertUnit(PieceSpy::numAssign == 0);
   assertUnit(PieceSpy::numMove == 0);
   
   // TEARDOWN
   delete board.board[2][5];
   delete board.board[4][4];
   board.board[2][5] = board.board[4][4] = nullptr;
}

/********************************************************
 *    e5c6r
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 * |                     |       |                     |
 * 8                     8       8                     8
 * 7       R             7       7                     7
 * 6                     6       6       n             6
 * 5          (n)        5       5           .         5
 * 4                     4  -->  4                     4
 * 3                     3       3                     3
 * 2                     2       2                     2
 * 1                     1       1                     1
 * |                     |       |                     |
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 ********************************************************/
void TestBoard::move_knightAttack()
{  // SETUP
   Move move;
   move.source.set(4, 4);
   move.dest.set(2, 5);
   move.capture = ROOK;
   move.promote = SPACE;
   move.isWhite = true;
   move.moveType = Move::MOVE;
   Board board(nullptr, true /*noreset*/);
   board.numMoves = 10;
   board.board[4][4] = new PieceSpy(4, 4, true  /*isWhite*/, KNIGHT);
   board.board[2][5] = new PieceSpy(2, 5, false /*isWhite*/, ROOK);
   board.board[4][4]->nMoves = 0;
   PieceSpy::reset();
   
   // EXERCISE
   board.move(move);
   
   // VERIFY
   assertUnit(11 == board.numMoves);
   assertUnit(SPACE == (board.board[4][4])->getType());
   assertUnit(KNIGHT == (board.board[2][5])->getType());
   assertUnit(PieceSpy::numConstruct == 0);
   assertUnit(PieceSpy::numCopy == 0);
   assertUnit(PieceSpy::numDelete == 1);
   assertUnit(PieceSpy::numAssign == 0);
   assertUnit(PieceSpy::numMove == 0);
   
   // TEARDOWN
   delete board.board[2][5];
   delete board.board[4][4];
   board.board[2][5] = board.board[4][4] = nullptr;
}

/********************************************************
 *   a2a3
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 * |                     |       |                     |
 * 8                     8       8                     8
 * 7                     7       7                     7
 * 6                     6       6                     6
 * 5                     5       5                     5
 * 4                     4  -->  4                     4
 * 3   .                 3       3   p                 3
 * 2  (p)                2       2   .                 2
 * 1                     1       1                     1
 * |                     |       |                     |
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 ********************************************************/
void TestBoard::move_pawnSimple()
{
   // SETUP
   Move move;
   move.source.set(0, 1);
   move.dest.set(0, 2);
   move.capture = SPACE;
   move.promote = SPACE;
   move.isWhite = true;
   move.moveType = Move::MOVE;
   Board board(nullptr, true /*noreset*/);
   board.numMoves = 10;
   board.board[0][1] = new PieceSpy(0, 1, true  /*isWhite*/, PAWN);
   board.board[0][2] = new PieceSpy(0, 2, false /*isWhite*/, SPACE);
   board.board[0][1]->nMoves = 0;
   PieceSpy::reset();
   
   // EXERCISE
   board.move(move);
   
   // VERIFY
   assertUnit(11 == board.numMoves);
   assertUnit(SPACE == (board.board[0][1])->getType());
   assertUnit(PAWN == (board.board[0][2])->getType());
   assertUnit(PieceSpy::numConstruct == 0);
   assertUnit(PieceSpy::numCopy == 0);
   assertUnit(PieceSpy::numDelete == 0);
   assertUnit(PieceSpy::numAssign == 0);
   assertUnit(PieceSpy::numMove == 0);
   
   // TEARDOWN
   delete board.board[0][2];
   delete board.board[0][1];
   board.board[0][2] = board.board[0][1] = nullptr;
}


/********************************************************
 *     a6b7r
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 * |                     |       |                     |
 * 8                     8       8                     8
 * 7     R               7       7    (p)              7
 * 6  (p)                6       6   .                 6
 * 5                     5       5                     5
 * 4                     4  -->  4                     4
 * 3                     3       3                     3
 * 2                     2       2                     2
 * 1                     1       1                     1
 * |                     |       |                     |
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 ********************************************************/
void TestBoard::move_pawnCapture()
{
   // SETUP
   Move move;
   move.source.set(0, 5);
   move.dest.set(1, 6);
   move.capture = ROOK;
   move.promote = SPACE;
   move.isWhite = true;
   move.moveType = Move::MOVE;
   Board board(nullptr, true /*noreset*/);
   board.numMoves = 10;
   board.board[0][5] = new PieceSpy(0, 5, true  /*isWhite*/, PAWN);
   board.board[1][6] = new PieceSpy(1, 6, false /*isWhite*/, ROOK);
   board.board[0][5]->nMoves = 0;
   PieceSpy::reset();
   
   // EXERCISE
   board.move(move);
   
   // VERIFY
   assertUnit(11 == board.numMoves);
   assertUnit(SPACE == (board.board[0][5])->getType());
   assertUnit(PAWN == (board.board[1][6])->getType());
   assertUnit(PieceSpy::numConstruct == 0);
   assertUnit(PieceSpy::numCopy == 0);
   assertUnit(PieceSpy::numDelete == 0);
   assertUnit(PieceSpy::numAssign == 0);
   assertUnit(PieceSpy::numMove == 0);
   
   // TEARDOWN
   delete board.board[1][6];
   delete board.board[0][5];
   board.board[1][6] = board.board[0][5] = nullptr;
}


/********************************************************
 *    e2e4
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 * |                     |       |                     |
 * 8                     8       8                     8
 * 7                     7       7                     7
 * 6                     6       6                     6
 * 5                     5       5                     5
 * 4           .         4  -->  4          (p)        4
 * 3                     3       3                     3
 * 2          (p)        2       2           .         2
 * 1                     1       1                     1
 * |                     |       |                     |
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 ********************************************************/
void TestBoard::move_pawnDouble()
{
   // SETUP
   Move move;
   move.source.set(4, 1);
   move.dest.set(4, 3);
   move.capture = SPACE;
   move.promote = SPACE;
   move.isWhite = true;
   move.moveType = Move::MOVE;
   Board board(nullptr, true /*noreset*/);
   board.numMoves = 10;
   board.board[4][1] = new PieceSpy(4, 1, true  /*isWhite*/, PAWN);
   board.board[4][3] = new PieceSpy(4, 3, false /*isWhite*/, SPACE);
   board.board[4][1]->nMoves = 0;
   PieceSpy::reset();
   
   // EXERCISE
   board.move(move);
   
   // VERIFY
   assertUnit(11 == board.numMoves);
   assertUnit(SPACE == (board.board[4][1])->getType());
   assertUnit(PAWN == (board.board[4][3])->getType());
   assertUnit(PieceSpy::numConstruct == 0);
   assertUnit(PieceSpy::numCopy == 0);
   assertUnit(PieceSpy::numDelete == 0);
   assertUnit(PieceSpy::numAssign == 0);
   assertUnit(PieceSpy::numMove == 0);
   
   // TEARDOWN
   delete board.board[4][3];
   delete board.board[4][1];
   board.board[4][3] = board.board[4][1] = nullptr;
}


/********************************************************
 *     a5b6E
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 * |                     |       |                     |
 * 8                     8       8                     8
 * 7                     7       7                     7
 * 6     .               6       6     p               6
 * 5  (p)P               5       5   . .               5
 * 4                     4  -->  4                     4
 * 3                     3       3                     3
 * 2                     2       2                     2
 * 1                     1       1                     1
 * |                     |       |                     |
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 ********************************************************/
void TestBoard::move_pawnEnpassant()
{
   // SETUP
   Move move;
   move.source.set(0, 5);
   move.dest.set(1, 6);
   move.capture = PAWN;
   move.promote = SPACE;
   move.isWhite = true;
   move.moveType = Move::ENPASSANT;
   Board board(nullptr, true /*noreset*/);
   board.numMoves = 10;
   board.board[0][5] = new PieceSpy(0, 5, true  /*isWhite*/, PAWN);
   board.board[1][6] = new PieceSpy(1, 6, false /*isWhite*/, PAWN);
   board.board[0][5]->nMoves = 0;
   PieceSpy::reset();
   
   // EXERCISE
   board.move(move);
   
   // VERIFY
   assertUnit(11 == board.numMoves);
   assertUnit(SPACE == (board.board[0][5])->getType());
   assertUnit(PAWN == (board.board[1][6])->getType());
   assertUnit(PieceSpy::numConstruct == 0);
   assertUnit(PieceSpy::numCopy == 0);
   assertUnit(PieceSpy::numDelete == 0);
   assertUnit(PieceSpy::numAssign == 0);
   assertUnit(PieceSpy::numMove == 0);
   
   // TEARDOWN
   delete board.board[1][6];
   delete board.board[0][5];
   board.board[1][6] = board.board[0][5] = nullptr;
}


/********************************************************
 *    a7a8Q
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 * |                     |       |                     |
 * 8   .                 8       8  (r)                8
 * 7  (p)                7       7   .                 7
 * 6                     6       6                     6
 * 5                     5       5                     5
 * 4                     4  -->  4                     4
 * 3                     3       3                     3
 * 2                     2       2                     2
 * 1                     1       1                     1
 * |                     |       |                     |
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 ********************************************************/
void TestBoard::move_pawnPromotion()
{
   // SETUP
   Move move;
   move.source.set(0, 6);
   move.dest.set(0, 7);
   move.capture = SPACE;
   move.promote = QUEEN;
   move.isWhite = true;
   move.moveType = Move::MOVE;
   Board board(nullptr, true /*noreset*/);
   board.numMoves = 10;
   board.board[0][6] = new PieceSpy(0, 6, true  /*isWhite*/, PAWN);
   board.board[0][7] = new PieceSpy(0, 7, false /*isWhite*/, SPACE);
   board.board[0][6]->nMoves = 0;
   PieceSpy::reset();
   
   // EXERCISE
   board.move(move);
   
   // VERIFY
   assertUnit(11 == board.numMoves);
   assertUnit(SPACE == (board.board[0][6])->getType());
   assertUnit(QUEEN == (board.board[0][7])->getType());
   assertUnit(PieceSpy::numConstruct == 0);
   assertUnit(PieceSpy::numCopy == 0);
   assertUnit(PieceSpy::numDelete == 1);
   assertUnit(PieceSpy::numAssign == 0);
   assertUnit(PieceSpy::numMove == 0);
   
   // TEARDOWN
   delete board.board[0][7];
   delete board.board[0][6];
   board.board[0][7] = board.board[0][6] = nullptr;
}


/********************************************************
 *    e5a5
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 * |                     |       |                     |
 * 8                     8       8                     8
 * 7                     7       7                     7
 * 6                     6       6                     6
 * 5   .      (r)        5       5  (r)      .         5
 * 4                     4  -->  4                     4
 * 3                     3       3                     3
 * 2                     2       2                     2
 * 1                     1       1                     1
 * |                     |       |                     |
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 ********************************************************/
void TestBoard::move_rookSlide()
{
   // SETUP
   Move move;
   move.source.set(4, 4);
   move.dest.set(0, 4);
   move.capture = SPACE;
   move.promote = SPACE;
   move.isWhite = true;
   move.moveType = Move::MOVE;
   Board board(nullptr, true /*noreset*/);
   board.numMoves = 10;
   board.board[4][4] = new PieceSpy(4, 4, true  /*isWhite*/, ROOK);
   board.board[0][4] = new PieceSpy(0, 4, false /*isWhite*/, SPACE);
   PieceSpy::reset();
   
   // EXERCISE
   board.move(move);
   
   // VERIFY
   assertUnit(11 == board.numMoves);
   assertUnit(SPACE == (board.board[4][4])->getType());
   assertUnit(ROOK == (board.board[0][4])->getType());
   assertUnit(PieceSpy::numConstruct == 0);
   assertUnit(PieceSpy::numCopy == 0);
   assertUnit(PieceSpy::numDelete == 0);
   assertUnit(PieceSpy::numAssign == 0);
   assertUnit(PieceSpy::numMove == 0);
   
   // TEARDOWN
   delete board.board[0][4];
   delete board.board[4][4];
   board.board[0][4] = board.board[4][4] = nullptr;
}


/********************************************************
 *    e5a5b
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 * |                     |       |                     |
 * 8                     8       8                     8
 * 7                     7       7                     7
 * 6                     6       6                     6
 * 5   B      (r)        5       5  (r)      .         5
 * 4                     4  -->  4                     4
 * 3                     3       3                     3
 * 2                     2       2                     2
 * 1                     1       1                     1
 * |                     |       |                     |
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 ********************************************************/
void TestBoard::move_rookAttack()
{
   // SETUP
   Move move;
   move.source.set(4, 4);
   move.dest.set(0, 4);
   move.capture = BISHOP;
   move.promote = SPACE;
   move.isWhite = true;
   move.moveType = Move::MOVE;
   Board board(nullptr, true /*noreset*/);
   board.numMoves = 10;
   board.board[4][4] = new PieceSpy(4, 4, true  /*isWhite*/, ROOK);
   board.board[0][4] = new PieceSpy(0, 4, false /*isWhite*/, BISHOP);
   PieceSpy::reset();
   
   // EXERCISE
   board.move(move);
   
   // VERIFY
   assertUnit(11 == board.numMoves);
   assertUnit(SPACE == (board.board[4][4])->getType());
   assertUnit(ROOK == (board.board[0][4])->getType());
   assertUnit(PieceSpy::numConstruct == 0);
   assertUnit(PieceSpy::numCopy == 0);
   assertUnit(PieceSpy::numDelete == 1);
   assertUnit(PieceSpy::numAssign == 0);
   assertUnit(PieceSpy::numMove == 0);
   
   // TEARDOWN
   delete board.board[0][4];
   delete board.board[4][4];
   board.board[0][4] = board.board[4][4] = nullptr;
}


/********************************************************
 *    e5g3
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 * |                     |       |                     |
 * 8                     8       8                     8
 * 7                     7       7                     7
 * 6                     6       6                     6
 * 5          (b)        5       5           .         5
 * 4                     4  -->  4                     4
 * 3               .     3       3               b     3
 * 2                     2       2                     2
 * 1                     1       1                     1
 * |                     |       |                     |
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 ********************************************************/
void TestBoard::move_bishopSlide()
{
   // SETUP
   Move move;
   move.source.set(4, 4);
   move.dest.set(6, 2);
   move.capture = SPACE;
   move.promote = SPACE;
   move.isWhite = true;
   move.moveType = Move::MOVE;
   Board board(nullptr, true /*noreset*/);
   board.numMoves = 10;
   board.board[4][4] = new PieceSpy(4, 4, true  /*isWhite*/, BISHOP);
   board.board[6][2] = new PieceSpy(6, 2, false /*isWhite*/, SPACE);
   PieceSpy::reset();
   
   // EXERCISE
   board.move(move);
   
   // VERIFY
   assertUnit(11 == board.numMoves);
   assertUnit(SPACE == (board.board[4][4])->getType());
   assertUnit(BISHOP == (board.board[6][2])->getType());
   assertUnit(PieceSpy::numConstruct == 0);
   assertUnit(PieceSpy::numCopy == 0);
   assertUnit(PieceSpy::numDelete == 0);
   assertUnit(PieceSpy::numAssign == 0);
   assertUnit(PieceSpy::numMove == 0);
   
   // TEARDOWN
   delete board.board[6][2];
   delete board.board[4][4];
   board.board[6][2] = board.board[4][4] = nullptr;
}


/********************************************************
 *    e5g3q
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 * |                     |       |                     |
 * 8                     8       8                     8
 * 7                     7       7                     7
 * 6                     6       6                     6
 * 5          (b)        5       5           .         5
 * 4                     4  -->  4                     4
 * 3               Q     3       3               b     3
 * 2                     2       2                     2
 * 1                     1       1                     1
 * |                     |       |                     |
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 ********************************************************/
void TestBoard::move_bishopAttack()
{
   // SETUP
   Move move;
   move.source.set(4, 4);
   move.dest.set(6, 2);
   move.capture = QUEEN;
   move.promote = SPACE;
   move.isWhite = true;
   move.moveType = Move::MOVE;
   Board board(nullptr, true /*noreset*/);
   board.numMoves = 10;
   board.board[4][4] = new PieceSpy(4, 4, true  /*isWhite*/, BISHOP);
   board.board[6][2] = new PieceSpy(6, 2, false /*isWhite*/, QUEEN);
   PieceSpy::reset();
   
   // EXERCISE
   board.move(move);
   
   // VERIFY
   assertUnit(11 == board.numMoves);
   assertUnit(SPACE == (board.board[4][4])->getType());
   assertUnit(BISHOP == (board.board[6][2])->getType());
   assertUnit(PieceSpy::numConstruct == 0);
   assertUnit(PieceSpy::numCopy == 0);
   assertUnit(PieceSpy::numDelete == 1);
   assertUnit(PieceSpy::numAssign == 0);
   assertUnit(PieceSpy::numMove == 0);
   
   // TEARDOWN
   delete board.board[6][2];
   delete board.board[4][4];
   board.board[6][2] = board.board[4][4] = nullptr;
}

/********************************************************
 *    e5g3
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 * |                     |       |                     |
 * 8                     8       8                     8
 * 7                     7       7                     7
 * 6                     6       6                     6
 * 5          (q)        5       5                     5
 * 4                     4  -->  4           .         4
 * 3                .    3       3               q     3
 * 2                     2       2                     2
 * 1                     1       1                     1
 * |                     |       |                     |
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 ********************************************************/
void TestBoard::move_queenSlide()
{
   // SETUP
   Move move;
   move.source.set(4, 4);
   move.dest.set(6, 2);
   move.capture = SPACE;
   move.promote = SPACE;
   move.isWhite = true;
   move.moveType = Move::MOVE;
   Board board(nullptr, true /*noreset*/);
   board.numMoves = 10;
   board.board[4][4] = new PieceSpy(4, 4, true  /*isWhite*/, QUEEN);
   board.board[6][2] = new PieceSpy(6, 2, false /*isWhite*/, SPACE);
   PieceSpy::reset();
   
   // EXERCISE
   board.move(move);
   
   // VERIFY
   assertUnit(11 == board.numMoves);
   assertUnit(SPACE == (board.board[4][4])->getType());
   assertUnit(QUEEN == (board.board[6][2])->getType());
   assertUnit(PieceSpy::numConstruct == 0);
   assertUnit(PieceSpy::numCopy == 0);
   assertUnit(PieceSpy::numDelete == 0);
   assertUnit(PieceSpy::numAssign == 0);
   assertUnit(PieceSpy::numMove == 0);
   
   // TEARDOWN
   delete board.board[6][2];
   delete board.board[4][4];
   board.board[6][2] = board.board[4][4] = nullptr;
}


/********************************************************
 *    e5a5b
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 * |                     |       |                     |
 * 8                     8       8                     8
 * 7                     7       7                     7
 * 6                     6       6                     6
 * 5   B      (q)        5       5  (q)      .         5
 * 4                     4  -->  4                     4
 * 3                     3       3                     3
 * 2                     2       2                     2
 * 1                     1       1                     1
 * |                     |       |                     |
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 ********************************************************/
void TestBoard::move_queenAttack()
{
   // SETUP
   Move move;
   move.source.set(4, 4);
   move.dest.set(0, 4);
   move.capture = BISHOP;
   move.promote = SPACE;
   move.isWhite = true;
   move.moveType = Move::MOVE;
   Board board(nullptr, true /*noreset*/);
   board.numMoves = 10;
   board.board[4][4] = new PieceSpy(4, 4, true  /*isWhite*/, QUEEN);
   board.board[0][4] = new PieceSpy(0, 4, false /*isWhite*/, BISHOP);
   PieceSpy::reset();
   
   // EXERCISE
   board.move(move);
   
   // VERIFY
   assertUnit(11 == board.numMoves);
   assertUnit(SPACE == (board.board[4][4])->getType());
   assertUnit(QUEEN == (board.board[0][4])->getType());
   assertUnit(PieceSpy::numConstruct == 0);
   assertUnit(PieceSpy::numCopy == 0);
   assertUnit(PieceSpy::numDelete == 1);
   assertUnit(PieceSpy::numAssign == 0);
   assertUnit(PieceSpy::numMove == 0);
   
   // TEARDOWN
   delete board.board[0][4];
   delete board.board[4][4];
   board.board[0][4] = board.board[4][4] = nullptr;
}


/********************************************************
 *  e1f1
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 * |                     |       |                     |
 * 8                     8       8                     8
 * 7                     7       7                     7
 * 6                     6       6                     6
 * 5                     5       5                     5
 * 4                     4  -->  4                     4
 * 3                     3       3                     3
 * 2                     2       2                     2
 * 1          (k).       1       1           . k       1
 * |                     |       |                     |
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 ********************************************************/
void TestBoard::move_kingMove()
{
   // SETUP
   Move move;
   move.source.set(4, 0);
   move.dest.set(5, 0);
   move.capture = SPACE;
   move.promote = SPACE;
   move.isWhite = true;
   move.moveType = Move::MOVE;
   Board board(nullptr, true /*noreset*/);
   board.numMoves = 10;
   board.board[4][0] = new PieceSpy(4, 0, true  /*isWhite*/, KING);
   board.board[5][0] = new PieceSpy(5, 0, false /*isWhite*/, SPACE);
   PieceSpy::reset();
   
   // EXERCISE
   board.move(move);
   
   // VERIFY
   assertUnit(11 == board.numMoves);
   assertUnit(SPACE == (board.board[4][0])->getType());
   assertUnit(KING == (board.board[5][0])->getType());
   assertUnit(PieceSpy::numConstruct == 0);
   assertUnit(PieceSpy::numCopy == 0);
   assertUnit(PieceSpy::numDelete == 0);
   assertUnit(PieceSpy::numAssign == 0);
   assertUnit(PieceSpy::numMove == 0);
   
   // TEARDOWN
   delete board.board[4][0];
   delete board.board[5][0];
   board.board[4][0] = board.board[5][0] = nullptr;
}


/********************************************************
 *    e1f1r
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 * |                     |       |                     |
 * 8                     8       8                     8
 * 7                     7       7                     7
 * 6                     6       6                     6
 * 5                     5       5                     5
 * 4                     4  -->  4                     4
 * 3                     3       3                     3
 * 2                     2       2                     2
 * 1          (k)R       1       1           . k       1
 * |                     |       |                     |
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 ********************************************************/
void TestBoard::move_kingAttack()
{
   // SETUP
   Move move;
   move.source.set(4, 0);
   move.dest.set(5, 0);
   move.capture = ROOK;
   move.promote = SPACE;
   move.isWhite = true;
   move.moveType = Move::MOVE;
   Board board(nullptr, true /*noreset*/);
   board.numMoves = 10;
   board.board[4][0] = new PieceSpy(4, 0, true  /*isWhite*/, KING);
   board.board[5][0] = new PieceSpy(5, 0, false /*isWhite*/, ROOK);
   PieceSpy::reset();
   
   // EXERCISE
   board.move(move);
   
   // VERIFY
   assertUnit(11 == board.numMoves);
   assertUnit(SPACE == (board.board[4][0])->getType());
   assertUnit(KING == (board.board[5][0])->getType());
   assertUnit(PieceSpy::numConstruct == 0);
   assertUnit(PieceSpy::numCopy == 0);
   assertUnit(PieceSpy::numDelete == 1);
   assertUnit(PieceSpy::numAssign == 0);
   assertUnit(PieceSpy::numMove == 0);
   
   // TEARDOWN
   delete board.board[4][0];
   delete board.board[5][0];
   board.board[4][0] = board.board[5][0] = nullptr;
}

/********************************************************
 *    e1g1c
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 * |                     |       |                     |
 * 8                     8       8                     8
 * 7                     7       7                     7
 * 6                     6       6                     6
 * 5                     5       5                     5
 * 4                     4  -->  4                     4
 * 3                     3       3                     3
 * 2                     2       2                     2
 * 1          (k). . r   1       1           . r k .   1
 * |                     |       |                     |
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 ********************************************************/
void TestBoard::move_kingShortCastle()
{
   // SETUP
   Move move;
   move.source.set(4, 0);
   move.dest.set(6, 0);
   move.capture = SPACE;
   move.promote = SPACE;
   move.isWhite = true;
   move.moveType = Move::CASTLE_KING;
   Board board(nullptr, true /*noreset*/);
   board.numMoves = 10;
   board.board[4][0] = new PieceSpy(4, 0, true  /*isWhite*/, KING);
   board.board[7][0] = new PieceSpy(7, 0, true /*isWhite*/, ROOK);
   board.board[4][0]->nMoves = 0;
   board.board[7][0]->nMoves = 0;
   PieceSpy::reset();
   
   // EXERCISE
   board.move(move);
   
   // VERIFY
   assertUnit(11 == board.numMoves);
   assertUnit(SPACE == board.board[4][0]->getType()); // KING original square
   assertUnit(SPACE == board.board[7][0]->getType()); // ROOK original square
   assertUnit(KING == board.board[6][0]->getType()); // KING destination g1
   assertUnit(ROOK == board.board[5][0]->getType()); // ROOK destination f1
   assertUnit(PieceSpy::numConstruct == 0);
   assertUnit(PieceSpy::numCopy == 0);
   assertUnit(PieceSpy::numDelete == 0);
   assertUnit(PieceSpy::numAssign == 0);
   assertUnit(PieceSpy::numMove == 0);
   
   // TEARDOWN
   delete board.board[5][0];
   delete board.board[6][0];
   board.board[5][0] = board.board[6][0] = nullptr;
   
}



/********************************************************
 *    e1c1C
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 * |                     |       |                     |
 * 8                     8       8                     8
 * 7                     7       7                     7
 * 6                     6       6                     6
 * 5                     5       5                     5
 * 4                     4  -->  4                     4
 * 3                     3       3                     3
 * 2                     2       2                     2
 * 1   r . . .(k)        1       1   . . k r .         1
 * |                     |       |                     |
 * +---a-b-c-d-e-f-g-h---+       +---a-b-c-d-e-f-g-h---+
 ********************************************************/
void TestBoard::move_kingLongCastle()
{
   // SETUP
   Move move;
   move.source.set(4, 0);
   move.dest.set(2, 0);
   move.capture = SPACE;
   move.promote = SPACE;
   move.isWhite = true;
   move.moveType = Move::CASTLE_QUEEN;
   Board board(nullptr, true /*noreset*/);
   board.numMoves = 10;
   board.board[4][0] = new PieceSpy(4, 0, true  /*isWhite*/, KING);
   board.board[0][0] = new PieceSpy(0, 0, true /*isWhite*/, ROOK);
   board.board[4][0]->nMoves = 0;
   board.board[0][0]->nMoves = 0;
   PieceSpy::reset();
   
   // EXERCISE
   board.move(move);
   
   // VERIFY
   assertUnit(11 == board.numMoves);
   assertUnit(SPACE == board.board[4][0]->getType()); // KING original square
   assertUnit(SPACE == board.board[0][0]->getType()); // ROOK original square
   assertUnit(KING == board.board[2][0]->getType()); // KING destination c1
   assertUnit(ROOK == board.board[3][0]->getType()); // ROOK destination d1
   assertUnit(PieceSpy::numConstruct == 0);
   assertUnit(PieceSpy::numCopy == 0);
   assertUnit(PieceSpy::numDelete == 0);
   assertUnit(PieceSpy::numAssign == 0);
   assertUnit(PieceSpy::numMove == 0);
   
   // TEARDOWN
   delete board.board[3][0];
   delete board.board[2][0];
   board.board[3][0] = board.board[2][0] = nullptr;
}




/********************************************************
 *        +---a-b-c-d-e-f-g-h---+
 *        |                     |
 *        8   R N B Q K B N R   8
 *        7   P P P P P P P P   7
 *        6                     6
 *        5                     5
 *   -->  4                     4
 *        3                     3
 *        2   p p p p p p p p   2
 *        1   r n b q k b n r   1
 *        |                     |
 *        +---a-b-c-d-e-f-g-h---+
 ********************************************************/
void TestBoard::construct_default()
{
   // SETUP
   Board board(nullptr, false /*noreset*/); // Use false to initialize default board
   PieceSpy::reset();
   
   // EXERCISE
   // (No moves, just check initial state)
   
   // VERIFY
   // Check white pieces (bottom two rows)
   assertUnit(board.board[0][0]->getType() == ROOK && board.board[0][0]->isWhite());
   assertUnit(board.board[1][0]->getType() == KNIGHT && board.board[1][0]->isWhite());
   assertUnit(board.board[2][0]->getType() == BISHOP && board.board[2][0]->isWhite());
   assertUnit(board.board[3][0]->getType() == QUEEN && board.board[3][0]->isWhite());
   assertUnit(board.board[4][0]->getType() == KING && board.board[4][0]->isWhite());
   assertUnit(board.board[5][0]->getType() == BISHOP && board.board[5][0]->isWhite());
   assertUnit(board.board[6][0]->getType() == KNIGHT && board.board[6][0]->isWhite());
   assertUnit(board.board[7][0]->getType() == ROOK && board.board[7][0]->isWhite());
   for (int i = 0; i < 8; i++)
      assertUnit(board.board[i][1]->getType() == PAWN && board.board[i][1]->isWhite());
   
   // Check black pieces (top two rows)
   assertUnit(board.board[0][7]->getType() == ROOK && !board.board[0][7]->isWhite());
   assertUnit(board.board[1][7]->getType() == KNIGHT && !board.board[1][7]->isWhite());
   assertUnit(board.board[2][7]->getType() == BISHOP && !board.board[2][7]->isWhite());
   assertUnit(board.board[3][7]->getType() == QUEEN && !board.board[3][7]->isWhite());
   assertUnit(board.board[4][7]->getType() == KING && !board.board[4][7]->isWhite());
   assertUnit(board.board[5][7]->getType() == BISHOP && !board.board[5][7]->isWhite());
   assertUnit(board.board[6][7]->getType() == KNIGHT && !board.board[6][7]->isWhite());
   assertUnit(board.board[7][7]->getType() == ROOK && !board.board[7][7]->isWhite());
   for (int i = 0; i < 8; i++)
      assertUnit(board.board[i][6]->getType() == PAWN && !board.board[i][6]->isWhite());
   
   // Check empty squares
   for (int x = 0; x < 8; x++)
      for (int y = 2; y <= 5; y++)
         assertUnit(board.board[x][y]->getType() == SPACE);
   
   // PieceSpy checks
   assertUnit(PieceSpy::numCopy == 0);
   assertUnit(PieceSpy::numDelete == 0);
   assertUnit(PieceSpy::numAssign == 0);
   assertUnit(PieceSpy::numMove == 0);
}
