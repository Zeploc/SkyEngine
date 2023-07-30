#include "TimeManager.h"

#include <chrono>

#include "LogManager.h"

using std::chrono::milliseconds;
using std::chrono::system_clock;

double TimeManager::CurrentFrameTime = 0.0;

double TimeManager::StartSystemTime = 0.0;
double TimeManager::LastSystemTime = 0.0;
double TimeManager::SystemTime = 0.0;
float TimeManager::DeltaTime = 0.0;

double TimeManager::TickRate = 60.0;

bool TimeManager::bCanTickThisFrame = true;
bool TimeManager::bTickRateEnabled = false;

void TimeManager::Start()
{		
	srand(unsigned int(time(NULL)));
	StartSystemTime = InternalGetSystemTime();
}

void TimeManager::Update()
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
	
	LogManager::GetInstance()->DisplayLogMessage("delta time " + std::to_string(DeltaTime));
}

double TimeManager::GetElapsedEngineTime()
{
	return SystemTime - StartSystemTime;
}

double TimeManager::InternalGetSystemTime()
{
	const auto MillisecondsSinceEpocLong = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	const double MillisecondsSinceEpoc = static_cast<double>(MillisecondsSinceEpocLong);
	return MillisecondsSinceEpoc / 1000.0;
}
