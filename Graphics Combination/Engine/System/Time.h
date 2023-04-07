// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

class Time
{
public:
	Time();

	~Time();

	static void Update();

	static double dCurrentTime;
	static double dTimeDelta;
	static double dFPS;
private:
	static double dPrevTime;
};
