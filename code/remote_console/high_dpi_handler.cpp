// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// AUTHOR
//   Forrest Smith


#include "high_dpi_handler.h"

// thirdparty libs
#include <imgui/imgui.h>

// platform
#ifdef FTS_WINDOWS
	#define WIN32_LEAN_AND_MEAN     
	#include <windows.h>
#endif


namespace fts {

#ifdef FTS_WINDOWS

    void InitDPI() {
		SetProcessDPIAware();

		HDC screen = GetDC(0);
		int system_dpi = GetDeviceCaps(screen, LOGPIXELSX);

		float base_dpi = 96.f;
		float scale = system_dpi / base_dpi;
		ImGui::GetIO().FontGlobalScale = scale;
    }

#else

    void InitDPI() {
    	// $$$FTS - Figure out high dpi configuration for Linux/OSX
    }

#endif 

} // namespace fts