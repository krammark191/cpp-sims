/***********************************************************************
 * Source File:
 *    MOVE
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Everything we need to know about a single chess move
 ************************************************************************/

#include "move.h"
#include "pieceType.h"
#include <cassert>
#include <sstream>
#include <cctype>

using namespace std;

/***************************************************
 * MOVE : DEFAULT CONSTRUCTOR
 ***************************************************/
Move::Move()
{
   source.setInvalid();
   dest.setInvalid();
   promote = SPACE;
   capture = SPACE;
   moveType = MOVE;
   isWhite = true;
   text = "";
}

/***************************************************
 * MOVE : CONSTRUCT FROM STRING
 ***************************************************/
Move::Move(const string& rhs)
{
   read(rhs);
}

/***************************************************
 * MOVE : CONSTRUCT FROM TWO POSITIONS
 ***************************************************/
Move::Move(const Position& source, const Position& dest, const set<Move>& possible)
{
   this->source = source;
   this->dest = dest;
   this->promote = SPACE;
   this->capture = SPACE;
   this->moveType = MOVE;
   this->isWhite = true;
   this->text = "";
   
   // Search for a matching move in the possible set
   for (const Move& m : possible)
   {
      if (m.source == source && m.dest == dest)
      {
         this->moveType = m.moveType;
         this->promote = m.promote;
         this->capture = m.capture;
         this->isWhite = m.isWhite;
         this->text = m.text;
         break;
      }
   }
}

/***************************************************
 * MOVE : OPERATOR <
 ***************************************************/
bool Move::operator<(const Move& rhs) const
{
   if (source != rhs.source)
      return source < rhs.source;
   if (dest != rhs.dest)
      return dest < rhs.dest;
   if (moveType != rhs.moveType)
      return moveType < rhs.moveType;
   if (promote != rhs.promote)
      return promote < rhs.promote;
   if (capture != rhs.capture)
      return capture < rhs.capture;
   return false;
}

/***************************************************
 * MOVE : OPERATOR ==
 ***************************************************/
bool Move::operator==(const Move& rhs) const
{
   return source == rhs.source &&
   dest == rhs.dest &&
   moveType == rhs.moveType &&
   promote == rhs.promote &&
   capture == rhs.capture;
}

/***************************************************
 * MOVE : READ
 * Parse a move string (e.g., "e2e4", "e7e8q", "e5d6r", "e5d6E", "e1g1c", "e1c1C")
 ***************************************************/
void Move::read(const string& rhs)
{
   text = rhs;
   source.setInvalid();
   dest.setInvalid();
   promote = SPACE;
   capture = SPACE;
   moveType = MOVE;
   isWhite = true;
   
   if (rhs.length() < 4)
      return;
   
   // Parse source and dest
   source = rhs.substr(0, 2).c_str();
   dest = rhs.substr(2, 2).c_str();
   
   // Default move type
   moveType = MOVE;
   
   // Promotion or capture or special move
   if (rhs.length() > 4)
   {
      char code = rhs[4];
      if (code == 'E')
         moveType = ENPASSANT;
      else if (code == 'c')
         moveType = CASTLE_KING;
      else if (code == 'C')
         moveType = CASTLE_QUEEN;
      else if (islower(code))
         capture = pieceTypeFromLetter(code);
      else if (isupper(code))
         promote = pieceTypeFromLetter(tolower(code));
   }
}

/***************************************************
 * MOVE : GET TEXT
 * Convert move to string (e.g., "e2e4", "e7e8q", "e5d6r", "e5d6E", "e1g1c", "e1c1C")
 ***************************************************/
string Move::getText() const
{
   stringstream ss;
   if (!source.isValid() || !dest.isValid())
      return "";
   
   // Source and dest
   ss << static_cast<char>('a' + source.getCol())
   << static_cast<char>('1' + source.getRow())
   << static_cast<char>('a' + dest.getCol())
   << static_cast<char>('1' + dest.getRow());
   
   // Special moves
   if (moveType == ENPASSANT)
      ss << 'E';
   else if (moveType == CASTLE_KING)
      ss << 'c';
   else if (moveType == CASTLE_QUEEN)
      ss << 'C';
   else if (capture != SPACE)
      ss << letterFromPieceType(capture);
   else if (promote != SPACE)
      ss << static_cast<char>(toupper(letterFromPieceType(promote)));
   
   return ss.str();
}

/***************************************************
 * MOVE : LETTER FROM PIECE TYPE
 ***************************************************/
char Move::letterFromPieceType(PieceType pt) const
{
   switch (pt)
   {
      case SPACE:  return ' ';
      case PAWN:   return 'p';
      case BISHOP: return 'b';
      case KNIGHT: return 'n';
      case ROOK:   return 'r';
      case QUEEN:  return 'q';
      case KING:   return 'k';
      default:     return '?';
   }
}

/***************************************************
 * MOVE : PIECE TYPE FROM LETTER
 ***************************************************/
PieceType Move::pieceTypeFromLetter(char letter) const
{
   switch (tolower(letter))
   {
      case ' ': return SPACE;
      case 'p': return PAWN;
      case 'b': return BISHOP;
      case 'n': return KNIGHT;
      case 'r': return ROOK;
      case 'q': return QUEEN;
      case 'k': return KING;
      default:  return SPACE;
   }
}
