#include "GodProj.h"
#include "DB_Skill.h"

UDB_Skill::UDB_Skill()
{
	loadData();
}
bool UDB_Skill::loadData()
{
	FString path = FPaths::GameDir() + "Content/DB/DB_Skill.txt";
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*path))
		return false;
	TArray<FString> db;
	FString contentStr;
	FFileHelper::LoadFileToString(contentStr,*path);
	contentStr.ParseIntoArray(db, TEXT("\\n"), false);
	for (int i = 0; i < db.Num(); i++)
	{
		FString aString = db[i];
		TArray<FString> array = {};
		aString.ParseIntoArray(array, TEXT(","), false);
		Skill dbS;
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

Skill UDB_Skill::getSkillById(int32 _id)
{
	return m_map.FindRef(_id);
}
