// Datastructures.hh
//
// Student name: Huy Duong Cat
// Student email: duong.cat@tuni.fi
// Student number: 151377381

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <iostream>
#include <map>
#include <unordered_set>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
#include <queue>

using namespace std;

// Types for IDs
using AffiliationID = std::string;
using PublicationID = unsigned long long int;
using Name = std::string;
using Year = unsigned short int;
using Weight = int;
struct Connection;
// Type for a distance (in arbitrary units)
using Distance = int;

using Path = std::vector<Connection>;
using PathWithDist = std::vector<std::pair<Connection,Distance>>;

// Return values for cases where required thing was not found
AffiliationID const NO_AFFILIATION = "---";
PublicationID const NO_PUBLICATION = -1;
Name const NO_NAME = "!NO_NAME!";
Year const NO_YEAR = -1;
Weight const NO_WEIGHT = -1;

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

struct Connection
{
    AffiliationID aff1 = NO_AFFILIATION;
    AffiliationID aff2 = NO_AFFILIATION;
    Weight weight = NO_WEIGHT;
    float conectionLength = 0;
    int friction = 1/weight;
    bool operator>(const Connection& other) const {
        return friction > other.friction;
    }
    bool operator==(const Connection& c1) const{
        return (aff1==c1.aff1) && (aff2==c1.aff2) && (weight==c1.weight);
    }
};
const Connection NO_CONNECTION{NO_AFFILIATION,NO_AFFILIATION,NO_WEIGHT};


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

struct Affiliation
{
    AffiliationID id;
    Name name;
    Coord coordinates;
    std::vector<PublicationID> init_publications;

    //conection key = affilition connecting to
    //value = conection it self
    std::unordered_map<AffiliationID, Connection> aff_connections;
};

struct Publication
{
    PublicationID id;
    Name name;
    Year year;
    std::vector<AffiliationID> init_affiliations;
    // Publication that reference from (take material)
    std::vector<Publication*> child;
    // Publication that referencing current publication
    Publication* parent;
};

class Datastructures
{
private:
    /* To do with affiliations */
    //boolean flag to notice change in data structure
    bool change = false;
    //sort history
    std::vector<AffiliationID> hist_name_sort;
    std::vector<AffiliationID> hist_dist_sort;


    // unordered map to store Affiliations
    std::unordered_map<AffiliationID, Affiliation> affiliations;

    // unordered map or map to store location of afiliation for ease of acess
    std::map<Coord, AffiliationID> coordinateToAffiliation;

    // name to affiliation
    std::map<Name, AffiliationID> nameToAffiliation;

    /* To do with publications */
    // unordered map to store Publications
    std::unordered_map<PublicationID, Publication> publications;

    // Helper function to find a publication by its ID
    Publication* findPublicationByID(PublicationID id);

    /* To do with connection */
    bool connection_changed = true;
    std::vector<Connection> conectionsHist;

    bool least_fric_hist_changed = true;

    std::map<std::pair<AffiliationID, AffiliationID>, Path> least_fric_hist;
    //----------- Helper Funtions --------------//
    //DFS helper function
    void DFS(AffiliationID current, AffiliationID target, std::vector<Connection>& path, std::set<AffiliationID>& visited, std::vector<std::vector<Connection>>& all_paths);

    //Function to get all posible path
    std::vector<std::vector<Connection>> get_all_paths(AffiliationID source, AffiliationID target);

    // Function to calculate friction for a given paths
    std::vector<std::vector<Connection>> paths_with_lowest_friction(const std::vector<std::vector<Connection>>& allPaths);

    // Function to calculate friction for a given paths
    float calculateFriction(const std::vector<Connection>& path);

    //----------- End Helper Funtions -----------//


public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: The function  call for size() one time
    unsigned int get_affiliation_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: The function call for clear() 1 time
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: The for loop have the time complexity of O(n)
    // call for push_back() with constant time complexity
    std::vector<AffiliationID> get_all_affiliations();

    // Estimate of performance: O(1)
    // Short rationale for estimate: find(), creaating new affiliation, add the
    // new affiliation to map all have time complexity of O(1)
    bool add_affiliation(AffiliationID id, Name const& name, Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find() of unorderedmap have time complexity of O(n)
    // in average.
    Name get_affiliation_name(AffiliationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find() of unorderedmap have time complexity of O(n)
    // in average.
    Coord get_affiliation_coord(AffiliationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: add member through the vector have time copmplexity of
    // O(1) and this operation was called n times by the for loop
    std::vector<AffiliationID> get_affiliations_alphabetically();

    // Estimate of performance: O(n log n),
    // Short rationale for estimate: iterating through codinates members have complexity of O(n)
    // and sort() have complexity of O(n log n)
    std::vector<AffiliationID> get_affiliations_distance_increasing();

    // Estimate of performance: O(log n)
    // Short rationale for estimate: find() in unordermap have time complexity of O(log n)
    AffiliationID find_affiliation_with_coord(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find() in unordered map have time copmplexity of O(n)
    bool change_affiliation_coord(AffiliationID id, Coord newcoord);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: Beside the interation through affiliations ids,
    // The other part is O(1). Iterating through affiliaitons ids have time complexity of
    // O(n)
    bool add_publication(PublicationID id, Name const& name, Year year, const std::vector<AffiliationID> & affiliations);

    // Estimate of performance: O(n)
    // Short rationale for estimate: iteraring throgh publication have time complexity of
    // O(n)
    std::vector<PublicationID> all_publications();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Acessing the publication name have the time complexity of
    // O(1)
    Name get_publication_name(PublicationID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: find the publication and accessing its member all have
    // time complexity of O(1)
    Year get_publication_year(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find() in unordered map have time copmplexity of O(n)
    std::vector<AffiliationID> get_affiliations(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find() in unordered map have time copmplexity of O(n)
    bool add_reference(PublicationID id, PublicationID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: iterator that iterate through child member have time
    // complexity of O(n) ans was called once.
    std::vector<PublicationID> get_direct_references(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find() in unordered map have time copmplexity of O(n)
    bool add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find() in unordered map have time copmplexity of O(n)
    std::vector<PublicationID> get_publications(AffiliationID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: All operation have time complexity of O(1)
    // and was called once
    PublicationID get_parent(PublicationID id);

    // Estimate of performance: O(n log n)
    // Short rationale for estimate: for loop with time complexity O(1) was called once
    // and sort() with complexity O(n lof n) was called once this balanced to O(n log n)
    std::vector<std::pair<Year, PublicationID>> get_publications_after(AffiliationID affiliationid, Year year);

    // Estimate of performance: O(n)
    // Short rationale for estimate: call n the length of the chain (or path). In the average case, the operation
    // to add a parent to a path is called n times, thus making the averave time complexity O(n)
    std::vector<PublicationID> get_referenced_by_chain(PublicationID id);


    // Non-compulsory operations

    // Estimate of performance: O(n)
    // Short rationale for estimate:
    bool dfs(PublicationID id, std::vector<PublicationID>& path);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PublicationID> get_all_references(PublicationID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<AffiliationID> get_affiliations_closest_to(Coord xy);

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_affiliation(AffiliationID id);

    // Estimate of performance:
    // Short rationale for estimate:
    PublicationID get_closest_common_parent(PublicationID id1, PublicationID id2);

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_publication(PublicationID publicationid);

    // PRG 2 functions:

    // Estimate of performance: O(n)
    // Short rationale for estimate: finding in unoredered map result in worse case which is O(n)
    // same with iterate though its member
    std::vector<Connection> get_connected_affiliations(AffiliationID id);

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: The 1st for loop iterate though unordered map have time
    // complexity of O(n) calling on another loop with the same complexity, result in O(n^2)
    std::vector<Connection> get_all_connections();

    bool find_any_path_dfs(AffiliationID current, AffiliationID target,
                                           std::set<AffiliationID>& visited,
                                           std::vector<Connection>& path);
    // Estimate of performance: O(n^2)
    // Short rationale for estimate: The dfs opperation have the time efficientcy of O(n)
    Path get_any_path(AffiliationID source, AffiliationID target);



    // PRG2 optional functions

    // Estimate of performance:
    // Short rationale for estimate:
    Path get_path_with_least_affiliations(AffiliationID source, AffiliationID target);

    // Estimate of performance:
    // Short rationale for estimate:
    Path get_path_of_least_friction(AffiliationID source, AffiliationID target);

    // Estimate of performance:
    // Short rationale for estimate:
    PathWithDist get_shortest_path(AffiliationID source, AffiliationID target);

};

#endif // DATASTRUCTURES_HH
