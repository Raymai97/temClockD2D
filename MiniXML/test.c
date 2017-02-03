#include "src/mxml.h"
#include <Windows.h>

int main() {
	FILE *file = NULL;
	mxml_node_t *xml = NULL, *data = NULL;
	xml = mxmlNewXML("1.0");
	data = mxmlNewElement(xml, "data");
	mxmlNewText(
		mxmlNewElement(data, "sound")
		, 0, "on");
	mxmlNewText(
		mxmlNewElement(data, "bullshit")
		, 0, "off");
	file = fopen("test_output.xml", "w");
	if (!file) {
		printf("fopen failed...\n");
		return 1;
	}
	if(-1 == mxmlSaveFile(xml, file, MXML_NO_CALLBACK)) {
		printf("mxmlSaveFile failed...\n");
		return 1;
	}
	fclose(file);
	printf("Program ended without error.\n");
	return 0;
}
