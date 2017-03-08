// Csv2Script.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "../../../Classes/CsvToScript.h"


int main(int argc, const char * argv[])
{
	if (argc != 4) {
		return -1;
	}

	CsvToScript *ctoScript = new CsvToScript();
	ctoScript->csvToScript(argv[1], argv[2], argv[3]);
	delete ctoScript;
	return 0;
}

