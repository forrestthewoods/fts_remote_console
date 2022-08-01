fts_remote_console
===

A cross-platform remote logging console written in C++

![Example screenshot](https://cdn-images-1.medium.com/max/800/1*-nVVEQ9gRQei1SSAwPeyGA.png)

FAQ
===

### Why does this exist?
This is a toy side-project written for my own educational purposes. I would not recommend you actually *use* this. But it may provide some mild value as a reference.

### What license?
My source code is embedded with the following license:

> This software is dual-licensed to the public domain and under the following license: you are granted a perpetual, irrevocable license to copy, modify, publish, and distribute this file as you see fit.

### What did you learn?

Quite a few things. I wrote a lengthy [blog post](https://blog.forrestthewoods.com/writing-a-cross-platform-remote-logging-console-in-c-f2e22d3fee5e) with details. Here's a brief summary:

* [ASIO](http://think-async.com/) — networking
* [Flatbuffers](https://github.com/google/flatbuffers) — network protocol
* [ImGui](https://github.com/ocornut/imgui) — user interface
* [GENie](https://github.com/bkaradzic/GENie) — project generation
* [Travis](https://travis-ci.org/forrestthewoods/fts_remote_console) / [Appveyor](https://ci.appveyor.com/project/forrestthewoods/fts-remote-console) — continuous integration
* [VMPlayer](http://www.vmware.com/products/player/playerpro-evaluation.html) — Linux virtual machine


Binaries
===

Here are some pre-compiled binaries if you're lazy but want to see it in action.

* [Windows](https://s3.amazonaws.com/forrestthewoods.binaries/fts_remote_console/fts_remote_console_windows_vs2015.zip)
* [Mac](https://s3.amazonaws.com/forrestthewoods.binaries/fts_remote_console/fts_remote_console_macosx.zip)
* [Linux](https://s3.amazonaws.com/forrestthewoods.binaries/fts_remote_console/fts_remote_console_linux.zip)

Build Status
===
Windows ([via Appveyor](https://www.appveyor.com/)): [![Build status](https://ci.appveyor.com/api/projects/status/n791a0uxq3layhm3?svg=true)](https://ci.appveyor.com/project/forrestthewoods/fts-remote-console)

macOS/Linux ([via Travis](https://travis-ci.org/)):  [![Build Status](https://travis-ci.org/forrestthewoods/fts_remote_console.svg?branch=master)](https://travis-ci.org/forrestthewoods/fts_remote_console)
