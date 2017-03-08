#pragma once

#include "DB_Skill.h"
#include "DB_Role.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DataSingletonManager.generated.h"

UCLASS(Blueprintable)
class GODPROJ_API UDataSingletonManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
private:
	static UDB_Role* m_DB_Role;
	static UDB_Skill* m_DB_Skill;
public:
	UFUNCTION(BlueprintCallable, Category = "DATA_DB")
	static UDB_Role* GetDB_Role();
	UFUNCTION(BlueprintCallable, Category = "DATA_DB")
	static UDB_Skill* GetDB_Skill();
};
