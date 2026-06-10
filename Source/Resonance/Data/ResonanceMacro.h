#pragma once
#include "CoreMinimal.h"

#define REVENT_MESSAGE_NOTIFY(Context, Type) \
	UREventManager::Get(Context)->Notify(Type, nullptr); \

#define REVENT_MESSAGE_NOTIFY_MSG(Context, Type, Msg) \
	UREventManager::Get(Context)->Notify(Type, &Msg);

#define REVENT_MESSAGE_ADD(Context, Receiver)	\
	UREventManager::Get(Context)->AddReceiver(Receiver) \

#define REVENT_MESSAGE_Remove(Context, Receiver)	\
	UREventManager::Get(Context)->RemoveReceiver(Receiver) \
