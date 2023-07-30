// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

class TimeManager
{
public:
	static void Start();
	static void Update();
	static double GetDeltaTime() { return DeltaTime; }
	static bool CanTickThisFrame() { return bCanTickThisFrame; }
	static double GetTickRate() { return TickRate; }
	static double GetSystemTime() { return SystemTime; }
	static double GetElapsedEngineTime();
	
protected:
	static double InternalGetSystemTime();
	
	static double CurrentFrameTime;
	
	static double StartSystemTime;
	static double LastSystemTime;
	static double SystemTime;
	static double DeltaTime;

	static double TickRate;

	static bool bCanTickThisFrame;
};
