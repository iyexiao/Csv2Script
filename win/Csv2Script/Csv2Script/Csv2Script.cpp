// Csv2Script.cpp : �������̨Ӧ�ó������ڵ㡣
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

