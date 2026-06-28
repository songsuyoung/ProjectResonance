#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "RSTTask_FarmWork.generated.h"

class AAIController;
class ARNPCCharacter;
class ARFarmPlotVolume;
class URJobComponent;
struct FRPlotData;
enum class ERequiredFarmTask : uint8;

USTRUCT(BlueprintType)                                                                                                                                                                                                                    
struct FToolMeshData                                                                                                                                                                                                                      
{                                                                                                                                                                                                                                         
	GENERATED_BODY()                                                                                                                                                                                                                      
                                                                                                                                                                                                                                            
	UPROPERTY(EditAnywhere, BlueprintReadWrite)                                                                                                                                                                                       
	TSoftObjectPtr<UStaticMesh> MeshObj;
	
	UPROPERTY(Transient)
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName = TEXT("hand_r");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RelativeLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator RelativeRotation = FRotator::ZeroRotator;
};


UCLASS()
class RESONANCE_API URSTTask_FarmWork : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<AAIController> AIController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<ARNPCCharacter> OwnerCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Plow|Settings")
	TObjectPtr<UAnimMontage> WorkMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TWeakObjectPtr<ARFarmPlotVolume> FarmVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Plow|Settings")
	FToolMeshData ToolMeshData;
	
protected:
	
	UPROPERTY(Transient)
	TWeakObjectPtr<URJobComponent> JobComponent;
	
	UPROPERTY(Transient)
	float WorkDuration;
	
	UPROPERTY(Transient)
	float ElapsedTime;
	
	UPROPERTY(Transient)
	int32 ActivePlotIndex;
};
