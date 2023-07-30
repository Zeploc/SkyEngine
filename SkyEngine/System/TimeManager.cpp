#include "TimeManager.h"

#include <chrono>

#include "LogManager.h"

using std::chrono::milliseconds;
using std::chrono::system_clock;

double TimeManager::CurrentFrameTime = 0.0;

double TimeManager::StartSystemTime = 0.0;
double TimeManager::LastSystemTime = 0.0;
double TimeManager::SystemTime = 0.0;
double TimeManager::DeltaTime = 0.0;

double TimeManager::TickRate = 60.0;

bool TimeManager::bCanTickThisFrame = true;

void TimeManager::Start()
{
	StartSystemTime = InternalGetSystemTime();
}

void TimeManager::Update()
{
	SystemTime = InternalGetSystemTime();
	DeltaTime = (SystemTime - LastSystemTime);// / 1000;
	LastSystemTime = SystemTime;//glutGet(GLUT_ELAPSED_TIME);
	CurrentFrameTime += DeltaTime;
	bCanTickThisFrame = false;
	if (CurrentFrameTime > 1.0 / TickRate)
	{
		CurrentFrameTime = 0.0;
		bCanTickThisFrame = true;
	}
	LogManager::GetInstance()->DisplayLogMessage("Current time " + std::to_string(GetElapsedEngineTime()));
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
