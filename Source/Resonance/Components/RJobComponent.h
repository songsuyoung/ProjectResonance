#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/StreamableManager.h"
#include "Resonance/Data/ResonanceEnums.h"
#include "Resonance/Data/ResonanceStructs.h"
#include "RJobComponent.generated.h"
class UStateTreeAIComponent;
class ARBaseCharacter;

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
	bool IsWorkTime() const { return bWork; }
	
protected:
	void OnHourChanged(int NewHour);	
	void OnMeshLoaded();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Job|Settings")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Job|Settings")
	TMap<ERToolType, FRToolMeshData> ToolMeshMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Job|Settings")
	int32 StartWorkHour;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Job|Settings")
	int32 EndWorkHour;

protected:
	UPROPERTY(Transient)
	TWeakObjectPtr<UStateTreeAIComponent> StateTreeAIComponent;
	
	UPROPERTY(Transient)
	TWeakObjectPtr<ARBaseCharacter> Character;
	
	UPROPERTY(Transient)
	ERToolType CurrentToolType = ERToolType::None;

	UPROPERTY(Transient)
	ERToolType PendingToolType = ERToolType::None;

	UPROPERTY(Transient)
	uint8 bWork : 1;
	
	TSharedPtr<FStreamableHandle> StreamableHandle;
};
