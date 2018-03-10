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
							}
						}

						if (result == nullptr)
						{
							FVector initialLocation(i * 100, j * 100, 0);
							result = GetWorld()->SpawnActor<ATObject>(ObjectType, initialLocation, FRotator::ZeroRotator);
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

	if (Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected)
		Socket->Close();
}
