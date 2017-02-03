#pragma once
#include <Windows.h>
#include <string>
#include "MyWinAPI/MyExePath.hpp"

namespace TemClockD2D {
	namespace Cfg {
		extern std::wstring		fontName, miniFontName;
		extern int				fontSize, miniFontSize;
	}
	auto const cfgFilePath = MyExeDirPathW() + L"temClockD2D.cfg";
	void LoadCfg(std::wstring const & filePath);
	void SaveCfg(std::wstring const & filePath);
}
