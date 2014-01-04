#ifdef DECOMPPROXY_EXPORTS
#define DECOMPPROXY_API __declspec(dllexport)
#else
#define DECOMPPROXY_API __declspec(dllimport)
#endif

DECOMPPROXY_API int Init(const char *filename);
DECOMPPROXY_API int Shutdown();
DECOMPPROXY_API int GetSize(unsigned char* section, int sectionsize);
DECOMPPROXY_API int DecompressSmart(unsigned char* section, int sectionsize, unsigned char* outbuffer);