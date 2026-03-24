#pragma once

#include <string>
#include <vector>

#include "app/control_binding.h"

namespace ControlBindingFormatter
{
std::string format(const ControlBinding & binding, bool bulleted = false);

std::vector<std::string> formatAll(const std::vector<ControlBinding> & bindings,
                                   bool bulleted = false);
}
