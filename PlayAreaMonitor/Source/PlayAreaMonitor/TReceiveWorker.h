#pragma once

#include "Runnable.h"
#include "RunnableThread.h"
#include "ThreadSafeCounter.h"
#include "PlatformProcess.h"
#include "TGameInstance.h"

class TReceiveWorker : public FRunnable
{
	static TReceiveWorker *Runnable;

	FRunnableThread *Thread;
	UTGameInstance *game;
	FThreadSafeCounter StopTaskCounter;

	TReceiveWorker(UTGameInstance *_game);
	virtual ~TReceiveWorker();

	void EnsureCompletion();

	bool Init()override;
	uint32 Run()override;
	void Stop()override;

public:
	static TReceiveWorker *Init(UTGameInstance *_game);
	static void Shutdown();
};
