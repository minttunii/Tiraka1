// Datastructures.cc
//
// Student name: Minttu Niiranen
// Student email: minttu.niiranen@tuni.fi
// Student number: 0408538846

#include "datastructures.hh"

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


unsigned int Datastructures::station_count()
{
    return stations.size();
}

void Datastructures::clear_all()
{
    stations.clear();
    regions.clear();
    stations_to_order.clear();
}

std::vector<StationID> Datastructures::all_stations()
{
    std::vector<StationID> all_stations;

    for(auto const& elem : stations){
        all_stations.push_back(elem.first);
    }
    return all_stations;
}

bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{
    // If station already exists, return false
    if(stations.count(id) == 1){
        return false;
    }
    station.name = name; station.coord = xy;
    stations.insert({id, station});
    stations_to_order.push_back({id, station});
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

    sort(stations_to_order.begin(), stations_to_order.end(), [](const std::pair<StationID, Station> &a, const std::pair<StationID, Station> &b)-> bool{
        return a.second.name < b.second.name;
    });

    for(auto &i : stations_to_order){
        stations_alph.push_back(i.first);
    }
    return stations_alph;
}

std::vector<StationID> Datastructures::stations_distance_increasing()
{
    std::vector<StationID> stations_dist;
    sort(stations_to_order.begin(), stations_to_order.end(), [](const std::pair<StationID, Station> &a, const std::pair<StationID, Station> &b) -> bool{
        int xa = a.second.coord.x; int ya = a.second.coord.y; int xb = b.second.coord.x; int yb = b.second.coord.y;
        if(sqrt(xa*xa+ya*ya) == sqrt(xb*xb+yb*yb)){ return ya < yb;}
        return sqrt(xa*xa+ya*ya) < sqrt(xb*xb+yb*yb);
    });

    for(auto &i : stations_to_order){
        stations_dist.push_back(i.first);
    }
    return stations_dist;
}

StationID Datastructures::find_station_with_coord(Coord xy)
{
    for(auto const& elem : stations){
        if(elem.second.coord == xy){
            return elem.first;
        }
    }
    return NO_STATION;
}

bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{
    auto it = stations.find(id);
    if(it == stations.end()){
        return false;
    }
    it->second.coord = newcoord;
    // Muutos täytyy päivittää myös toiseen tietorakenteeseen
    for(auto &elem : stations_to_order){
        if(elem.first == id){
            elem.second.coord = newcoord;
            return true;
        }
    }
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
    // Tarkistetaan löytyykö jo vastaavaa junavuoroa
    if(it2 != trains.end()){
        for(auto i = it2; i != trains.end(); i++){
            if(i->first == time && i->second == trainid){
                return false;
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
    // Tarkistetaan löytyykö poistettavaa junavuoroa
    if(it2 != trains.end()){
        for(auto i = it2; i != trains.end(); i++){
            if(i->first == time && i->second == trainid){
                trains.erase(i);
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
    for(auto &elem : regions){
        all_regions.push_back(elem.first);
    }
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

bool Datastructures::add_subregion_to_region(RegionID /*id*/, RegionID /*parentid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("add_subregion_to_region()");
}

bool Datastructures::add_station_to_region(StationID /*id*/, RegionID /*parentid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("add_station_to_region()");
}

std::vector<RegionID> Datastructures::station_in_regions(StationID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("station_in_regions()");
}

std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("all_subregions_of_region()");
}

std::vector<StationID> Datastructures::stations_closest_to(Coord /*xy*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("stations_closest_to()");
}

bool Datastructures::remove_station(StationID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("remove_station()");
}

RegionID Datastructures::common_parent_of_regions(RegionID /*id1*/, RegionID /*id2*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("common_parent_of_regions()");
}
