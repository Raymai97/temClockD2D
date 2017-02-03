#include "Cfg.hpp"
#include "Except.hpp"
#include "TryParse.hpp"
#include "MyWinAPI/WideUtf8Conv.hpp"
#include "MiniXML/src/mxml.h"

namespace TemClockD2D {
	namespace Cfg {
		std::wstring	fontName{ L"Tahoma" }, miniFontName{ L"Arial" };
		int				fontSize{ 18 }, miniFontSize{ 11 };
	}
}

void TemClockD2D::LoadCfg(std::wstring const & filePath)
{
	FILE *fp{};
	if (_wfopen_s(&fp, filePath.c_str(), L"r") != 0) {
		throw Except(ExceptReason::FileOpenReadFailed);
	}
	auto xml = mxmlLoadFile(nullptr, fp, MXML_OPAQUE_CALLBACK);
	fclose(fp);
	if (!xml) {
		throw Except(ExceptReason::MxmlOpenFileFailed);
	}
	auto root = mxmlFindPath(xml, "temClockD2D");
	if (!root) {
		throw Except(ExceptReason::InvalidCfgData,
			L"Can't find <TemClock>.");
	}
	auto fontNameVal = mxmlGetOpaque(mxmlFindPath(root, "FontName"));
	if (!fontNameVal) {
		throw Except(ExceptReason::InvalidCfgData,
			L"Can't find <FontName> or bad value.");
	}
	Cfg::fontName = WideFromUtf8(fontNameVal);
	auto fontSizeVal = mxmlGetOpaque(mxmlFindPath(root, "FontSize"));
	if (!fontSizeVal || !TryParseInt(fontSizeVal, &Cfg::fontSize)) {
		throw Except(ExceptReason::InvalidCfgData,
			L"Can't find <FontSize> or bad value.");
	}
	auto miniFontNameVal = mxmlGetOpaque(mxmlFindPath(root, "MiniFontName"));
	if (!miniFontNameVal) {
		throw Except(ExceptReason::InvalidCfgData,
			L"Can't find <MiniFontName> or bad value.");
	}
	Cfg::miniFontName = WideFromUtf8(miniFontNameVal);
	auto miniFontSizeVal = mxmlGetOpaque(mxmlFindPath(root, "MiniFontSize"));
	if (!miniFontSizeVal || !TryParseInt(miniFontSizeVal, &Cfg::miniFontSize)) {
		throw Except(ExceptReason::InvalidCfgData,
			L"Can't find <MiniFontSize> or bad value.");
	}
}

void TemClockD2D::SaveCfg(std::wstring const & filePath)
{
	auto xml = mxmlNewXML("1.0");
	auto root = mxmlNewElement(xml, "temClockD2D");
	mxmlNewOpaque(
		mxmlNewElement(root, "FontName"),
		Utf8FromWide(Cfg::fontName).c_str());
	mxmlNewOpaque(
		mxmlNewElement(root, "FontSize"),
		std::to_string(Cfg::fontSize).c_str());
	mxmlNewOpaque(
		mxmlNewElement(root, "MiniFontName"),
		Utf8FromWide(Cfg::miniFontName).c_str());
	mxmlNewOpaque(
		mxmlNewElement(root, "MiniFontSize"),
		std::to_string(Cfg::miniFontSize).c_str());
	FILE *fp{};
	if (_wfopen_s(&fp, filePath.c_str(), L"w") != 0) {
		throw Except(ExceptReason::FileOpenWriteFailed);
	}
	if (mxmlSaveFile(xml, fp, MXML_NO_CALLBACK) == -1) {
		throw Except(ExceptReason::MxmlSaveFileFailed);
	}
	fclose(fp);
}
