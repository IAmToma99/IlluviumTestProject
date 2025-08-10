#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapGenerator.generated.h"

class UHierarchicalInstancedStaticMeshComponent;

UCLASS()
class ILLUVIUMTEST_API AMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:
	
	AMapGenerator();

protected:

	UFUNCTION(CallInEditor, Category = "Map Generator")
	void GenerateMap() const;
	
	UPROPERTY(VisibleAnywhere)
	UHierarchicalInstancedStaticMeshComponent* InstancedStaticMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Map Generator")
	UStaticMesh* HexMesh;

	UPROPERTY(EditAnywhere, Category = "Map Generator")
	UMaterialInterface* HexMaterial;
	
	UPROPERTY(EditAnywhere, Category = "Map Generator")
	int32 Radius = 50;

	UPROPERTY(EditAnywhere, Category = "Map Generator")
	float HexSize = 100.f;

private:
	
	FVector HexToWorld(int32 Q, int32 R) const;

};
