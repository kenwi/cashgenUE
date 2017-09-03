#pragma once
#include "cashgen.h"
#include "UFNNoiseGenerator.h"
#include "Struct/CGLODConfig.h"
#include "UFNBlendModule.h"
#include "CGTerrainConfig.generated.h"


/** Struct defines all applicable attributes for managing generation of a single zone */
USTRUCT(BlueprintType)
struct FCGTerrainConfig
{
	GENERATED_USTRUCT_BODY()
		/** Noise Generator configuration struct */
	UPROPERTY()
		UUFNNoiseGenerator* NoiseGenerator;
	UPROPERTY()
		UUFNNoiseGenerator* BiomeBlendGenerator;
	/** Number of blocks along a zone's X axis */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashGen | Main")
		int32 TileXUnits = 32;
	/** Number of blocks along a zone's Y axis */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashGen | Main")
		int32 TileYUnits = 32;
	/** Size of a single block in world units */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashGen | Main")
		float UnitSize = 300.0f;
	/** Multiplier for heightmap*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "cashGen | Main")
		float Amplitude = 5000.0f;
	/** Droplet erosion droplet amount *EXPERIMENTAL* **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashGen | Erosion")
		int32 DropletAmount = 0;
	/** Droplet erosion deposition rate *EXPERIMENTAL* **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashGen | Erosion")
		float DropletErosionMultiplier = 1.0f;
	/** Droplet erosion deposition rate *EXPERIMENTAL* **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashGen | Erosion")
		float DropletDespositionMultiplier = 1.0f;
	/** Droplet erosion deposition Theta *EXPERIMENTAL* **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashGen | Erosion")
		float DropletSedimentCapacity = 10.0f;
	/** Droplet erosion evaporation rate *EXPERIMENTAL* **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashGen | Erosion")
		float DropletEvaporationRate = 0.1f;
	/** Erosion floor cutoff **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashGen | Erosion")
		float DropletErosionFloor = 0.0f;
	/** Size of MeshData pool */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashGen | Main")
		uint8 MeshDataPoolSize = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashGen | Main")
		uint8 NumberOfThreads = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashGen | Main")
		uint8 MeshUpdatesPerFrame = 1;
	/** Material for the terrain mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashGen | Materials")
	UMaterial* TerrainMaterial;
	/** If checked and numLODs > 1, material will be instanced and TerrainOpacity parameters used to dither LOD transitions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashGen | Materials")
	bool  DitheringLODTransitions;
	/** If no TerrainMaterial and LOD transitions disabled, just use the same static instance for all LODs **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashGen | Materials")
	UMaterialInstance* TerrainMaterialInstance;
	/** Make a dynamic material instance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashGen | Materials")
	bool MakeDynamicMaterialInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashGen | LODs")
	TArray<FCGLODConfig> LODs;

};