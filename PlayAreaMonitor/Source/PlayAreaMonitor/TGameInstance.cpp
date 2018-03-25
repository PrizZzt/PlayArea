// Fill out your copyright notice in the Description page of Project Settings.

#include "TGameInstance.h"
#include "Engine.h"
#include "TReceiveWorker.h"

void UTGameInstance::Shutdown()
{
	Stop();
}

void UTGameInstance::Start(FString _address, int32 _port)
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool is_valid;
	addr->SetIp(L"127.0.0.1", is_valid);
	addr->SetPort(1234);

	Socket->Connect(*addr);

	TReceiveWorker::Init(this);
}

void UTGameInstance::Stop()
{
	if (Socket && Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected)
		Socket->Close();

	TReceiveWorker::Shutdown();

	delete Socket;
	Socket = nullptr;

	AsyncTask(ENamedThreads::GameThread, [&]()
	{
		if (objects.Num() > 0)
		{
			objects.RemoveAll([](ATObject *_object)
			{
				_object->Destroy();
				return true;
			});
		}
	});
}

void UTGameInstance::UpdateObject(uint8_t _id, uint8_t _type, uint8_t _x, uint8_t _y)
{
	uint8_t group = ATObject::GetGroup(_type);
	ATObject *result = nullptr;

	if (objects.Num() > 0)
	{
		ATObject **resultPtr = objects.FindByPredicate([&](const ATObject *t)
		{
			return
				(
					t->ID == 0 &&
					ATObject::GetGroup(t->Type) == 0 &&
					t->BaseX == _x &&
					t->BaseY == _y
					)
				||
				(
					t->ID == _id &&
					ATObject::GetGroup(t->Type) != 0 &&
					ATObject::GetGroup(t->Type) == group
					);
		});
		if (resultPtr)
		{
			result = *resultPtr;
			result->to_delete = false;
		}
	}

	if (result == nullptr)
	{
		FVector initialLocation(_x * 100, _y * 100, 0);
		switch (_type)
		{
		case 1://UNDESTRUCTIBLE_WALL
			result = GetWorld()->SpawnActor<ATObject>(UndestructibleWall, initialLocation, FRotator::ZeroRotator);
			break;

		case 2://DESTRUCTIBLE_WALL
		case 3://DESTROYED_WALL
			result = GetWorld()->SpawnActor<ATObject>(DestructibleWall, initialLocation, FRotator::ZeroRotator);
			break;

		case 4://PLAYER
		case 5://PLAYER_WITH_BOMB_1
		case 6://PLAYER_WITH_BOMB_2
		case 7://PLAYER_WITH_BOMB_3
		case 8://PLAYER_WITH_BOMB_4
		case 9://PLAYER_WITH_BOMB_5
		case 10://DEAD_PLAYER
			result = GetWorld()->SpawnActor<ATObject>(Player, initialLocation, FRotator::ZeroRotator);
			check_names = true;
			break;

		case 11://MEATCHOPPER
		case 12://DEAD_MEATCHOPPER
			result = GetWorld()->SpawnActor<ATObject>(Meatchopper, initialLocation, FRotator::ZeroRotator);
			break;

		case 13://BOOM
			result = GetWorld()->SpawnActor<ATObject>(Boom, initialLocation, FRotator::ZeroRotator);
			break;

		case 14://BOMB_1
		case 15://BOMB_2
		case 16://BOMB_3
		case 17://BOMB_4
		case 18://BOMB_5
			result = GetWorld()->SpawnActor<ATObject>(Bomb, initialLocation, FRotator::ZeroRotator);
			break;

		default:
			return;
		}
		result->ID = _id;
		result->BaseX = _x;
		result->BaseY = _y;
		objects.Add(result);
	}

	result->SetTarget(_x, _y);
	result->SetType(_type);
}

void UTGameInstance::UpdatePlayerInfo(uint8_t _id, FString &_name)
{
	if (objects.Num() > 0)
	{
		ATObject **resultPtr = objects.FindByPredicate([&](const ATObject *_t)
		{
			return (_t->ID == _id && ATObject::GetGroup(_t->Type) == ATObject::PLAYER_GROUP);
		});
		if (resultPtr)
		{
			(*resultPtr)->SetText_BP(_name);
		}
	}

	FPlayerInfo *resultPtr = PlayerInfo.FindByPredicate([&](const FPlayerInfo &_t)
	{
		return _t.ID == _id;
	});
	if (resultPtr)
	{
		(*resultPtr).Name = _name;
	}
	else
	{
		PlayerInfo.Add(FPlayerInfo(_id, _name));
	}
}

void UTGameInstance::UpdatePlayerInfo(uint8_t _id, int32_t _points)
{
	FPlayerInfo *resultPtr = PlayerInfo.FindByPredicate([&](const FPlayerInfo &_t)
	{
		return _t.ID == _id;
	});
	if (resultPtr)
	{
		(*resultPtr).Points = _points;
	}
	else
	{
		PlayerInfo.Add(FPlayerInfo(_id, _points));
	}
}

void UTGameInstance::CheckAdditionalInfo()
{
	uint8 data = 7;
	int32 sent;
	if (check_names)
	{
		data = 7;
		Socket->Send(&data, 1, sent);
		check_names = false;
	}
	//TODO Выяснить, почему не работает запрос имен при раскомменчивании этого кода
	//data = 8;
	//Socket->Send(&data, 1, sent);
}
