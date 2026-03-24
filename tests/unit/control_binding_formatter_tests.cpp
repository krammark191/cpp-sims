#include <cassert>
#include <vector>

#include "app/control_binding_formatter.h"

void runControlBindingFormatterTests()
{
   {
      const ControlBinding binding{"Rotate left", "Left Arrow", "A"};
      assert(ControlBindingFormatter::format(binding) == "Rotate left: Left Arrow / A");
      assert(ControlBindingFormatter::format(binding, true) == " - Rotate left: Left Arrow / A");
   }

   {
      const ControlBinding binding{"Pause menu", "Esc", nullptr};
      assert(ControlBindingFormatter::format(binding) == "Pause menu: Esc");
   }

   {
      const std::vector<ControlBinding> bindings =
      {
         {"Rotate left", "Left Arrow", "A"},
         {"Pause menu", "Esc", nullptr}
      };

      const auto lines = ControlBindingFormatter::formatAll(bindings, true);
      assert(lines.size() == 2);
      assert(lines[0] == " - Rotate left: Left Arrow / A");
      assert(lines[1] == " - Pause menu: Esc");
   }
}
