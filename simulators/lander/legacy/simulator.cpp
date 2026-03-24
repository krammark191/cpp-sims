/**********************************************************************
 * LAB 06
 * Lunar Lander simulation. This is the Game class and main()
 **********************************************************************/

 /*************************************************************
 * 1. Name:
 * Taden Marston & Mark Van Horn
 * 2. Assignment Name:
 * Lab 06: Apollo 11 Final
 * 3. Assignment Description:
 * Simulate the Apollo 11 landing
 * 4. What was the hardest part? Be as specific as possible.
 * The hardest part was getting the text to draw on screen.
 * That is what took up the majority of our time was getting
 * the text accurate and properly aligned. The other part that
 * Gave us difficulty was debugging some errors in calculating
 * the altitude giving incorrect collision. After both of those
 * issues were resolved the project went by very quickly.
 * 5. How long did it take for you to complete the assignment?
 * This assignment took us roughly 4 hrs to complete.
 **************************************************************/

#include "position.h"    // everything should have a point
#include "acceleration.h"// for ACCELERATION
#include "lander.h"      // for LANDER
#include "star.h"        // for STAR
#include "uiInteract.h"  // for INTERFACE
#include "uiDraw.h"      // for RANDOM and DRAW*
#include "ground.h"      // for GROUND
#include "test.h"        // for the unit tests
#include <cmath>         // for SQRT
#include <cassert>       // for ASSERT
#include <vector>        // for VECTOR
#include <iomanip>       // for SETW and SETPRECISION
using namespace std;


/*************************************************************************
 * SIMULATOR
 * Everything pertaining to the simulator.
 *************************************************************************/
class Simulator
{
public:
   Simulator(const Position& posUpperRight) :
      posUpperRight(posUpperRight),
      ground(posUpperRight),
      lander(posUpperRight)
   {
      lander.reset(posUpperRight);
   }
   Position posUpperRight;
   Ground ground;
   Lander lander;
   Thrust thrust;
   vector<Star> stars;
   bool isRunning = true;  // Flag to indicate if the simulation is running
   bool landed;
};

/****************************
 * Function to center text
 ****************************/
void drawCenteredText(ogstream& gout, const std::string& text, const Position& screenSize, double yPos)
{
   double textWidth = text.length() * 6; // Approximate width of each character
   double xPos = (screenSize.getX() - textWidth) / 2;
   gout.setPosition(Position(xPos, yPos));
   gout << text;
}

/*************************************
 * CALLBACK
 * Handle one frame of the simulator
 **************************************/
void callBack(const Interface* pUI, void* p)
{
   Simulator* pSimulator = (Simulator*)p;

   ogstream gout;

   // If stars have not been initialized, initialize them
   if (pSimulator->stars.empty())
   {
      const int STAR_COUNT = pSimulator->posUpperRight.getX() / 8;
      for (int i = 0; i < STAR_COUNT; i++)
      {
         Star star;
         star.reset(pSimulator->posUpperRight.getX(), pSimulator->posUpperRight.getY());
         pSimulator->stars.push_back(star);
      }
   }

   // Draw all stars
   for (Star& star : pSimulator->stars)
   {
      star.draw(gout);
   }

   // Draw the ground
   pSimulator->ground.draw(gout);

   // Draw the lander
   pSimulator->lander.draw(pSimulator->thrust, gout);

   if (pSimulator->isRunning)
   {
      // Update the lander's position
      pSimulator->thrust.set(pUI);
      Acceleration accel = pSimulator->lander.input(pSimulator->thrust, -1.625);
      pSimulator->lander.coast(accel, 0.1);

      if (pSimulator->ground.onPlatform(pSimulator->lander.getPosition(),
         pSimulator->lander.getWidth()))
      {
         if (pSimulator->lander.getSpeed() < 3)
         {
            pSimulator->lander.land();
            pSimulator->landed = true;
            pSimulator->isRunning = false;  // Stop the simulation
            return;
         }
      }
      // Check for ground collision and handle landing or crashing
      else if (pSimulator->ground.hitGround(pSimulator->lander.getPosition(),
         pSimulator->lander.getWidth()))
      {
         pSimulator->lander.crash();
         pSimulator->landed = false;
         pSimulator->isRunning = false;  // Stop the simulation
         return;
      }
   }

   // Calculate the altitude
   double altitude = pSimulator->lander.getPosition().getY() - pSimulator->ground.getElevation(pSimulator->lander.getPosition());

   // Construct the text to be displayed
   stringstream ss;
   ss << left << setw(15) << "Fuel: " << setw(1) << left
      << pSimulator->lander.getFuel() << " lbs\n";
   ss << left << setw(15) << "Altitude: " << setw(1) << left
      << static_cast<int>(altitude) << " meters\n";
   ss << left << setw(14) << "Velocity: " << setw(1) << left
      << fixed << setprecision(2) << pSimulator->lander.getSpeed() << " m/s";

   // Move the position to the top left corner
   gout.setPosition(Position(pSimulator->posUpperRight.getX() * 0.02, pSimulator->posUpperRight.getY() * 0.95));

   // Use the stream to draw the text
   gout << ss.str();

   if (!pSimulator->isRunning)
   {
      if (pSimulator->landed)
      {
         drawCenteredText(gout, "The Eagle has landed!", pSimulator->posUpperRight, pSimulator->posUpperRight.getY() * 0.7);
      }
      else
      {
         drawCenteredText(gout, "Houston, we have a problem!", pSimulator->posUpperRight, pSimulator->posUpperRight.getY() * 0.7);
      }
   }
}

/*********************************
 * Main is pretty sparse.  Just initialize
 * my LM type and call the display engine.
 * That is all!
 *********************************/
#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(
   _In_ HINSTANCE hInstance,
   _In_opt_ HINSTANCE hPrevInstance,
   _In_ LPSTR pCmdLine,
   _In_ int nCmdShow)
#else // !_WIN32
int main(int argc, char** argv)
#endif // !_WIN32
{
   // Run the unit tests
   testRunner();

   // Initialize OpenGL
   Position posUpperRight(1280, 720);
   Interface ui("Lunar Lander", posUpperRight);

   // Initialize the game class
   Simulator simulator(posUpperRight);

   // set everything into action
   ui.run(callBack, (void*)&simulator);

   return 0;
}
