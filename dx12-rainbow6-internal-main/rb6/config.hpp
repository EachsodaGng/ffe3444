#pragma once
#include ".../../External/imgui/imgui.h"
#include <Windows.h>

namespace d3d12 {
	inline HMODULE mainModule;
	inline HWND mainWindow = NULL;
	inline int uninjectKey;
	inline int openMenuKey;
}

namespace visuals {


	inline bool bEnable;
	
	inline bool bSnaplines;
	inline ImVec4 SnaplineRGB = { 1.f, 1.f, 1.f, 1.f };

	inline bool bBox;
	inline ImVec4 BoxRGB = { 1.f, 1.f, 1.f, 1.f };

	inline int iBoxype = 0;
	inline const char* cBoxType[3] = { "3D Box", "2D Box", "2D Cornered" };


}


