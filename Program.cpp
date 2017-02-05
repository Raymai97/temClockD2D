#include <Windows.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include "Cfg.hpp"
#include "Except.hpp"
#include "D2DUIWindow.hpp"
#include "D2DUIElement.hpp"

using namespace std;
using namespace TemClockD2D;

D2DUIWindow mainWindow(L"temClock");
D2DUIElement btnTest(D2DUIElement::Type::Button);
D2DUIElement btnExit(D2DUIElement::Type::Button);

int Main(); // Program Entry Point
void InitUI();
void btnTest_Click(D2DUIClickInfo const &);

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	try {
		return Main();
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
}

int Main() {
	InitUI();
	MSG msg{};
	while (GetMessageW(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return static_cast<int>(msg.wParam);
}

void InitUI() {
	mainWindow
		.SetClientSize(600, 400)
		.CenterScreen()
		.Show();
	mainWindow += btnTest
		.X(80).Y(60)
		.Width(100).Height(35)
		.Text(L"Test")
		.SetClickHandler(btnTest_Click);
	mainWindow += btnExit
		.X(280).Y(60)
		.Width(100).Height(35)
		.Text(L"Close")
		.SetClickHandler([](D2DUIClickInfo const &) { exit(0); });
}

void btnTest_Click(D2DUIClickInfo const &) {
	btnTest.X(btnTest.X() + 20);
	MessageBoxW(mainWindow, L"You've click on Test button.", L"Test", MB_ICONINFORMATION);
}
