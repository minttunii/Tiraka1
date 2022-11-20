// Datastructures.cc
//
// Student name: Minttu Niiranen
// Student email: minttu.niiranen@tuni.fi
// Student number: 0408538846

#include "datastructures.hh"

#include <memory>
#include <random>
#include <iostream>
#include <cmath>

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


Datastructures::Datastructures()
{

}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here

}

/*!
 * \brief Datastructures::station_count counts the ammount of stations
 * \return number of stations
 */
unsigned int Datastructures::station_count()
{
    return stations.size();
}

/*!
 * \brief Datastructures::clear_all clears all data structures from elements
 */
void Datastructures::clear_all()
{
    stations.clear();
    regions.clear();
    stations_to_order.clear();
}

/*!
 * \brief Datastructures::all_stations saves all stations IDs into a vector
 * \return vector with all station IDs
 */
std::vector<StationID> Datastructures::all_stations()
{
    std::vector<StationID> all_stations;

    for(auto const& elem : stations){
        all_stations.push_back(elem.first);
    }
    return all_stations;
}

/*!
 * \brief Datastructures::add_station adds new station to the datastructures
 * \param id station id
 * \param name name of the station
 * \param xy station's coordinates
 * \return false if station already exists, true if new station is added successfully
 */
bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{
    // If station already exists, return false
    if(stations.find(id) != stations.end()){
        return false;
    }
    station.name = name; station.coord = xy;
    std::shared_ptr<Station> station_ptr = std::make_shared<Station>(station);
    stations.insert({id, station_ptr});
    stations_to_order.push_back({id, station_ptr});
    return true;
}

/*!
 * \brief Datastructures::get_station_name search for the given station's name based on the id
 * \param id station id
 * \return name of the station
 */
Name Datastructures::get_station_name(StationID id)
{
    auto it = stations.find(id);
    // If station is not found
    if(it == stations.end()){
        return NO_NAME;
    }
    return it->second->name;
}

/*!
 * \brief Datastructures::get_station_coordinates search station's coordinates based on the id
 * \param id station id
 * \return station coordinates
 */
Coord Datastructures::get_station_coordinates(StationID id)
{
    auto it = stations.find(id);
    // If station is not found
    if(it == stations.end()){
        return NO_COORD;
    }
    return it->second->coord;
}

/*!
 * \brief Datastructures::stations_alphabetically sorts station alphabetically and
 *  adds station ids in vector in that order
 * \return station id vector in order
 */
std::vector<StationID> Datastructures::stations_alphabetically()
{
    std::vector<StationID> stations_alph;
    stations_alph.reserve(stations.size());

    sort(stations_to_order.begin(), stations_to_order.end(),
         [](const std::pair<StationID, std::shared_ptr<Station>> &a,
         const std::pair<StationID, std::shared_ptr<Station>> &b)-> bool{

            return a.second->name < b.second->name; });

    for(auto const &elem : stations_to_order){
        stations_alph.push_back(elem.first);
    }
    return stations_alph;
}

/*!
 * \brief Datastructures::stations_distance_increasing orders vector based on
 * coordinates and adds station ids in vector
 * \return station id vector in order
 */
std::vector<StationID> Datastructures::stations_distance_increasing()
{
    std::vector<StationID> stations_dist;
    stations_dist.reserve(stations.size());

    sort(stations_to_order.begin(), stations_to_order.end(),
         [](const std::pair<StationID, std::shared_ptr<Station>> &a,
         const std::pair<StationID, std::shared_ptr<Station>> &b) -> bool{
        int xa = a.second->coord.x; int ya = a.second->coord.y;
        int xb = b.second->coord.x; int yb = b.second->coord.y;

        if(xa*xa+ya*ya == xb*xb+yb*yb){ return ya < yb;}
        return xa*xa+ya*ya < xb*xb+yb*yb;
    });

    for(auto const &elem : stations_to_order){
        stations_dist.push_back(elem.first);
    }
    return stations_dist;
}

/*!
 * \brief Datastructures::find_station_with_coord finds station id based on the coordinate
 * \param xy station coordinate
 * \return station id of station is found, otherwise NO_STATION
 */
StationID Datastructures::find_station_with_coord(Coord xy)
{
    for(auto const& elem : stations){
        if(elem.second->coord == xy){
            return elem.first;
        }
    }
    return NO_STATION;
}

/*!
 * \brief Datastructures::change_station_coord changes the given station's coordinates
 * \param id station id
 * \param newcoord new coordinates of station
 * \return false if station is not found, true if coordinate is changed
 */
bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{
    auto it = stations.find(id);
    if(it == stations.end()){
        return false;
    }
    it->second->coord = newcoord;
    return true;
}

/*!
 * \brief Datastructures::add_departure adds departure to the station
 * \param stationid station id
 * \param trainid train id
 * \param time time the train leaves the station
 * \return false if station is not found or departure already exist, true if departure is added
 */
bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{
    auto it = stations.find(stationid);
    if(it == stations.end()){
        return false;
    }
    auto trains = it->second->trains;
    auto it2 = trains.find(time);

    // Check if departure already exist
    if(it2 != trains.end()){
        for(auto i = it2; i != trains.end(); i++){
            if(i->first == time && i->second == trainid){
                return false;
            }
            else if(i->first != time){
                break;
            }
        }
    }
    trains.insert({time, trainid});
    it->second->trains = trains;
    return true;
}

/*!
 * \brief Datastructures::remove_departure remove departure from station
 * \param stationid station id
 * \param trainid train id
 * \param time time when the train leaves station
 * \return false if station or departure doesn't exist, true if departure is removed
 */
bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{
    auto it = stations.find(stationid);
    if(it == stations.end()){
        return false;
    }

    auto trains = it->second->trains;
    auto it2 = trains.find(time);
    // Check if departure to remove exist
    if(it2 != trains.end()){
        for(auto i = it2; i != trains.end(); i++){
            if(i->first == time && i->second == trainid){
                trains.erase(i);
                it->second->trains = trains;
                return true;
            }
        }
    }
    return false;
}

/*!
 * \brief Datastructures::station_departures_after search all station departures after given time
 * \param stationid station id
 * \param time time after all departures are searched
 * \return vector of departures after given time or {NO_TIME, NO_TRAIN} if station is not found
 */
std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{
    std::vector<std::pair<Time, TrainID>> departures;

    auto it = stations.find(stationid);
    if(it == stations.end()){
        return {{NO_TIME, NO_TRAIN}};
    }

    auto trains = it->second->trains;
    for(auto &elem : trains){
        if(elem.first >= time){
            departures.push_back({elem.first, elem.second});
        }
    }
    return departures;
}

/*!
 * \brief Datastructures::add_region adds new region
 * \param id region id
 * \param name region name
 * \param coords region coordinates
 * \return false if region already exists, true if region is added
 */
bool Datastructures::add_region(RegionID id, const Name &name, std::vector<Coord> coords)
{
    auto it = regions.find(id);
    if(it != regions.end()){
        return false;
    }
    region.name = name; region.coords = coords;
    regions.insert({id, region});
    return true;
}

/*!
 * \brief Datastructures::all_regions return all reagion ids in a vector
 * \return vector with region ids or empty vector if there are no regions
 */
std::vector<RegionID> Datastructures::all_regions()
{
    std::vector<RegionID> all_regions;
    // If there are no regions
    if(regions.empty()){
        return all_regions;
    }

    for(auto &elem : regions){
        all_regions.push_back(elem.first);
    }
    return all_regions;
}

/*!
 * \brief Datastructures::get_region_name get given region's name
 * \param id region id
 * \return name of the region or NO_NAME if region is not found
 */
Name Datastructures::get_region_name(RegionID id)
{
    auto it = regions.find(id);
    if(it == regions.end()){
        return NO_NAME;
    }
    return it->second.name;
}

/*!
 * \brief Datastructures::get_region_coords get given region's coordinates
 * \param id region id
 * \return region coordinates or NO_COORDS if region is not found
 */
std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{
    auto it = regions.find(id);
    if(it == regions.end()){
        return {NO_COORD};
    }
    return it->second.coords;
}

/*!
 * \brief Datastructures::add_subregion_to_region adds subregion to parent region
 * \param id subregion id
 * \param parentid parent region id
 * \return false if reagions doesn't exist or subregion already belongs to a region
 */
bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    auto it = regions.find(id); // Subregion
    auto it2 = regions.find(parentid); // Parent region

    // If regions doesn't exist or subregion belongs to a region
    if(it == regions.end() || it2 == regions.end() || it->second.upper_region != nullptr){
        return false;
    }
    // Create pointer to parent region and add it to subregion
    std::pair<const RegionID, Region>* parent = &(*it2);
    it->second.upper_region = parent;
    // Create pointer to subreagion and add it to parent region
    std::pair<const RegionID, Region>* sub = &(*it);
    it2->second.sub_regions.insert(sub);
    return true;
}

bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    auto it = stations.find(id);
    auto it2 = regions.find(parentid);
    // If station or region doesn't exist or station already belongs to region
    if(it == stations.end() || it2 == regions.end() || it->second->upper_id != 0){
        return false;
    }
    it2->second.stations_in_region.insert(id);
    it->second->upper_id = parentid;
    return true;
}

std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    auto it = stations.find(id);
    // If station is not found
    if(it == stations.end()){
        return {NO_REGION};
    }
    //If station doesn't belong to the region
    else if(it->second->upper_id == 0){
        return {};
    }

    std::vector<RegionID> station_regions;

    // Search for area where the station belongs to
    auto it2 = regions.find(it->second->upper_id);
    station_regions.push_back(it2->first);

    // Search for parent area
    auto iter = it2->second.upper_region;

    while(iter != nullptr){
        station_regions.push_back(iter->first);
        iter = iter->second.upper_region;
    }
    delete iter;
    return station_regions;
}

/*!
 * \brief Datastructures::subregions search all subregions of region recursively
 * \param sub_regions set of all subreagion of certain region
 * \param sub_IDs vector to store region ids
 * \return vector of subregion ids
 */
std::vector<RegionID> Datastructures::subregions(
        std::unordered_set<std::pair<const RegionID, Region>*> sub_regions,
                                                 std::vector<RegionID>& sub_IDs){

    if(sub_regions.empty()){
        return sub_IDs;
    }
    for(auto item = sub_regions.begin(); item != sub_regions.end(); item++){
        sub_IDs.push_back((*item)->first);
        subregions((*item)->second.sub_regions, sub_IDs);
    }
    return sub_IDs;
}

/*!
 * \brief Datastructures::all_subregions_of_region search all subregions of given
 *  region with recursive function subregions
 * \param id region id
 * \return vector with all subregion ids or NO_REGION if region is not found
 */
std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID id)
{
    auto it = regions.find(id);
    // If region is not found
    if(it == regions.end()){
        return {NO_REGION};
    }
    // if area doesn't have sub regions
    else if(it->second.sub_regions.empty()){
        return {};
    }

    auto sub_region = it->second.sub_regions;
    std::vector<RegionID> sub_IDs;
    return subregions(sub_region, sub_IDs);
}

/*!
 * \brief Datastructures::stations_closest_to return three closest station of given station
 * \param xy given station coordinates
 * \return vector with closest station ids
 */
std::vector<StationID> Datastructures::stations_closest_to(Coord xy)
{
    std::multimap<int, StationID> distances;
    std::vector<StationID> closest;

    for(auto const &elem: stations){
        int x2 = elem.second->coord.x;
        int y2 = elem.second->coord.y;
        distances.insert({(xy.x-x2)*(xy.x-x2)+(xy.y-y2)*(xy.y-y2), elem.first});
    }

    int k = 3;
    for(auto it = distances.begin(); it != distances.end() && k > 0 ; it++, k-- ){
        closest.push_back(it->second);
    }
    return closest;
}

/*!
 * \brief Datastructures::remove_station remove given station
 * \param id station id
 * \return false is station is not found, true if station is removed
 */
bool Datastructures::remove_station(StationID id)
{
    auto it = stations.find(id);
    if(it == stations.end()){
        return false;
    }

    //Station has to be deleted from region
    if(it->second->upper_id != 0){
        auto it2 = regions.find(it->second->upper_id);
        auto stations_in_region = it2->second.stations_in_region;
        auto iter = stations_in_region.find(id);
        if(iter != stations_in_region.end()){
            stations_in_region.erase(iter);
            it2->second.stations_in_region = stations_in_region;
        }
    }
    //Station has to be deleted from stations_to_order vector
    for(auto i = stations_to_order.begin(); i != stations_to_order.end(); i++){
        if(i->first == id){
            stations_to_order.erase(i);
            break;
        }
    }
    stations.erase(it);
    return true;
}

/*!
 * \brief Datastructures::common_parent recursive function to search common parent of two regions
 * \param prev1 pointer to the previous upper region of the first region
 * \param prev2 pointer to the previous upper region of the second region
 * \param curr1 pointer to the next upper region of the first region
 * \param curr2 pointer to the next upper region of the second region
 * \return region id of common parent or NO_REGION if there is no common parent
 */
RegionID Datastructures::common_parent(std::pair<const RegionID, Region>* prev1,
                                       std::pair<const RegionID, Region>* prev2,
                                       std::pair<const RegionID, Region>* curr1,
                                       std::pair<const RegionID, Region>* curr2){
    //Cases where common region can be found
    if(prev1->first == prev2->first){
        return prev1->first;
    }
    else if(curr1 != nullptr && prev1->first == curr1->first){
        return prev1->first;
    }
    else if(curr1 != nullptr  && prev2->first == curr1->first){
        return prev2->first;
    }
    else if(curr2 != nullptr  && prev1->first == curr2->first){
        return prev1->first;
    }
    else if(curr1 != nullptr && curr2 != nullptr && curr1->first == curr2->first){
        return curr1->first;
    }
    else{
        if(curr1 != nullptr && curr2 != nullptr && curr1->second.upper_region != nullptr
                && curr2->second.upper_region != nullptr){
            common_parent(curr1, curr2, curr1->second.upper_region, curr2->second.upper_region);
        }
        else if(curr1 != nullptr && curr2 != nullptr && curr1->second.upper_region == nullptr
                && curr2->second.upper_region != nullptr){
            common_parent(curr1, curr2, curr1, curr2->second.upper_region);
        }
        else if(curr1 != nullptr && curr2 != nullptr && curr1->second.upper_region != nullptr
                && curr2->second.upper_region == nullptr){
            common_parent(curr1, curr2, curr1->second.upper_region, curr2);
        }
        else{
            return NO_REGION;
        }
    }
    return NO_REGION;
}

/*!
 * \brief Datastructures::common_parent_of_regions search common parent of given
 *  regions with recursive function common_parent
 * \param id1 id of first region
 * \param id2 id of second region
 * \return region id of the common parent, or NO_REGION if common parent or regions
 * doesn't exist
 */
RegionID Datastructures::common_parent_of_regions(RegionID id1, RegionID id2)
{
    auto it = regions.find(id1);
    auto it2 = regions.find(id2);
    // If regions are not found
    if(it == regions.end() || it2 == regions.end()){
        return NO_REGION;
    }
    auto parent1 = it->second.upper_region;
    auto parent2 = it2->second.upper_region;

    // If parent regions are not found
    if(parent1 == nullptr || parent2 == nullptr){
        return NO_REGION;
    }
    return common_parent(parent1, parent2, parent1->second.upper_region, parent2->second.upper_region);
}
