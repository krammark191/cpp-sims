/***********************************************************************
 * Header File:
 *    MOVE
 * Author:
 *   Taden Marston & Mark Van Horn
 * Summary:
 *    Everything we need to know about a single chess move
 ************************************************************************/

#pragma once

#include <string>
#include <set>
#include "position.h"  // Every move has two Positions as attributes
#include "pieceType.h" // A piece type


class TestMove;
class TestBoard;

/***************************************************
 * MOVE
 * One move across the board
 ***************************************************/
class Move
{
public:
   enum MoveType { MOVE, ENPASSANT, CASTLE_KING, CASTLE_QUEEN, MOVE_ERROR };
   
   friend TestMove;
   friend TestBoard;
   
   // constructor
   Move();
   Move(const string& rhs);
   Move(const Position& source,
        const Position& dest,
        const std::set <Move>& possible); // Added constructor
   bool operator<(const Move& rhs) const;
   bool operator==(const Move& rhs) const;
   void read(const string& rhs);
   string getText() const;
   
   // getters
   Position getSource() const { return source; }
   Position getDest()   const { return dest;   }
   PieceType getPromote() const { return promote; }
   PieceType getCapture() const { return capture; }
   MoveType  getMoveType() const { return moveType; }
   bool      isWhiteTurn() const { return isWhite; }
   
   // setters
   void setSource(const Position& pos) { source = pos; }
   void setDest(const Position& pos)   { dest = pos;   }
   void setPromote(PieceType pt)       { promote = pt; }
   void setCapture(PieceType pt)       { capture = pt; }
   void setMoveType(MoveType mt)       { moveType = mt; }
   void setWhiteTurn(bool isWhite)     { this->isWhite = isWhite; }
   
   
private:
   char letterFromPieceType(PieceType pt)     const;
   PieceType pieceTypeFromLetter(char letter) const;
   
   
   
   Position  source;    // where the move originated from
   Position  dest;      // where the move finished
   PieceType promote;   // piece to be promoted to
   PieceType capture;   // did a capture happen this move?
   MoveType  moveType;  // what type of move is this?
   bool      isWhite;   // whose turn is it anyway?
   string    text;      // what is the textual version of the move?
};
