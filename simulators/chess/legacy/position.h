/***********************************************************************
 * Header File:
 *    POSITION
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    The position of a piece, the cursor, or a possible move on a chess board
 ************************************************************************/

#pragma once

#include <string>
#include <cstdint>
using std::string;
using std::ostream;
using std::istream;

const int SIZE_SQUARE = 56;   // number of pixels in a square by default
const int OFFSET_BOARD = 50;   // boarder between the board and the edge of screen

/***********************************************
 * DELTA
 * Movement in a direction (dRow and dCol)
 **********************************************/
struct Delta
{
   int dRow;
   int dCol;
};

const Delta ADD_R = { 1,  0 };
const Delta ADD_C = { 0,  1 };
const Delta SUB_R = { -1,  0 };
const Delta SUB_C = { 0, -1 };


class PositionTest;

/***************************************************
 * POSITION
 * The location of a piece on the board
 ***************************************************/
class Position
{
   friend class PositionTest;
public:
   
   // Position :    The Position class can work with other positions,
   //               Allowing for comparisions, copying, etc.
   Position(const Position& rhs) { set(rhs.colRow); }
   Position() : colRow(0x99) { set(0xff); }
   bool isInvalid() const { return (colRow == 0xff) || (colRow & 0x88); }
   bool isValid() const { return !isInvalid(); }
   void setValid()   { colRow = 0x00; }
   void setInvalid() { colRow = 0xff; }
   bool operator <  (const Position & rhs) const { return this->colRow < rhs.colRow; }
   bool operator == (const Position & rhs) const { return this->colRow == rhs.colRow; }
   bool operator != (const Position & rhs) const { return this->colRow != rhs.colRow; }
   const Position& operator = (const Position& rhs) { set(rhs.colRow); return *this; }
   
   // Location : The Position class can work with locations, which
   //            are 0...63 where we start in row 0, then row 1, etc.
   Position(int location) : colRow(0x99) { setLocation(location); }
   int getLocation() const
   {
      if (isInvalid()) return -1;
      int col = getCol();
      int row = getRow();
      if (col < 0 || row < 0) return -1;
      return row * 8 + col;
   }
   void setLocation(int location)
   {
      if (0 <= location && location < 64)
         set(location % 8, location / 8);
      else
         set(0xff);
   }
   
   
   // Row/Col : The position class can work with row/column,
   //           which are 0..7 and 0...7
   Position(int c, int r) : colRow(0x99) { set(c, r); }
   virtual int getCol() const
   {
      if (isInvalid()) return -1;
      int col = (colRow & 0xf0) >> 4;
      if (col > 7) return -1;
      return col;
   }
   virtual int getRow() const
   {
      if (isInvalid()) return -1;
      int row = (colRow & 0x0f);
      if (row > 7) return -1;
      return row;
   }
   void setCol(int c)
   {
      if (c >= 0 && c < 8)
         set((colRow & 0x0f) | ((c & 0x0f) << 4));
      else
         set(0xff);
   }
   void setRow(int r)
   {
      if (r >= 0 && r < 8)
         set((colRow & 0xf0) | (r & 0x0f));
      else
         set(0xff);
   }
   void set(int c, int r)
   {
      if (c < 0 || c > 7 || r < 0 || r > 7)
         colRow = 0xff;
      else
         colRow = ((c & 0x0f) << 4) | (r & 0x0f);
   }
   
   // Text:    The Position class can work with textual coordinates,
   //          such as "d4"
   
   Position(const char* s) : colRow(0x99)
   {
      if (!s || s[0] < 'a' || s[0] > 'h' || s[1] < '1' || s[1] > '8')
      {
         colRow = 0xff;
      }
      else
      {
         int col = s[0] - 'a';
         int row = s[1] - '1';
         set(col, row);
      }
   }
   const Position& operator =  (const char* rhs)
   {
      if (!rhs || rhs[0] < 'a' || rhs[0] > 'h' || rhs[1] < '1' || rhs[1] > '8')
      {
         colRow = 0xff;
      }
      else
      {
         int col = rhs[0] - 'a';
         int row = rhs[1] - '1';
         set(col, row);
      }
      return *this;
   }
   const Position& operator =  (const string& rhs)
   {
      *this = rhs.c_str();
      return *this;
   }
   
   
   // Pixels:    The Position class can work with screen coordinates,
   //            a.k.a. Pixels, these are X and Y coordinates. Note that
   //            we need to scale them according to the size of the board.
   
   int getX()   const
   {
      return (int)((double)getCol() * getSquareWidth() + getSquareWidth());
   }
   int getY()   const
   {
      return (int)((double)getRow() * getSquareHeight() + getSquareHeight());
   }
   void setXY(double x, double y)
   {
      int col = (int)(x / getSquareWidth()) - 1;
      int row = 8 - (int)(y / getSquareHeight());
      if (col < 0 || col > 7)
         colRow = 0xff;
      else if (row < 0 || row > 7)
         colRow = 0x3f;
      else
         set(col, row);
   }
   double getSquareWidth()  const { return squareWidth; }
   double getSquareHeight() const { return squareHeight; }
   void setBoardWidthHeight(int widthBoard, int heightBoard)
   {
      if (widthBoard < 0 || heightBoard < 0)
         return;
      squareWidth = (double)widthBoard / 10.0; // 8 squares + 2 spaces
      squareHeight = (double)heightBoard / 10.0;
   }
   
   // Delta:    The Position class can work with deltas, which are
   //           offsets from a given location. This helps pieces move
   //           on the chess board.
   Position(const Position& rhs, const Delta& delta) : colRow(-1) {  }
   void adjustCol(int dCol)
   {
      if (isInvalid()) return;
      int newCol = getCol() + dCol;
      if (newCol < 0 || newCol > 7)
         colRow = 0xff;
      else
         setCol(newCol);
   }
   void adjustRow(int dRow)
   {
      if (isInvalid()) return;
      int newRow = getRow() + dRow;
      if (newRow < 0 || newRow > 7)
         colRow = 0xff;
      else
         setRow(newRow);
   }
   const Position& operator += (const Delta& rhs)
   {
      if (isInvalid())
         return *this;
      int newCol = getCol() + rhs.dCol;
      int newRow = getRow() + rhs.dRow;
      if (newCol < 0 || newCol > 7 || newRow < 0 || newRow > 7)
         colRow = 0xff;
      else
         set(newCol, newRow);
      return *this;
   }
   Position operator + (const Delta& rhs) const
   {
      Position temp(*this);
      temp += rhs;
      return temp;
   }
   
private:
   void set(uint8_t colRowNew) { colRow = colRowNew; }
   
   uint8_t colRow;
   static double squareWidth;
   static double squareHeight;
};


ostream& operator << (ostream& out, const Position& pos);
istream& operator >> (istream& in, Position& pos);
