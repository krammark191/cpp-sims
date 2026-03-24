/***********************************************************************
 * Source File:
 *    POSITION
 * Author:
 *    Mark Van Horn & Taden Marston
 * Summary:
 *    The position of a piece, the cursor, or a possible move on a chess board
 ************************************************************************/

#include "position.h"
#include <iostream>

/******************************************
 * POSITION INSERTION OPERATOR
 ******************************************/
ostream& operator << (ostream& out, const Position& rhs)
{
   if (rhs.isValid())
      out << (char)(rhs.getCol() + 'a')
      << (char)(rhs.getRow() + '1');
   else
      out << "error";
   return out;
}

/*************************************
 * POSITION EXTRACTION OPERATOR
 **************************************/
istream& operator >> (istream& in, Position& rhs)
{
   char text[3] = {};
   in >> text[0] >> text[1];
   if (in.fail())
   {
      in.clear();
      in.ignore();
      throw string("Error reading coordinates");
   }
   else
      rhs = text;
   
   return in;
}

