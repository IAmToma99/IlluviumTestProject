#include "MapGenerator.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

AMapGenerator::AMapGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

    InstancedStaticMeshComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("InstancedStaticMeshComponent"));
    SetRootComponent(InstancedStaticMeshComponent);
}

void AMapGenerator::GenerateMap() const
{
    if (!HexMesh)
        return;
    
    InstancedStaticMeshComponent->SetStaticMesh(HexMesh);
    InstancedStaticMeshComponent->SetMaterial(0, HexMaterial);
    InstancedStaticMeshComponent->ClearInstances();
    
    for (int32 Q = -Radius; Q <= Radius; Q++)
    {
        int32 R1 = FMath::Max(-Radius, -Q - Radius);
        int32 R2 = FMath::Min(Radius, -Q + Radius);
        
        for (int32 R = R1; R <= R2; R++)
        {
            const FVector SpawnLocation = HexToWorld(Q, R);
            const FTransform TileTransform(FRotator::ZeroRotator, SpawnLocation, FVector(1.f));
            InstancedStaticMeshComponent->AddInstance(TileTransform);
        }
    }
}

FVector AMapGenerator::HexToWorld(int32 Q, int32 R) const
{
    float X = HexSize * 1.5f * Q;
    float Y = HexSize * FMath::Sqrt(3.f) * (R + Q / 2.f);
    return FVector(X, Y, 0);
}