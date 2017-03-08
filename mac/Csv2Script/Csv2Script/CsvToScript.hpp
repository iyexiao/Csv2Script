//
//  CsvToScript.hpp
//  Csv2Script
//
//  Created by yexiao on 17/3/6.
//  Copyright © 2017年 yexiao. All rights reserved.
//

#ifndef CsvToScript_hpp
#define CsvToScript_hpp

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

using namespace std;

// class XmlToScript is an interface for script language.
// such as Lua, Javascript.
class CsvToScript {
private:
    string m_dir;
    string m_proj;
    vector<string> typeVector;//表格数据类型
public:
    CsvToScript();
    ~CsvToScript(){};
    void csvToScript(string csvFile,string path,string proj);
    
    bool readFile(const string &filename, string &content);
    vector<string> splitStringByDelim (const string str, const string delim);
    vector<string> parseCsvLine(const string str);
    
    void writeHeadScript(string csvFile,vector<string> attrinames,vector<string> attributes);
    void writeCppScript(string csvFile,vector<string> attrinames,vector<string> attributes);
};
#endif /* CsvToScript_hpp */
