#pragma once
#include "D2DUI.hpp"

class D2DUIElement;
typedef std::vector<D2DUIElement*>	D2DUIElements;

class D2DUIElement {
public:
	enum class Type {
		Button,
		TextBlock,
		GroupBox
	};
private:
	D2DUIElements	m_childs{};
	Type	m_type{};
	bool	m_dirty{};
	bool	m_visible{};
	bool	m_focused{};
	int		m_x{};
	int		m_y{};
	int		m_cx{};
	int		m_cy{};
	int		m_z{};	// z-order
	std::wstring	m_text{};
	std::wstring	m_fontName{};
	int				m_fontSize{};
	COLORREF		m_textColor{};
	COLORREF		m_lineColor{};
	COLORREF		m_fillColor{};
	COLORREF		m_hoverFillColor{};
	COLORREF		m_pressFillColor{};
	double			m_hoveringBias{};
	double			m_pressingBias{};
	
	D2DUIClickHandler	m_clickHandler;
public:
	D2DUIElement(Type const &);
	~D2DUIElement();

	auto& operator+=(D2DUIElement & child) {
		m_childs.push_back(&child);
		return *this;
	}
	bool operator==(Type type) const {
		return m_type == type;
	}

	auto& Click(D2DUIClickType type = D2DUIClickType::LeftClick) {
		if (m_clickHandler) { m_clickHandler(D2DUIClickInfo(type)); }
		return *this;
	}
	auto& SetClickHandler(D2DUIClickHandler const & handler) {
		m_clickHandler = handler;
		return *this;
	}
	auto& Focus(bool val) {
		if (m_type == Type::Button) {
			m_focused = val;
		}
		return *this;
	}
	bool HitTest(int x, int y) {
		return x >= m_x && y >= m_y &&
			x <= m_x + m_cx && y <= m_y + m_cy;
	}
	
	auto  CanFocus() const { return m_type == Type::Button; }
	auto  IsVisible() const { return m_visible; }
	auto  IsFocused() const { return m_focused; }
	auto  IsDirty() const { return m_dirty; }
	auto& Visible(bool val) { m_visible = val; m_dirty = true; return *this; }
	auto& Dirty(bool val) { m_dirty = val; return *this; }
	auto  GetHoverBias() const { return m_hoveringBias; }
	auto  GetPressingBias() const { return m_pressingBias; }
	auto& Hovering(bool hovering) {
		bool dirty{true};
		if (hovering && m_hoveringBias < 1.0) {
			m_hoveringBias += 0.1;
		}
		else if (!hovering && m_hoveringBias > 0.0) {
			m_hoveringBias -= 0.1;
		}
		else { dirty = false; }
		if (dirty) { Dirty(true); }
		return *this;
	}
	auto& Pressing(bool pressing) {
		bool dirty{ true };
		if (pressing && m_pressingBias < 1.0) {
			m_pressingBias += 0.1;
		}
		else if (!pressing && m_pressingBias > 0.0) {
			m_pressingBias -= 0.1;
		}
		else { dirty = false; }
		if (dirty) { Dirty(true); }
		return *this;
	}
	
	auto  X() const { return m_x; }
	auto  Y() const { return m_y; }
	auto  Z() const { return m_z; }
	auto  Right() const { return m_x + m_cx; }
	auto  Bottom() const { return m_y + m_cy; }
	auto  Width() const { return m_cx; }
	auto  Height() const { return m_cy; }
	auto& Text() const { return m_text; }
	auto& X(int val) { m_x = val; m_dirty = true; return *this; }
	auto& Y(int val) { m_y = val; m_dirty = true; return *this; }
	auto& Z(int val) { m_z = val; m_dirty = true; return *this; }
	auto& Width(int val) { m_cx = val; m_dirty = true; return *this; }
	auto& Height(int val) { m_cy = val; m_dirty = true; return *this; }
	auto& Text(std::wstring const & val) { m_text = val; m_dirty = true; return *this; }

	auto& FontName() const { return m_fontName; }
	auto  FontSize() const { return m_fontSize; }
	auto  FillColor() const { return m_fillColor; }
	auto  HoverFillColor() const { return m_hoverFillColor; }
	auto  PressFillColor() const { return m_pressFillColor; }
	auto  LineColor() const { return m_lineColor; }
	auto  TextColor() const { return m_textColor; }
	auto& FontName(std::wstring const & val);
	auto& FontSize(int val);
	auto& FillColor(COLORREF val);
	auto& HoverFillColor(COLORREF val);
	auto& PressFillColor(COLORREF val);
	auto& LineColor(COLORREF val);
	auto& TextColor(COLORREF val);

	RECT RECT() const { return{ X(), Y(), Right(), Bottom() }; }
	auto RectF() const {
		return D2D1::RectF(
			static_cast<FLOAT>(X()),
			static_cast<FLOAT>(Y()),
			static_cast<FLOAT>(Right()),
			static_cast<FLOAT>(Bottom()));
	}
private:
};

inline D2DUIElement::D2DUIElement(
	Type const & type) :
	m_type(type),
	m_fontName(L"Tahoma"),
	m_fontSize(MulDiv(9,96,72)),
	m_fillColor(RGB(130, 130, 0)),
	m_hoverFillColor(RGB(180, 180, 0)),
	m_pressFillColor(RGB(100, 100, 0)),
	m_textColor(RGB(255, 255, 255)),
	m_lineColor(RGB(255, 255, 255))
{
}

inline D2DUIElement::~D2DUIElement() {
}

inline auto& D2DUIElement::FontName(std::wstring const & val) {
	m_fontName = val;
	m_dirty = true;
	return *this;
}
inline auto& D2DUIElement::FontSize(int val) {
	m_fontSize = val;
	m_dirty = true;
	return *this;
}
inline auto& D2DUIElement::FillColor(COLORREF val) {
	m_fillColor = val;
	m_dirty = true;
	return *this;
}
inline auto& D2DUIElement::HoverFillColor(COLORREF val) {
	m_hoverFillColor = val;
	m_dirty = true;
	return *this;
}
inline auto& D2DUIElement::PressFillColor(COLORREF val) {
	m_pressFillColor = val;
	m_dirty = true;
	return *this;
}
inline auto& D2DUIElement::LineColor(COLORREF val) {
	m_lineColor = val;
	m_dirty = true;
	return *this;
}
inline auto& D2DUIElement::TextColor(COLORREF val) {
	m_textColor = val;
	m_dirty = true;
	return *this;
}
