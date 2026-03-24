#pragma once

#include <filesystem>
#include <string>

namespace RuntimePaths
{
std::filesystem::path executablePath();
std::filesystem::path resolveRelativePath(const std::filesystem::path & relativePath);
std::string resolveRelativePathString(const std::string & relativePath);
}
