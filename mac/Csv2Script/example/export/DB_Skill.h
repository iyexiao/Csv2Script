#pragma once

#include "UObject/NoExportTypes.h"
#include "DB_Skill.generated.h"

USTRUCT(BlueprintType)
struct Skill
{
    GENERATED_USTRUCT_BODY()
public:
    Skill(){};

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
class GODPROJ_API UDB_Skill : public UObject
{
	GENERATED_BODY()
public:
	TMap<int32, Skill> m_map;

	UDB_Skill();
	~UDB_Skill(){};
    bool loadData();


	UFUNCTION(BlueprintCallable, Category = "DATA_DB")
    Skill getSkillById(int32 _id);
};
