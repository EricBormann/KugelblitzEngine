#pragma once
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <ctime>
#include <string>

class Time
{
public:
	Time::Time();
	int Time::getCurrentHour();
	int Time::getCurrentMinute();
	int Time::getCurrentSecond();
	std::string Time::getCurrentTime();
	virtual Time::~Time();
};

