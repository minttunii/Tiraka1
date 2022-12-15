// Datastructures.cc
//
// Student name: Minttu Niiranen
// Student email: minttu.niiranen@tuni.fi
// Student number: H291834

#include "datastructures.hh"

#include <memory>
#include <random>

#include <cmath>
#include <queue>

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
    // Write any initialization you need here
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
    station_names.clear();
    station_coords.clear();
}

/*!
 * \brief Datastructures::all_stations saves all stations IDs into a vector
 * \return vector with all station IDs
 */
std::vector<StationID> Datastructures::all_stations()
{
    std::vector<StationID> all_stations;
    all_stations.reserve(stations.size());

    std::for_each(stations.begin(),stations.end(),
                     [&all_stations](const std::pair<StationID,Station> &s)
                     { all_stations.push_back(s.first); });
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
    station.name = name; station.coord = xy; station.stationid = id;
    stations.insert({id, station});
    station_coords.insert({xy, id});
    station_names.insert({name, id});
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
    return it->second.name;
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
    return it->second.coord;
}

/*!
 * \brief Datastructures::stations_alphabetically station ids are in alphabetical order and
 *  station ids sre added to vector in that order
 * \return station id vector in order
 */
std::vector<StationID> Datastructures::stations_alphabetically()
{
    std::vector<StationID> stations_alph;
    stations_alph.reserve(stations.size());

    std::for_each(station_names.begin(),station_names.end(),
                         [&stations_alph](const std::pair<Name, StationID> &s)
                         { stations_alph.push_back(s.second); });
    return stations_alph;
}

/*!
 * \brief Datastructures::stations_distance_increasing orders vector based on
 * coordinates and adds station ids in vector
 * \return station id vector in order
 */
std::vector<StationID> Datastructures::stations_distance_increasing()
{
    std::vector<StationID> station_dist;
    station_dist.reserve(stations.size());

    std::for_each(station_coords.begin(),station_coords.end(),
                         [&station_dist](const std::pair<Coord, StationID> &s)
                         { station_dist.push_back(s.second);});
    return station_dist;
}

/*!
 * \brief Datastructures::find_station_with_coord finds station id based on the coordinate
 * \param xy station coordinate
 * \return station id of station is found, otherwise NO_STATION
 */
StationID Datastructures::find_station_with_coord(Coord xy)
{
    auto it = station_coords.find(xy);
    if(it == station_coords.end()){
        return NO_STATION;
    }
    return it->second;
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
    // If station is not found
    if(it == stations.end()){
        return false;
    }
    auto it2 = station_coords.find(it->second.coord);
    it->second.coord = newcoord;
    station_coords.erase(it2);
    station_coords.insert({newcoord, id});

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
    auto departures = it->second.departures;
    auto it2 = departures.find(time);

    // Check if departure already exist
    if(it2 != departures.end()){
        for(auto i = it2; i != departures.end(); i++){
            if(i->first == time && i->second == trainid){
                return false;
            }
            else if(i->first != time){
                break;
            }
        }
    }
    departures.insert({time, trainid});
    it->second.departures = departures;
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

    auto departures = it->second.departures;
    auto it2 = departures.find(time);
    // Check if departure to remove exist
    if(it2 != departures.end()){
        for(auto i = it2; i != departures.end(); i++){
            if(i->first == time && i->second == trainid){
                departures.erase(i);
                it->second.departures = departures;
                return true;
            }
        }
    }
    return false;
}

/*!
 * \brief Datastructures::station_departures_after searches all station departures after given time
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

    auto trains = it->second.departures;
    auto itlow = trains.lower_bound(time);

    for(auto i = itlow; i != trains.end(); i++){
        departures.push_back({i->first, i->second});
    }
    return departures;
}

/*!
 * \brief Datastructures::add_region adds new region to datastructure
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
    all_regions.reserve(regions.size());
    // If there are no regions
    if(regions.empty()){
        return all_regions;
    }

    std::for_each(regions.begin(), regions.end(),
                     [&all_regions](const std::pair<RegionID, Region> &r)
                     { all_regions.push_back(r.first); });
    return all_regions;
}

/*!
 * \brief Datastructures::get_region_name get the given region's name
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
 * \brief Datastructures::get_region_coords get the given region's coordinates
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

/*!
 * \brief Datastructures::add_station_to_region adds station to region
 * \param id station id
 * \param parentid region id
 * \return false if station doesn't exist or station already belongs to a region,
 * true is station is added to the region
 */
bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    auto it = stations.find(id);
    auto it2 = regions.find(parentid);
    // If station or region doesn't exist or station already belongs to a region
    if(it == stations.end() || it2 == regions.end() || it->second.upper_id != 0){
        return false;
    }
    it2->second.stations_in_region.insert(id);
    it->second.upper_id = parentid;
    return true;
}

/*!
 * \brief Datastructures::station_in_regions adds all reagion ids that the station
 * belongs to directly or indirectly
 * \param id station id
 * \return vector with region ids where the station belongs to, NO_REGION if station
 * doesn't exist and empty vector if station doesn't belong to any region
 */
std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    auto it = stations.find(id);
    // If station is not found
    if(it == stations.end()){
        return {NO_REGION};
    }
    //If station doesn't belong to the region
    else if(it->second.upper_id == 0){
        return {};
    }

    std::vector<RegionID> station_regions;

    // Search for area where the station belongs to
    auto it2 = regions.find(it->second.upper_id);
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
 * \brief Datastructures::subregions searches all subregions of region recursively
 * \param sub_regions set of all subregion of a certain region
 * \param sub_IDs vector to store the region ids
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
    // If area doesn't have sub regions
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
    std::vector<StationID> closest_to;
    std::multimap<int, StationID> sorted_distances;

    // Find the closest element of given point xy
    auto it = std::min_element(station_coords.begin(), station_coords.end(),
                               [&xy](const auto &c1, const auto &c2){
        int x = xy.x; int y = xy.y;
        int x1 = c1.first.x; int y1 = c1.first.y;
        int x2 = c2.first.x; int y2 = c2.first.y;

       if((x-x1)*(x-x1)+(y-y1)*(y-y1) == (x-x2)*(x-x2)+(y-y2)*(y-y2)) {
           return y1 < y2;
       }
       else{
           return (x-x1)*(x-x1)+(y-y1)*(y-y1) < (x-x2)*(x-x2)+(y-y2)*(y-y2);
       }
    });

    // The next closest should be found before or after the minimum since
    // station_coords is sorted by coordinates
    if(it != station_coords.end()){
        int k = 3;
        // Add min element's and next two element's distance from xy to map
        for(auto i = it; i != station_coords.end() && k > 0 ; i++, k-- ){
            int x2 = i->first.x;
            int y2 = i->first.y;
            sorted_distances.insert({(xy.x-x2)*(xy.x-x2)+(xy.y-y2)*(xy.y-y2), i->second});
        }
        k = 2;
        // Add two previous coordinates' of min element distance from xy to map
        if(it != station_coords.begin()){
            auto j = std::prev(it, 1);
            for(; k > 0 ; k-- ){
                int x2 = j->first.x;
                int y2 = j->first.y;
                sorted_distances.insert({(xy.x-x2)*(xy.x-x2)+(xy.y-y2)*(xy.y-y2), j->second});
                if(j != station_coords.begin()){
                    j = std::prev(j,1);
                }
                else{ break; }
            }
        }
        //Add three closest points' ids to the vector
        k = 3;
        for(auto iter = sorted_distances.begin(); iter != sorted_distances.end() && k > 0 ; iter++, k-- ){
            closest_to.push_back(iter->second);
        }
    }
    return closest_to;

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
    if(it->second.upper_id != 0){
        auto it2 = regions.find(it->second.upper_id);
        auto stations_in_region = it2->second.stations_in_region;
        auto iter = stations_in_region.find(id);
        if(iter != stations_in_region.end()){
            stations_in_region.erase(iter);
            it2->second.stations_in_region = stations_in_region;
        }
    }

    // Station has to be deleted from station_coords and station_names maps
    auto iter = station_coords.find(it->second.coord);
    auto iter2 = station_names.find(it->second.name);
    station_coords.erase(iter);
    station_names.erase(iter2);
    stations.erase(it);
    return true;
}

/*!
 * \brief Datastructures::common_parent recursive function to search common parent of two regions
 * \param prev1 pointer to the previous upper region of the first region
 * \param prev2 pointer to the previous upper region of the second region
 * \param curr1 pointer to the current upper region of the first region
 * \param curr2 pointer to the current upper region of the second region
 * \return region id of common parent or NO_REGION if there is no common parent
 */
RegionID Datastructures::common_parent(std::pair<const RegionID, Region>* prev1,
                                       std::pair<const RegionID, Region>* prev2,
                                       std::pair<const RegionID, Region>* curr1,
                                       std::pair<const RegionID, Region>* curr2){
    //Cases where the common region can be found
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
        // Search common parent deeper in the tree
        if(curr1 != nullptr && curr2 != nullptr && curr1->second.upper_region != nullptr
                && curr2->second.upper_region != nullptr){
            common_parent(curr1, curr2, curr1->second.upper_region, curr2->second.upper_region);
        }
        // If curr1 is a nullpointer but curr2 has upper region
        else if(curr1 != nullptr && curr2 != nullptr && curr1->second.upper_region == nullptr
                && curr2->second.upper_region != nullptr){
            common_parent(curr1, curr2, curr1, curr2->second.upper_region);
        }
        // If curr1 has upper region but curr2 is a null pointer
        else if(curr1 != nullptr && curr2 != nullptr && curr1->second.upper_region != nullptr
                && curr2->second.upper_region == nullptr){
            common_parent(curr1, curr2, curr1->second.upper_region, curr2);
        }
        // If there is nowhere to continue in the tree
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
    return common_parent(parent1, parent2,
                         parent1->second.upper_region, parent2->second.upper_region);
}

/*!
 * \brief Datastructures::add_train add new trains to datastructures
 * \param trainid train id
 * \param stationtimes train's route
 * \return true, if train is added succesfully, false if train already exists or
 *  if station in train's route doesn't exist
 */
bool Datastructures::add_train(TrainID trainid, std::vector<std::pair<StationID, Time> > stationtimes)
{
    auto it = all_trains.find(trainid);
    // If train already exists
    if(it != all_trains.end()){
        return false;
    }

    for(auto i = stationtimes.begin(); i != stationtimes.end(); i++){
        // Find station
        auto iter = stations.find(i->first);
        // If station doens't exist
        if(iter == stations.end()){
            return false;
        }

        // Train needs to be added to station's departure and train information
        iter->second.departures.insert({i->second, trainid});
        train.trainroute = stationtimes;
        iter->second.station_trains.insert({trainid, train});

        // Add neighbour to station
        if(i+1 != stationtimes.end()){
            auto neighbour = stations.find((i+1)->first);
            iter->second.neigbours.push_back(neighbour->first);
        }
    }
    all_trains.insert(trainid);
    return true;
}

/*!
 * \brief Datastructures::next_stations_from find next stations from train routes from the
 * given station
 * \param id station id
 * \return vector of the next stations' ids
 */
std::vector<StationID> Datastructures::next_stations_from(StationID id)
{
    auto it = stations.find(id);
    // If station is not found
    if(it == stations.end()){
        return {NO_STATION};
    }

    std::vector<StationID> next_stations;
    auto neighbours = it->second.neigbours; // Station neighbours

    // If station has no neighbours
    if(neighbours.empty()){
        return {};
    }
    next_stations.reserve(neighbours.size());

    for(auto &station : neighbours){
        next_stations.push_back(station);
    }
    return next_stations;
}

/*!
 * \brief Datastructures::train_stations_from find all stations next on the given
 * stations route after given station
 * \param stationid station id
 * \param trainid train id
 * \return vector of train's route stations' ids
 */
std::vector<StationID> Datastructures::train_stations_from(StationID stationid, TrainID trainid)
{
    auto it = stations.find(stationid);
    // If station is not found
    if(it == stations.end()){
        return {NO_STATION};
    }

    auto it2 = it->second.station_trains.find(trainid);
    // If train is not found
    if(it2 == it->second.station_trains.end()){
        return {NO_STATION};
    }

    std::vector<StationID> stations_from;

    // Station needs to be searched from train route
    auto route = it2->second.trainroute;
    for(auto i = route.begin(); i != route.end(); i++){
        if(i->first == stationid && i+1 != route.end()){
            // Add the stations from route
            std::for_each(i+1, route.end(), [&stations_from](const std::pair<StationID, Time> &station) {
                stations_from.push_back(station.first);
            });
            break;
        } 
    }
    // If train doesn't leave the given station
    if(stations_from.empty()){
        return {NO_STATION};
    }
    return stations_from;

}

/*!
 * \brief Datastructures::clear_trains clear all trains from datastructures
 */
void Datastructures::clear_trains()
{
    all_trains.clear();
    // Trains needs to be cleared from stations
    for(auto &elem : stations){
        elem.second.departures.clear();
        elem.second.station_trains.clear();
        elem.second.neigbours.clear();
        // Station certain attributes need to be changed
        elem.second.path_back = nullptr;
        elem.second.colour = white;
        elem.second.distance = 0;
        elem.second.need_refresh = false;
    }
}

int Datastructures::dist(const Coord &c1, const Coord &c2){
    int diffx = floor((c1.x-c2.x)*(c1.x-c2.x));
    int diffy = floor((c1.y-c2.y)*(c1.y-c2.y));
    return (int) sqrt(diffx + diffy);
}

/*!
 * \brief Datastructures::route_any Finds any route between to stations
 * \param fromid start station id
 * \param toid destination station id
 * \return vector of station ids in route and the stations' distance from start station,
 * empy vector if no root is found and {NO_STATION, NO_DISTANCE} if stations don't exist.
 */
std::vector<std::pair<StationID, Distance>> Datastructures::route_any(StationID fromid, StationID toid)
{
    auto fromit = stations.find(fromid); auto toit = stations.find(toid);
    // If one of the stations don't exist
    if(fromit == stations.end() || toit == stations.end()){
        return {{NO_STATION, NO_DISTANCE}};
    }
    std::vector<std::pair<StationID, Distance>> route;
    // If start or end station doesn't have trains
    if(fromit->second.departures.empty() || toit->second.departures.empty()){
        return route;
    }

    std::queue<StationID> Q;
    fromit->second.distance = 0;
    fromit->second.path_back = nullptr;
    fromit->second.colour = gray;
    fromit->second.need_refresh = true;
    Q.push(fromit->first);
    bool end_found = false;

    while (!Q.empty() && !end_found) {
       StationID s = Q.front(); // Take new stationID
       Q.pop();
       auto neighbours_it = stations.find(s);
       if(neighbours_it != stations.end()){
           // Go through all the neighbours
           for(const StationID &u : neighbours_it->second.neigbours){
               auto stationit = stations.find(u); // Find station
               if(stationit != stations.end()){
                   auto st = stationit->second;
                   if(st.need_refresh){
                       st.colour = white;
                       st.distance = 0;
                       st.path_back = nullptr;
                       st.need_refresh = false;
                   }
                   if(st.colour == white){
                       st.colour = gray;
                       std::shared_ptr<Station> pi = std::make_shared<Station>(neighbours_it->second);
                       // Pointer to station before in route
                       st.path_back = pi;
                       Q.push(stationit->first);
                       if(st.path_back != nullptr){
                           st.distance = dist(st.path_back->coord, st.coord) + st.path_back->distance;
                       } else {
                           st.distance = 0;
                       }
                       st.need_refresh = true;
                   }
                   stationit->second = st;
               }
               // If the end stations is found
               if(stationit->first == toid){
                   end_found = true;
                   break;
               }
           }
           neighbours_it->second.colour = black;
       }
    }
    if(!end_found){ return {}; } // No route found

    // End node
    route.push_back({toit->first, toit->second.distance});
    auto s = toit->second.path_back;
    while(true && s != nullptr){
        route.push_back({s->stationid, s->distance});
        if(s->stationid == fromid){
            s->need_refresh = true;
            break;
        }
        s->need_refresh = true;
        s = s->path_back;   
    }
    // Path is found from end to start, so we need to reverse the route
    std::reverse(route.begin(), route.end());
    return route;
}

std::vector<std::pair<StationID, Distance>> Datastructures::route_least_stations(StationID fromid, StationID toid)
{
    auto fromit = stations.find(fromid); auto toit = stations.find(toid);
    // If one of the stations don't exist
    if(fromit == stations.end() || toit == stations.end()){
        return {{NO_STATION, NO_DISTANCE}};
    }
    std::vector<std::pair<StationID, Distance>> route;
    // If start or end station doesn't have trains
    if(fromit->second.departures.empty() || toit->second.departures.empty()){
        return route;
    }

    std::queue<StationID> Q;
    fromit->second.distance = 0;
    fromit->second.path_back = nullptr;
    fromit->second.colour = gray;
    fromit->second.need_refresh = true;
    Q.push(fromit->first);
    bool end_found = false;

    while (!Q.empty() && !end_found) {
       StationID s = Q.front(); // Take new stationID
       Q.pop();
       auto neighbours_it = stations.find(s);
       if(neighbours_it != stations.end()){
           // Go through all the neighbours
           for(const StationID &u : neighbours_it->second.neigbours){
               auto stationit = stations.find(u); // Find station
               if(stationit != stations.end()){
                   auto st = stationit->second;
                   if(st.need_refresh){
                       st.colour = white;
                       st.distance = 0;
                       st.path_back = nullptr;
                       st.need_refresh = false;
                   }
                   if(st.colour == white){
                       st.colour = gray;
                       std::shared_ptr<Station> pi = std::make_shared<Station>(neighbours_it->second);
                       // Pointer to station before in route
                       st.path_back = pi;
                       Q.push(stationit->first);
                       if(st.path_back != nullptr){
                           st.distance = dist(st.path_back->coord, st.coord) + st.path_back->distance;
                       } else {
                           st.distance = 0;
                       }
                       st.need_refresh = true;
                   }
                   stationit->second = st;
               }
               // If the end stations is found
               if(stationit->first == toid){
                   end_found = true;
                   break;
               }
           }
           neighbours_it->second.colour = black;
       }
    }
    if(!end_found){ return {}; } // No route found

    // End node
    route.push_back({toit->first, toit->second.distance});
    auto s = toit->second.path_back;
    while(true && s != nullptr){
        route.push_back({s->stationid, s->distance});
        if(s->stationid == fromid){
            s->need_refresh = true;
            break;
        }
        s->need_refresh = true;
        s = s->path_back;
    }
    // Path is found from end to start, so we need to reverse the route
    std::reverse(route.begin(), route.end());
    return route;
}

std::vector<StationID> Datastructures::route_with_cycle(StationID /*fromid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_with_cycle()");
}

std::vector<std::pair<StationID, Distance>> Datastructures::route_shortest_distance(StationID /*fromid*/, StationID /*toid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_shortest_distance()");
}

std::vector<std::pair<StationID, Time>> Datastructures::route_earliest_arrival(StationID /*fromid*/, StationID /*toid*/, Time /*starttime*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_earliest_arrival()");
}
