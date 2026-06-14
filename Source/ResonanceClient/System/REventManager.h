#pragma once

#include "CoreMinimal.h"
#include "Interface/RMessageReceiver.h"
#include "Containers/List.h"
#include "REventManager.generated.h"

enum class ERMessageType;
struct FRMessage;

// EventManager - Subject
UCLASS()
class RESONANCECLIENT_API UREventManager : public UObject
{
	GENERATED_BODY()

public:

	static UREventManager* Get(UObject* Context);
	
	void AddReceiver(IRMessageReceiver* Receiver);
	void RemoveReceiver(IRMessageReceiver* Receiver);
	void Notify(ERMessageType Type, FRMessage* Message = nullptr);
protected:

	TDoubleLinkedList<TWeakInterfacePtr<IRMessageReceiver>> Receivers;
};
