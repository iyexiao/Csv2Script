#include "GodProj.h"
#include "DB_Role.h"

UDB_Role::UDB_Role()
{

}
bool UDB_Role::loadData()
{
	FString path = FPaths::GameDir() + "Content/DB/DB_Role.txt";
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
		FRole dbS;
		dbS.id = FCString::Atoi(*array[0]);
		dbS.roleType = FCString::Atoi(*array[1]);
		dbS.roleSex = FCString::Atoi(*array[2]);
		dbS.roleDetail = *array[3];
 		if (FCString::Atoi(*array[4]) == 1)
			dbS.isOpen = true;
		else
			dbS.isOpen = false;
		m_map.Add(dbS.id, dbS);
	}
	return true;
}

FRole UDB_Role::getRoleById(int32 _id)
{
	return m_map.FindRef(_id);
}
