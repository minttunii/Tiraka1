
#include "datastructures.hh"
#include <random>
#include <cmath>
#include <queue>
#include <unordered_set>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

/**
 * @brief Does nothing
 */
Datastructures::Datastructures()
{
    // Write any initialization you need here
}

/**
 * @brief Deletes stations
 */
Datastructures::~Datastructures()
{
    delete_stations_if_needed();
}

/**
 * @brief Returns the number of stations
 *
 * @return unsigned int The number of stations
 */
unsigned int Datastructures::station_count()
{
    // Replace the line below with your implementation
    return stations_from_id.size();
}

/**
 * @brief Removes all stations, regions and other data that is related to these
 */
void Datastructures::clear_all()
{
    // Replace the line below with your implementation
    stations_from_id.clear();
    stations_from_coord.clear();
    regions_from_id.clear();
    alphabetic_stations.clear();
    distance_stations.clear();
    alphabetic_stations_vector.clear();
    distance_stations_vector.clear();
    alphabetic_stations_needs_update = true;
    distance_stations_needs_update = true;
    removed_stations.clear();
    stations_list.clear();
}

/**
 * @brief Returns the IDs of all stations
 *
 * @return std::vector<StationID> All station IDs
 */
std::vector<StationID> Datastructures::all_stations()
{
    // Replace the line below with your implementation
    std::vector<StationID> stations;
    stations.reserve(stations_from_id.size());
    for (auto&& station : stations_from_id) stations.push_back(station.first);
    return stations;
}

/**
 * @brief Adds a new station
 *
 * @param id Station ID for the new station. ID should be unique.
 * @param name Name for the new station
 * @param xy Station coordinates for the new station
 * @return true if successful i.e. the ID is not in the data
 * @return false if not successful i.e. the ID already exists in the data
 */
bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if (stations_from_id.count(id)) return false;

    stations_list.push_front({id, name, xy});
    auto it = stations_list.begin();
    stations_from_id.insert({ id, it });
    stations_from_coord.insert({ xy, it });
    alphabetic_stations.insert(it);
    distance_stations.insert(it);
    alphabetic_stations_needs_update = true;
    distance_stations_needs_update = true;
    return true;
}

/**
 * @brief Returns the name of the station whose ID corresponds to the argument
 *
 * @param id ID of the station
 * @return Name The name of the station
 */
Name Datastructures::get_station_name(StationID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    auto stationIt = stations_from_id.find(id);
    if (stationIt == stations_from_id.end()) return NO_NAME;
    auto station = stationIt->second;

    return station->name;
}

/**
 * @brief Returns the coordinates of the station whose ID corresponds to the
 * argument
 *
 * @param id ID of the station
 * @return Coord The coordinates of the station
 */
Coord Datastructures::get_station_coordinates(StationID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    auto stationIt = stations_from_id.find(id);
    if (stationIt == stations_from_id.end()) return NO_COORD;
    auto& station = stationIt->second;

    return station->coord;
}

/**
 * @brief Returns all stations in alphabetic order
 *
 * @return std::vector<StationID> All stations in alphabetic order
 */
std::vector<StationID> Datastructures::stations_alphabetically()
{
    // Replace the line below with your implementation
    if (alphabetic_stations_needs_update) {
        delete_stations_if_needed();
        alphabetic_stations_vector = setToVector(alphabetic_stations);
        alphabetic_stations_needs_update = false;
    }
    return alphabetic_stations_vector;
}

/**
 * @brief Returns all stations in the increasing order of the distance
 * from origin
 *
 * @return std::vector<StationID> All stations in the increasing order of
 * the distance from origin
 */
std::vector<StationID> Datastructures::stations_distance_increasing()
{
    // Replace the line below with your implementation
    if (distance_stations_needs_update) {
        delete_stations_if_needed();
        distance_stations_vector = setToVector(distance_stations);
        distance_stations_needs_update = false;
    }
    return distance_stations_vector;
}

/**
 * @brief Returns the ID of the station whose coordinates corresponds to the
 * argument
 *
 * @param xy Coordinates of the station
 * @return StationID ID of the station
 */
StationID Datastructures::find_station_with_coord(Coord xy)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    auto stationIt = stations_from_coord.find(xy);
    if (stationIt == stations_from_coord.end()) return NO_STATION;
    auto& station = stationIt->second;

    return station->id;
}

/**
 * @brief Changes the coordinates of the station
 *
 * @param id The ID of the station
 * @param newcoord The new coordinates of the station
 * @return true if successful i.e. station was found and the coordinates were changed
 * @return false if not successful i.e. station was not found
 */
bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    auto stationIt = stations_from_id.find(id);
    if (stationIt == stations_from_id.end()) return false;
    auto& station = stationIt->second;

    // Remove
    stations_from_coord.erase(station->coord);
    distance_stations.erase(station);
    // Change
    station->coord = newcoord;
    // Add
    stations_from_coord.insert({ newcoord, station });
    distance_stations.insert(station);
    distance_stations_needs_update = true;
    return true;
}

/**
 * @brief Adds a new train to the station whose ID corresponds to the argument
 *
 * @param stationid The ID of the station
 * @param trainid The ID of the new train
 * @param time The departure time of the new train
 * @return true if successful i.e. the train was added
 * @return false if the station was not found or the train already exists
 */
bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    auto stationIt = stations_from_id.find(stationid);
    if (stationIt == stations_from_id.end()) return false;
    auto& station = stationIt->second;

    auto& trains = station->train_departure_times;
    if (trains.count({ time, trainid })) return false;
    trains.insert({ time, trainid });
    return true;
}

/**
 * @brief Removes the train departure from the station at a specific
 * departure time.
 *
 * The train and the station are identified by their ID in the argument.
 *
 * The departure time, in the argument, must be the same as the one given in
 * #add_departure
 *
 *
 * @param stationid ID of the station
 * @param trainid ID of the train
 * @param time Departure time of the train
 * @return true if successful
 * @return false if the station or the train was not found
 */
bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    auto stationIt = stations_from_id.find(stationid);
    if (stationIt == stations_from_id.end()) return false;
    auto& station = stationIt->second;

    auto& trains = station->train_departure_times;
    auto trainIt = trains.find({ time, trainid });
    if (trainIt == trains.end()) return false;
    trains.erase(trainIt);
    return true;
}

/**
 * @brief Returns the trains that leave from the station after the given
 * departure time, including the departure time.
 * The train is identified by its ID in the argument.
 *
 *
 * @param stationid ID of the station
 * @param time Departure time
 * @return std::vector<std::pair<Time, TrainID>> The trains that leave from
 * the station after the given departure time, including the departure time
 */
std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    auto stationIt = stations_from_id.find(stationid);
    if (stationIt == stations_from_id.end()) return {{ NO_TIME, NO_TRAIN }};
    auto& station = stationIt->second;
    auto& times = station->train_departure_times;

    std::vector<std::pair<Time, TrainID>> result;
    std::copy(times.lower_bound({ time, "" }), times.end(), std::back_inserter(result));
    return result;
}

/**
 * @brief Adds the region to the data
 *
 * @param id Unique ID of the new region
 * @param name Name of the region
 * @param coords Coordinates of the region. The coordinate vector makes a
 * polygon area by drawing a line between each consecutive point in the vector.
 * The first and last point, in the vector, are connected.
 * @return true if successful i.e. the ID is not in the data
 * @return false if not successful i.e. the ID already exists in the data
 */
bool Datastructures::add_region(RegionID id, const Name& name, std::vector<Coord> coords)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if (regions_from_id.count(id)) return false;

    regions_from_id.insert({ id, { id, name, coords } });
    return true;
}

/**
 * @brief Returns the ID of all regions
 *
 * @return std::vector<RegionID> Region IDs
 */
std::vector<RegionID> Datastructures::all_regions()
{
    // Replace the line below with your implementation
    std::vector<RegionID> regions;
    regions.reserve(regions_from_id.size());
    for (auto&& [id, _] : regions_from_id) regions.push_back(id);
    return regions;
}

/**
 * @brief Returns the name of the region whose ID corresponds to the argument
 *
 * @param id ID of the region
 * @return Name The name of the region
 */
Name Datastructures::get_region_name(RegionID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    auto regionIt = regions_from_id.find(id);
    if (regionIt == regions_from_id.end()) return NO_NAME;
    auto& region = regionIt->second;

    return region.name;
}

/**
 * @brief Returns the coordinates of the region whose ID corresponds to the
 * argument
 *
 * @param id ID of the region
 * @return std::vector<Coord> The coordinates of the region
 */
std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    auto regionIt = regions_from_id.find(id);
    if (regionIt == regions_from_id.end()) return { NO_COORD };
    auto& region = regionIt->second;

    return region.coord;
}

/**
 * @brief Makes the region whose ID is "id" be a subregion of the region
 * whose ID is "parentid". If subregion already has a parent region,
 * nothing is done.
 *
 * @param id Region that will be the subregion
 * @param parentid Region that will be the parent region
 * @return true if successful
 * @return false if nothing was changed i.e. IDs were not found or
 * the subregion already has a parent region
 */
bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    auto regionIt = regions_from_id.find(id);
    if (regionIt == regions_from_id.end()) return false;
    auto& region = regionIt->second;

    if (region.parentRegion) return false;

    auto parentRegionIt = regions_from_id.find(parentid);
    if (parentRegionIt == regions_from_id.end()) return false;
    auto& parentRegion = parentRegionIt->second;

    region.parentRegion = &parentRegion;
    parentRegion.subRegions.push_back(&region);
    return true;
}

/**
 * @brief Adds the station to the region.
 * If the station already belongs to a region, the method does nothing and
 * returns false
 *
 * @param id ID of the station
 * @param parentid ID of the region
 * @return true if successful
 * @return false if station ID or region ID were not found or station already
 * belongs to a region
 */
bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    auto stationIt = stations_from_id.find(id);
    if (stationIt == stations_from_id.end()) return false;
    auto& station = stationIt->second;

    if (station->region) return false;

    auto regionIt = regions_from_id.find(parentid);
    if (regionIt == regions_from_id.end()) return false;
    auto& region = regionIt->second;

    station->region = &region;
    return true;
}

/**
 * @brief Returns the region IDs of all the regions in which the station is in,
 * including the parent regions of the station region.
 *
 * @param id Station ID
 * @return std::vector<RegionID> The region IDs of all the regions in which the
 * station is in, including the parent regions of the station region.
 */
std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    auto stationIt = stations_from_id.find(id);
    if (stationIt == stations_from_id.end()) return { NO_REGION };
    auto& station = stationIt->second;

    std::vector<RegionID> regions;
    Region* regionIt = station->region;
    while (regionIt)
    {
        regions.push_back(regionIt->id);
        regionIt = regionIt->parentRegion;
    }
    return regions;
}

/**
 * @brief Returns all subregions of the region whose ID corresponds to the
 * argument.
 *
 * @param id ID of the region
 * @return std::vector<RegionID> All subregions of the region whose ID
 * corresponds to the argument
 */
std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    auto regionIt = regions_from_id.find(id);
    if (regionIt == regions_from_id.end()) return { NO_REGION };
    auto& parentRegion = regionIt->second;

    std::vector<RegionID> subregions;
    iterateTree(subregions, parentRegion);
    return subregions;
}

/**
 * @brief Returns the three most closest stations to the given coordinates
 *
 * @param xy The coordinates to which the closest stations are searched
 * @return std::vector<StationID> The three most closest stations to the
 * given coordinates
 */
std::vector<StationID> Datastructures::stations_closest_to(Coord xy)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    std::set<StationComp> closest3;
    for (auto&& station : stations_from_id)
    {
        StationComp comp { station.second, xy };

        if (closest3.size() < 3)
        {
            closest3.insert(comp);
            continue;
        }
        auto last_closest3 = std::prev(closest3.end());
        if (comp < *last_closest3)
        {
            closest3.erase(last_closest3);
            closest3.insert(comp);
        }
    }
    std::vector<StationID> result;
    for (auto&& station_comp : closest3) result.push_back(station_comp.id);
    return result;
}

/**
 * @brief Removes the station from the data structure
 *
 * @param id ID of the station
 * @return true if successful
 * @return false if ID was not found
 */
bool Datastructures::remove_station(StationID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    auto stationIt = stations_from_id.find(id);
    if (stationIt == stations_from_id.end()) return false;
    auto station = stationIt->second;

    stations_from_id.erase(stationIt);
    stations_from_coord.erase(station->coord);

    // Erasing elements from set is not a constant operation.
    // Do it somewhere else.
    removed_stations.push_back(station);
    // alphabetic_stations.erase(station);
    // distance_stations.erase(station);
    alphabetic_stations_needs_update = true;
    distance_stations_needs_update = true;

    return true;
}

/**
 * @brief Returns a region that is a parent or a distant parent of the two
 * given regions
 *
 * @param id1 ID of a region
 * @param id2 ID of another region
 * @return RegionID Region that is a parent or a distant parent of the two
 * given regions
 */
RegionID Datastructures::common_parent_of_regions(RegionID id1, RegionID id2)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    auto regionIt1 = regions_from_id.find(id1);
    if (regionIt1 == regions_from_id.end()) return NO_REGION;
    Region* regIt1 = &regionIt1->second;

    auto regionIt2 = regions_from_id.find(id2);
    if (regionIt2 == regions_from_id.end()) return NO_REGION;
    Region* regIt2 = &regionIt2->second;

    std::unordered_set<RegionID> parentRegions1;
    regIt1 = regIt1->parentRegion;
    while (regIt1)
    {
        parentRegions1.insert({ regIt1->id });
        regIt1 = regIt1->parentRegion;
    }
    while (regIt2)
    {
        if (parentRegions1.count(regIt2->id))
        {
            return regIt2->id;
        }
        regIt2 = regIt2->parentRegion;
    }

    return NO_REGION;
}
