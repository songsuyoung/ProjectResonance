#include "System/REventManager.h"

// Newly Created Files...
#include "System/RBaseGameInstance.h"

UREventManager* UREventManager::Get(UObject* Object)
{
    // UObject�� World �� ������ ���� �ʴ�.
    // �׷��⶧���� �ܺ� ���ͷκ��� World�� �����;��Ѵ�.
    UWorld* World = Object->GetWorld();

    check(World);

    URBaseGameInstance* GameInstance = Cast<URBaseGameInstance>(World->GetGameInstance());

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
        // GetValue -> UObject�� ����.
        IRMessageReceiver* Receiver = Cast<IRMessageReceiver>(NextReceiver->GetValue().Get());

        if (nullptr == Receiver)
        {
            Receivers.RemoveNode(Receiver);
            NextReceiver = NextReceiver->GetNextNode();
            continue;
        }

        // �޽��� ȣ��
        Receiver->OnMessage(Type, Message);
        NextReceiver = NextReceiver->GetNextNode();
    }
}