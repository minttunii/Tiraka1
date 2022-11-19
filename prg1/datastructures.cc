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
    if(stations.find(id) != stations.end()){
        return false;
    }
    station.name = name; station.coord = xy;
    std::shared_ptr<Station> station_ptr = std::make_shared<Station>(station);
    stations.insert({id, station_ptr});
    stations_to_order.push_back({id, station_ptr});
    return true;
}

Name Datastructures::get_station_name(StationID id)
{
    auto it = stations.find(id);
    // If station is not found
    if(it == stations.end()){
        return NO_NAME;
    }
    return it->second->name;
}

Coord Datastructures::get_station_coordinates(StationID id)
{
    auto it = stations.find(id);
    // If station is not found
    if(it == stations.end()){
        return NO_COORD;
    }
    return it->second->coord;
}

std::vector<StationID> Datastructures::stations_alphabetically()
{
    std::vector<StationID> stations_alph;
    stations_alph.reserve(stations.size());

    sort(stations_to_order.begin(), stations_to_order.end(), [](const std::pair<StationID, std::shared_ptr<Station>> &a, const std::pair<StationID, std::shared_ptr<Station>> &b)-> bool{
            return a.second->name < b.second->name; });

    for(auto const &elem : stations_to_order){
        stations_alph.push_back(elem.first);
    }
    return stations_alph;
}

std::vector<StationID> Datastructures::stations_distance_increasing()
{
    std::vector<StationID> stations_dist;
    stations_dist.reserve(stations.size());

    sort(stations_to_order.begin(), stations_to_order.end(), [](const std::pair<StationID, std::shared_ptr<Station>> &a, const std::pair<StationID, std::shared_ptr<Station>> &b) -> bool{
        int xa = a.second->coord.x; int ya = a.second->coord.y; int xb = b.second->coord.x; int yb = b.second->coord.y;
        if(xa*xa+ya*ya == xb*xb+yb*yb){ return ya < yb;}
        return xa*xa+ya*ya < xb*xb+yb*yb;
    });

    for(auto const &elem : stations_to_order){
        stations_dist.push_back(elem.first);
    }
    return stations_dist;
}

StationID Datastructures::find_station_with_coord(Coord xy)
{
    for(auto const& elem : stations){
        if(elem.second->coord == xy){
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
    it->second->coord = newcoord;
    return true;
}

bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{
    auto it = stations.find(stationid);
    if(it == stations.end()){
        return false;
    }
    auto trains = it->second->trains;
    auto it2 = trains.find(time);

    // Tarkistetaan löytyykö jo vastaavaa junavuoroa, sillä samoja kellonaikoja voi olla useita
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

bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{
    auto it = stations.find(stationid);
    if(it == stations.end()){
        return false;
    }

    auto trains = it->second->trains;
    auto it2 = trains.find(time);
    // Tarkistetaan löytyykö poistettavaa junavuoroa
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
    // Jos alueita ei olla vielä lisätty
    if(regions.empty()){
        return all_regions;
    }

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

bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    auto it = regions.find(id); // Alue, joka lisätään alialueeksi
    auto it2 = regions.find(parentid); // Ylempi alue

    //Jos alueita ei ole olemassa tai alialue kuuluu jo johonkin alueeseen
    if(it == regions.end() || it2 == regions.end() || it->second.upper_region != nullptr){
        return false;
    }
    //Luodaan osoitin ylempään alueeseen ja lisätään alialueeseen
    std::pair<const RegionID, Region>* parent = &(*it2);
    it->second.upper_region = parent;
    // Luodaan osoitin alialuesseen ja lisätään se ylempään alueeseen
    std::pair<const RegionID, Region>* sub = &(*it);
    it2->second.sub_regions.insert(sub);
    return true;
}

bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    auto it = stations.find(id);
    auto it2 = regions.find(parentid);
    // Jos asemaa tai aluetta ei ole tai asema kuuluu jo alueeseen
    if(it == stations.end() || it2 == regions.end() || it->second->upper_id != 0){
        return false;
    }
    it2->second.stations_in_region.insert(id);
    //it2->second.stations_in_region.push_back(id);
    it->second->upper_id = parentid;
    return true;
}

std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    auto it = stations.find(id);
    // Jos asemaa ei löydy
    if(it == stations.end()){
        return {NO_REGION};
    }
    // Jos asema ei kuulu alueeseen
    else if(it->second->upper_id == 0){
        return {};
    }

    std::vector<RegionID> station_regions;

    // Etsitään alue johon asema kuuluu
    auto it2 = regions.find(it->second->upper_id);
    station_regions.push_back(it2->first);

    // Etsitään ylemmät alueet osoittimien avulla
    auto iter = it2->second.upper_region;

    while(iter != nullptr){
        station_regions.push_back(iter->first);
        iter = iter->second.upper_region;
    }
    delete iter;
    return station_regions;
}

std::vector<RegionID> Datastructures::subregions(std::unordered_set<std::pair<const RegionID, Region>*> sub_regions,
                                                 std::vector<RegionID>& sub_IDs){
    if(sub_regions.empty()){
        return sub_IDs;
    }
    // Lisätään alialueiden id vektoriin ja etsitään rekursiivisesti alialueiden alialueita
    for(auto item = sub_regions.begin(); item != sub_regions.end(); item++){
        sub_IDs.push_back((*item)->first);
        subregions((*item)->second.sub_regions, sub_IDs);
    }
    return sub_IDs;
}

std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID id)
{
    auto it = regions.find(id);
    // Jos aluetta ei löydy
    if(it == regions.end()){
        return {NO_REGION};
    }
    // Jos alueella ei ole alialueita
    else if(it->second.sub_regions.empty()){
        return {};
    }

    auto sub_region = it->second.sub_regions;
    std::vector<RegionID> sub_IDs;
    return subregions(sub_region, sub_IDs);
}

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

bool Datastructures::remove_station(StationID id)
{
    auto it = stations.find(id);
    if(it == stations.end()){
        return false;
    }

    //Asema täytyy poistaa aseman alueen tiedoista
    if(it->second->upper_id != 0){
        auto it2 = regions.find(it->second->upper_id);
        auto stations_in_region = it2->second.stations_in_region;
        auto iter = stations_in_region.find(id);
        if(iter != stations_in_region.end()){
            stations_in_region.erase(iter);
            it2->second.stations_in_region = stations_in_region;
        }
    }
    //Aseman poistaminen stations_to_order vektorista
    for(auto i = stations_to_order.begin(); i != stations_to_order.end(); i++){
        if(i->first == id){
            stations_to_order.erase(i);
            break;
        }
    }
    stations.erase(it);
    return true;
}

RegionID Datastructures::common_parent_of_regions(RegionID id1, RegionID id2)
{
    auto it = regions.find(id1);
    auto it2 = regions.find(id2);
    // Jos alueita ei löydy
    if(it == regions.end() || it2 == regions.end()){
        return NO_REGION;
    }

    return NO_REGION;
}
