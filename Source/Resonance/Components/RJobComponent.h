#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RJobComponent.generated.h"

struct FToolMeshData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RESONANCE_API URJobComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URJobComponent();

	virtual void BeginPlay() override;
public:
	void SetToolMesh(const FToolMeshData& ToolMeshData);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tool|Settings")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
};
