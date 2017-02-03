#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <Windows.h>
#include <mxml.h>

using namespace std;

auto const xmlFilePath = "_MyOutput.xml";

inline int mbox(
	LPCSTR text,
	LPCSTR title = nullptr,
	UINT uTypes = MB_ICONINFORMATION)
{
	return MessageBoxA(nullptr,
		text ? text : "nullptr",
		title ? title : "nullptr",
		uTypes);
}

void TestOpen() {
	auto file = fopen(xmlFilePath, "r");
	if (!file) { throw "Failed to fopen (read mode)"; }
	auto xmlRoot = mxmlLoadFile(nullptr, file, MXML_OPAQUE_CALLBACK);
	if (!xmlRoot) { throw "Failed to mxmlLoadFile"; }
	fclose(file);

	auto xmlElement1 = mxmlFindElement(xmlRoot, xmlRoot,
		"Element1", nullptr, nullptr, MXML_DESCEND_FIRST);
	if (!xmlElement1) { throw "Failed to mxmlFindElement for Element1"; }
	mbox(mxmlGetOpaque(xmlElement1));

	auto xmlCCC = mxmlFindPath(xmlRoot, "Element2/CanContainChild");
	if (xmlCCC) {
		mbox(xmlCCC->value.opaque, xmlCCC->parent->value.opaque);
	}
}

void TestSave() {
	auto file = fopen(xmlFilePath, "w");
	if (!file) { throw "Failed to fopen (write mode)"; }
	auto xmlRoot = mxmlNewElement(nullptr, "root");
	auto xmlElement1 = mxmlNewElement(xmlRoot, "Element1");
	mxmlNewText(xmlElement1, 0, "With-some-text");

	auto xmlElement2 = mxmlNewElement(xmlRoot, "Element2");
	mxmlNewText(
		mxmlNewElement(xmlElement2, "CanContainChild"), 0, "true");
	mxmlNewText(
		mxmlNewElement(xmlElement2, "PreferVisualStudio"), 0, "TRUE");

	auto ret = mxmlSaveFile(xmlRoot, file, MXML_NO_CALLBACK);
	if (ret == -1) { throw "Failed to mxmlSaveFile"; }
	fclose(file);
}

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	try {
		TestSave();
		TestOpen();
	}
	catch (char const *errMsg) {
		MessageBoxA(nullptr, errMsg, nullptr, MB_ICONERROR);
		return 1;
	}
	return 0;
}
