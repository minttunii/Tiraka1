// Datastructures.cc
//
// Student name: Minttu Niiranen
// Student email: minttu.niiranen@tuni.fi
// Student number: H291834

#include "datastructures.hh"

#include <random>

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
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

unsigned int Datastructures::station_count()
{
    return stations.size();
}

void Datastructures::clear_all()
{
    stations.clear();
    regions.clear();
    station_names.clear();
    station_coords.clear();
}

std::vector<StationID> Datastructures::all_stations()
{
    std::vector<StationID> all_stations;
    all_stations.reserve(stations.size());

    std::for_each(stations.begin(),stations.end(),
                     [&all_stations](const std::pair<StationID,Station> &s)
                     { all_stations.push_back(s.first); });
    return all_stations;
}

bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{
    // If station already exists, return false
    if(stations.find(id) != stations.end()){
        return false;
    }
    station.name = name; station.coord = xy;
    stations.insert({id, station});
    station_coords.insert({xy, id});
    station_names.insert({name, id});
    return true;
}

Name Datastructures::get_station_name(StationID id)
{
    auto it = stations.find(id);
    // If station is not found
    if(it == stations.end()){
        return NO_NAME;
    }
    return it->second.name;
}

Coord Datastructures::get_station_coordinates(StationID id)
{
    auto it = stations.find(id);
    // If station is not found
    if(it == stations.end()){
        return NO_COORD;
    }
    return it->second.coord;
}

std::vector<StationID> Datastructures::stations_alphabetically()
{
    std::vector<StationID> stations_alph;
    stations_alph.reserve(stations.size());

    std::for_each(station_names.begin(),station_names.end(),
                         [&stations_alph](const std::pair<Name, StationID> &s)
                         { stations_alph.push_back(s.second); });
    return stations_alph;
}

std::vector<StationID> Datastructures::stations_distance_increasing()
{
    std::vector<StationID> station_dist;
    station_dist.reserve(stations.size());

    std::for_each(station_coords.begin(),station_coords.end(),
                         [&station_dist](const std::pair<Coord, StationID> &s)
                         { station_dist.push_back(s.second);});

    return station_dist;
}

StationID Datastructures::find_station_with_coord(Coord xy)
{
    auto it = station_coords.find(xy);
    if(it == station_coords.end()){
        return NO_STATION;
    }
    return it->second;
}

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

bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{
    auto it = stations.find(stationid);
    if(it == stations.end()){
        return false;
    }
    auto trains = it->second.trains;
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
    it->second.trains = trains;
    return true;
}

bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{
    auto it = stations.find(stationid);
    if(it == stations.end()){
        return false;
    }

    auto trains = it->second.trains;
    auto it2 = trains.find(time);
    // Check if departure to remove exist
    if(it2 != trains.end()){
        for(auto i = it2; i != trains.end(); i++){
            if(i->first == time && i->second == trainid){
                trains.erase(i);
                it->second.trains = trains;
                return true;
            }
        }
    }
    return false;
}

std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{
    std::vector<std::pair<Time, TrainID>> departures;

    auto it = stations.find(stationid);
    if(it == stations.end()){
        return {{NO_TIME, NO_TRAIN}};
    }

    auto trains = it->second.trains;
    for(auto &elem : trains){
        if(elem.first >= time){
            departures.push_back({elem.first, elem.second});
        }
    }
    return departures;
}

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

Name Datastructures::get_region_name(RegionID id)
{
    auto it = regions.find(id);
    if(it == regions.end()){
        return NO_NAME;
    }
    return it->second.name;
}

std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{
    auto it = regions.find(id);
    if(it == regions.end()){
        return {NO_COORD};
    }
    return it->second.coords;
}

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
    // If station or region doesn't exist or station already belongs to a region
    if(it == stations.end() || it2 == regions.end() || it->second.upper_id != 0){
        return false;
    }
    it2->second.stations_in_region.insert(id);
    it->second.upper_id = parentid;
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

bool Datastructures::add_train(TrainID trainid, std::vector<std::pair<StationID, Time> > stationtimes)
{
    auto it = trains.find(trainid);
    // If train already exists
    if(it != trains.end()){
        return false;
    }

    for(auto &pair : stationtimes){
        auto iter = stations.find(pair.first);
        // If station doens't exist
        if(iter == stations.end()){
            return false;
        }
        // Train needs to be added to station's train information
        iter->second.trains.insert({pair.second, trainid});
    }

    // Add train to the trains map
    train.trainroute = stationtimes;
    trains.insert({trainid, train});
    return true;
}

std::vector<StationID> Datastructures::next_stations_from(StationID id)
{
    auto it = stations.find(id);
    // If station is not found
    if(it == stations.end()){
        return {NO_STATION};
    }

    auto station_trains = it->second.trains;
    // If no trains are leaving the station
    if(station_trains.empty()){
        return {};
    }

    std::vector<StationID> next_stations;

    // Find the next stations
    for(auto &pair : station_trains){
        TrainID train = pair.second;
        auto iter = trains.find(train);

        if(iter != trains.end()){
            // Find and add the next station on route
            auto route = iter->second.trainroute;
            for(unsigned long long i = 1; i < route.size()+1; i++){
                if(route.at(i-1).first == id && i < route.size()){
                    // The next station was found
                    next_stations.push_back(route.at(i).first);
                    break;
                }
            }
        }
    }
    return next_stations;
}

std::vector<StationID> Datastructures::train_stations_from(StationID stationid, TrainID trainid)
{
    auto it = stations.find(stationid);
    auto it2 = trains.find(trainid);
    // If station or train is not found
    if(it == stations.end() || it2 == trains.end()){
        return {NO_STATION};
    }

    std::vector<StationID> stations_from;

    // Station needs to be searched from train route
    auto route = it2->second.trainroute;
    for(auto i = route.begin(); i != route.end(); i++){
        if(i->first == stationid){
            // Add the stations from route
            std::for_each(i+1, route.end(), [&stations_from](std::pair<StationID, Time> &station) {
                stations_from.push_back(station.first);
            });
            break;
        }
    }

    return stations_from;

}

void Datastructures::clear_trains()
{
    trains.clear();
}

std::vector<std::pair<StationID, Distance>> Datastructures::route_any(StationID /*fromid*/, StationID /*toid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_any()");
}

std::vector<std::pair<StationID, Distance>> Datastructures::route_least_stations(StationID /*fromid*/, StationID /*toid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_least_stations()");
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
