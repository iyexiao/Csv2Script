#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
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
class GODPROJ_API UDB_Role : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UDB_Role();
	~UDB_Role(){};
    bool loadData();


	UFUNCTION(BlueprintCallable, Category = "DATA_DB")
    static FRole getRoleById(int32 _id);
	UFUNCTION(BlueprintCallable, Category = "DATA_DB")
    static TArray<FRole> getAllRoleDB();
};
