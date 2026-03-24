#include "app/runtime_paths.h"

#include <string>

#if defined(__APPLE__)
#include <mach-o/dyld.h>
#elif defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

namespace RuntimePaths
{
std::filesystem::path executablePath()
{
#if defined(__APPLE__)
   uint32_t size = 0;
   _NSGetExecutablePath(nullptr, &size);
   std::string buffer(size, '\0');
   if (_NSGetExecutablePath(buffer.data(), &size) == 0)
      return std::filesystem::weakly_canonical(std::filesystem::path(buffer.c_str()));
#elif defined(_WIN32)
   std::wstring buffer(MAX_PATH, L'\0');
   DWORD length = 0;
   while (true)
   {
      length = GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
      if (length == 0)
         break;
      if (length < buffer.size() - 1)
         return std::filesystem::weakly_canonical(std::filesystem::path(buffer.data(), buffer.data() + length));
      buffer.resize(buffer.size() * 2);
   }
#endif

   return {};
}

std::filesystem::path resolveRelativePath(const std::filesystem::path & relativePath)
{
   if (relativePath.empty())
      return relativePath;

   if (relativePath.is_absolute() && std::filesystem::exists(relativePath))
      return relativePath;

   if (std::filesystem::exists(relativePath))
      return std::filesystem::weakly_canonical(relativePath);

   const std::filesystem::path exePath = executablePath();
   if (!exePath.empty())
   {
      const std::filesystem::path exeDir = exePath.parent_path();
      const std::filesystem::path besideExe = exeDir / relativePath;
      if (std::filesystem::exists(besideExe))
         return std::filesystem::weakly_canonical(besideExe);

      const std::filesystem::path bundleResources = exeDir.parent_path() / "Resources" / relativePath;
      if (std::filesystem::exists(bundleResources))
         return std::filesystem::weakly_canonical(bundleResources);
   }

   return relativePath;
}

std::string resolveRelativePathString(const std::string & relativePath)
{
   return resolveRelativePath(std::filesystem::path(relativePath)).string();
}
}
