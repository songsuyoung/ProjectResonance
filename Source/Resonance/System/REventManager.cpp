#include "System/REventManager.h"

// Newly Created Files...
#include "System/RGameInstance.h"

UREventManager* UREventManager::Get(UObject* Object)
{
    // UObject는 World 를 가지고 있지 않다.
    // 그렇기때문에 외부 액터로부터 World를 가져와야한다.
    UWorld* World = Object->GetWorld();

    check(World);

    URGameInstance* GameInstance = Cast<URGameInstance>(World->GetGameInstance());

    check(GameInstance);

    return GameInstance->GetEventManager();
}

void UREventManager::AddReceiver(IRMessageReceiver* Receiver)
{
    Receivers.AddTail(Receiver);
}

void UREventManager::RemoveReceiver(IRMessageReceiver* Receiver)
{
    Receivers.RemoveNode(Receiver);
}

void UREventManager::Notify(ERMessageType Type, FRMessage* Message)
{
    auto* NextReceiver = Receivers.GetHead();

    while (nullptr != NextReceiver)
    {
        // GetValue -> UObject를 던짐.
        IRMessageReceiver* Receiver = Cast<IRMessageReceiver>(NextReceiver->GetValue());

        if (nullptr == Receiver)
        {
            Receivers.RemoveNode(Receiver);
            NextReceiver = NextReceiver->GetNextNode();
            continue;
        }

        // 메시지 호출
        Receiver->OnMessage(Type, Message);
        NextReceiver = NextReceiver->GetNextNode();
    }
}