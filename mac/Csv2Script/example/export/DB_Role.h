#pragma once

#include "UObject/NoExportTypes.h"
#include "DB_Role.generated.h"

USTRUCT(BlueprintType)
struct FRole
{
    GENERATED_USTRUCT_BODY()
public:
    FRole(){};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DATA_DB")
    int32 id;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DATA_DB")
    int32 roleType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DATA_DB")
    int32 roleSex;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DATA_DB")
    FString roleDetail;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DATA_DB")
    bool isOpen;
};


UCLASS(Blueprintable)
class GODPROJ_API UDB_Role : public UObject
{
	GENERATED_BODY()
public:
	TMap<int32, FRole> m_map;

	UDB_Role();
	~UDB_Role(){};
    bool loadData();


	UFUNCTION(BlueprintCallable, Category = "DATA_DB")
    FRole getRoleById(int32 _id);
};
