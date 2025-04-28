#include "functions.h"

/**
 * GetTime - Converts a time_t value to a human-readable date and time string.
 */
void GetTime(time_t timer, char sendBuff[])
{
	strcpy(sendBuff, ctime(&timer));  // Convert time_t to string
	sendBuff[strlen(sendBuff) - 1] = '\0'; // Remove the newline at the end
}

/**
 * GetTimeWithoutDate - Extracts and returns only the time (HH:MM:SS) from the time_t value.
 */
void GetTimeWithoutDate(time_t timer, char sendBuff[])
{
	char* timeStr = ctime(&timer);
	char timeOnly[9];
	strncpy(timeOnly, timeStr + 11, 8); // Get only the HH:MM:SS part
	timeOnly[8] = '\0';
	strcpy(sendBuff, timeOnly);
}

/**
 * GetTimeSinceEpoch - Converts the time_t value to seconds since January 1, 1970 (epoch).
 */
void GetTimeSinceEpoch(time_t timer, char sendBuff[])
{
	char seconds_as_string[32];
	sprintf(seconds_as_string, "%ld", timer); // Convert time to seconds since epoch
	strcpy(sendBuff, seconds_as_string);
}

/**
 * GetTimeWithoutDateOrSeconds - Extracts and returns only the time (HH:MM) from the time_t value.
 */
void GetTimeWithoutDateOrSeconds(time_t timer, char sendBuff[])
{
	char* timeStr = ctime(&timer);
	char timeOnly[6];
	strncpy(timeOnly, timeStr + 11, 5); // Get only the HH:MM part
	timeOnly[5] = '\0';
	strcpy(sendBuff, timeOnly);
}

/**
 * GetYear - Extracts and returns only the year from the time_t value.
 */
void GetYear(time_t timer, char sendBuff[])
{
	char* timeStr = ctime(&timer);
	char year[5];
	strncpy(year, timeStr + 20, 4); // Extract the year part
	year[4] = '\0';
	strcpy(sendBuff, year);
}

/**
 * GetMonthAndDay - Extracts and returns the month and day (Mon DD) from the time_t value.
 */
void GetMonthAndDay(time_t timer, char sendBuff[])
{
	char* timeStr = ctime(&timer);
	char monthAndDay[7];
	strncpy(monthAndDay, timeStr + 4, 7); // Extract the month and day part
	monthAndDay[6] = '\0';
	strcpy(sendBuff, monthAndDay);
}

/**
 * GetSecondsSinceBeginingOfMonth - Calculates and returns the number of seconds since the start of the current month.
 */
void GetSecondsSinceBeginingOfMonth(time_t timer, char sendBuff[])
{
	time_t currentTime = time(NULL);
	struct tm* tm_info = localtime(&currentTime);

	struct tm startOfMonth = *tm_info;
	startOfMonth.tm_mday = 1; // First day of the month
	startOfMonth.tm_hour = 0; // 00:00 hour
	startOfMonth.tm_min = 0; // 00 minutes
	startOfMonth.tm_sec = 0; // 00 seconds

	// Convert to time
	time_t startOfMonthTime = mktime(&startOfMonth);

	int secondsSinceStartOfMonth = difftime(currentTime, startOfMonthTime);

	char seconds_as_string[32];  // Ensure the buffer is large enough
	sprintf(seconds_as_string, "%ld", secondsSinceStartOfMonth);
	strcpy(sendBuff, seconds_as_string);
}

/**
 * GetWeekOfYear - Returns the current week number of the year.
 */
void GetWeekOfYear(time_t timer, char sendBuff[])
{
	time_t currentTime = time(NULL);
	struct tm* tm_info = localtime(&currentTime);
	int weekNumber = tm_info->tm_yday / 7 + 1;

	char week_as_string[4];  // Ensure the buffer is large enough
	sprintf(week_as_string, "%ld", weekNumber);
	strcpy(sendBuff, week_as_string);
}

/**
 * GetDylightSavings - Checks if Daylight Saving Time (DST) is active and returns "1" for active, "0" for not active.
 */
void GetDylightSavings(time_t timer, char sendBuff[])
{
	time_t currentTime = time(NULL);
	struct tm* tm_info = localtime(&currentTime);

	if (tm_info->tm_isdst > 0)
		strcpy(sendBuff, "1");
	else if (tm_info->tm_isdst == 0)
		strcpy(sendBuff, "0");
}

/**
 * GetTimeWithoutDateInCity - Adjusts time for a specific city and returns the time (HH:MM:SS) in that city.
 */
void GetTimeWithoutDateInCity(time_t timer, char sendBuff[], char recvBuff[])
{
	string request(recvBuff + 3); // Remove the "12" prefix
	map<string, int> cityOffsets = {
		{"Doha", 3},
		{"Prague", 1},
		{"New York", -5},
		{"Berlin", 1}
	};

	if (cityOffsets.find(request) != cityOffsets.end()) // Checks if the city is in the map
	{
		int offset = cityOffsets[request];
		timer += offset * 3600;
		struct tm* localTime = gmtime(&timer);
		strftime(sendBuff, 256, "%H:%M:%S", localTime);
	}
	else
	{
		struct tm* utcTime = gmtime(&timer);
		strftime(sendBuff, 256, "UTC Time: %H:%M:%S", utcTime);
	}
}

/**
 * MeasureTimeLap - Measures the time between two requests. If more than 3 minutes have passed, stops the measurement.
 */
void MeasureTimeLap(char sendBuff[])
{
	static time_t measurementStart = 0;
	static bool isMeasuring = false;
	static int requestCount = 0; // Number of requests for measurement

	time_t currentTime = time(NULL); // Current time

	if (requestCount == 2)
		requestCount = 0;

	if (!isMeasuring || requestCount == 0)
	{
		isMeasuring = true;
		measurementStart = currentTime;
		requestCount = 1;
		strcpy(sendBuff, "Measurement started.");
	}
	else
	{
		double elapsedTime = difftime(currentTime, measurementStart);

		if (elapsedTime >= 180) // 3 minutes passed
		{
			isMeasuring = false;
			requestCount = 0;
		}
		else
		{
			requestCount++;
			if (requestCount > 2) // More than 2 requests
			{
				isMeasuring = false;
			}
			else
			{
				sprintf(sendBuff, "Elapsed time: %.0f seconds", elapsedTime);
			}
		}
	}
}
