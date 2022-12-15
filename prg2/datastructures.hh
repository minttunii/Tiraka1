// Datastructures.hh
//
// Student name: Minttu Niiranen
// Student email: minttu.niiranen@tuni.fi
// Student number: H291834

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <cmath>
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
#include <set>

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

    //
    // Old assignment 1 operations
    //

    // Estimate of performance:
    // Short rationale for estimate:
    unsigned int station_count();

    // Estimate of performance:
    // Short rationale for estimate:
    void clear_all();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StationID> all_stations();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_station(StationID id, Name const& name, Coord xy);

    // Estimate of performance:
    // Short rationale for estimate:
    Name get_station_name(StationID id);

    // Estimate of performance:
    // Short rationale for estimate:
    Coord get_station_coordinates(StationID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StationID> stations_alphabetically();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StationID> stations_distance_increasing();

    // Estimate of performance:
    // Short rationale for estimate:
    StationID find_station_with_coord(Coord xy);

    // Estimate of performance:
    // Short rationale for estimate:
    bool change_station_coord(StationID id, Coord newcoord);

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<Time, TrainID>> station_departures_after(StationID stationid, Time time);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_region(RegionID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<RegionID> all_regions();

    // Estimate of performance:
    // Short rationale for estimate:
    Name get_region_name(RegionID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<Coord> get_region_coords(RegionID id);

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_station_to_region(StationID id, RegionID parentid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<RegionID> station_in_regions(StationID id);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<RegionID> all_subregions_of_region(RegionID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StationID> stations_closest_to(Coord xy);

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_station(StationID id);

    // Estimate of performance:
    // Short rationale for estimate:
    RegionID common_parent_of_regions(RegionID id1, RegionID id2);

    //
    // New assignment 2 operations
    //

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: For loop that iterates stationtimes is linear
    // and inserting train to stations in is log(n). Inserting to trains map is constant on average.
    bool add_train(TrainID trainid, std::vector<std::pair<StationID, Time>> stationtimes);

    // Estimate of performance: O(n)
    // Short rationale for estimate: For loop is linear and vector pushback is constant on average.
    std::vector<StationID> next_stations_from(StationID id);

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: for loop is linear and for each is linear in worst case
    std::vector<StationID> train_stations_from(StationID stationid, TrainID trainid);

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: Clear function and for loop are linear.
    void clear_trains();

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: BFS algorithm is O(n+k), where n is number of
    // nodes and k is numer of connections.
    std::vector<std::pair<StationID, Distance>> route_any(StationID fromid, StationID toid);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<StationID, Distance>> route_least_stations(StationID fromid, StationID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StationID> route_with_cycle(StationID fromid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<StationID, Distance>> route_shortest_distance(StationID fromid, StationID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<StationID, Time>> route_earliest_arrival(StationID fromid, StationID toid, Time starttime);

private:
    struct SetComp {
        bool operator() (const std::pair<StationID, Time>& a,
                        const std::pair<StationID,Time>& b) const
        {
            return a.second < b.second;
        }
    };

    enum Colour {white, gray, black};

    struct Train {
        std::vector<std::pair<StationID, Time>> trainroute;
        //std::set<std::pair<StationID, Time>, SetComp> trainroute;

    } train;

    struct Station {
        StationID stationid;
        Name name;
        Coord coord;
        std::multimap<Time, TrainID> departures;
        RegionID upper_id = 0;
        std::unordered_map<TrainID, Train> station_trains;
        Colour colour = white;
        int distance = 0;
        std::shared_ptr<Station> path_back = nullptr;
        bool need_refresh = false;
        std::vector<StationID> neigbours;
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
    int dist(const Coord &c1, const Coord &c2);
    std::unordered_set<TrainID> all_trains;
};

#endif // DATASTRUCTURES_HH
