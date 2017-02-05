#pragma once
#include "D2DUI.hpp"
#include "D2DUIElement.hpp"

class D2DUIWindow {
	class Renderer {
		ID2D1Factory*			m_d2d{};
		ID2D1HwndRenderTarget*	m_hrt{};
		IDWriteFactory*			m_dw{};
		D2DUIWindow*			m_window{};
	public:
		Renderer();
		~Renderer();
		void SetDest(D2DUIWindow *);
		void RenderAll() const;

	private:
		void InitHwndRenderTarget();
		void DrawElement(D2DUIElement const &) const;
		void DrawButton(D2DUIElement const &) const;
		void DrawGroupBox(D2DUIElement const &) const;
		void DrawTextBlock(D2DUIElement const &) const;
		COLORREF BiasColor(COLORREF cr1, COLORREF cr2, double bias) const;
	}				m_renderer{};
	D2DUIElements	m_childs{};
	HWND			m_hwnd{ nullptr };
public:
	D2DUIWindow(
		std::wstring const & windowTitle,
		HWND hwndParent = nullptr);
	~D2DUIWindow();

	operator HWND() const { return m_hwnd; }
	bool operator==(D2DUIWindow const & that) {
		return (m_hwnd == that.m_hwnd);
	}
	void operator+=(D2DUIElement & child) {
		m_childs.push_back(&child);
	}
	
	auto GetWindowTitle() const -> std::wstring;
	auto SetWindowTitle(std::wstring const & windowTitle)->D2DUIWindow&;

	auto GetClientWidth() const -> int;
	auto GetClientHeight() const -> int;
	auto SetClientSize(int width, int height) -> D2DUIWindow &;
	
	auto GetWindowLeft() const -> int;
	auto GetWindowTop() const -> int;
	auto SetWindowPos(int x, int y) -> D2DUIWindow&;

	auto CenterScreen(bool excludeTaskbar = false) -> D2DUIWindow&;
	auto Minimize() -> D2DUIWindow &;
	auto Maximize() -> D2DUIWindow &;
	auto Restore() -> D2DUIWindow &;
	auto Hide() -> D2DUIWindow &;
	auto Show() -> D2DUIWindow &;

	auto  GetBgColor() { return m_bgColor; }
	auto& SetBgColor(COLORREF cr) {
		m_bgColor = cr;
		m_renderer.RenderAll();
		return *this;
	}

private:
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
	void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags);
	void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags);
	void OnPaint(HWND hwnd);
	void OnSize(HWND hwnd, UINT state, int cx, int cy);
	void OnTimer(HWND hwnd, UINT id);
	D2DUIElement * ChildAt(int x, int y);

	static LPCWSTR		m_szClassName;
	static UINT const	m_checkDirtyTimerId;
	static UINT const	m_transitionTimerId;
	static int			m_refCount;

	COLORREF	m_bgColor{RGB(150,150,150)};
	bool	m_isLMouseDown{};
	int		m_mouseX{};
	int		m_mouseY{};

	static LRESULT CALLBACK StaticWndProc(HWND, UINT, WPARAM, LPARAM);
};
