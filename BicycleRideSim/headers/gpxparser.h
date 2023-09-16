#pragma once

#define _USE_MATH_DEFINES

#include <chrono>
#include <math.h>
#include <string>
#include <vector>

#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
#include <rapidxml_utils.hpp>

#include <trip.h>

bool parse_gpx_file(std::string &filePath, trip &tr)
{
	try
	{
		rapidxml::file<> file(filePath.c_str());
		rapidxml::xml_document<> doc;
		doc.parse<0>(file.data());
		rapidxml::xml_node<>* gpx = doc.first_node("gpx");

		tr.time_initial_point = gpx->first_node("metadata")->first_node("time")->value();
		

		rapidxml::xml_node<>* trk = gpx->first_node("trk");
		tr.name = trk->first_node("name")->value();
		tr.type = trk->first_node("type")->value();

		rapidxml::xml_node<>* trkseg = trk->first_node("trkseg");

		std::string prev_time = "start";
		double prev_elevation = 0.0;
		double prev_latitude;
		double prev_longitude;

		for (rapidxml::xml_node<>* trkpt = trkseg->first_node("trkpt"); trkpt; trkpt = trkpt->next_sibling())
		{
			record rec;

			rec.latitude = std::atof(trkpt->first_attribute("lat")->value());
			rec.longitude = std::atof(trkpt->first_attribute("lon")->value());
			rec.elevation = std::atof(trkpt->first_node("ele")->value());
			rec.timestamp = trkpt->first_node("time")->value();
			auto extensions = trkpt->first_node("extensions");
			
			if (extensions)
			{
				rec.temperature = std::atof(extensions->first_node("gpxtpx:TrackPointExtension")->first_node("gpxtpx:atemp")->value());
			}

			if (prev_time.compare("start") != 0)
			{
				auto prev = getTimePointFromString(prev_time);
				auto current = getTimePointFromString(rec.timestamp);
				rec.timeDeltaMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(current - prev).count();
				tr.IntegrateTimeMilliseconds(rec.timeDeltaMilliseconds);
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
				tr.IntegraeteDistanceMeters(rec.distanceMetersFromPreviousReceord);
				tr.IntegrateElevationGainMeters(rec.elevationDelta > 0 ? rec.elevationDelta : 0.0);
				tr.IntegrateElevationLossMeters(rec.elevationDelta < 0 ? rec.elevationDelta : 0.0);

			}

			prev_time = rec.timestamp;
			prev_elevation = rec.elevation;
			prev_latitude = rec.latitude;
			prev_longitude = rec.longitude;


			tr.points.push_back(rec);
		}
	}
	catch (std::exception& e)
	{
		std::cout << "error during gpx parser: " << e.what() << "\n";
		return false;
	}

	return true;
}