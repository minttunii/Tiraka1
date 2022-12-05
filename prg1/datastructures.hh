#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <set>
#include <list>

// Types for IDs
using StationID = std::string;
using TrainID = std::string;
using RegionID = unsigned long long int;
using Name = std::string;
using Time = unsigned short int;

// Return values for cases where required thing was not found
StationID const NO_STATION = "---";
TrainID const NO_TRAIN = "---";
RegionID const NO_REGION = -1;
Name const NO_NAME = "!NO_NAME!";
Time const NO_TIME = 9999;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();


// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: std::unordered_map::size is O(1)
    unsigned int station_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: ::clear methods for different datatypes in clear_all are O(n)
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Iteration of std::list is O(n)
    std::vector<StationID> all_stations();

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::insert is O(n)
    // and std::set::insert is O(log n)
    bool add_station(StationID id, Name const& name, Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find is O(n)
    Name get_station_name(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find is O(n)
    Coord get_station_coordinates(StationID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n log n)
    // Short rationale for estimate: Iterating through std::set is O(n)
    // It is also possible that the method deletes all elements, which is O(nlogn)
    std::vector<StationID> stations_alphabetically();

    // Estimate of performance: O(n log n)
    // Short rationale for estimate: Iterating through std::set is O(n).
    // It is also possible that the method deletes all elements, which is O(nlogn)
    std::vector<StationID> stations_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find is O(n)
    StationID find_station_with_coord(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::erase and insert
    // are O(n) and std::set::erase and insert are O(log n)
    bool change_station_coord(StationID id, Coord newcoord);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find is O(n)
    // and std::set is O(log n) in this function
    bool add_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find is O(n)
    // and std::set is O(log n) in this function
    bool remove_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::vector::push_back is O(n), but
    // filling a vector by calling std::vector::push_back n times is O(n)
    // because the vector reserves 2n elements in memory (1+2+4+8+16+32+...≈2n).
    // This is repeated n times.
    std::vector<std::pair<Time, TrainID>> station_departures_after(StationID stationid, Time time);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::insert is O(n)
    bool add_region(RegionID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Iteration of std::unordered_map is O(n)
    std::vector<RegionID> all_regions();

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find is O(n)
    Name get_region_name(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find is O(n)
    std::vector<Coord> get_region_coords(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find and insert are O(n)
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::find is O(n)
    bool add_station_to_region(StationID id, RegionID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Iterating through all parent regions is O(n)
    std::vector<RegionID> station_in_regions(StationID id);

    // Non-compulsory operations

    // Estimate of performance: O(n)
    // Short rationale for estimate: Iterating through all subregions is O(n)
    std::vector<RegionID> all_subregions_of_region(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Iteration of all stations is O(n)
    std::vector<StationID> stations_closest_to(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::erase
    // and std::vector::push_back are O(n)
    bool remove_station(StationID id);

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: std::unordered_set::insert is O(n)
    // and this is repeated n times
    RegionID common_parent_of_regions(RegionID id1, RegionID id2);

private:
    // Add stuff needed for your class implementation here
    struct Region
    {
        RegionID id;
        Name name;
        std::vector<Coord> coord;
        Region* parentRegion;
        std::vector<Region*> subRegions;

        Region(RegionID id, Name name, std::vector<Coord> coord)
            : id(id)
            , name(name)
            , coord(coord)
            , parentRegion(nullptr)
            , subRegions() {}
    };
    std::unordered_map<RegionID, Region> regions_from_id;

    struct Station
    {
        StationID id;
        Name name;
        Coord coord;
        std::set<std::pair<Time, TrainID>> train_departure_times;
        Region* region;

        Station(StationID id, Name name, Coord coord)
            : id(id)
            , name(name)
            , coord(coord)
            , train_departure_times()
            , region(nullptr) {}
    };
    using StationIt = std::list<Station>::iterator;
    std::list<Station> stations_list;
    std::unordered_map<StationID, StationIt> stations_from_id;
    std::unordered_map<Coord, StationIt, CoordHash> stations_from_coord;

    struct AlphabeticOrder
    {
        bool operator()(const StationIt& itA, const StationIt& itB) const
        {
            return itA->name < itB->name;
        }
    };
    struct DistanceOrder
    {
        bool operator()(const StationIt& itA, const StationIt& itB) const
        {
            auto [xA, yA] = itA->coord;
            auto [xB, yB] = itB->coord;

            // Instead of comparing sqrt(x^2+y^2), compare x^2+y^2 because it is
            // easier to compare integer values
            int sqSumA = xA*xA + yA*yA;
            int sqSumB = xB*xB + yB*yB;
            if (sqSumA != sqSumB) {
                return sqSumA < sqSumB;
            }

            // Compare y coordinates
            return yA < yB;
        }
    };
    // Store stations, so methods stations_alphabetically and
    // stations_distance_increasing can run faster
    std::set<StationIt, AlphabeticOrder> alphabetic_stations;
    std::set<StationIt, DistanceOrder> distance_stations;
    std::vector<StationID> alphabetic_stations_vector;
    std::vector<StationID> distance_stations_vector;
    bool alphabetic_stations_needs_update = true;
    bool distance_stations_needs_update = true;

    // Handle removed stations in alphabetic_stations and distance_stations
    std::vector<StationIt> removed_stations;
    void delete_stations_if_needed()
    {
        for (auto &&station : removed_stations)
        {
            alphabetic_stations.erase(station);
            distance_stations.erase(station);
            stations_list.erase(station);
        }
        removed_stations.clear();
    }

    // Struct for comparing distances in stations_closest_to
    struct StationComp {
        int dist;
        int y;
        const StationID& id;

        StationComp(const StationIt& station, const Coord& xy)
        : id(station->id)
        {
            int x = xy.x - station->coord.x;
            y = xy.y - station->coord.y;
            dist = x*x + y*y;
        }

        bool operator<(const StationComp& cmp) const
        {
            if (dist != cmp.dist) return dist < cmp.dist;
            if (y != cmp.y) return y < cmp.y;
            return id < cmp.id;
        }
    };

    // Recursive function for Datastructures::all_subregions_of_region
    void iterateTree(std::vector<RegionID>& subregions, Region& region)
    {
        for (auto next_region : region.subRegions)
        {
            subregions.push_back(next_region->id);
            iterateTree(subregions, *next_region);
        }
    }

    // Convert set to vector
    template<typename T>
    std::vector<StationID> setToVector(T set)
    {
        std::vector<StationID> v;
        v.reserve(set.size());
        for (auto &&s : set)
        {
            v.push_back(s->id);
        }
        return v;
    }
};

#endif // DATASTRUCTURES_HH


























// Datastructures.hh
//
// Student name: Minttu Niiranen
// Student email: minttu.niiranen@tuni.fi
// Student number: H291834

/*

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <map>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>

// Types for IDs
using StationID = std::string;
using TrainID = std::string;
using RegionID = unsigned long long int;
using Name = std::string;
using Time = unsigned short int;

// Return values for cases where required thing was not found
StationID const NO_STATION = "---";
TrainID const NO_TRAIN = "---";
RegionID const NO_REGION = -1;
Name const NO_NAME = "!NO_NAME!";
Time const NO_TIME = 9999;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();


// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if(c1.x*c1.x + c1.y*c1.y == c2.x*c2.x + c2.y*c2.y){
         if(c1.y < c2.y){ return true;}
         else{ return false;}
    }
    else{
        if(c1.x*c1.x + c1.y*c1.y < c2.x*c2.x + c2.y*c2.y){ return true;}
        else{ return false;}
    }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: size function is constant
    unsigned int station_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Clear function is linear on size
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Iterating map is linear in worst case,
    // vector pushback is constant and reserve is linear in worst case
    std::vector<StationID> all_stations();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find function and inserting to unsorted map
    // is constant on average but linear in worst case.
    // Inserting to map is logarithmic.
    bool add_station(StationID id, Name const& name, Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find function is constant on average and in
    // worst case linear in container size
    Name get_station_name(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find function is constant on average and in
    // worst case linear in container size
    Coord get_station_coordinates(StationID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: For loop for creating vector is linear.
    // Reserve is linear at most if reallocating happens.
    std::vector<StationID> stations_alphabetically();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Creating vector is linear.
    // Reserve is linear at most if reallocating happens.
    std::vector<StationID> stations_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find is linear in worst case
    StationID find_station_with_coord(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find is linear in worst case but average on constant.
    // Erase is constant and insert to map is logarithmic.
    bool change_station_coord(StationID id, Coord newcoord);

    // Estimate of performance: O(n))
    // Short rationale for estimate: For loop is linear in worst case.
    // Insert and find are logarithmic.
    bool add_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find is logaritmhmic and for loop is linear
    // in worst case. Erase is constant.
    bool remove_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find is logarithmic and creating vector in for loop is linear
    std::vector<std::pair<Time, TrainID>> station_departures_after(StationID stationid, Time time);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find function and insert are linear in worst case but
    // insert is constant on average.
    bool add_region(RegionID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(n)
    // Short rationale for estimate: For loop is linear and vector pushback and
    // empty function is constant. Reserve is linear in worst case.
    std::vector<RegionID> all_regions();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find function is linear in worst case
    Name get_region_name(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find function is linear in worst case
    std::vector<Coord> get_region_coords(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find function is linear at worst case but constant on average.
    // Vector pushback is constant but linear in worst case.
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find function is linear at worst case but constant on average.
    bool add_station_to_region(StationID id, RegionID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find is linear in worst case and constant on average. While loop
    // can be linear. Vector pushback is constant but linear in worst case..
    std::vector<RegionID> station_in_regions(StationID id);

    // Non-compulsory operations

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Subregions are found recursively. For loop in the function
    // is linear and it is called as far when there are subregions.
    std::vector<RegionID> all_subregions_of_region(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Finding min element and prev is linear.
    // Inserting to multimap is locarithmic but it is done at most five times.
    // Vector pushback is constant on average.
    std::vector<StationID> stations_closest_to(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find is linear in worst case but constant on average
    // for unsorted map but logartihmic for map. Erase is constant on average but linear
    // in worst case
    bool remove_station(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find is constant on average and linear in worst case.
    // Problem is solved recursively.
    RegionID common_parent_of_regions(RegionID id1, RegionID id2);

private:
    struct Station {
        Name name;
        Coord coord;
        std::multimap<Time, TrainID> trains;
        RegionID upper_id = 0;
    } station;

    struct Region {
        Name name;
        std::vector<Coord> coords;
        std::unordered_set<std::pair<const RegionID, Region>*> sub_regions;
        std::pair<const RegionID, Region>* upper_region = nullptr;
        std::unordered_set<StationID> stations_in_region;
    } region;

    std::unordered_map<StationID, Station> stations;
    std::unordered_map<RegionID, Region> regions;
    std::map<Coord, StationID> station_coords;
    std::map<Name, StationID> station_names;
    std::vector<RegionID> subregions(std::unordered_set<std::pair<const RegionID, Region>*>,
                                     std::vector<RegionID>&);
    RegionID common_parent(std::pair<const RegionID, Region>*,
                           std::pair<const RegionID, Region>*,
                           std::pair<const RegionID, Region>*,
                           std::pair<const RegionID, Region>*);
};

#endif // DATASTRUCTURES_HH
*/
