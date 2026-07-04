#include "RTimeManager.h"

#include "RGameInstance.h"

URTimeManager::URTimeManager()
	: Super()
	, CurrentHour(0)
	, CurrentDay(1)
	, ElapsedTime(0.f)
	, HourDuration(0.f)
	, HoursPerDay(0.f)
{
}

URTimeManager* URTimeManager::Get(UObject* Context)
{
	UWorld* World = Context->GetWorld();

	check(World);
	
	return World->GetSubsystem<URTimeManager>();
}

void URTimeManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	const URTimeSettings* TimeSettings = GetDefault<URTimeSettings>();
	check(TimeSettings);
	HourDuration = TimeSettings->HourDuration;
	HoursPerDay = TimeSettings->HoursPerDay;
}

void URTimeManager::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	TWeakObjectPtr<URTimeManager> ThisPtr = this;
	
	InWorld.GetTimerManager().SetTimerForNextTick([ThisPtr]()
	  {
		if (false == ThisPtr.IsValid())
		{
			return;
		}
		ThisPtr->OnDayChanged.Broadcast(ThisPtr->CurrentDay);
	  });
}

TStatId URTimeManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(URTimeManager, STATGROUP_Tickables); 
}

void URTimeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ElapsedTime += DeltaTime;
	
	if (ElapsedTime > HourDuration)
	{
		ElapsedTime = 0.f;
		CurrentHour += 1;
		OnHourChanged.Broadcast(CurrentHour);
		
		if (CurrentHour >= HoursPerDay)
		{
			CurrentHour = 0;
			CurrentDay += 1;
			OnDayChanged.Broadcast(CurrentDay);
		}
	}
}
