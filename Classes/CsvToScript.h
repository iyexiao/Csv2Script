
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
    string structName;
    string fileName;
    vector<string> typeVector;//table save type
public:
    CsvToScript();
    ~CsvToScript(){};
    void csvToScript(string csvFile,string path,string proj);
    
    bool readFile(const string &filename, string &content);
    vector<string> splitStringByDelim (const string str, const string delim);
    vector<string> parseCsvLine(const string str);
    
    void writeHeadScript(vector<string> attrinames,vector<string> attributes);
    void writeCppScript(vector<string> attrinames,vector<string> attributes);
};

