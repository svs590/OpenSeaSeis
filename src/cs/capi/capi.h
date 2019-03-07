#ifdef _WIN32
#	define CS_CAPI extern "C" __declspec(dllexport)
#elif
#	define CS_CAPI __attribute__((visibility("default")))
#endif