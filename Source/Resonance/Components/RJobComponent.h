#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/StreamableManager.h"
#include "Resonance/Data/ResonanceEnums.h"
#include "Resonance/Data/ResonanceStructs.h"
#include "RJobComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RESONANCE_API URJobComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URJobComponent();

	virtual void BeginPlay() override;

public:
	void SetToolMesh(const FRToolMeshData& ToolMeshData);
	void SetPendingToolType(ERToolType InToolType);
	void ApplyToolMesh();
	void ClearToolMesh();
	bool IsToolChangeRequired() const;
	ERToolType GetPendingToolType() const { return PendingToolType; }
	bool HasTool() const;
private:
	void OnMeshLoaded();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tool|Settings")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tool|Settings")
	TMap<ERToolType, FRToolMeshData> ToolMeshMap;

	UPROPERTY(Transient)
	ERToolType CurrentToolType = ERToolType::None;

	UPROPERTY(Transient)
	ERToolType PendingToolType = ERToolType::None;

	TSharedPtr<FStreamableHandle> StreamableHandle;
};
