/***********************************************************************
 * Header File:
 *    TEST MOVE
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    test the Move class
 ************************************************************************/


#include "testMove.h"
#include "move.h"
#include <cassert>

 /*************************************
 * Constructor : default
 * Input:
 * Output: source=INVALID
 *         dest  =INVALID
 **************************************/
void TestMove::constructor_default()
{
   // Set Up
   Move move;

   // Act & Assert
   assertUnit(move.source.isInvalid());
   assertUnit(move.dest.isInvalid());
}

/*************************************
 * CONSTRUCTOR : standard string move
 * Input:  e5e6
 * Output: source=4,4
 *         dest  =4,5
 *         type  =MOVE
 **************************************/
void TestMove::constructString_simple()
{
   // SETUP
   string text = "e5e6";

   // EXERCISE
   Move move(text);

   // VERIFY
   assertUnit(move.source.getCol() == 4);
   assertUnit(move.source.getRow() == 4);
   assertUnit(move.dest.getCol() == 4);
   assertUnit(move.dest.getRow() == 5);
   assertUnit(move.moveType == Move::MOVE);
}  // Teardown


/*************************************
 * READ simple move
 * Input:  e5d6
 * Output: source=4,4
 *         dest  =3,5
 *         type  =MOVE
 **************************************/
void TestMove::read_simple()
{
   // SETUP
   string text = "e5d6";

   // EXERCISE
   Move move(text);

   // VERIFY
   assertUnit(move.source.getCol() == 4);
   assertUnit(move.source.getRow() == 4);
   assertUnit(move.dest.getCol() == 3);
   assertUnit(move.dest.getRow() == 5);
   assertUnit(move.moveType == Move::MOVE);
}  // Teardown

/*************************************
 * READ capture move
 * Input:  e5d6r
 * Output: source=4,4
 *         dest  =3,5
 *         type  =MOVE
 *         capture=ROOK
 **************************************/
void TestMove::read_capture()
{
   // SETUP
   string text = "e5d6r";

   // EXERCISE
   Move move(text);

   // VERIFY
   assertUnit(move.source.getCol() == 4);
   assertUnit(move.source.getRow() == 4);
   assertUnit(move.dest.getCol() == 3);
   assertUnit(move.dest.getRow() == 5);
   assertUnit(move.moveType == Move::MOVE);
   assertUnit(move.capture == PieceType::ROOK);
}  // Teardown

/*************************************
 * READ enpassant move
 * Input:  e5d6E
 * Output: source=4,4
 *         dest  =3,5
 *         type  =ENPASSANT
 **************************************/
void TestMove::read_enpassant()
{
   // SETUP
   string text = "e5d6E";

   // EXERCISE
   Move move(text);

   // VERIFY
   assertUnit(move.source.getCol() == 4);
   assertUnit(move.source.getRow() == 4);
   assertUnit(move.dest.getCol() == 3);
   assertUnit(move.dest.getRow() == 5);
   assertUnit(move.moveType == Move::ENPASSANT);
}  // Teardown

/*************************************
 * READ king side castle
 * Input:  e1g1c
 * Output: source=4,0
 *         dest  =6,0
 *         type  =CASTLE_KING
 **************************************/
void TestMove::read_castleKing()
{
   // SETUP
   string text = "e1g1c";

   // EXERCISE
   Move move(text);

   // VERIFY
   assertUnit(move.source.getCol() == 4);
   assertUnit(move.source.getRow() == 0);
   assertUnit(move.dest.getCol() == 6);
   assertUnit(move.dest.getRow() == 0);
   assertUnit(move.moveType == Move::CASTLE_KING);
}  // Teardown


/*************************************
 * READ queen side castle
 * Input:  e1c1C
 * Output: source=4,0
 *         dest  =2,0
 *         type  =CASTLE_QUEEN
 **************************************/
void TestMove::read_castleQueen()
{
   // SETUP
   string text = "e1c1C";

   // EXERCISE
   Move move(text);

   // VERIFY
   assertUnit(move.source.getCol() == 4);
   assertUnit(move.source.getRow() == 0);
   assertUnit(move.dest.getCol() == 2);
   assertUnit(move.dest.getRow() == 0);
   assertUnit(move.moveType == Move::CASTLE_QUEEN);
}  // Teardown

/*************************************
 * ASSIGN simple move
 * Input:  e1c1C
 * Output: source=4,0
 *         dest  =2,0
 *         type  =CASTLE_QUEEN
 **************************************/
void TestMove::assign_simple()
{
   // SETUP
   string text = "e1c1C";

   // EXERCISE
   Move move(text);

   // VERIFY
   assertUnit(move.source.getCol() == 4);
   assertUnit(move.source.getRow() == 0);
   assertUnit(move.dest.getCol() == 2);
   assertUnit(move.dest.getRow() == 0);
   assertUnit(move.moveType == Move::CASTLE_QUEEN);
}  // Teardown

/*************************************
 * ASSIGN capture move
 * Input:  e5d6r
 * Output: source=4,4
 *         dest  =5,5
 *         type  =MOVE
 *         capture=ROOK
 **************************************/
void TestMove::assign_capture()
{
   // SETUP
   string text = "e5d6r";

   // EXERCISE
   Move move(text);

   // VERIFY
   assertUnit(move.source.getCol() == 4);
   assertUnit(move.source.getRow() == 4);
   assertUnit(move.dest.getCol() == 3);
   assertUnit(move.dest.getRow() == 5);
   assertUnit(move.moveType == Move::MOVE);
   assertUnit(move.capture == PieceType::ROOK);
}  // Teardown

/*************************************
 * ASSIGN enpassant move
 * Input:  e5d6E
 * Output: source=4,4
 *         dest  =3,5
 *         type  =ENPASSANT
 **************************************/
void TestMove::assign_enpassant()
{
   // SETUP
   string text = "e5d6E";

   // EXERCISE
   Move move(text);

   // VERIFY
   assertUnit(move.source.getCol() == 4);
   assertUnit(move.source.getRow() == 4);
   assertUnit(move.dest.getCol() == 3);
   assertUnit(move.dest.getRow() == 5);
   assertUnit(move.moveType == Move::ENPASSANT);
}  // Teardown

/*************************************
 * ASSIGN king side castle
 * Input:  e1g1c
 * Output: source=4,0
 *         dest  =6,0
 *         type  =CASTLE_KING
 **************************************/
void TestMove::assign_castleKing()
{
   // SETUP
   string text = "e1g1c";

   // EXERCISE
   Move move(text);

   // VERIFY
   assertUnit(move.source.getCol() == 4);
   assertUnit(move.source.getRow() == 0);
   assertUnit(move.dest.getCol() == 6);
   assertUnit(move.dest.getRow() == 0);
   assertUnit(move.moveType == Move::CASTLE_KING);
}  // Teardown

/*************************************
 * ASSIGN queen side castle
 * Input:  e1c1C
 * Output: source=4,0
 *         dest  =2,0
 *         type  =CASTLE_QUEEN
 **************************************/
void TestMove::assign_castleQueen()
{
   // SETUP
   string text = "e1c1C";

   // EXERCISE
   Move move(text);

   // VERIFY
   assertUnit(move.source.getCol() == 4);
   assertUnit(move.source.getRow() == 0);
   assertUnit(move.dest.getCol() == 2);
   assertUnit(move.dest.getRow() == 0);
   assertUnit(move.moveType == Move::CASTLE_QUEEN);
}  // Teardown

/*************************************
 * GET TEXT simple move
 * Input : source=4,4
 *         dest  =4,5
 *         type  =MOVE
 * Input:  e5e6
 **************************************/
void TestMove::getText_simple()
{
   // SETUP
   Move move;
   move.source = Position(4, 4);
   move.dest = Position(4, 5);
   move.moveType = Move::MOVE;

   // EXERCISE
   std::string text = move.getText();

   // VERIFY
   assertUnit(text == "e5e6");
} // Teardown

/*************************************
 * GET TEXT capture
 * Input : source=4,4
 *         dest  =4,5
 *         type  =MOVE
 *         capture=ROOK
 * Input:  e5e6r
 **************************************/
void TestMove::getText_capture()
{
   // SETUP
   Move move;
   move.source = Position(4, 4);
   move.dest = Position(4, 5);
   move.moveType = Move::MOVE;
   move.capture = PieceType::ROOK;

   // EXERCISE
   std::string text = move.getText();

   // VERIFY
   assertUnit(text == "e5e6r");
} // Teardown

/*************************************
 * GET TEXT en passant
 * Input : source=4,4
 *         dest  =5,5
 *         type  =ENPASSANT
 *         capture=PAWN
 * Input:  e5f6E
 **************************************/
void TestMove::getText_enpassant()
{
   // SETUP
   Move move;
   move.source = Position(4, 4);
   move.dest = Position(5, 5);
   move.moveType = Move::ENPASSANT;
   move.capture = PieceType::PAWN;

   // EXERCISE
   std::string text = move.getText();

   // VERIFY
   assertUnit(text == "e5f6E");
} // Teardown

/*************************************
 * GET TEXT king side castle
 * Input : source=4,0
 *         dest  =6,0
 *         type  =CASTLE_KING
 * Input:  e1g1c
 **************************************/
void TestMove::getText_castleKing()
{
   // SETUP
   Move move;
   move.source = Position(4, 0);
   move.dest = Position(6, 0);
   move.moveType = Move::CASTLE_KING;

   // EXERCISE
   std::string text = move.getText();

   // VERIFY
   assertUnit(text == "e1g1c");
} // Teardown

/*************************************
 * GET TEXT queen side castle
 * Input : source=4,0
 *         dest  =2,0
 *         type  =CASTLE_QUEEN
 * Input:  e1c1C
 **************************************/
void TestMove::getText_castleQueen()
{
   // SETUP
   Move move;
   move.source = Position(4, 0);
   move.dest = Position(2, 0);
   move.moveType = Move::CASTLE_QUEEN;

   // EXERCISE
   std::string text = move.getText();

   // VERIFY
   assertUnit(text == "e1c1C");
} // Teardown

/*************************************
 * LETTER FROM PIECE TYPE space
 * Input :  SPACE
 * Output:  ' '
 **************************************/
void TestMove::letterFromPieceType_space()
{
   // Set Up
   Move move;
   PieceType spacePiece = SPACE;

   // Act
   char result = move.letterFromPieceType(spacePiece);

   // Assert
   assertUnit(result == ' ');
} // TearDown

/*************************************
 * LETTER FROM PIECE TYPE pawn
 * Input : PAWN
 * Output:  'p'
 **************************************/
void TestMove::letterFromPieceType_pawn()
{
   // Set Up
   Move move;
   PieceType pawnPiece = PAWN;

   // Act
   char result = move.letterFromPieceType(pawnPiece);

   // Assert
   assertUnit(result == 'p');
}  // TearDown

/*************************************
 * LETTER FROM PIECE TYPE bishop
 * Input : BISHOP
 * Output:  'b'
 **************************************/
void TestMove::letterFromPieceType_bishop()
{
   // Set Up
   Move move;
   PieceType bishopPiece = BISHOP;

   // Act
   char result = move.letterFromPieceType(bishopPiece);

   // Assert
   assertUnit(result == 'b');
}  // TearDown

/*************************************
 * LETTER FROM PIECE TYPE knight
 * Input : KNIGHT
 * Output:  'n'
 **************************************/
void TestMove::letterFromPieceType_knight()
{
   // Set Up
   Move move;
   PieceType knightPiece = KNIGHT;

   // Act
   char result = move.letterFromPieceType(knightPiece);

   // Assert
   assertUnit(result == 'n');
}  // TearDown

/*************************************
 * LETTER FROM PIECE TYPE rook
 * Input : ROOK
 * Output:  'r'
 **************************************/
void TestMove::letterFromPieceType_rook()
{
   // Set Up
   Move move;
   PieceType rookPiece = ROOK;

   // Act
   char result = move.letterFromPieceType(rookPiece);

   // Assert
   assertUnit(result == 'r');
}  // TearDown

/*************************************
 * LETTER FROM PIECE TYPE queen
 * Input : QUEEN
 * Output:  'q'
 **************************************/
void TestMove::letterFromPieceType_queen()
{
   // Set Up
   Move move;
   PieceType queenPiece = QUEEN;

   // Act
   char result = move.letterFromPieceType(queenPiece);

   // Assert
   assertUnit(result == 'q');
}  // TearDown

/*************************************
 * LETTER FROM PIECE TYPE king
 * Input : KING
 * Output:  'k'
 **************************************/
void TestMove::letterFromPieceType_king()
{
   // Set Up
   Move move;
   PieceType kingPiece = KING;

   // Act
   char result = move.letterFromPieceType(kingPiece);

   // Assert
   assertUnit(result == 'k');
}  // TearDown

/*************************************
 * PIECE TYPE FROM LETTER pawn
 * Input : 'p'
 * Output:  PAWN
 **************************************/
void TestMove::pieceTypeFromLetter_pawn()
{
   // Set Up
   Move move;
   char pawnPiece = 'p';

   // Act
   char result = move.pieceTypeFromLetter(pawnPiece);

   // Assert
   assertUnit(result == PAWN);
}  // TearDown

/*************************************
 * PIECE TYPE FROM LETTER bishop
 * Input : 'b'
 * Output:  BISHOP
 **************************************/
void TestMove::pieceTypeFromLetter_bishop()
{
   // Set Up
   Move move;
   char bishopPiece = 'b';

   // Act
   char result = move.pieceTypeFromLetter(bishopPiece);

   // Assert
   assertUnit(result == BISHOP);
}  // TearDown

/*************************************
 * PIECE TYPE FROM LETTER knight
 * Input : 'n'
 * Input:  KNIGHT
 **************************************/
void TestMove::pieceTypeFromLetter_knight()
{
   // Set Up
   Move move;
   char bishopPiece = 'n';

   // Act
   char result = move.pieceTypeFromLetter(bishopPiece);

   // Assert
   assertUnit(result == KNIGHT);
}  // TearDown

/*************************************
 * PIECE TYPE FROM LETTER rook
 * Input : 'r'
 * Input:  ROOK
 **************************************/
void TestMove::pieceTypeFromLetter_rook()
{
   // Set Up
   Move move;
   char bishopPiece = 'r';

   // Act
   char result = move.pieceTypeFromLetter(bishopPiece);

   // Assert
   assertUnit(result == ROOK);
}  // TearDown

/*************************************
 * PIECE TYPE FROM LETTER queen
 * Input : 'q'
 * Input:  QUEEN
 **************************************/
void TestMove::pieceTypeFromLetter_queen()
{
   // Set Up
   Move move;
   char queenPiece = 'q';

   // Act
   char result = move.pieceTypeFromLetter(queenPiece);

   // Assert
   assertUnit(result == QUEEN);
}  // TearDown

/*************************************
 * PIECE TYPE FROM LETTER king
 * Input : 'k'
 * Input:  KING
 **************************************/
void TestMove::pieceTypeFromLetter_king()
{
   // Set Up
   Move move;
   char kingPiece = 'k';

   // Act
   char result = move.pieceTypeFromLetter(kingPiece);

   // Assert
   assertUnit(result == KING);
}  // TearDown