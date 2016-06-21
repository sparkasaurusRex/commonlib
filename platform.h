#pragma once

#if defined (_WIN32)
#define stricmp _stricmp
#else
#define stricmp strcasecmp
#endif
