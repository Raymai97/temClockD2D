#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <vector>
#include <Windows.h>
#include <windowsx.h>
#include <d2d1.h>
#include <dwrite.h>

// Event Handler - Click ......

enum class D2DUIClickType {
	LeftClick,
	RightClick,
	MiddleClick
};

class D2DUIClickInfo {
	D2DUIClickType const m_clickType;
public:
	D2DUIClickInfo(D2DUIClickType const & clickType) :
		m_clickType(clickType) {}

	auto Type() const { return m_clickType; }
};

typedef std::function<void(D2DUIClickInfo)> D2DUIClickHandler;

// ... Click
