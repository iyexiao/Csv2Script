#include "GodProj.h"
#include "DB_Skill.h"

static TMap<int32, FSkill> m_map;

UDB_Skill::UDB_Skill()
{
     loadData();
}
bool UDB_Skill::loadData()
{
    m_map.Empty();
	FString path = FPaths::GameDir() + "Content/DB/DB_Skill.txt";
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*path))
		return false;
	TArray<FString> db;
	FString contentStr;
	FFileHelper::LoadFileToString(contentStr,*path);
	contentStr.ParseIntoArray(db, TEXT("\n"), false);
	for (int i = 0; i < db.Num(); i++)
	{
		FString aString = db[i];
		TArray<FString> array = {};
		aString.ParseIntoArray(array, TEXT(","), false);
		FSkill dbS;
		dbS.id = FCString::Atoi(*array[0]);
		dbS.petId = FCString::Atoi(*array[1]);
		dbS.skillId = FCString::Atoi(*array[2]);
		dbS.skillName = *array[3];
		dbS.skillEffect = *array[4];
 		if (FCString::Atoi(*array[5]) == 1)
			dbS.isEffect = true;
		else
			dbS.isEffect = false;
		m_map.Add(dbS.id, dbS);
	}
	return true;
}

FSkill UDB_Skill::getSkillById(int32 _id)
{
	return m_map.FindRef(_id);
}
TArray<FSkill> UDB_Skill::getAllSkillDB()
{
    TArray<FSkill> db;
    for (TPair<int32,FSkill>& element : m_map)
    {
        db.Add(element.Value);
    }
    return db;
