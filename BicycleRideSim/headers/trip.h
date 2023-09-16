#pragma once

#include <chrono>

std::chrono::system_clock::time_point getTimePointFromString(std::string& time_string)
{
	std::istringstream ss(time_string);

	// Define a std::tm structure to hold the parsed date and time
	std::tm tm;

	// Use std::get_time to parse the string into the tm structure
	ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");

	// Convert tm structure into std::chrono::system_clock::time_point
	auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));

	return tp;
}

struct record
{
	record() : timeDeltaMilliseconds(0), elevationDelta(0) {}
	std::string timestamp;
	double latitude;
	double longitude;
	double elevation;
	size_t timeDeltaMilliseconds;
	double elevationDelta;
	double distanceMetersFromPreviousReceord;
	double temperature;
};

struct trip
{
private:
	double integratedDistanceMeters;
	double integratedTimeMilliseconds;

public:
	std::vector<record> points;
	std::string name;
	std::string time_initial_point;
	std::string type;

	trip() : integratedDistanceMeters(0), integratedTimeMilliseconds(0) {}

	void IntegrateTimeMilliseconds(double timeMs)
	{
		integratedTimeMilliseconds += timeMs;
	}

	void IntegraeteDistanceMeters(double distanceM)
	{
		integratedDistanceMeters += distanceM;
	}

	double GetIntegratedDistanceMeters() const
	{
		return integratedDistanceMeters;
	}

	double GetIntegratedTimeMilliseconds() const
	{
		return integratedTimeMilliseconds;
	}
};