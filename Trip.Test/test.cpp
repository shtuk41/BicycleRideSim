#include "pch.h"

#include <trip.h>
#include <gpxparser.h>

TEST(TestCaseName, ParsingTest) 
{
	trip _trip;

	std::string pathToGpxFile = "C:\\Files\\BicycleRideSim\\gpx\\Great_Weather_Ride.gpx";

	if (!parse_gpx_file(pathToGpxFile, _trip))
	{
		std::cout << "parsing failed" << std::endl;
		return;
	}

	EXPECT_EQ(_trip.name, "Great Weather Ride");
	EXPECT_EQ(_trip.time_initial_point, "2023-09-10T16:13:06Z");
	EXPECT_EQ(_trip.type, "cycling");
	EXPECT_NEAR(_trip.GetIntegratedDistanceMeters(), 43327.7, 0.1);
	EXPECT_NEAR(_trip.GetIntegratedTimeMilliseconds(), 5831000, 10.0);

}