#include "D2DUIWindow.hpp"

D2DUIWindow::Renderer::Renderer() {
	auto hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		IID_ID2D1Factory,
		reinterpret_cast<void**>(&m_d2d));
	if (FAILED(hr)) { throw std::exception(); }
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&m_dw));
	if (FAILED(hr)) { throw std::exception(); }
}

D2DUIWindow::Renderer::~Renderer() {
	if (m_dw) { m_dw->Release(); }
	if (m_hrt) { m_hrt->Release(); }
	if (m_d2d) { m_d2d->Release(); }
}

void D2DUIWindow::Renderer::SetDest(D2DUIWindow * window)
{
	if (m_hrt) { m_hrt->Release(); m_hrt = nullptr; }
	m_window = window;
	InitHwndRenderTarget();
}

void D2DUIWindow::Renderer::RenderAll() const {
	if (!m_window || !m_hrt) { return; }
	m_hrt->BeginDraw();
	m_hrt->Clear(D2D1::ColorF(m_window->GetBgColor()));
	for (auto child : m_window->m_childs) {
		DrawElement(*child);
	}
	m_hrt->EndDraw();
}

void D2DUIWindow::Renderer::InitHwndRenderTarget() {
	if (m_hrt) { m_hrt->Release(); m_hrt = nullptr; }
	auto hr = m_d2d->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE),
		D2D1::HwndRenderTargetProperties(
			*m_window,
			D2D1::SizeU(
				m_window->GetClientWidth(),
				m_window->GetClientHeight())),
		&m_hrt);
	if (FAILED(hr)) { throw std::exception(); }
}

void D2DUIWindow::Renderer::DrawElement(
	D2DUIElement const & ele) const
{
	using Type = D2DUIElement::Type;
	if (ele == Type::Button) { DrawButton(ele); }
	else if (ele == Type::GroupBox) { DrawGroupBox(ele); }
	else if (ele == Type::TextBlock) { DrawTextBlock(ele); }
}

void D2DUIWindow::Renderer::DrawButton(
	D2DUIElement const & ele) const
{
	ID2D1Brush *bgBrush{}, *textBrush{}, *lineBrush;
	m_hrt->CreateSolidColorBrush(
		D2D1::ColorF(
			ele.GetPressingBias() > 0.01 ?
			BiasColor(
				ele.GetHoverBias() ? ele.HoverFillColor() : ele.FillColor(),
				ele.PressFillColor(), ele.GetPressingBias()) :
			BiasColor(
				ele.FillColor(), ele.HoverFillColor(), ele.GetHoverBias())
		),
		reinterpret_cast<ID2D1SolidColorBrush**>(&bgBrush));
	m_hrt->CreateSolidColorBrush(
		D2D1::ColorF(ele.TextColor()),
		reinterpret_cast<ID2D1SolidColorBrush**>(&textBrush));
	m_hrt->CreateSolidColorBrush(
		D2D1::ColorF(ele.LineColor()),
		reinterpret_cast<ID2D1SolidColorBrush**>(&lineBrush));
	IDWriteTextFormat *textFormat{};
	m_dw->CreateTextFormat(
		ele.FontName().c_str(),
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		static_cast<FLOAT>(ele.FontSize()),
		L"", &textFormat);
	if (!bgBrush || !textBrush || !lineBrush || !textFormat) { return; }
	textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	auto rect = ele.RectF();
	auto rrect = D2D1::RoundedRect(rect, 9.0f, 9.0f);
	m_hrt->FillRoundedRectangle(rrect, bgBrush);
	m_hrt->DrawRoundedRectangle(rrect, lineBrush);
	m_hrt->DrawTextW(ele.Text().c_str(), ele.Text().length(),
		textFormat, rect, textBrush);

	bgBrush->Release();
	textBrush->Release();
	lineBrush->Release();
	textFormat->Release();
}

void D2DUIWindow::Renderer::DrawGroupBox(
	D2DUIElement const & ele) const
{
}

void D2DUIWindow::Renderer::DrawTextBlock(
	D2DUIElement const & ele) const
{
}

COLORREF D2DUIWindow::Renderer::BiasColor(
	COLORREF cr1, COLORREF cr2, double bias) const
{
	if (bias > 1.0) { bias = 1.0; }
	if (bias < 0.0) { bias = 0.0; }
	if (bias == 0.0) { return cr1; }
	if (bias == 1.0) { return cr2; }
	double r = GetRValue(cr1);
	double g = GetGValue(cr1);
	double b = GetBValue(cr1);
	r += (GetRValue(cr2) - GetRValue(cr1)) * bias;
	g += (GetGValue(cr2) - GetGValue(cr1)) * bias;
	b += (GetBValue(cr2) - GetBValue(cr1)) * bias;
	return RGB(r, g, b);
}
