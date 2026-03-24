#include "app/window_shell_runner.h"

int main(int argc, const char * argv[])
{
   (void)argc;
   (void)argv;

   WindowShellRunner runner;
   return runner.run();
}
