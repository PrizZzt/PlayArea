#include "TReceiveWorker.h"
#include "Engine.h"
#include "../../../PlayArea/PlayArea/contstants.h"

union IntCharConvertor
{
	int32_t i;
	uint8_t c[4];
};

TReceiveWorker *TReceiveWorker::Runnable = nullptr;

TReceiveWorker::TReceiveWorker(UTGameInstance *_game) :
	game(_game),
	StopTaskCounter(0)
{
	Thread = FRunnableThread::Create(this, TEXT("TReceiveWorker"), 0, TPri_BelowNormal);
}

TReceiveWorker::~TReceiveWorker()
{
	delete Thread;
	Thread = NULL;
}

bool TReceiveWorker::Init()
{
	return true;
}

uint32 TReceiveWorker::Run()
{
	uint8 data[MAX_LENGTH_SERVER];
	int32 readed;
	uint32 position;

	while (
		StopTaskCounter.GetValue() == 0 &&
		game->Socket &&
		game->Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected
		)
	{
		game->Socket->Recv((uint8*)&data, MAX_LENGTH_SERVER, readed);
		if (readed == 0)continue;

		AsyncTask(ENamedThreads::GameThread, [&]()
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, "Sum = " + FString::FromInt(readed));
			switch (data[0])
			{
			case 1://MAP
				for (auto &object : game->objects)
				{
					object->to_delete = true;
				}

				game->SetSize(data[1], data[2]);
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, "X = " + FString::FromInt(game->SizeX));
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, "Y = " + FString::FromInt(game->SizeY));
				position = 3;
				for (uint8_t j = 0; j < game->SizeY; j++)
				{
					for (uint8_t i = 0; i < game->SizeX; i++)
					{
						if (data[position] != 0)
						{
							uint8_t id = data[position + 1];
							uint8_t type = data[position];

							game->UpdateObject(id, type, i, j);
						}
						position += 2;
					}
				}

				game->objects.RemoveAll([](ATObject *_object)
				{
					if (_object->to_delete)
					{
						_object->Destroy();
						return true;
					}
					return false;
				});

				game->CheckAdditionalInfo();
				break;

			case 4://NAMES_LIST
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "NAMES_LIST");
				game->PointsTableInstance->StartUpdate();
				position = 2;
				for (uint8 i = 0; i < data[1]; i++)
				{
					uint8 id = data[position];
					position++;
					uint8 player_name_length = data[position];
					position++;
					FString player_name = FString();
					for (uint8 j = 0; j < player_name_length; j++)
					{
						player_name.AppendChar(data[position]);
						position++;
					}
					game->PointsTableInstance->SetPlayerName(id, player_name);
					game->UpdatePlayerName(id, player_name);
				}
				game->PointsTableInstance->EndUpdate();
				break;

			case 5://POINTS_LIST
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "POINTS_LIST");
				game->PointsTableInstance->StartUpdate();
				position = 2;
				for (uint8 i = 0; i < data[1]; i++)
				{
					uint8_t player_id = data[position];
					int32_t points = (data[position + 1] << 24) + (data[position + 2] << 16) + (data[position + 3] << 8) + data[position + 4];
					game->PointsTableInstance->SetPlayerPoints(player_id, points);
					position += 5;
				}
				game->PointsTableInstance->EndUpdate();
				break;

			default:
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Unknown message type!");
				break;
			}
		});
	}

	return 0;
}

void TReceiveWorker::Stop()
{
	StopTaskCounter.Increment();
}

TReceiveWorker* TReceiveWorker::Init(UTGameInstance *_game)
{
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new TReceiveWorker(_game);
	}
	return Runnable;
}

void TReceiveWorker::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

void TReceiveWorker::Shutdown()
{
	if (Runnable)
	{
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = nullptr;
	}
}
