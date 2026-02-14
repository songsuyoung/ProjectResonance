// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RClimbableActor.h"

// Sets default values
ARClimbableActor::ARClimbableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARClimbableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARClimbableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

