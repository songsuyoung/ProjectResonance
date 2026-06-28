#include "Actor/RToolStorage.h"


ARToolStorage::ARToolStorage()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	RootComponent = MeshComponent;
}

void ARToolStorage::BeginPlay()
{
	Super::BeginPlay();
}
