#pragma once

#if defined (_WIN32)
#define FOPEN(f, fname, mode) fopen_s(&f, fname, mode)
#define stricmp _stricmp
#else
#define FOPEN(f, fname, mode) f = fopen(fname, mode)
#define stricmp strcasecmp
#endif
