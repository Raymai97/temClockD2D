#include <Windows.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include "Cfg.hpp"
#include "Except.hpp"
#include "MiniXML/src/mxml.h"

using namespace std;
using namespace TemClockD2D;

void Main(); // Program Entry Point

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	try {
		Main();
	}
	catch (Except const & ex) {
		wstringstream ss;
		ss << L"Unexpected error has occured.";
		ss << endl << endl << ExceptReasonToString(ex.GetReason());
		if (ex.HasCustomMsg()) {
			ss << endl << endl << ex.GetCustomMsg();
		}
		if (ex.HasCustomHr()) {
			ss << endl << endl << L"HRESULT: 0x" <<
				setfill(L'0') << setw(8) << hex << uppercase <<
				ex.GetCustomHr();
		}
		MessageBoxW(GetForegroundWindow(),
			ss.str().c_str(), L"Unexpected!", MB_ICONERROR);
		return 1;
	}
	return 0;
}

void Main() {
	SaveCfg(cfgFilePath);
	LoadCfg(cfgFilePath);
}
