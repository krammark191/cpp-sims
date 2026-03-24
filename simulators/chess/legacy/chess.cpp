/**********************************************************************
 * Source File:
 *    Lab 06: Chess
 * Author:
 *    Taden Marston & Mark Van Horn
 * Summary:
 *    Play the game of chess
 ************************************************************************/


#include "uiInteract.h"   // for Interface
#include "uiDraw.h"       // for OGSTREAM
#include "position.h"     // for POSITION
#include "piece.h"        // for PIECE and company
#include "board.h"        // for BOARD
#include "test.h"
#include <set>            // for STD::SET
#include <cassert>        // for ASSERT
#include <fstream>        // for IFSTREAM
#include <string>         // for STRING
using namespace std;


/*************************************
 * All the interesting work happens here, when
 * I get called back from OpenGL to draw a frame.
 * When I am finished drawing, then the graphics
 * engine will wait until the proper amount of
 * time has passed and put the drawing on the screen.
 **************************************/
void callBack(Interface* pUI, void* p)
{
   Board* pBoard = (Board*)p;
   
   Move move;
   set <Move> possible;
   
   Position src = pUI->getPreviousPosition();
   Position dest = pUI->getSelectPosition();
   
   if (src.isValid())
      (*pBoard)[src].getMoves(possible, *pBoard);
   
   // If the source and destination are valid, and the move is possible
   if (dest.isValid() && src.isValid())
      move = Move(src, dest, possible);
   
   // move
   if (possible.find(move) != possible.end())
   {
      // Perform the move
      pBoard->move(move);
      pUI->clearSelectPosition();
   }
   
   // If no possible move was found, draw current selection possible moves
   else if (dest.isValid())
   {
      possible.clear();
      (*pBoard)[dest].getMoves(possible, *pBoard);
   }
   
   // if blank spot clicked, clear selection
   if (dest.isValid() && (*pBoard)[dest].getType() == SPACE)
      pUI->clearSelectPosition();
   
   // Draw the board
   pBoard->display(pUI->getHoverPosition(), pUI->getSelectPosition());
   
}


/*********************************
 * MAIN - Where it all begins...
 *********************************/
#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(
                   _In_ HINSTANCE hInstance,
                   _In_opt_ HINSTANCE hPrevInstance,
                   _In_ PSTR pCmdLine,
                   _In_ int nCmdShow)
#else // !_WIN32
int main(int argc, char** argv)
#endif // !_WIN32
{
   
   // run all the unit tests
   testRunner();
   
   // Instantiate the graphics window
   Interface ui("Chess");
   
   // Initialize the game class
   ogstream* pgout = new ogstream;
   Board board(pgout);
   
   // set everything into action
   ui.run(callBack, (void*)(&board));
   
   // All done.
   delete pgout;
   return 0;
}
