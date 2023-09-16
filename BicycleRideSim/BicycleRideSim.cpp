// BicycleRideSim.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <trip.h>
#include <gpxparser.h>

int main()
{
	trip _trip;

	std::string pathToGpxFile = "C:\\Files\\BicycleRideSim\\gpx\\Great_Weather_Ride.gpx";

	if (!parse_gpx_file(pathToGpxFile, _trip))
	{
		std::cout << "parsing failed" << std::endl;
		return 0;
	}

	unsigned int num = 0;

	for (auto r : _trip.points)
	{
		std::cout << num << " ele delta: " << r.elevationDelta << " dis: " << r.distanceMetersFromPreviousReceord << std::endl;
		num++;
	}

	std::cout << "Distance traveled in meters: " << _trip.GetIntegratedDistanceMeters() << std::endl;
	double integratedTimeSeconds = _trip.GetIntegratedTimeMilliseconds() / 1000.0;
	auto work = 100 * _trip.GetIntegratedDistanceMeters() * _trip.GetIntegratedDistanceMeters() / (integratedTimeSeconds * integratedTimeSeconds);
	std::cout << "Work in Joules: " << work << std::endl;
}