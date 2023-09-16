// BicycleRideSim.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _USE_MATH_DEFINES

#include <chrono>
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
#include <rapidxml_utils.hpp>

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
};

std::chrono::system_clock::time_point getTimePointFromString(std::string &time_string)
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

int main()
{
	std::vector<record> records;
	int weightKg = 100;

	char xmldoc[] = "C:\\Files\\BicycleRideSim\\gpx\\Great_Weather_Ride.gpx";
	rapidxml::file<> file(xmldoc);
	rapidxml::xml_document<> doc;
	doc.parse<0>(file.data());
	rapidxml::xml_node<>* gpx = doc.first_node("gpx");
	rapidxml::xml_node<>* trk = gpx->first_node("trk");
	rapidxml::xml_node<>* trkseg = trk->first_node("trkseg");

	std::string prev_time = "start";
	double prev_elevation = 0.0;
	double prev_latitude;
	double prev_longitude;
	double integratedDistanceMeters = 0;
	double integratedTimeMilliseconds = 0;

	for (rapidxml::xml_node<>* trkpt = trkseg->first_node("trkpt"); trkpt; trkpt = trkpt->next_sibling())
	{	
		record rec;

		rec.latitude = std::atof(trkpt->first_attribute("lat")->value());
		rec.longitude = std::atof(trkpt->first_attribute("lon")->value());
		rec.elevation = std::atof(trkpt->first_node("ele")->value());
		rec.timestamp = trkpt->first_node("time")->value();

		if (prev_time.compare("start") != 0)
		{
			auto prev = getTimePointFromString(prev_time);
			auto current = getTimePointFromString(rec.timestamp);
			rec.timeDeltaMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(current - prev).count();
			integratedTimeMilliseconds += rec.timeDeltaMilliseconds;
			rec.elevationDelta = rec.elevation - prev_elevation;
			
			//TODO: Consider Haversine
			double perdeg = M_PI / 180.0;
			double latRad = rec.latitude * perdeg;
			double lonRad = rec.longitude * perdeg;
			double prev_latitudeRad = prev_latitude * perdeg;
			double prev_longitudeRad = prev_longitude * perdeg;

			double distanceMiles = 3963.0 * acos((sin(latRad) * sin(prev_latitudeRad)) + cos(latRad) * cos(prev_latitudeRad) * cos(lonRad - prev_longitudeRad));
			double distanceKm = 1.609344 * distanceMiles;

			rec.distanceMetersFromPreviousReceord = distanceKm * 1000.0f;
			integratedDistanceMeters += rec.distanceMetersFromPreviousReceord;
		}

		prev_time = rec.timestamp;
		prev_elevation = rec.elevation;
		prev_latitude = rec.latitude;
		prev_longitude = rec.longitude;


		records.push_back(rec);
	}

	int num = 0;

	for (auto r : records)
	{
		std::cout << num << " ele delta: " << r.elevationDelta << " dis: " << r.distanceMetersFromPreviousReceord << std::endl;
	}

	std::cout << "Distance traveled in meters: " << integratedDistanceMeters << std::endl;

	double integratedTimeSeconds = integratedTimeMilliseconds / 1000.0;

	auto work = 100 * integratedDistanceMeters * integratedDistanceMeters / (integratedTimeSeconds * integratedTimeSeconds);

	std::cout << "Work in Joules: " << work << std::endl;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
