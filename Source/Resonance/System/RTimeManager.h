#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RTimeManager.generated.h"

UCLASS(config=Game, defaultconfig)                                                                                                                                                                                                        
class URTimeSettings : public UDeveloperSettings                                                                                                                                                                                          
{             
	GENERATED_BODY()
public:
	
	// 1시간 60초
	UPROPERTY(EditAnywhere, config, Category="Time")                                                                                                                                                                                      
	float HourDuration = 60.f;

	// 하루 길이
	UPROPERTY(EditAnywhere, config, Category="Time")                                                                                                                                                                                      
	int32 HoursPerDay = 24;                                                                                                                                                                                                            
};                                                                                                                                                                                                                                    

UCLASS()
class RESONANCE_API URTimeManager : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
public:
	URTimeManager();
	
	static URTimeManager* Get(UObject* Context);
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual TStatId GetStatId() const override;
	
	const int32 GetHourTime() const { return CurrentHour; }
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnHourChanged, int32 /*NewHour*/);
	FOnHourChanged OnHourChanged;
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnDayChanged, int32 /*NewDay*/);
	FOnDayChanged OnDayChanged;
	
	virtual void Tick(float DeltaTime) override;
protected:
	
	UPROPERTY(Transient)
	int32 CurrentHour;
	
	UPROPERTY(Transient)
	int32 CurrentDay;
	
	UPROPERTY(Transient)
	float ElapsedTime;           // 누적 게임 시간
	
	UPROPERTY(Transient)
	float HourDuration;           // 하루 길이 (초)
	
	UPROPERTY(Transient)
	int32 HoursPerDay;
	
};
