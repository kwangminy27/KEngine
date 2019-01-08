#pragma once

#ifdef ENGINE_EXPORT
#define ENGINE_DLL __declspec(dllexport)
#else
#define ENGINE_DLL __declspec(dllimport)
#endif
