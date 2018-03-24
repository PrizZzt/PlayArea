#include "TReceiveWorker.h"
#include "Engine.h"
#include "../../../PlayArea/PlayArea/contstants.h"

TReceiveWorker *TReceiveWorker::Runnable = nullptr;

TReceiveWorker::TReceiveWorker(UTGameInstance *_game) :
	game(_game),
	StopTaskCounter(0)
{
	Thread = FRunnableThread::Create(this, TEXT("TReceiveWorker"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
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

	while (
		StopTaskCounter.GetValue() == 0 &&
		game->Socket &&
		game->Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected)
	{
		game->Socket->Recv((uint8*)&data, MAX_LENGTH_SERVER, readed);
		if (readed == 0)continue;

		if (data[0] != 1)
		{
			AsyncTask(ENamedThreads::GameThread, [&]() {GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Not map arrived!"); });
			return 0;
		}

		for (auto object : game->objects)
		{
			object->to_delete = true;
		}

		AsyncTask(ENamedThreads::GameThread, [&]()
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, "Sum = " + FString::FromInt(readed));

			game->SetSize(data[1], data[2]);
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, "X = " + FString::FromInt(game->size_x));
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, "Y = " + FString::FromInt(game->size_y));
			int position = 3;
			for (uint8_t j = 0; j < game->size_y; j++)
			{
				for (uint8_t i = 0; i < game->size_x; i++)
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
