// Fill out your copyright notice in the Description page of Project Settings.

#include "TGameInstance.h"
#include "../../../PlayArea/PlayArea/contstants.h"
#include "Engine.h"

void UTGameInstance::receive_loop()
{
	uint8 data[MAX_LENGTH_SERVER];
	int32 readed;

	while (is_running)
	{
		Socket->Recv((uint8*)&data, MAX_LENGTH_SERVER, readed);
		if (readed == 0)continue;

		if (data[0] != 1)
		{
			AsyncTask(ENamedThreads::GameThread, [&]() {GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Not map arrived!"); });
			continue;
		}

		for (auto object : objects)
		{
			object->to_delete = true;
		}

		AsyncTask(ENamedThreads::GameThread, [&]()
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, "Sum = " + FString::FromInt(readed));

			size_x = data[1];
			size_y = data[2];
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, "X = " + FString::FromInt(size_x));
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, "Y = " + FString::FromInt(size_y));
			int position = 3;
			for (uint8_t j = 0; j < size_y; j++)
			{
				for (uint8_t i = 0; i < size_x; i++)
				{
					if (data[position] != 0)
					{
						uint8_t id = data[position + 1];
						uint8_t type = data[position];
						uint8_t group = ATObject::GetGroup(type);
						ATObject *result = nullptr;

						if (objects.Num() > 0)
						{
							ATObject **resultPtr = objects.FindByPredicate([&](const ATObject *t)
							{
								return
									(
										t->ID == 0 &&
										ATObject::GetGroup(t->Type) == 0 &&
										t->BaseX == i &&
										t->BaseY == j
										)
									||
									(
										t->ID == id &&
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
							FVector initialLocation(i * 100, j * 100, 0);
							switch (type)
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
							result->ID = id;
							result->BaseX = i;
							result->BaseY = j;
							objects.Add(result);
						}

						result->SetTarget(i, j);
						result->SetType(type);
					}
					position += 2;
				}
			}

			objects.RemoveAll([](ATObject *_object)
			{
				if (_object->to_delete)
				{
					_object->Destroy();
					return true;
				}
				return false; 
			});
		});
	}
}

void UTGameInstance::Start(FString _address, int32 _port)
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool is_valid;
	addr->SetIp(L"127.0.0.1", is_valid);
	addr->SetPort(1234);

	Socket->Connect(*addr);

	is_running = true;
	receive_thread = std::thread(&UTGameInstance::receive_loop, this);
}

void UTGameInstance::Stop()
{
	if (is_running)
	{
		is_running = false;
		receive_thread.join();
	}

	if (Socket && Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected)
		Socket->Close();
}
