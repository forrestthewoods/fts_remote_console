// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith

#include "net_utils.h"

#undef UNICODE

// platform
#ifdef FTS_WINDOWS
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <unistd.h>
#endif // FTS_WINDOWS


// lang
#include <stdio.h>
#include <string>


namespace fts {

#ifdef FTS_WINDOWS
    bool GetHostname(char * hostname, int len) {
        if (gethostname(hostname, len) == SOCKET_ERROR)
            return false;

        return true;
    }

    int GetProcessId() {
        return GetCurrentProcessId();
    }
#else
    bool GetHostname(char * hostname, int len) {
        return gethostname(hostname, len) == 0;
    }

    int GetProcessId() {
        return ::getpid();
    }
#endif

}