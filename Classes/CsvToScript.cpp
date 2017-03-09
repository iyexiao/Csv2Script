
#include "CsvToScript.h"
#include <sys/stat.h>
#include <stdlib.h>

CsvToScript::CsvToScript(){
    typeVector = {"string","int","bool"};//now only three type,bool just contain 0 or 1
}
// read file 
bool CsvToScript::readFile(const string &filename, string &content)
{
    content.clear();


    struct stat st;
    
    if (stat (filename.c_str(), &st) >= 0) {

		//char *pFileContent = (char *)malloc(st.st_size + 1);
		//FILE *outfile = fopen(filename.c_str(), "r");
		//fread(pFileContent, sizeof(char), st.st_size, outfile);
		//fclose(outfile);

        char *pFileContent = (char *)malloc(st.st_size+1);
        if (pFileContent == NULL) {
            cout << "Error: failed to malloc size= " << st.st_size << " memory." << endl;
            exit(-1);
        }
        pFileContent[st.st_size] = '\0';
        FILE *fp_cfg = fopen(filename.c_str(), "r");
        int count = 0;
        while (count < st.st_size) {
            fread(pFileContent, 1, st.st_size, fp_cfg);
			count++;
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
    // expect csv
    string::size_type pos = csvFile.find(".csv");
    if (pos == string::npos || pos != (csvFile.length() - 4)) {
        cout << "Warning: " << m_dir << "/" << csvFile << " is not a regular csv file." << endl;
		exit(-1);
    }
    // get path
    string csvFullFilename(m_dir);
    csvFullFilename.append("/csv/");
    csvFullFilename.append(csvFile);
    
    vector<string> attributes;//type
    vector<string> attrinames;//value name
    string txtContent;//out txt


    string cfgFileContent;
    vector<string> fileLines;
    // read csv
    readFile (csvFullFilename, cfgFileContent);
    if (cfgFileContent.length() > 0) {
        vector<string> tmpV = splitStringByDelim(cfgFileContent, "\n");
        
        if (tmpV.size() < 4) {
            cout << "Error. File " << csvFullFilename << " \'s row is illegal. we need at least 4 rows. but the file only contains " << tmpV.size() << " rows." << endl;
            exit(-1);
        }
        
        vector<string>::iterator itr=tmpV.begin();
        // expect first cow
        itr ++;
        // parse type
        attributes = parseCsvLine(*itr);
        // check type and name
        for (int i = 0; i < attributes.size(); i ++) {
            bool isHave = false;
            for (int j = 0; j<typeVector.size(); j++) {
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
        //get value name
        attrinames = parseCsvLine(*itr);
        
        if (attrinames.size() != attributes.size()) {
            cout << "Error. File " << csvFullFilename << ". The type and attribute are different." << endl;
            exit(-1);
        }
        itr ++;
        for (; itr !=tmpV.end(); itr++){
            if ((*itr).compare("") == 0) {
                //none data
                continue;
            }
            if (txtContent.compare("")!=0) {
                txtContent.append("\n");//add \n when first cow
            }
            vector<string> tmpV01 = parseCsvLine(*itr);
            for (int i = 0; i < tmpV01.size(); i ++) {
                string value = tmpV01.at(i);
                //check first row is int 
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
}

void CsvToScript::writeHeadScript(vector<string> attrinames,vector<string> attributes){
    string PROJ = m_proj;
    
    //upper
    for (int i = 0; i < PROJ.size(); i++) {
        PROJ[i] = toupper(PROJ[i]);
    }
    //write .h
    string className = "DB_";
    className.append(fileName);
    //write include
    string dataStr;
    dataStr.append("#pragma once\n\n");
    dataStr.append("#include \"Kismet/BlueprintFunctionLibrary.h\"\n");
    dataStr.append("#include \"").append(className).append(".generated.h\"\n\n");
    //write struct
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
    //write class
    dataStr.append("UCLASS(Blueprintable)\n");
    dataStr.append("class ").append(PROJ).append("_API UDB_").append(fileName).append(" : public UBlueprintFunctionLibrary\n");
    dataStr.append("{\n");
    dataStr.append("	GENERATED_BODY()\n");
    dataStr.append("public:\n\n");
    dataStr.append("	UDB_").append(fileName).append("();\n");
    dataStr.append("	~UDB_").append(fileName).append("(){};\n");
    dataStr.append("    bool loadData();\n");
    dataStr.append("\n\n");
    dataStr.append("	UFUNCTION(BlueprintCallable, Category = \"DATA_DB\")\n");
    dataStr.append("    static ").append(structName).append(" get").append(fileName).append("ById(int32 _id);\n");
    dataStr.append("	UFUNCTION(BlueprintCallable, Category = \"DATA_DB\")\n");
    dataStr.append("    static TArray<").append(structName).append("> getAll").append(fileName).append("DB();\n");
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
    //upper
    for (int i = 0; i < PROJ.size(); i++) {
        PROJ[i] = toupper(PROJ[i]);
    }
    
    string className = "DB_";
    className.append(fileName);
    
    string dataStr= "";
    dataStr.append("#include \"").append(m_proj).append(".h\"\n");
    dataStr.append("#include \"").append(className).append(".h\"\n\n");
    //
    
    dataStr.append("static TMap<int32, ").append(structName).append("> m_map;\n\n");
    dataStr.append("UDB_").append(fileName).append("::UDB_").append(fileName).append("()\n");
    dataStr.append("{\n     loadData();\n}\n");
    // add loadData function
    dataStr.append("bool UDB_").append(fileName).append("::loadData()\n");
    dataStr.append("{\n");
    dataStr.append("    m_map.Empty();\n");
    dataStr.append("	FString path = FPaths::GameDir() + \"Content/DB/DB_").append(fileName).append(".txt\";\n");
    dataStr.append("	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*path))\n");
    dataStr.append("		return false;\n");
    dataStr.append("	TArray<FString> db;\n");
    dataStr.append("	FString contentStr;\n");
    dataStr.append("	FFileHelper::LoadFileToString(contentStr,*path);\n");
    dataStr.append("	contentStr.ParseIntoArray(db, TEXT(\"\\n\"), false);\n");
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
    //add getXXXById() function
    dataStr.append(structName).append(" UDB_").append(fileName).append("::get").append(fileName).append("ById(int32 _id)\n");
    dataStr.append("{\n");
    dataStr.append("	return m_map.FindRef(_id);\n");
    dataStr.append("}\n");
    //add getAllXXXDB function
    dataStr.append("TArray<").append(structName).append("> UDB_").append(fileName).append("::getAll").append(fileName).append("DB()\n");
    dataStr.append("{\n");
    dataStr.append("    TArray<").append(structName).append("> db;\n");
    dataStr.append("    for (TPair<int32,F").append(fileName).append(">& element : m_map)\n");
    dataStr.append("    {\n");
    dataStr.append("        db.Add(element.Value);\n");
    dataStr.append("    }\n");
    dataStr.append("    return db;\n");
    
    
    //write .cpp
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
