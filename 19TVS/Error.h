 #pragma once
#include <fstream>
#include <Windows.h>
#include <chrono>
#include <string>
#include <ctime>
#include <stdio.h>
#include <iomanip>
#include <map>



void Error(const wchar_t mText[200], const wchar_t hText[50]);



class LoggerClass {
public:
	LoggerClass(const std::string & name) {
		GiveNewPoint(name);
		CallLogFile("Start", 0);
	}


	~LoggerClass() {

		CallLogFile("End", 1);
	}

	void CallLogFile(std::string header, bool isItEnd)
	{
		char space = ' ';
		std::string logFilePath = Path_Finder("LOGGER");
		ofstream logFile(logFilePath, ios::app);
		logFile << setw(7) << setfill(' ') << header << space;
		logFile << setw(30) << setfill(' ') << TakeCurrentTime() << space;
		if (isItEnd) {
			logFile << space << setw(15) << setfill(' ') << TakeDuration(TakeFunctionName());
			logFile << space << setw(30) << setfill(' ') << TakeFunctionName() <<
				setw(10) << setfill(' ') << std::endl;
		}
		else
		{
			logFile << space << setw(46) << setfill(' ') << TakeFunctionName() << std::endl;
		}


		logFile.close();
	}

	std::string TakeCurrentTime() const
	{
		SYSTEMTIME st;
		GetLocalTime(&st);

		std::string date =
			std::to_string(st.wYear) + "-"
			+ std::to_string(st.wMonth) + "-"
			+ std::to_string(st.wDay) + " "
			+ std::to_string(st.wHour) + ":"
			+ std::to_string(st.wMinute) + ":"
			+ std::to_string(st.wSecond) + "."
			+ std::to_string(st.wMilliseconds);
		return date;
	}

	void GiveNewPoint(const std::string & name)
	{
		auto start_time = std::chrono::steady_clock::now();
		m_functionName = name;
		m_seTime[name] = start_time;
	}

	int TakeDuration(const std::string & name)
	{
		auto finish_time = std::chrono::steady_clock::now();
		return(std::chrono::duration_cast<std::chrono::milliseconds>(finish_time - m_seTime[name]).count());
	}

	std::string TakeFunctionName() const
	{
		return m_functionName;
	}

private:

	std::string m_functionName;
	std::map<std::string, std::chrono::steady_clock::time_point> m_seTime;
};
