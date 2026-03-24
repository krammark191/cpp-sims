/***********************************************************************
 * Source File:
 *    Test : Test runner
 * Author:
 *    Br. Helfrich
 * Summary:
 *    The test runner for all the unit tests
 ************************************************************************/

#include "test.h"
#include "testPosition.h"
#include "testAcceleration.h"
#include "testVelocity.h"
#include "testAngle.h"
#include "testOrbitalObject.h" 
#include "testSatellite.h"     
#include "testGPS.h"
#include "testHubble.h"
#include "testSputnik.h"
#include "testDragon.h"
#include "testStarlink.h"
#include "testDreamChaser.h"
#include "testPart.h"
#include "testFragment.h"

#ifdef _WIN32
#include <windows.h>
#include <iostream>
using namespace std;
#endif

/*****************************************************************
 * TEST RUNNER
 * Runs all the unit tests
 ****************************************************************/
void testRunner()
{
#ifdef _WIN32
   AllocConsole();
   FILE* stream;
   errno_t err;
   err = freopen_s(&stream, "CONOUT$", "a", stdout);
#endif // _WIN32

   TestPosition().run();
   TestAcceleration().run();
   TestVelocity().run();
   TestAngle().run();
   TestOrbitalObject().run(); 
   TestSatellite().run();     
   TestGPS().run();
   TestHubble().run();
   TestSputnik().run();
   TestDragon().run();
   TestStarlink().run();
   TestDreamChaser().run();
   TestPart().run();
   TestFragment().run();
}
