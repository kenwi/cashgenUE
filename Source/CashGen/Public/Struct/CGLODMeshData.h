#pragma once
#include "CGLODMeshData.generated.h"

USTRUCT(BlueprintType)
struct FCGLODMeshData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCGMeshData> Data;

};