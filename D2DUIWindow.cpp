#include "D2DUIWindow.hpp"
#include "MyWinAPI/all.h"

LPCWSTR		D2DUIWindow::m_szClassName{ L"D2DUIWindow" };
UINT const	D2DUIWindow::m_checkDirtyTimerId{ WM_USER + 99 };
UINT const	D2DUIWindow::m_transitionTimerId{ WM_USER + 98 };
int			D2DUIWindow::m_refCount{ 0 };

D2DUIWindow::D2DUIWindow(
	std::wstring const & windowTitle,
	HWND hwndParent)
{
	WNDCLASSEX wcex{};
	wcex.cbSize = sizeof(wcex);
	wcex.hCursor = ::LoadCursorW(nullptr, IDC_ARROW);
	wcex.lpfnWndProc = this->StaticWndProc;
	wcex.lpszClassName = m_szClassName;
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	::RegisterClassExW(&wcex);
	m_hwnd = ::CreateWindowExW(0,
		m_szClassName, windowTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 300, 300,
		hwndParent, nullptr, nullptr,
		reinterpret_cast<LPVOID>(this));
	if (!m_hwnd) { throw std::exception(); }
	m_renderer.SetDest(this);
	::SetTimer(m_hwnd, m_checkDirtyTimerId, 20, nullptr);
	::SetTimer(m_hwnd, m_transitionTimerId, 20, nullptr);
	++m_refCount;
}

D2DUIWindow::~D2DUIWindow() {
	::KillTimer(m_hwnd, m_checkDirtyTimerId);
	::DestroyWindow(m_hwnd);
}

auto D2DUIWindow::GetWindowTitle() const -> std::wstring {
	auto cch = GetWindowTextLengthW(m_hwnd) + 1;
	auto sz = new wchar_t[cch];
	::GetWindowTextW(m_hwnd, sz, cch);
	return std::wstring(sz);
}

auto D2DUIWindow::SetWindowTitle(
	std::wstring const & windowTitle) -> D2DUIWindow &
{
	SetWindowTextW(m_hwnd, windowTitle.c_str());
	return *this;
}

auto D2DUIWindow::GetClientWidth() const -> int {
	RECT rc{};
	::GetClientRect(m_hwnd, &rc);
	return rc.right - rc.left;
}

auto D2DUIWindow::GetClientHeight() const -> int {
	RECT rc{};
	::GetClientRect(m_hwnd, &rc);
	return rc.bottom - rc.top;
}

auto D2DUIWindow::SetClientSize(
	int width, int height) -> D2DUIWindow &
{
	::SetWindowClientSize(m_hwnd, width, height);
	return *this;
}

auto D2DUIWindow::GetWindowLeft() const -> int {
	RECT rc{};
	::GetWindowRect(m_hwnd, &rc);
	return rc.left;
}

auto D2DUIWindow::GetWindowTop() const -> int {
	RECT rc{};
	::GetWindowRect(m_hwnd, &rc);
	return rc.top;
}

auto D2DUIWindow::SetWindowPos
	(int x, int y) -> D2DUIWindow&
{
	::SetWindowPos(m_hwnd, nullptr, x, y, 0, 0,
		SWP_NOSIZE | SWP_NOZORDER);
	return *this;
}

auto D2DUIWindow::CenterScreen(
	bool excludeTaskbar) -> D2DUIWindow&
{
	::CenterWindow(m_hwnd, excludeTaskbar);
	return *this;
}

auto D2DUIWindow::Minimize() -> D2DUIWindow & {
	::ShowWindow(m_hwnd, SW_MINIMIZE);
	return *this;
}

auto D2DUIWindow::Maximize() -> D2DUIWindow & {
	::ShowWindow(m_hwnd, SW_MAXIMIZE);
	return *this;
}

auto D2DUIWindow::Restore() -> D2DUIWindow & {
	::ShowWindow(m_hwnd, SW_RESTORE);
	return *this;
}

auto D2DUIWindow::Hide() -> D2DUIWindow & {
	::ShowWindow(m_hwnd, SW_HIDE);
	return *this;
}

auto D2DUIWindow::Show() -> D2DUIWindow & {
	::ShowWindow(m_hwnd, SW_SHOW);
	return *this;
}

LRESULT CALLBACK D2DUIWindow::StaticWndProc(
	HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	if (msg == WM_NCCREATE) {
		auto p = reinterpret_cast<LPCREATESTRUCT>(l);
		::SetWindowLongPtrW(hwnd, GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(p->lpCreateParams));
	}
	auto pSelf = ::GetWindowLongPtrW(hwnd, GWLP_USERDATA);
	if (pSelf) {
		auto self = reinterpret_cast<D2DUIWindow*>(pSelf);
		return self->WndProc(hwnd, msg, w, l);
	}
	return ::DefWindowProcW(hwnd, msg, w, l);
}

LRESULT CALLBACK D2DUIWindow::WndProc(
	HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	switch (msg) {
	case WM_LBUTTONUP:
		HANDLE_WM_LBUTTONUP(hwnd, w, l, OnLButtonUp);
		break;
	case WM_LBUTTONDOWN:
		HANDLE_WM_LBUTTONDOWN(hwnd, w, l, OnLButtonDown);
		break;
	case WM_MOUSEMOVE:
		HANDLE_WM_MOUSEMOVE(hwnd, w, l, OnMouseMove);
		break;
	case WM_PAINT:
		HANDLE_WM_PAINT(hwnd, w, l, OnPaint);
		break;
	case WM_SIZE:
		HANDLE_WM_SIZE(hwnd, w, l, OnSize);
		break;
	case WM_TIMER:
		HANDLE_WM_TIMER(hwnd, w, l, OnTimer);
		break;
	case WM_ERASEBKGND: break;
	case WM_DESTROY:
		--m_refCount;
		if (m_refCount == 0) { PostQuitMessage(0); }
		break;
	default:
		return ::DefWindowProcW(hwnd, msg, w, l);
	}
	return 0;
}

void D2DUIWindow::OnLButtonDown(
	HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	m_isLMouseDown = true;
}

void D2DUIWindow::OnLButtonUp(
	HWND hwnd, int x, int y, UINT keyFlags)
{
	m_isLMouseDown = false;
	auto child = ChildAt(x, y);
	if (child) { child->Click(); }
}

void D2DUIWindow::OnMouseMove(
	HWND hwnd, int x, int y, UINT keyFlags)
{
	m_mouseX = x;
	m_mouseY = y;
}

void D2DUIWindow::OnPaint(HWND hwnd) {
	// BeginPaint() and EndPaint() must be called to
	// tell Windows that we've painted.
	static PAINTSTRUCT ps{};
	BeginPaint(hwnd, &ps);
	m_renderer.RenderAll();
	EndPaint(hwnd, &ps);
}

void D2DUIWindow::OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	m_renderer.SetDest(this);
}

void D2DUIWindow::OnTimer(HWND hwnd, UINT id)
{
	if (id == m_checkDirtyTimerId) {
		for (auto child : m_childs) {
			if (child->IsDirty()) {
				InvalidateRect(hwnd, nullptr, FALSE);
				child->Dirty(false);
				break;
			}
		}
	}
	else if (id == m_transitionTimerId) {
		auto hoverChild = ChildAt(m_mouseX, m_mouseY);
		for (auto child : m_childs) {
			if (child == hoverChild) {
				child->Hovering(true);
				child->Pressing(m_isLMouseDown);
			}
			else {
				child->Hovering(false);
				child->Pressing(false);
			}
		}
	}
}

D2DUIElement *
	D2DUIWindow::ChildAt(int x, int y)
{
	for (D2DUIElement * p : m_childs) {
		if (p->HitTest(x, y)) { return p; }
	}
	return nullptr;
}

