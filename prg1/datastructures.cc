// Datastructures.cc


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
    all_stations_ = {};
    all_region_ = {};
    all_station_pointer_ = {};

}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
    // Deleting reserved memory
    for(auto& i : all_stations_)
    {
        delete i.second;
    }

    for(auto& i : all_region_)
    {
        delete i.second;
    }
    all_stations_.clear();
    all_region_.clear();
    all_station_pointer_.clear();

}

unsigned int Datastructures::station_count()
{
    return all_stations_.size();
}

void Datastructures::clear_all()
{
    for(auto& i : all_stations_)
    {
        delete i.second;
    }

    for(auto& i : all_region_)
    {
        delete i.second;
    }

    all_stations_.clear();
    all_region_.clear();
    all_station_pointer_.clear();

}

std::vector<StationID> Datastructures::all_stations()
{

    std::vector<StationID> all_id;
    for(auto& i : all_stations_)
    {
        all_id.push_back(i.second->station_id_);
    }
    return all_id;

}

bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{

    std::map<Coord, Station_*>::iterator it;
    it = std::find_if(all_stations_.begin(), all_stations_.end(),
         [&id](std::pair<Coord, Station_*> i){return (i.second->station_id_ == id);});
    if(it == all_stations_.end() || it->second->station_id_ != id)
    {
        Station_* new_station = new Station_();
        new_station->station_id_ = id;
        new_station->station_name_ = name;
        new_station->station_coord_ = xy;

        all_stations_.insert({xy, new_station});
        all_station_pointer_.push_back(new_station);
        return true;

    }
    return false;
}

Name Datastructures::get_station_name(StationID id)
{
    std::map<Coord, Station_*>::iterator it;
    it = std::find_if(all_stations_.begin(), all_stations_.end(),
         [&id](std::pair<Coord, Station_*> i){return (i.second->station_id_ == id);});
    if(it != all_stations_.end())
    {
        return it->second->station_name_;
    }

    return NO_NAME;
}

Coord Datastructures::get_station_coordinates(StationID id)
{
    std::map<Coord, Station_*>::iterator it;
    it = std::find_if(all_stations_.begin(), all_stations_.end(),
         [&id](std::pair<Coord, Station_*> i){return (i.second->station_id_ == id);});

    if(it != all_stations_.end())
    {
        return it->first;
    }
    return NO_COORD;
}

std::vector<StationID> Datastructures::stations_alphabetically()
{
    std::vector<StationID> sorted_ids;
    sorted_ids.reserve(all_station_pointer_.size());

    std::sort(all_station_pointer_.begin(), all_station_pointer_.end(),
              [](Station_* &first, Station_* &second){
        return first->station_name_ < second->station_name_;
    });
    std::transform(all_station_pointer_.begin(), all_station_pointer_.end(),
                   std::back_inserter(sorted_ids), [](Station_* &i){
        return i->station_id_;
    });

    return sorted_ids;
}

std::vector<StationID> Datastructures::stations_distance_increasing()
{
    std::vector<StationID>  sort_name_ids;
    sort_name_ids.reserve(all_station_pointer_.size());

    for(const auto& i : all_stations_)
    {
        sort_name_ids.push_back(i.second->station_id_);
    }
    return sort_name_ids;
}

StationID Datastructures::find_station_with_coord(Coord xy)
{
    std::map<Coord, Station_*>::iterator it = all_stations_.find(xy);

    if(it != all_stations_.end())
    {
        return it->second->station_id_;
    }
    return NO_STATION;
}

bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{
    std::map<Coord, Station_*>::iterator it;
    it = std::find_if(all_stations_.begin(), all_stations_.end(),
              [&id](std::pair<Coord, Station_*> i){return i.second->station_id_ == id;});

    if(it == all_stations_.end())
    {
        return false;
    }
    auto nodeHandler = all_stations_.extract(it->first);
    nodeHandler.key() = newcoord;
    all_stations_.insert(std::move(nodeHandler));
    return true;
}

bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{

    std::pair departure_pair = std::pair(time, trainid);

    std::map<std::pair<Time, TrainID>, StationID>::iterator it =
             std::find_if(departures_.begin(), departures_.end(),
             [departure_pair, stationid](std::pair<std::pair<Time, TrainID>, TrainID> i)
             {return i.first == departure_pair && i.second == stationid;});

    if(it==departures_.end())
    {
        departures_.insert({departure_pair, stationid});
        return true;
    }
    return false;
}

bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{

    std::pair departure_pair = std::make_pair(time, trainid);

    std::map<std::pair<Time, TrainID>, StationID>::iterator it =
             std::find_if(departures_.begin(), departures_.end(),
             [departure_pair, stationid](std::pair<std::pair<Time, TrainID>, TrainID> i)
             {return i.first == departure_pair && i.second == stationid;});

    if(it != departures_.end())
    {
        departures_.erase(it);
        return true;
    }
    return false;

}

std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{
    std::vector<std::pair<Time, TrainID>> departures_sort;


    for(auto& i : departures_)
    {

        if(i.second == stationid && i.first.first >= time)
        {
            departures_sort.push_back(i.first);
        }
    }
    if(std::find_if(all_station_pointer_.begin(), all_station_pointer_.end(),
      [&stationid](Station_* i){return i->station_id_ == stationid;}) == all_station_pointer_.end())
    {
        departures_sort.push_back({NO_TIME, NO_TRAIN});
        return departures_sort;
    }

    sort(departures_sort.begin(), departures_sort.end(), [](const std::pair<Time, TrainID> &a,
        const std::pair<Time, TrainID> &b){
        if(a.first == b.first){return a.second < b.second;}
        return a.first < b.first; });

    return departures_sort;
}

bool Datastructures::add_region(RegionID id, const Name &name, std::vector<Coord> coords)
{


    if(all_region_.find(id) == all_region_.end())
    {
        Region_* new_region = new Region_();
        new_region->region_id_ = id;
        new_region->region_name_ = name;
        new_region->region_coord_ = coords;

        std::pair<RegionID, Region_*> r(id, new_region);
        all_region_.insert(r);
        return true;
    }
    return false;



}

std::vector<RegionID> Datastructures::all_regions()
{
    std::vector<RegionID> all_regions_id;

    for(auto& i : all_region_)
    {
        all_regions_id.push_back(i.first);
    }
    return all_regions_id;
}

Name Datastructures::get_region_name(RegionID id)
{
    std::map<RegionID, Region_*>::iterator it = all_region_.find(id);
    if(it == all_region_.end())
    {
        return NO_NAME;
    }
    return it->second->region_name_;
}

std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{
    std::map<RegionID, Region_*>::iterator it = all_region_.find(id);
    if(it == all_region_.end())
    {
        std::vector<Coord> no_find;
        no_find.push_back(NO_COORD);
        return no_find;
    }
    return it->second->region_coord_;
}

bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{

    std::map<RegionID, Region_*>::iterator it = all_region_.find(id);
    std::map<RegionID, Region_*>::iterator it_parent = all_region_.find(parentid);

    if(it != all_region_.end() && it_parent != all_region_.end())
    {
        if(it->second->next_region_ == nullptr){
            it->second->next_region_ = it_parent->second;
            return  true;
        }
    }
    return false;

}

bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    std::map<Coord, Station_*>::iterator it;
    it = std::find_if(all_stations_.begin(), all_stations_.end(),
         [&id](std::pair<Coord, Station_*> i){return i.second->station_id_ == id;});
    std::map<RegionID, Region_*>::iterator it_parent = all_region_.find(parentid);

    if(it != all_stations_.end() && it_parent != all_region_.end() && it->second->region_ == nullptr)
    {
        it->second->region_ = it_parent->second;
        return true;
    }
    return false;


}

std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    std::vector<RegionID> all_station_region;

    std::map<Coord, Station_*>::iterator it;
    it = std::find_if(all_stations_.begin(), all_stations_.end(),
         [&id](std::pair<Coord, Station_*> i){return i.second->station_id_ == id;});

    if(it != all_stations_.end())
    {
        Region_* next_pointer = it->second->region_;
        while (next_pointer != nullptr) {
            all_station_region.push_back(next_pointer->region_id_);
            next_pointer = next_pointer->next_region_;
        }
        return all_station_region;
    }

    all_station_region.push_back(NO_REGION);
    return all_station_region;

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



