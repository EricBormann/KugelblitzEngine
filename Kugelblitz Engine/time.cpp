#include "time.h"



Time::Time()
{
}

int Time::getCurrentHour() 
{
	struct tm *theTime;
	time_t tim;
	time(&tim);
	theTime = localtime(&tim);
	int hours = theTime->tm_hour;
	return hours;
}

int Time::getCurrentMinute() 
{
	struct tm *theTime;
	time_t tim;
	time(&tim);
	theTime = localtime(&tim);
	int minutes = theTime->tm_min;
	return minutes;
}

int Time::getCurrentSecond() 
{
	struct tm *theTime;
	time_t tim;
	time(&tim);
	theTime = localtime(&tim);
	int seconds = theTime->tm_sec;
	return seconds;
}

std::string Time::getCurrentTime() {
	struct tm *theTime;
	time_t tim;
	time(&tim);
	theTime = localtime(&tim);
	int seconds = theTime->tm_sec;
	int minutes = theTime->tm_min;
	int hours = theTime->tm_hour;
	std::string returnValue = "["+ std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds) + "] ";
	return returnValue;
}

Time::~Time()
{
}
