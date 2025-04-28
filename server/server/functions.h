#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#include <time.h>
#include <map>

void GetTime(time_t timer, char sendBuff[]);
void GetTimeWithoutDate(time_t timer, char sendBuff[]);
void GetTimeSinceEpoch(time_t timer, char sendBuff[]);
void GetTimeWithoutDateOrSeconds(time_t timer, char sendBuff[]);
void GetYear(time_t timer, char sendBuff[]);
void GetMonthAndDay(time_t timer, char sendBuff[]);
void GetSecondsSinceBeginingOfMonth(time_t timer, char sendBuff[]);
void GetWeekOfYear(time_t timer, char sendBuff[]);
void GetDylightSavings(time_t timer, char sendBuff[]);
void GetTimeWithoutDateInCity(time_t timer, char sendBuff[], char recvBuff[]);
void MeasureTimeLap(char sendBuff[]);