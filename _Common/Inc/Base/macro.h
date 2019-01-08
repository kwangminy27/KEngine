#pragma once

#ifdef COMMON_EXPORT
#define COMMON_DLL __declspec(dllexport)
#else
#define COMMON_DLL __declspec(dllimport)
#endif
