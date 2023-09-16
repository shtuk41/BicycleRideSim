#include "pch.h"

#include <trip.h>
#include <gpxparser.h>

TEST(TestCaseName, TripParsingTest_GreatWeatherRide) 
{
	trip _trip;

	std::string pathToGpxFile = "..\\gpx\\Great_Weather_Ride.gpx";

	if (!parse_gpx_file(pathToGpxFile, _trip))
	{
		std::cout << "parsing failed" << std::endl;
		return;
	}

	EXPECT_EQ(_trip.name, "Great Weather Ride");
	EXPECT_EQ(_trip.time_initial_point, "2023-09-10T16:13:06Z");
	EXPECT_EQ(_trip.type, "cycling");
	EXPECT_NEAR(_trip.GetIntegratedDistanceMeters(), 43327.7, 0.1);
	EXPECT_EQ(_trip.GetIntegratedTimeMilliseconds(), 5831000);
	EXPECT_NEAR(_trip.GetIntegratedElevationGainMeters(), 293, 1.0);
	EXPECT_NEAR(_trip.GetIntegratedElevationLossMeters(), -294, 1.0);

}

TEST(TestCaseName, TripParsingTest_EveningWalk)
{
	trip _trip;

	std::string pathToGpxFile = "..\\gpx\\Evening_Walk.gpx";

	if (!parse_gpx_file(pathToGpxFile, _trip))
	{
		std::cout << "parsing failed" << std::endl;
		return;
	}

	EXPECT_EQ(_trip.name, "Evening Walk");
	EXPECT_EQ(_trip.time_initial_point, "2023-09-14T22:56:40Z");
	EXPECT_EQ(_trip.type, "walking");
	EXPECT_NEAR(_trip.GetIntegratedDistanceMeters(), 5070.19, 0.1);
	EXPECT_EQ(_trip.GetIntegratedTimeMilliseconds(), 3134000);
	EXPECT_NEAR(_trip.GetIntegratedElevationGainMeters(), 26.8, 1.0);
	EXPECT_NEAR(_trip.GetIntegratedElevationLossMeters(), -27.8, 1.0);

}

