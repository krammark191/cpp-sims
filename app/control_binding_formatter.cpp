#include "app/control_binding_formatter.h"

namespace ControlBindingFormatter
{
std::string format(const ControlBinding & binding, bool bulleted)
{
   std::string line;
   if (bulleted)
      line += " - ";

   line += binding.action;
   line += ": ";
   line += binding.primaryKey;

   if (binding.secondaryKey)
   {
      line += " / ";
      line += binding.secondaryKey;
   }

   return line;
}

std::vector<std::string> formatAll(const std::vector<ControlBinding> & bindings,
                                   bool bulleted)
{
   std::vector<std::string> lines;
   lines.reserve(bindings.size());

   for (const auto & binding : bindings)
      lines.push_back(format(binding, bulleted));

   return lines;
}
}
