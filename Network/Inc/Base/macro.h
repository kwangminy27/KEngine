#pragma once

#ifdef NETWORK_EXPORT
#define NETWORK_DLL __declspec(dllexport)
#else
#define NETWORK_DLL __declspec(dllimport)
#endif
