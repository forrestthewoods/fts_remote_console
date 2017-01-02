fts_remote_console
===

A cross-platform remote logging console written in C++

FAQ
===

### Why does this exist?
This is a toy side-project written for my own educational purposes. I would not recommend using it. But if you'd like to learn some of the things I learned take a gander.

#### What license?
All source code is embedded with the following license:

> This software is dual-licensed to the public domain and under the following license: you are granted a perpetual, irrevocable license to copy, modify, publish, and distribute this file as you see fit.

### What technologies did you learn about?
Several!

ASIO - C++ Networking
ImGui - C++ UI
Genie - Project Generation
AppVeyor - Windows continuous integration
Travis - OSX /Linux continuous integration

Networking: ASIO
UI: ImGui
Project Generation: Genie
Continuous Integration: AppVeyor, Travis

### What else did you learn about?
Quite a lot! I wrote a blog post detailing my adventure. You can read it here.

Binaries
===

Here are some binaries if you're lazy but want to see it in action.

Windows: [Remote Console](https://s3.amazonaws.com/forrestthewoods.binaries/fts_remote_console/projects/vs2015/bin/x64_release/fts_remote_console/fts_remote_console.exe) [Example Game](https://s3.amazonaws.com/forrestthewoods.binaries/fts_remote_console/projects/vs2015/bin/x64_release/example_game/example_game.exe)

Build Status
===
Windows ([Appveyor](https://www.appveyor.com/)): [![Build status](https://ci.appveyor.com/api/projects/status/n791a0uxq3layhm3?svg=true)](https://ci.appveyor.com/project/forrestthewoods/fts-remote-console)