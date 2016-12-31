// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith


#ifndef FTS_NET_UTILS_H
#define FTS_NET_UTILS_H

#include <stdint.h>
#include <string>

namespace fts {
    bool GetHostname(char * hostname, int len);
    int GetProcessId();
}

#endif
