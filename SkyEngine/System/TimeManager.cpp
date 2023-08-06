// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "TimeManager.h"

#include <chrono>

#include "LogManager.h"

using std::chrono::milliseconds;
using std::chrono::system_clock;

double CTimeManager::CurrentFrameTime = 0.0;

double CTimeManager::StartSystemTime = 0.0;
double CTimeManager::LastSystemTime = 0.0;
double CTimeManager::SystemTime = 0.0;
float CTimeManager::DeltaTime = 1.0f / 60.0f;

double CTimeManager::TickRate = 60.0;

bool CTimeManager::bCanTickThisFrame = true;

bool CTimeManager::bTickRateEnabled = false;
double CTimeManager::FPS = 0.0f;

void CTimeManager::Start()
{		
	srand(unsigned int(time(NULL)));
	StartSystemTime = InternalGetSystemTime();
}

void CTimeManager::Update()
{
	const double DoubleDeltaTime = InternalGetSystemTime() - LastSystemTime;
	CurrentFrameTime += DoubleDeltaTime;

	if (bTickRateEnabled)
	{
		bCanTickThisFrame = false;
		if (CurrentFrameTime > 1.0 / TickRate)
		{
			CurrentFrameTime = 0.0;
			bCanTickThisFrame = true;
		}
		else
		{
			return;
		}
	}
	
	SystemTime = InternalGetSystemTime();
	LastSystemTime = SystemTime;
	DeltaTime = static_cast<float>(DoubleDeltaTime);
	FPS = 1.0 / DeltaTime;
	
	// LogManager::GetInstance()->DisplayLogMessage("delta time " + std::to_string(DeltaTime));
}

double CTimeManager::GetElapsedEngineTime()
{
	return SystemTime - StartSystemTime;
}

double CTimeManager::InternalGetSystemTime()
{
	const auto MillisecondsSinceEpocLong = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	const double MillisecondsSinceEpoc = static_cast<double>(MillisecondsSinceEpocLong);
	return MillisecondsSinceEpoc / 1000.0;
}
