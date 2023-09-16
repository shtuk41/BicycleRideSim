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
	size_t integratedTimeMilliseconds;
	double integratedElevationGainMeters;
	double integratedElevationLossMeters;

public:
	std::vector<record> points;
	std::string name;
	std::string time_initial_point;
	std::string type;

	trip() : integratedDistanceMeters(0), 
		integratedTimeMilliseconds(0),
		integratedElevationGainMeters(0),
		integratedElevationLossMeters(0) {}

	void IntegrateTimeMilliseconds(size_t timeMs)
	{
		integratedTimeMilliseconds += timeMs;
	}

	void IntegraeteDistanceMeters(double distanceM)
	{
		integratedDistanceMeters += distanceM;
	}

	void IntegrateElevationGainMeters(double elev_delta)
	{
		integratedElevationGainMeters += elev_delta;
	}

	void IntegrateElevationLossMeters(double elev_delta)
	{
		integratedElevationLossMeters += elev_delta;
	}

	double GetIntegratedDistanceMeters() const
	{
		return integratedDistanceMeters;
	}

	size_t GetIntegratedTimeMilliseconds() const
	{
		return integratedTimeMilliseconds;
	}

	double GetIntegratedElevationGainMeters() const
	{
		return integratedElevationGainMeters;
	}

	double GetIntegratedElevationLossMeters() const
	{
		return integratedElevationLossMeters;
	}
};