//
//  main.cpp
//  Csv2Script
//
//  Created by yexiao on 17/3/6.
//  Copyright © 2017年 yexiao. All rights reserved.
//

#include <iostream>
#include "CsvToScript.h"

int main(int argc, const char * argv[]) {
    if (argc != 4) {
        return -1;
    }
    
    CsvToScript *ctoScript = new CsvToScript();
    ctoScript->csvToScript(argv[1],argv[2],argv[3]);
    delete ctoScript;
    return 0;
}
