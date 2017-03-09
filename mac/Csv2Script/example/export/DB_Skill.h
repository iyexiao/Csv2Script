#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "DB_Skill.generated.h"

USTRUCT(BlueprintType)
struct FSkill
{
    GENERATED_USTRUCT_BODY()
public:
    FSkill(){};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DATA_DB")
    int32 id;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DATA_DB")
    int32 petId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DATA_DB")
    int32 skillId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DATA_DB")
    FString skillName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DATA_DB")
    FString skillEffect;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DATA_DB")
    bool isEffect;
};


UCLASS(Blueprintable)
class GODPROJ_API UDB_Skill : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UDB_Skill();
	~UDB_Skill(){};
    bool loadData();


	UFUNCTION(BlueprintCallable, Category = "DATA_DB")
    static FSkill getSkillById(int32 _id);
	UFUNCTION(BlueprintCallable, Category = "DATA_DB")
    static TArray<FSkill> getAllSkillDB();
};
