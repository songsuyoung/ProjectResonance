#include "RAIController.h"

// UE 5.
#include "Components/StateTreeAIComponent.h"

ARAIController::ARAIController()
{
	StateTreeComponent = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeAIComponent"));
}
