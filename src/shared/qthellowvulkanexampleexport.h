#pragma once

#if defined(_WIN32) || defined(_WIN64) || defined(WINCE) || defined(__MINGW32__)
#ifdef QTHELLOVULKANEXAMPLELIB
#define TRIANGLERENDEREREXPORT __declspec(dllexport)
#else
#define TRIANGLERENDEREREXPORT __declspec(dllimport)
#endif
#else
#define TRIANGLERENDEREREXPORT
#endif
