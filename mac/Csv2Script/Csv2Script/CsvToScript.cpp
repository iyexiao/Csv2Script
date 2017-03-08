//
//  CsvToScript.cpp
//  Csv2Script
//
//  Created by yexiao on 17/3/6.
//  Copyright © 2017年 yexiao. All rights reserved.
//

#include "CsvToScript.hpp"
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

CsvToScript::CsvToScript(){
    typeVector = {"string","int","bool"};//目前只允许这三个类型,bool值只能填0，1
}
// 读文件内容
bool CsvToScript::readFile(const string &filename, string &content)
{
    content.clear();
    struct stat st;
    
    if (stat (filename.c_str(), &st) >= 0) {
        char *pFileContent = (char *)malloc(st.st_size+1);
        if (pFileContent == NULL) {
            cout << "Error: failed to malloc size= " << st.st_size << " memory." << endl;
            exit(-1);
        }
        pFileContent[st.st_size] = '\0';
        FILE *fp_cfg = fopen(filename.c_str(), "r");
        int count = 0;
        while (count < st.st_size) {
            count += fread(pFileContent, 1, st.st_size, fp_cfg);
            usleep(1000);
        }
        fclose(fp_cfg);
        
        content.append(pFileContent);
        free (pFileContent);
    } else {
        cout << "Error: can`t find file: " << filename << endl;
        return false;
    }
    
    return true;
}
vector<string> CsvToScript::splitStringByDelim (const string str, const string delim)
{
    vector<string> vct;
    string::size_type prev_pos=0, curr_pos=0;
    
    while ((curr_pos = str.find(delim, curr_pos)) != string::npos) {
        string sub_str(str.substr(prev_pos, curr_pos-prev_pos));
        curr_pos += delim.length();
        prev_pos = curr_pos;
        vct.push_back(sub_str);
    }
    
    return vct;
}
vector<string> CsvToScript::parseCsvLine(const string str)
{
    vector<string> vct;
    char *pos01=NULL, *pos02=NULL;
    int dmq = 0;
    
    char *str00 = (char*) malloc(str.length()+1);
    str00[str.length()] = 0;
    strncpy(str00, str.c_str(), str.length());
    
    char *pstr00 = NULL;
    pstr00 = str00;
    long offset = 0;
    while((pos01 = strchr(str00 + offset, ',')) != NULL) {
        dmq = 0;
        char *str02 = NULL;
        char *pstr02 = NULL;
        if (pos01 != str00) {
            str02 = (char *) malloc(pos01 - str00 + 1);
            strncpy(str02, str00, pos01 - str00);
            str02[pos01 - str00] = 0;
        }
        pstr02 = str02;
        while (str02 != NULL && (pos02 = strchr(str02, '"')) != NULL) {
            dmq ++;
            str02 = pos02 +1;
        }
        if (pstr02)
            free(pstr02);
        if (dmq%2 == 1) {
            offset = pos01-str00+1;
            continue;
        }
        char* str01 = NULL;
        
        if (pos01 != str00) {
            str01 = (char *) malloc(pos01 - str00 + 1);
            strncpy(str01, str00, pos01 - str00);
            str01[pos01 - str00] = 0;
        }
        char *pstr01 = str01;
        str00 = pos01 + 1;
        offset = 0;
        
        if (str01) {
            if (str01[0] == 0x22)
                str01 ++;
            if (str01[strlen(str01)-1] == 0x22)
                str01[strlen(str01)-1] = 0;
            vct.push_back(str01);
            free(pstr01);
        } else {
            vct.push_back("");
        }
    }
    if (str00 - pstr00 != str.length()) {
        string finalstr = str.substr(str00-pstr00);
        if (finalstr.length() > 0 && finalstr.c_str()[0] == '"') {
            finalstr.erase(0, 1);
        }
        if (finalstr.length() > 0 && finalstr.c_str()[finalstr.length()-1] == '"') {
            finalstr.erase(finalstr.length()-1);
        }
        vct.push_back(finalstr);
    }
    
    free(pstr00);
    
    return vct;
}
void string_replace(string&s1,const string&s2,const string&s3)
{
    string::size_type pos=0;
    string::size_type a=s2.size();
    string::size_type b=s3.size();
    while((pos=s1.find(s2,pos))!=string::npos)
    {
        s1.replace(pos,a,s3);
        pos+=b;
    }
}
void CsvToScript::csvToScript(string csvFile,string path,string proj){
    string sStr = csvFile;
    sStr[0] = toupper(sStr[0]);
    string_replace(sStr,".csv","");
    
    m_dir = path;
    m_proj = proj;
    fileName = sStr;
    
    structName = "F";
    structName.append(fileName);
    
    // 判断是否是xml后缀，非xml后缀则过滤掉
    string::size_type pos = csvFile.find(".csv");
    if (pos == string::npos || pos != (csvFile.length() - 4)) {
        cout << "Warning: " << m_dir << "/" << csvFile << " is not a regular csv file." << endl;
        return;
    }
    // 构建Xml文件的完整路径
    string csvFullFilename(m_dir);
    csvFullFilename.append("/csv/");
    csvFullFilename.append(csvFile);
    
    vector<string> attributes;//类型
    vector<string> attrinames;//属性名称
    string txtContent;//输出的txt数据
    int index = 0;//数据
    
    string cfgFileContent;
    vector<string> fileLines;
    // 读取csv文件内容
    readFile (csvFullFilename, cfgFileContent);
    if (cfgFileContent.length() > 0) {
        vector<string> tmpV = splitStringByDelim(cfgFileContent, "\n");
        
        if (tmpV.size() < 4) {
            cout << "Error. File " << csvFullFilename << " \'s row is illegal. we need at least 4 rows. but the file only contains " << tmpV.size() << " rows." << endl;
            exit(-1);
        }
        
        vector<string>::iterator itr=tmpV.begin();
        // 过滤第一行，第一行为注释行
        itr ++;
        // 解析第二行数据，取得该列数据的“类型”。
        attributes = parseCsvLine(*itr);
        //判断类型是否正确
        for (int i = 0; i < attributes.size(); i ++) {
            bool isHave = false;
            for (int j = 0; j<typeVector.size(); j++) {
//                cout<<"sss"<<attributes.at(i)<<typeVector.at(j)<<"--\n"<<endl;
                if (attributes.at(i).compare(typeVector.at(j)) == 0) {
                    isHave = true;
                    break;
                }
            }
            if (!isHave) {
                cout << "Error. type " << csvFullFilename << ". The type."<<attributes.at(i)<< endl;
                exit(-1);
            }
        }
        itr ++;
        //获取CSV文件第3行数据，第3行为属性名字
        attrinames = parseCsvLine(*itr);
        
        if (attrinames.size() != attributes.size()) {
            cout << "Error. File " << csvFullFilename << ". The type and attribute are different." << endl;
            exit(-1);
        }
        itr ++;
        for (; itr !=tmpV.end(); itr++){
            if ((*itr).compare("") == 0) {
                //若该行没数据，则跳过
                continue;
            }
            if (txtContent.compare("")!=0) {
                txtContent.append("\n");//有数据，则换行变成下一条数据
            }
            vector<string> tmpV01 = parseCsvLine(*itr);
            for (int i = 0; i < tmpV01.size(); i ++) {
                string value = tmpV01.at(i);
                //判断第一行不能为空，并且为数字
                if (i == 0) {
                    if (value.compare("")==0 || attributes.at(0).compare("int")!=0 || std::atoi(value.c_str()) <= 0) {
                        cout << "Error. the  " << attributes.at(0) << " is null. or not type int" << endl;
                        exit(-1);
                    }
                }
                if (attributes.at(i).compare("bool")==0) {
                    if (!(value.compare("0") == 0||value.compare("1")==0)) {
                        cout << "Error. File " << csvFullFilename << ". The type bool error." << endl;
                        exit(-1);
                    }
                }
                else if(attributes.at(i).compare("string")==0){
//                    string_replace(value,"\"","\"");
                    string_replace(value,",",".");
                }else if (attributes.at(i).compare("int")==0){
                    if (value.compare("") == 0) {
                        value = "0";
                    }
                }
                txtContent.append(value).append(",");
                
            }
            index ++;
        }
    }
    string outFullFilename(m_dir);
    outFullFilename.append("/export/DB_");
    outFullFilename.append(fileName).append(".txt");
    
    FILE *fp = fopen (outFullFilename.c_str(), "w");
    if (fp == NULL) {
        printf ("Error: failed to open file %s, %s\n", outFullFilename.c_str(), strerror(errno));
        exit(-1);
    }
    fprintf(fp, "%s", txtContent.c_str());
    
    fflush(fp);
    fclose(fp);
    
    
    writeHeadScript(attrinames, attributes);
    writeCppScript(attrinames,attributes);
    wirteDataSingletonManager();
}

void CsvToScript::writeHeadScript(vector<string> attrinames,vector<string> attributes){
    string PROJ = m_proj;
    
    //转换为大写
    for (int i = 0; i < PROJ.size(); i++) {
        PROJ[i] = toupper(PROJ[i]);
    }
    //写入.h文件
    string className = "DB_";
    className.append(fileName);
    //写入头文件引用
    string dataStr;
    dataStr.append("#pragma once\n\n");
    dataStr.append("#include \"UObject/NoExportTypes.h\"\n");
    dataStr.append("#include \"").append(className).append(".generated.h\"\n\n");
    //写入数据结构体
    dataStr.append("USTRUCT(BlueprintType)\n");
    dataStr.append("struct ").append(structName).append("\n");
    dataStr.append("{\n");
    dataStr.append("    GENERATED_USTRUCT_BODY()\n");
    dataStr.append("public:\n");
    dataStr.append("    ").append(structName).append("(){};\n");
    for (int i =0; i<attrinames.size(); i++) {
        string typeStr;
        if (attributes.at(i).compare("int") == 0) {
            typeStr = "int32";
        }else if (attributes.at(i).compare("string") == 0){
            typeStr = "FString";
        }else if(attributes.at(i).compare("bool") == 0){
            typeStr = "bool";
        }
        dataStr.append("\n");
        dataStr.append("    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = \"DATA_DB\")\n");
        dataStr.append("    ").append(typeStr).append(" ").append(attrinames.at(i)).append(";\n");
    }
    dataStr.append("};\n\n\n");
    //写入类
    dataStr.append("UCLASS(Blueprintable)\n");
    dataStr.append("class ").append(PROJ).append("_API UDB_").append(fileName).append(" : public UObject\n");
    dataStr.append("{\n");
    dataStr.append("	GENERATED_BODY()\n");
    dataStr.append("public:\n");
    dataStr.append("	TMap<int32, ").append(structName).append("> m_map;\n\n");
    dataStr.append("	UDB_").append(fileName).append("();\n");
    dataStr.append("	~UDB_").append(fileName).append("(){};\n");
    dataStr.append("    bool loadData();\n");
    dataStr.append("\n\n");
    dataStr.append("	UFUNCTION(BlueprintCallable, Category = \"DATA_DB\")\n");
    dataStr.append("    ").append(structName).append(" get").append(fileName).append("ById(int32 _id);\n");
    dataStr.append("};\n");
    
    
    string outHeadFilename(m_dir);
    outHeadFilename.append("/export/DB_");
    outHeadFilename.append(fileName).append(".h");
    
    FILE *fp = fopen (outHeadFilename.c_str(), "w");
    if (fp == NULL) {
        printf ("Error: failed to open file %s, %s\n", outHeadFilename.c_str(), strerror(errno));
        exit(-1);
    }
    fprintf(fp, "%s", dataStr.c_str());
    
    fflush(fp);
    fclose(fp);

}
void CsvToScript::writeCppScript(vector<string> attrinames, vector<string> attributes)
{
    string PROJ = m_proj;
    //转换为大写
    for (int i = 0; i < PROJ.size(); i++) {
        PROJ[i] = toupper(PROJ[i]);
    }
    
    string className = "DB_";
    className.append(fileName);
    
    string dataStr= "";
    dataStr.append("#include \"").append(m_proj).append(".h\"\n");
    dataStr.append("#include \"").append(className).append(".h\"\n\n");
    //构造方法
    dataStr.append("UDB_").append(fileName).append("::UDB_").append(fileName).append("()\n{\n\n}\n");
    //添加loadData方法
    dataStr.append("bool UDB_").append(fileName).append("::loadData()\n");
    dataStr.append("{\n");
    dataStr.append("	FString path = FPaths::GameDir() + \"Content/DB/DB_").append(fileName).append(".txt\";\n");
    dataStr.append("	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*path))\n");
    dataStr.append("		return false;\n");
    dataStr.append("	TArray<FString> db;\n");
    dataStr.append("	FString contentStr;\n");
    dataStr.append("	FFileHelper::LoadFileToString(contentStr,*path);\n");
    dataStr.append("	contentStr.ParseIntoArray(db, TEXT(\"\\\\n\"), false);\n");
    dataStr.append("	for (int i = 0; i < db.Num(); i++)\n");
    dataStr.append("	{\n");
    dataStr.append("		FString aString = db[i];\n");
    dataStr.append("		TArray<FString> array = {};\n");
    dataStr.append("		aString.ParseIntoArray(array, TEXT(\",\"), false);\n");
    dataStr.append("		").append(structName).append(" dbS;\n");
    for (int i =0; i<attrinames.size(); i++) {
        if (attributes.at(i).compare("int") == 0) {
            string valueStr="		dbS.";
            valueStr.append(attrinames.at(i));
            valueStr.append(" = FCString::Atoi(*array[").append(to_string(i)).append("]);\n");
            dataStr.append(valueStr);
        }else if (attributes.at(i).compare("string") == 0){
            string valueStr="		dbS.";
            valueStr.append(attrinames.at(i));
            valueStr.append(" = *array[").append(to_string(i)).append("];\n");
            dataStr.append(valueStr);
        }else if(attributes.at(i).compare("bool") == 0){
            string valueStr =" 		if (FCString::Atoi(*array[";
            valueStr.append(to_string(i)).append("]) == 1)\n");
            valueStr.append("			dbS.").append(attrinames.at(i)).append(" = true;\n");
            valueStr.append("		else\n");
            valueStr.append("			dbS.").append(attrinames.at(i)).append(" = false;\n");
            dataStr.append(valueStr);
        }
        
    }
    dataStr.append("		m_map.Add(dbS.").append(attrinames.at(0)).append(", dbS);\n");
    dataStr.append("	}\n");
    dataStr.append("	return true;\n");
    dataStr.append("}\n\n");
    //添加getXXXById方法

    dataStr.append(structName).append(" UDB_").append(fileName).append("::get").append(fileName).append("ById(int32 _id)\n");
    dataStr.append("{\n");
    dataStr.append("	return m_map.FindRef(_id);\n");
    dataStr.append("}\n");
    //写入.cpp文件
    string outHeadFilename(m_dir);
    outHeadFilename.append("/export/DB_");
    outHeadFilename.append(fileName).append(".cpp");
    FILE *fp = fopen (outHeadFilename.c_str(), "w");
    if (fp == NULL) {
        printf ("Error: failed to open file %s, %s\n", outHeadFilename.c_str(), strerror(errno));
        exit(-1);
    }
    fprintf(fp, "%s", dataStr.c_str());
    fflush(fp);
    fclose(fp);
}

void CsvToScript::wirteDataSingletonManager()
{
    //写h文件
    string hName =m_dir;
    hName.append("/export/DataSingletonManager.h");
    string contentStr;
    bool read = readFile(hName,contentStr);
    if (!read) {
         //文件不存在所以重新写入
        contentStr.append("#pragma once\n\n");
        contentStr.append("#include \"Kismet/BlueprintFunctionLibrary.h\"\n");
        contentStr.append("#include \"DataSingletonManager.generated.h\"\n\n");
        contentStr.append("UCLASS(Blueprintable)\n");
        string PROJ = m_proj;
        //转换为大写
        for (int i = 0; i < PROJ.size(); i++) {
            PROJ[i] = toupper(PROJ[i]);
        }
        contentStr.append("class ").append(PROJ).append("_API UDataSingletonManager : public UBlueprintFunctionLibrary\n");
        contentStr.append("{\n");
        contentStr.append("	GENERATED_BODY()\n");
        contentStr.append("private:\n");
        contentStr.append("public:\n");
        contentStr.append("};\n");
    }

    string newContent;
    vector<string> tmpV = splitStringByDelim(contentStr, "\n");
    vector<string>::iterator itr=tmpV.begin();
    
    for (; itr !=tmpV.end(); itr++){
        string tmp = (*itr);
        if (itr == tmpV.begin() + 2) {
            newContent.append("#include \"DB_").append(fileName).append(".h\"\n");
        }
        if (tmp.compare("public:") == 0) {
            newContent.append("	static UDB_").append(fileName).append("* m_DB_").append(fileName).append(";\n");
        }
        if (itr == tmpV.end() - 1) {
            newContent.append("	UFUNCTION(BlueprintCallable, Category = \"DATA_DB\")\n");
            newContent.append("	static UDB_").append(fileName).append("* GetDB_").append(fileName).append("();\n");
            
        }
        newContent.append(tmp).append("\n");
    }
    
    FILE *fp = fopen (hName.c_str(), "w");
    if (fp == NULL) {
        printf ("Error: failed to open file %s, %s\n", hName.c_str(), strerror(errno));
        exit(-1);
    }
    fprintf(fp, "%s", newContent.c_str());
    fflush(fp);
    fclose(fp);
    
    //写cpp文件
    string cppName =m_dir;
    cppName.append("/export/DataSingletonManager.cpp");
    string cppStr;
    bool readC = readFile(cppName,cppStr);
    if (!readC) {
        //文件不存在所以重新写入
        cppStr.append("#include \"GodProj.h\"\n");
        cppStr.append("#include \"DataSingletonManager.h\"\n\n");
    }
    
    cppStr.append("UDB_").append(fileName).append("* UDataSingletonManager::m_DB_").append(fileName).append("(NULL);\n");
    cppStr.append("UDB_").append(fileName).append("* UDataSingletonManager::GetDB_").append(fileName).append("()\n");
    cppStr.append("{");
    cppStr.append("    if (!m_DB_").append(fileName).append(")\n");
    cppStr.append("    {\n");
    cppStr.append("        m_DB_").append(fileName).append(" = NewObject<UDB_").append(fileName).append(">();\n");
    cppStr.append("        m_DB_").append(fileName).append("->loadData();\n");
    cppStr.append("    }\n");
    cppStr.append("    return m_DB_").append(fileName).append(";\n");
    cppStr.append("}\n\n");

    FILE *fpc = fopen (cppName.c_str(), "w");
    if (fpc == NULL) {
        printf ("Error: failed to open file %s, %s\n", cppName.c_str(), strerror(errno));
        exit(-1);
    }
    fprintf(fpc, "%s", cppStr.c_str());
    fflush(fpc);
    fclose(fpc);
}


