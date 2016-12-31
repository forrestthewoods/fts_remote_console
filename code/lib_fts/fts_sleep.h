// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith


#ifndef FTS_SLEEP_H
#define FTS_SLEEP_H

#ifdef FTS_WINDOWS
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

namespace fts {

    void Sleep(int milliseconds) {
        #ifdef FTS_WINDOWS
            ::Sleep(milliseconds);
        #else
            ::usleep(milliseconds * 1000);
        #endif
    }    

} // namespace fts

#endif FTS_SLEEP_H