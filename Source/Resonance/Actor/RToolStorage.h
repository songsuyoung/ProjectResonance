#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Resonance/Data/ResonanceEnums.h"
#include "Resonance/Data/ResonanceStructs.h"
#include "RToolStorage.generated.h"

UCLASS()
class RESONANCE_API ARToolStorage : public AActor
{
	GENERATED_BODY()

public:
	ARToolStorage();

	virtual void BeginPlay() override;

	ERToolType GetToolType() const { return ToolType; }
	const FRToolMeshData& GetToolMeshData() const { return ToolMeshData; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tool|Settings")
	ERToolType ToolType = ERToolType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tool|Settings")
	FRToolMeshData ToolMeshData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> MeshComponent;
};
