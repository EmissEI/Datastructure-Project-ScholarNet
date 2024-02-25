// Datastructures.cc
//
// Student name:
// Student email:
// Student number:

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

unsigned int Datastructures::get_affiliation_count()
{
    return affiliations.size();
}

void Datastructures::clear_all()
{
    //clear() have time complexity of O(n)
    affiliations.clear();
    publications.clear();
    coordinateToAffiliation.clear();
    nameToAffiliation.clear();
    hist_name_sort.clear();
    hist_dist_sort.clear();

    conectionsHist.clear();
    least_fric_hist.clear();
}

std::vector<AffiliationID> Datastructures::get_all_affiliations()
{
    //Initilize the vector
    std::vector<AffiliationID> allAffiliations;
    allAffiliations.reserve(affiliations.size());
    //O(n)
    for (const auto& pair : affiliations) {
        // O(1)
        allAffiliations.push_back(pair.first); // Collect AffiliationIDs
    }

    return allAffiliations;
}

bool Datastructures::add_affiliation(AffiliationID id, const Name & name, Coord xy)
{
    // Check if affliliation already exists
    // O(1)
    if (affiliations.find(id) != affiliations.end()){
        return false;
    }
    // Create new affiliation
    // O(1)
    Affiliation newAffiliation;
    newAffiliation.id = id;
    newAffiliation.name = name;
    newAffiliation.coordinates = xy;

    // Add to maps
    // O(1)
    affiliations[id] = newAffiliation;
    coordinateToAffiliation[xy] = id;
    nameToAffiliation[name] = id;
    std::cerr<<"xy and id "<<coordinateToAffiliation[xy]<<std::endl;



    //change flag
    change = true;
    return true;
}

Name Datastructures::get_affiliation_name(AffiliationID id)
{
    // Iterator to search for affiliation
    // O(n)
    auto it = affiliations.find(id);
    if (it != affiliations.end()) {
        // Affiliation exists, return its name
        //O(1)
        return it->second.name;
    } else {
        // Affiliation not found, return NO_NAME
        return NO_NAME;
    }
}

Coord Datastructures::get_affiliation_coord(AffiliationID id)
{
    // O(1)
    auto it = affiliations.find(id); // Attempt to find the affiliation
    // O(1)
    if (it != affiliations.end()) {
        // O(1)
        return it->second.coordinates;
    } else {
        // Affiliation not found, return NO_COORD
        return NO_COORD;
    }
}

std::vector<AffiliationID> Datastructures::get_affiliations_alphabetically()
{
    // O(n)
    std::vector<AffiliationID> sortedAffiliations;
    sortedAffiliations.reserve(nameToAffiliation.size());
    if (change == true){
        for (const auto& pair : nameToAffiliation) {
            // O(1)
            sortedAffiliations.push_back(pair.second);
        }
        hist_name_sort = sortedAffiliations;
        change = false;
        return sortedAffiliations;
    }
    else{
        return hist_name_sort;
    }
}


std::vector<AffiliationID> Datastructures::get_affiliations_distance_increasing() {
    if(change == false && hist_dist_sort.size() == affiliations.size())
    {
        return hist_dist_sort;
    }
    else{
        // Create vector for affiliations and cordinates(distance)
        std::vector<AffiliationID> sortedAffiliations;
        sortedAffiliations.reserve(coordinateToAffiliation.size());

        std::vector<std::pair<Coord, AffiliationID>> sortedCoords;
        sortedCoords.reserve(coordinateToAffiliation.size());

        //Populate cords vector
        // O(n)
        for(const auto& pair : coordinateToAffiliation){
            sortedCoords.emplace_back(pair.first, pair.second);
        }

        // O(n log n)
        std::sort(sortedCoords.begin(), sortedCoords.end(),
                  [](const auto& a, const auto& b) {
                      int dist1 = a.first.x * a.first.x + a.first.y * a.first.y;
                      int dist2 = b.first.x * b.first.x + b.first.y * b.first.y;

                      if (dist1 != dist2) {
                          return dist1 < dist2;
                      } else {
                          // If distances are equal, compare y-values
                          return a.first.y < b.first.y;
                      }
                  });

        //O(n)
        for (const auto& pair : sortedCoords) {
            sortedAffiliations.push_back(pair.second);
        }
        // add to history
        hist_dist_sort = sortedAffiliations;
        //change flag
        change = false;
        return sortedAffiliations;
    }
}

AffiliationID Datastructures::find_affiliation_with_coord(Coord xy)
{
    // Search for the given coordinates in the map
    // for map O(log n)
    auto it = coordinateToAffiliation.find(xy);
    // O(1)
    if (it != coordinateToAffiliation.end()) {
        // Coordinates match, return the ID of the affiliation
        return it->second;
    }

    // No affiliation found with the given coordinates, return NO_AFFILIATION
    return NO_AFFILIATION;

}

bool Datastructures::change_affiliation_coord(AffiliationID id, Coord newcoord)
{
    //O(n)
    auto it = affiliations.find(id);
    // O(1)
    if (it != affiliations.end()) {
        Coord oldcoord = it->second.coordinates;
        // Affiliation exists, update its coordinates
        it->second.coordinates = newcoord;

        //update coordinateToAffiliation
        coordinateToAffiliation.erase(oldcoord);
        coordinateToAffiliation[newcoord] = id;
        change = true;
        return true; // Return true indicating successful update
    } else {
        // Affiliation not found, return false
        return false;
    }
}

bool Datastructures::add_publication(PublicationID id, const Name &name, Year year,
                                     const std::vector<AffiliationID> &affiliations_id)
{
    // Check if the publication ID already exists
    // O(1)
    if (publications.find(id) != publications.end()) {
        // Publication ID already exists, return false (no action performed)
        return false;
    }

    // Create a new publication and populate its information
    // O(1)
    Publication newPublication;
    newPublication.name = name;
    newPublication.year = year;
    newPublication.init_affiliations = affiliations_id;
    newPublication.id = id;
    newPublication.parent = nullptr;
    newPublication.child = std::vector<Publication*>();

    // Add the new publication to the publications map
    // O(1)
    publications[id] = newPublication;

    // Add the new publication to affiliations map
    // O(k) with k the affiliation
    for (const auto& it_affiliation : affiliations_id)
    {
        affiliations[it_affiliation].init_publications.push_back(id);
    }

    // Add connection:
    if(size(affiliations_id)>1)
    {
        cerr<<"aff id input > 1"<<endl;
        for(const auto& aff_iter1 : affiliations_id)
        {
            Affiliation& a = affiliations[aff_iter1];
            cerr<<"Loop 1-----------------"<<endl;
            cerr<<"Id of aff1"<< a.id << endl;

            for(const auto& aff_iter2 : affiliations_id)
            {
                Affiliation& b = affiliations[aff_iter2];
                cerr<<"Loop 2--------------"<<endl;
                cerr<<"Id of aff2" << b.id << endl;

                if(a.id != b.id)
                {
                    // Check if a connection already exists
                    if (a.aff_connections.find(aff_iter2) == a.aff_connections.end())
                    {
                        float dist = sqrt(pow((a.coordinates.x - b.coordinates.x),2) +
                                          pow((a.coordinates.y - b.coordinates.y),2));
                        Connection newConnection;
                        newConnection.aff1 = a.id;
                        newConnection.aff2 = b.id;
                        newConnection.conectionLength = dist;
                        newConnection.weight = 1;

                        a.aff_connections[aff_iter2] = newConnection;
                    }
                    else
                    {
                        a.aff_connections[aff_iter2].weight++;
                    }
                }
            // debug
            cerr << endl ;
            cerr << "Test aff connection in a:" << endl;
            cerr << "ID of main aff" << affiliations[aff_iter1].id << endl;
            cerr << "connected to:" << endl;
            for(auto& i : affiliations[aff_iter1].aff_connections)
            {cerr <<i.second.aff2 << endl;}

            }
        }
    }

    // Return true indicating successful addition of the publication

    //Flag for functons that need history
    least_fric_hist_changed = true;
    connection_changed = true;
    return true;
}

std::vector<PublicationID> Datastructures::all_publications()
{
    // Create vector
    std::vector<PublicationID> allPubIDs;

    // Iterate through the publications map and collect all publication IDs
    // O(n)
    for (const auto& pair : publications) {
        allPubIDs.push_back(pair.first);
    }

    return allPubIDs;

}

Name Datastructures::get_publication_name(PublicationID id)
{
    auto it = publications.find(id);
    //O(1)
    if (it != publications.end()){
        // Publication found, return name
        return it->second.name;
    }
    else {
        // Publication not found
        return NO_NAME;
    }
}

Year Datastructures::get_publication_year(PublicationID id)
{
    // O(1)
    auto it = publications.find(id);
    // O(1)
    if(it != publications.end()){
        return it->second.year;
    }
    else{
        return NO_YEAR;
    }
}

std::vector<AffiliationID> Datastructures::get_affiliations(PublicationID id)
{
    // O(n)
    auto it = publications.find(id);
    //O(1)
    if (it != publications.end()) {
        // Publication found, return its affiliations
        return it->second.init_affiliations;
    } else {
        // Publication not found, return a vector with NO_AFFILIATION
        return {NO_AFFILIATION};
    }
}

bool Datastructures::add_reference(PublicationID id, PublicationID parentid)
{
    //std::cerr<<"original child id, parent id "<< id <<" "<< parentid <<std::endl;
    // O(n)
    auto child_id = publications.find(id);
    // O(n)
    auto parent_id = publications.find(parentid);
    //debug
    std::cerr<<"parent stat "<<parent_id->second.name<<" "<<
        parent_id->second.year<<" "<<
        parent_id->second.id<<std::endl;

    if (child_id == publications.end() || parent_id == publications.end())
    {
        return false;
    }
    // Add child to parent
    // O(1)
    parent_id->second.child.push_back(&(child_id->second));
    child_id->second.parent= &(parent_id->second);

    //std::cerr<<"name "<<child_id->second.parent->name<<std::endl;
    //std::cerr<<"year "<<child_id->second.parent->year<<std::endl;
    //std::cerr<<"id "<<child_id->second.parent->id<<std::endl;
    //std::cerr<<std::endl;
    return true;
}

Publication *Datastructures::findPublicationByID(PublicationID id) {
    auto it = publications.find(id);
    if (it != publications.end()) {
        return &(it->second);
    }
    return nullptr;
}

std::vector<PublicationID> Datastructures::get_direct_references(PublicationID id)
{
    // O(1)
    auto it = publications.find(id);

    if (it != publications.end())
    {
        std::vector<PublicationID> childIDs;
        //O(n)
        for (const auto& child : it->second.child) {
            childIDs.push_back(child->id);
        }
        return childIDs;
    }
    else
    {
        return {NO_PUBLICATION};
    }
}

bool Datastructures::add_affiliation_to_publication(AffiliationID affiliationid,
                                                    PublicationID publicationid)
{
    // O(n)
    auto it_affiliation = affiliations.find(affiliationid);
    auto it_publication = publications.find(publicationid);
    // O(1)
    if(it_affiliation == affiliations.end() || it_publication == publications.end())
    {
        return false;
    }
    // O(1)
    it_publication->second.init_affiliations.push_back(affiliationid);
    it_affiliation->second.init_publications.push_back(publicationid);

    auto& current_publication = publications[publicationid];
    auto& a = affiliations[affiliationid];
    cerr << "----------------Add affiliations to Publication-------------"<< endl;
    cerr << "ID of affiliation " <<a.id << endl;

    // Add connections between affiliations
    for (auto& aff_iter : current_publication.init_affiliations) {
        Affiliation& b = affiliations[aff_iter];
        cerr << "----------loop 1-----------" <<endl;
        cerr << "Id of 2nd aff: " << b.id << endl;

        if (a.id != b.id) {
            // Check if a connection already exists
            cerr<<"There is different, modding connection"<<endl;
            if (a.aff_connections.find(aff_iter) == a.aff_connections.end())
            {
            cerr << "Add new connection" <<endl;
                float dist = sqrt(pow((a.coordinates.x - b.coordinates.x), 2) +
                                  pow((a.coordinates.y - b.coordinates.y), 2));

                Connection newConnection1;
                newConnection1.aff1 = a.id;
                newConnection1.aff2 = b.id;
                newConnection1.conectionLength = dist;
                newConnection1.weight = 1;

                Connection newConnection2;
                newConnection2.aff1 = b.id;
                newConnection2.aff2 = a.id;
                newConnection2.conectionLength = dist;
                newConnection2.weight = 1;

                a.aff_connections[b.id] = newConnection1;
                b.aff_connections[a.id] = newConnection2;



            }
            else
            {
                cerr << "Connection already exist" <<endl;
                a.aff_connections[b.id].weight++;
                b.aff_connections[a.id].weight++;
            }

        }
    }

    // Flag for function that need history
    least_fric_hist_changed = true;
    connection_changed = true;
    return true;
}

std::vector<PublicationID> Datastructures::get_publications(AffiliationID id)
{
    // O(n)
    auto it_affiliation = affiliations.find(id);
    //O(1)
    if (it_affiliation != affiliations.end())
    {
        return it_affiliation->second.init_publications;
    }
    else
    {
        return {NO_PUBLICATION};
    }
}

PublicationID Datastructures::get_parent(PublicationID id)
{
    // O(1)
    auto it_publication = publications.find(id);

    if (it_publication == publications.end()) {
        //O(1)
        return NO_PUBLICATION; // If the publication doesn't exist, return NO_PUBLICATION

    }
    else {
        // Check if the publication has any referencing publications (parents)
        if (it_publication->second.parent == nullptr) {
            //O(1)
            return NO_PUBLICATION; // If there's no referencing publications, return NO_PUBLICATION


        } else {
            // O(1)
            return it_publication->second.parent->id;

        }
    }

}

std::vector<std::pair<Year, PublicationID> > Datastructures::get_publications_after(AffiliationID affiliationid, Year year)
{
    // O(n)
    auto it_affiliation = affiliations.find(affiliationid);
    // O(1)
    if (it_affiliation == affiliations.end()) {
        return {{NO_YEAR, NO_PUBLICATION}}; // If the affiliation doesn't exist, return {NO_YEAR, NO_PUBLICATION}
    }

    std::vector<std::pair<Year, PublicationID>> result;
    //O(n)
    for (const auto& publication_id : it_affiliation->second.init_publications) {
        // O(1)
        auto it_publication = publications.find(publication_id);
        //O(1)
        if (it_publication != publications.end() && it_publication->second.year >= year) {
            result.push_back({it_publication->second.year, publication_id});
        }
    }

    // Sort the result vector by year and then by publication ID
    // O(n log n)
    std::sort(result.begin(), result.end());

    return result;
}

std::vector<PublicationID> Datastructures::get_referenced_by_chain(PublicationID id)
{
    std::vector<PublicationID> path;
    //O(1)
    auto it_publication = publications.find(id);
    //O(1)
    if (it_publication == publications.end()) {
        return {NO_PUBLICATION}; // If the publication doesn't exist, return a vector with NO_PUBLICATION
    }
    // O(n)
    if (it_publication != publications.end())
    {
        Publication* current = &(it_publication->second);
        if(current->parent == nullptr)
        {
            return{};
        }
        //Go up from the leave to root
        // Avoid to get the leaf in the path
        current = current ->parent;
        //O(n)
        while(current != nullptr)
        {
            //O(1)
            path.push_back(current->id);
            current = current ->parent;
        }
        //std::reverse(path.begin(), path.end());
    }
    return path;

}

// O(n)
bool Datastructures::dfs(PublicationID id, std::vector<PublicationID>& path) {

    auto it = publications.find(id);
    if (it == publications.end()) {
        return false; // ID not found
    }

    path.push_back(id);

    // Traverse children
    for (Publication* child : it->second.child) {
        dfs(child->id, path);
    }

    return true;
}


std::vector<PublicationID> Datastructures::get_all_references(PublicationID id) {
    // Check if the given publication ID exists
    // O(1)
    auto it = publications.find(id);
    if (it == publications.end()) {
        return {NO_PUBLICATION};
    }
    std::vector<PublicationID> result;
    // Start DFS from the given publication ID
    // O(n)
    dfs(id, result);
    // O(n) due to result.begin() -> shift element
    result.erase(result.begin());
    return result;
}


std::vector<AffiliationID> Datastructures::get_affiliations_closest_to(Coord xy)
{
    std::vector<std::pair<Coord, AffiliationID>> sortedDistances;
    sortedDistances.reserve(coordinateToAffiliation.size());

    //Populate cords vector
    // Make sure to exclue the affiliation in question
    // O(n)
    for(const auto& pair : coordinateToAffiliation){
        if(pair.first != xy){
            sortedDistances.emplace_back(pair.first, pair.second);
        }
    }


    // O(n log n)
    std::sort(sortedDistances.begin(), sortedDistances.end(),
              [&xy](const auto& a, const auto& b) {
                  double dist1 = std::sqrt(std::pow(a.first.x - xy.x,2) +
                                           std::pow(a.first.y - xy.y,2));
                  double dist2 = std::sqrt(std::pow(b.first.x - xy.x,2) +
                                           std::pow(b.first.y - xy.y,2));
                  if (dist1 != dist2) {
                      return dist1 < dist2;
                  } else {
                      // If distances are equal, compare y-values
                      return a.first.y < b.first.y;
                  }
              });

    // Collect closest three or all affiliations
    std::vector<AffiliationID> closestAffiliations;
    closestAffiliations.reserve(3);
    for (const auto& pair : sortedDistances) {
        closestAffiliations.push_back(pair.second);
        if (closestAffiliations.size() == 3) {
            break;
        }
    }

    return closestAffiliations;
}

bool Datastructures::remove_affiliation(AffiliationID id) {
    // O(n)
    auto affiliationIter = affiliations.find(id);

    // Check if affiliation exists
    if (affiliationIter == affiliations.end()) {
        return false; // Affiliation doesn't exist
    }
    change = true;
    // Remove the affiliation from affiliations map
    auto xy =  affiliationIter->second.coordinates;
    auto name = affiliationIter->second.name;
    affiliations.erase(affiliationIter);
    coordinateToAffiliation.erase(xy);
    nameToAffiliation.erase(name);

    // Remove the affiliation from publications
    // Most dominant
    // O(n)
    for (auto& publicationPair : publications)
    {
        auto& initAffiliations = publicationPair.second.init_affiliations;
        auto it = std::find(initAffiliations.begin(), initAffiliations.end(), id);

        if (it != initAffiliations.end())
        {
            initAffiliations.erase(it);
        }
    }

    return true; // Affiliation removed successfully
}


PublicationID Datastructures::get_closest_common_parent(PublicationID id1, PublicationID id2) {
    // Get pointers to the publications using their IDs
    Publication* pub1 = findPublicationByID(id1);
    Publication* pub2 = findPublicationByID(id2);

    if (!pub1 || !pub2) {
        return NO_PUBLICATION; // Either id1 or id2 doesn't exist
    }

    // Find the nearest common parent using the parent relationship
    std::unordered_set<PublicationID> ancestors;
    while (pub1 != nullptr) {
        ancestors.insert(pub1->id);
        pub1 = pub1->parent;
    }

    while (pub2 != nullptr) {
        if (ancestors.find(pub2->id) != ancestors.end() && pub2->id != id1 ) {
            return pub2->id; // Found the nearest common parent
        }
        pub2 = pub2->parent;
    }

    return NO_PUBLICATION; // No common parent found

}


bool Datastructures::remove_publication(PublicationID publicationid) {
    auto pubIt = publications.find(publicationid);

    // Check if the publication exists
    if (pubIt == publications.end()) {
        return false; // Publication with the given ID doesn't exist
    }

    // Remove references to this publication in affiliations
    for (auto& affiliation : affiliations) {
        auto& initPubs = affiliation.second.init_publications;
        auto it = std::remove(initPubs.begin(), initPubs.end(), publicationid);
        initPubs.erase(it, initPubs.end());
    }

    // Remove this publication's parent and child relationships
    for (auto& pub : publications) {
        auto& children = pub.second.child;
        children.erase(std::remove_if(children.begin(), children.end(),
                                      [publicationid](Publication* child) { return child->id == publicationid; }),
                       children.end());

        if (pub.second.parent && pub.second.parent->id == publicationid) {
            pub.second.parent = nullptr;
        }
    }

    // Remove the publication itself
    publications.erase(publicationid);

    return true; // Successfully removed the publication
}


// Focus
std::vector<Connection> Datastructures::get_connected_affiliations(AffiliationID id)
{
    //O(n)
    const auto& a = affiliations.find(id);
    //O(1)
    if (a == affiliations.end())
    {
        return {};
    }
    //O(n)
    else{
        std::vector<Connection> connections_list;
        connections_list.reserve(size(a->second.aff_connections));
        cerr << "Length of connection list" << size(a->second.aff_connections) <<endl;
        for(const auto& iter : a->second.aff_connections){
            connections_list.push_back(iter.second);
        }
        return connections_list;
    }
}


std::vector<Connection> Datastructures::get_all_connections()
{
    std::vector<Connection> all_connections;

    if(connection_changed)
    {
        // Use a set to avoid duplicates
        std::set<std::pair<AffiliationID, AffiliationID>> connection_set;

        // Iterate through all affiliations' connections
        //O(n^2)
        for (const auto& pair : affiliations) {
            const Affiliation& affiliation = pair.second;
            //O(n)
            for (const auto& connection : affiliation.aff_connections) {
                const Connection& current_connection = connection.second;

                // Ensure smaller affiliation_id comes first in the pair
                AffiliationID smaller_id = std::min(current_connection.aff1, current_connection.aff2);
                AffiliationID larger_id = std::max(current_connection.aff1, current_connection.aff2);

                //O(1)
                // Add the connection to the set if it's not a duplicate
                if (connection_set.find({smaller_id, larger_id}) == connection_set.end()) {
                    Connection new_connection;
                    new_connection.aff1 = smaller_id;
                    new_connection.aff2 = larger_id;
                    new_connection.conectionLength = current_connection.conectionLength;
                    new_connection.weight = current_connection.weight;

                    all_connections.push_back(new_connection);
                    connection_set.insert({smaller_id, larger_id});
                }
            }
        }
        conectionsHist = all_connections;
    }

    else{
        all_connections = conectionsHist;
    }

    connection_changed = false;
    return all_connections;
}

bool Datastructures::find_any_path_dfs(AffiliationID current, AffiliationID target,
                                       std::set<AffiliationID>& visited,
                                       std::vector<Connection>& path) {
    if (current == target) {
        return true; // Target found, stop searching
    }

    auto it_current = affiliations.find(current);
    if (it_current == affiliations.end()) {
        return false; // If current affiliation does not exist, return false
    }

    // Iterate through connections of the current affiliation
    // O(n)
    for (const auto& connection : it_current->second.aff_connections) {
        AffiliationID next_affiliation_id = connection.first;

        // Check if the next affiliation has not been visited
        if (visited.find(next_affiliation_id) == visited.end()) {
            visited.insert(next_affiliation_id); // Mark the next affiliation as visited

            // Add the connection to the path
            path.push_back(connection.second);

            // Recursively search for a path from the next affiliation to the target
            bool found = find_any_path_dfs(next_affiliation_id, target, visited, path);

            if (found) {
                return true; // Path found, stop searching
            }

            // If no path is found, backtrack and remove the connection from the path
            path.pop_back();
        }
    }

    return false; // No path found from current affiliation
}


Path Datastructures::get_any_path(AffiliationID source, AffiliationID target)
{
    std::vector<Connection> path;
    // O(n)
    auto it_source = affiliations.find(source);
    auto it_target = affiliations.find(target);

    // Check if source or target affiliation does not exist
    if (it_source == affiliations.end() || it_target == affiliations.end()) {
        return path; // Return an empty vector if source or target affiliation does not exist
    }

    // Use a set to track visited affiliations to avoid cycles
    std::set<AffiliationID> visited;
    visited.insert(source); // Mark the source as visited

    // O(n)?
    // Perform depth-first search (DFS) to find a path between source and target
    bool found = find_any_path_dfs(source, target, visited, path);

    if (!found) {
        // If no path is found, return an empty vector
        path.clear();
    }

    return path;
}


//OPTIONAL
Path Datastructures::get_path_with_least_affiliations(AffiliationID source, AffiliationID target)
{
    std::vector<Connection> path;

    auto it_source = affiliations.find(source);
    auto it_target = affiliations.find(target);

    // Check if source or target affiliation does not exist
    if (it_source == affiliations.end() || it_target == affiliations.end()) {
        return path; // Return an empty vector if source or target affiliation does not exist
    }

    // Use a queue for BFS
    std::queue<std::pair<AffiliationID, std::vector<Connection>>> q;

    // Set to track visited affiliations
    std::set<AffiliationID> visited;
    visited.insert(source);

    // Start BFS from the source affiliation
    q.push({source, {}});

    while (!q.empty()) {
        auto current = q.front();
        q.pop();
        // q right now is empty

        AffiliationID current_id = current.first;
        std::vector<Connection> current_path = current.second;

        // Check if the target affiliation is reached
        if (current_id == target) {
            path = current_path; // Found a path, assign it to the result
            break;
        }

        auto it_current = affiliations.find(current_id);
        if (it_current == affiliations.end()) {
            continue; // If current affiliation does not exist, skip
        }

        // Iterate through connections of the current affiliation
        for (const auto& connection : it_current->second.aff_connections) {
            AffiliationID next_affiliation_id = connection.first;

            // Check if the next affiliation has not been visited
            if (visited.find(next_affiliation_id) == visited.end()) {
                visited.insert(next_affiliation_id); // Mark the next affiliation as visited

                std::vector<Connection> next_path = current_path;
                next_path.push_back(connection.second); // Extend the path

                // Add the next affiliation and its extended path to the queue
                q.push({next_affiliation_id, next_path});
            }
        }
    }

    return path;
}

void Datastructures::DFS(AffiliationID current, AffiliationID target, std::vector<Connection>& path,
                         std::set<AffiliationID>& visited, std::vector<std::vector<Connection>>& all_paths)
{
    visited.insert(current);

    if (current == target) {
        all_paths.push_back(path);
    } else {
        auto it_current = affiliations.find(current);
        if (it_current != affiliations.end()) {
            for (const auto& connection : it_current->second.aff_connections) {
                AffiliationID next_affiliation_id = connection.first;

                if (visited.find(next_affiliation_id) == visited.end()) {
                    path.push_back(connection.second);
                    DFS(next_affiliation_id, target, path, visited, all_paths);
                    path.pop_back();
                }
            }
        }
    }

    visited.erase(current);
}

//Function to get all posible path
std::vector<Path> Datastructures::get_all_paths(AffiliationID source, AffiliationID target)
{
    std::vector<std::vector<Connection>> all_paths;
    std::vector<Connection> path;
    std::set<AffiliationID> visited;
    DFS(source, target, path, visited, all_paths);

    return all_paths;
}

// Function to calculate friction for a given paths
float Datastructures::calculateFriction(const std::vector<Connection>& path)
{
    if (path.empty()) {
        return std::numeric_limits<float>::max(); // Return a high value for an empty path
    }

    int minWeight = std::numeric_limits<int>::max();

    // Finding the minimum connection weight in the path
    for (const auto& connection : path) {
        if (connection.weight < minWeight) {
            minWeight = connection.weight;
        }
    }

    // Calculate friction based on the minimum weight and path length
    float friction = 1.0f / minWeight;
    cerr<<"Friction: "<<friction<<endl;
    return friction;
}

// Funtion to return all the path with lowest friction
std::vector<std::vector<Connection>> Datastructures::paths_with_lowest_friction(const std::vector<std::vector<Connection>>& allPaths) {
    std::vector<std::vector<Connection>> lowestFrictionPaths;
    float minFriction = std::numeric_limits<float>::max();

    // Calculate the friction for each path and find the minimum friction
    for (const auto& path : allPaths) {
        float currentFriction = calculateFriction(path);

        if (currentFriction < minFriction) {
            minFriction = currentFriction;
            lowestFrictionPaths.clear(); // Clear previous lowest friction paths
            lowestFrictionPaths.push_back(path);
        } else if (currentFriction == minFriction) {
            lowestFrictionPaths.push_back(path);
        }
    }
    cerr<<"size: "<<lowestFrictionPaths.size()<<endl;
    return lowestFrictionPaths;
}


std::vector<Connection> Datastructures::get_path_of_least_friction(AffiliationID source, AffiliationID target) {
    cerr<<"-----------------get_path_of_least_friction----------------"<<endl;
    std::vector<Connection> path;
    std::pair<AffiliationID, AffiliationID>key = {source, target};
    auto hist_it = least_fric_hist.find(key);

    if(least_fric_hist_changed)
    {
        least_fric_hist.clear();
    }
    if(least_fric_hist_changed ||  hist_it == least_fric_hist.end())
    // if there is change in the connections structure or history not found
    {
        cerr<<"Initiate if clause"<<endl;
        cerr<<"---------get all path"<<endl;
        auto all_path = get_all_paths( source,  target);

        cerr<<"---------get path with lowest friction"<<endl;
        auto lowestFrictionPaths = paths_with_lowest_friction(all_path);

        size_t min_connections = std::numeric_limits<size_t>::max();
        std::vector<Connection> path_with_least_connections;

        for (const auto& path : lowestFrictionPaths) {
            if (path.size() < min_connections) {
                min_connections = path.size();
                path_with_least_connections = path;
            }
        }
        //add to history
        least_fric_hist_changed = false;
        least_fric_hist[{source, target}] = path_with_least_connections;
        return path_with_least_connections;
    }
    else
    // history found
    {
        cerr<<"Found in HIST"<<endl;
        return least_fric_hist[{source, target}];
    }

}


PathWithDist Datastructures::get_shortest_path(AffiliationID source, AffiliationID target) {
    // Create a priority queue to store nodes and their distances
    std::priority_queue<std::pair<Distance, AffiliationID>,
                        std::vector<std::pair<Distance, AffiliationID>>,
                        std::greater<std::pair<Distance, AffiliationID>>> pq;

    // Map to store distances from the source to each affiliation
    std::unordered_map<AffiliationID, Distance> distances;

    // Initialize distances to infinity
    for (const auto& [id, affiliation] : affiliations) {
        distances[id] = std::numeric_limits<Distance>::max();
    }

    // Set distance of source to 0 and push it into the priority queue
    distances[source] = 0;
    pq.push({0, source});

    // Map to store the previous node in the shortest path
    std::unordered_map<AffiliationID, Connection> prev;

    while (!pq.empty()) {
        auto [distance, current] = pq.top();
        pq.pop();

        if (current == target) {
            break; // Reached the target, exit the loop
        }

        for (const auto& [neighbour_id, connection] : affiliations[current].aff_connections) {
            // Calculate distance between current and neighbour based on coordinates
            float dist = sqrt(pow((affiliations[current].coordinates.x - affiliations[neighbour_id].coordinates.x), 2) +
                              pow((affiliations[current].coordinates.y - affiliations[neighbour_id].coordinates.y), 2));

            if (distances[current] + dist < distances[neighbour_id]) {
                distances[neighbour_id] = distances[current] + dist;
                pq.push({distances[neighbour_id], neighbour_id});
                prev[neighbour_id] = connection;
            }
        }
    }

    // Reconstruct the shortest path from target to source using 'prev'
    PathWithDist shortest_path;
    AffiliationID current = target;
    while (prev.find(current) != prev.end()) {
        Connection connection = prev[current];
        float dist = sqrt(pow((affiliations[current].coordinates.x - affiliations[connection.aff1].coordinates.x), 2) +
                          pow((affiliations[current].coordinates.y - affiliations[connection.aff1].coordinates.y), 2));
        shortest_path.push_back({connection, dist});
        current = connection.aff1;
    }
    std::reverse(shortest_path.begin(), shortest_path.end());

    return shortest_path;
}




