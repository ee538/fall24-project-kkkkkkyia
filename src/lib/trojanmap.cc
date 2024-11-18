#include "trojanmap.h"

//-----------------------------------------------------
// TODO: Students should implement the following:
//-----------------------------------------------------
// Letters in lower case
std::string ToLower(const std::string& str) {
    std::string result = str;
    for (auto& c : result) {
        c = std::tolower(c);
    }
    return result;
}
/**
 * GetLat: Get the latitude of a Node given its id. If id does not exist, return
 * -1.
 *
 * @param  {std::string} id : location id
 * @return {double}         : latitude
 */
double TrojanMap::GetLat(const std::string &id) { 
   if (data.find(id) != data.end()) {
        return data[id].lat;
    }
    return -1;  // Return -1 if the node ID does not exist
}


/**
 * GetLon: Get the longitude of a Node given its id. If id does not exist,
 * return -1.
 *
 * @param  {std::string} id : location id
 * @return {double}         : longitude
 */
double TrojanMap::GetLon(const std::string &id) {
  if (data.find(id) != data.end()) {
        return data[id].lon;
    }
    return -1;  // Return -1 if the node ID does not exist
}



/**
 * GetName: Get the name of a Node given its id. If id does not exist, return
 * "NULL".
 *
 * @param  {std::string} id : location id
 * @return {std::string}    : name
 */
std::string TrojanMap::GetName(const std::string &id) {
  if (data.find(id) != data.end()) {
        return data[id].name;
    }
    return "NULL";  // Return "NULL" if the node ID does not exist
}


/**
 * GetNeighborIDs: Get the neighbor ids of a Node. If id does not exist, return
 * an empty vector.
 *
 * @param  {std::string} id            : location id
 * @return {std::vector<std::string>}  : neighbor ids
 */
std::vector<std::string> TrojanMap::GetNeighborIDs(const std::string &id) {
  if (data.find(id) != data.end()) {
        return data[id].neighbors;
    }
    return {};  // Return empty vector if the node ID does not exist
}


/**
 * GetID: Given a location name, return the id.
 * If the node does not exist, return an empty string.
 * The location name must be unique, which means there is only one node with the name.
 *
 * @param  {std::string} name          : location name
 * @return {std::string}               : id
 */
std::string TrojanMap::GetID(const std::string &name) {
  std::string name_lower = name;
    std::transform(name_lower.begin(), name_lower.end(), name_lower.begin(), ::tolower);

    for (const auto& cur : data) {
        std::string node_name = cur.second.name;
        std::transform(node_name.begin(), node_name.end(), node_name.begin(), ::tolower);
        if (node_name == name_lower) {
            return cur.first;  // Return the node ID
        }
    }
    return "";  // Return empty string if the node name does not exist
}

/**
 * GetPosition: Given a location name, return the position. If id does not
 * exist, return (-1, -1).
 *
 * @param  {std::string} name          : location name
 * @return {std::pair<double,double>}  : (lat, lon)
 */
std::pair<double, double> TrojanMap::GetPosition(std::string name) {
  std::pair<double, double> results(-1, -1);
  // std::map<std::string, Node>::iterator iter;
  for(auto iter = data.begin(); iter != data.end(); iter++){
    if(iter->second.name == name){
      results.first = iter->second.lat;
      results.second = iter->second.lon;
      return results;
    }
  }
  return results;
}

/**
 * CalculateEditDistance: Calculate edit distance between two location names
 * @param  {std::string} a          : first string
 * @param  {std::string} b          : second string
 * @return {int}                    : edit distance between two strings
 */
// The edit distance is the minimum number of operations required to transform one string into another
int TrojanMap::CalculateEditDistance(std::string a, std::string b) {  
  int m = a.size();
  int n = b.size();
  std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

  for (int i = 0; i <= m; i++){
    for (int j = 0; j <= n; j++){
      if (i == 0){
        dp[i][j] = j; // copy b on the first row
      }
      else if (j == 0){
        dp[i][j] = i; // delete a on the first column
      }
      else if (a[i - 1] == b[j - 1]){
        dp[i][j] = dp[i - 1][j - 1];
      }
      else{
        dp[i][j] = 1 + std::min({dp[i - 1][j],     // Delete
                                 dp[i][j - 1],     // Insert
                                 dp[i - 1][j - 1]  // Replace
                                });
      }
    }
  }
  return dp[m][n];
}

/**
 * FindClosestName: Given a location name, return the name with the smallest edit
 * distance.
 *
 * @param  {std::string} name          : location name
 * @return {std::string} tmp           : the closest name
 */
std::string TrojanMap::FindClosestName(std::string name) {
  std::string tmp = "";
  int minDistance = INT_MAX;
  //INT_MAX: used as a placeholder value to represent infinity or an unreachable value in algorithms 
  std::string LowerName = ToLower(name);

  for (auto& pair : data){
    std::string LocationName = ToLower(pair.second.name);
    // data.first is just an unique id but not the location name
    int distance = CalculateEditDistance(LowerName, LocationName);
    if (distance < minDistance){
      minDistance = distance;
      tmp = pair.second.name;
    }
  }
  return tmp;
}

/**
 * Autocomplete: Given a parital name return all the possible locations with
 * partial name as the prefix. The function should be case-insensitive.
 *
 * @param  {std::string} name          : partial name
 * @return {std::vector<std::string>}  : a vector of full names
 */
std::vector<std::string> TrojanMap::Autocomplete(std::string name) {
  std::vector<std::string> results;

  // If the input name is empty, return an empty vector.if (name.empty()) freturn results;
  if (name.empty()){
    return results;
  }

  name = ToLower(name);
  for (auto iter = data.begin(); iter != data.end(); iter++) {
    std::string cur_node = iter->second.name;
    // Create a lowercase
    std::string lower_curNode = ToLower(cur_node);

    // Check if the lowercase (lower_curNode) starts with name
    if (lower_curNode.substr(0, name.size()) == name) {
      results.push_back(cur_node); 
    }
  }
  return results;
}


/**
 * GetAllCategories: Return all the possible unique location categories, i.e.
 * there should be no duplicates in the output.
 *
 * @return {std::vector<std::string>}  : all unique location categories
 */
std::vector<std::string> TrojanMap::GetAllCategories() {
  // set: avoid duplicates here
  std::unordered_set<std::string> categories_set;
  
  for (auto& pair : data){
    categories_set.insert(pair.second.attributes.begin(), pair.second.attributes.end());
  }
  // set -> vector
  std::vector<std::string> Categories(categories_set.begin(), categories_set.end());
  std::sort(Categories.begin(), Categories.end());
  return Categories;
}

/**
 * GetAllLocationsFromCategory: Return all the locations of the input category (i.e.
 * 'attributes' in data.csv). If there is no location of that category, return
 * (-1, -1). The function should be case-insensitive.
 *
 * @param  {std::string} category         : category name (attribute)
 * @return {std::vector<std::string>}     : ids
 */
std::vector<std::string> TrojanMap::GetAllLocationsFromCategory(
    std::string category) {
  std::vector<std::string> res;

  // Convert category to lowercase for case-insensitive comparison
  std::transform(category.begin(), category.end(), category.begin(), ::tolower);

  // // Iterate over each node in the map.
  for (const auto& pair : data) {
    const Node& node = pair.second;

    // Check if any attribute matches the input category.
    for (const auto& attribute : node.attributes) {
      std::string lower_attribute = attribute;
      std::transform(lower_attribute.begin(), lower_attribute.end(), lower_attribute.begin(), ::tolower);
      
      // If there is a match, add the node's id to the result vector.
      if (lower_attribute == category) {
        res.push_back(node.id);
        break;  // No need to check further attributes for this node.
      }
    }
  }
  return res;
}



/**
 * GetLocationRegex: Given the regular expression of a location's name, your
 * program should first check whether the regular expression is valid, and if so
 * it returns all locations that match that regular expression.
 *
 * @param  {std::regex} location name      : the regular expression of location
 * names
 * @return {std::vector<std::string>}     : ids
 */
std::vector<std::string> TrojanMap::GetLocationRegex(std::regex location) {
  std::vector<std::string> result;
  
  // Iterate over each node in the map to check if the name matches the regex.
  for (const auto& pair : data) {
    const Node& node = pair.second;
    
    if (std::regex_match(node.name, location)) {
      result.push_back(node.id);
    }
  }
  
  return result;
}

/**
 * CalculateDistance: Get the distance between 2 nodes.
 * We have provided the code for you. Please do not need to change this function.
 * You can use this function to calculate the distance between 2 nodes.
 * The distance is in mile.
 * The distance is calculated using the Haversine formula.
 * https://en.wikipedia.org/wiki/Haversine_formula
 * 
 * @param  {std::string} a  : a_id
 * @param  {std::string} b  : b_id
 * @return {double}  : distance in mile
 */
double TrojanMap::CalculateDistance(const std::string &a_id,
                                    const std::string &b_id) {
  // Do not change this function
  Node a = data[a_id];
  Node b = data[b_id];
  double dlon = (b.lon - a.lon) * M_PI / 180.0;
  double dlat = (b.lat - a.lat) * M_PI / 180.0;
  double p = pow(sin(dlat / 2), 2.0) + cos(a.lat * M_PI / 180.0) *
                                           cos(b.lat * M_PI / 180.0) *
                                           pow(sin(dlon / 2), 2.0);
  double c = 2 * asin(std::min(1.0, sqrt(p)));
  return c * 3961;
}

/**
 * CalculatePathLength: Calculates the total path length for the locations
 * inside the vector.
 * We have provided the code for you. Please do not need to change this function.
 * 
 * @param  {std::vector<std::string>} path : path
 * @return {double}                        : path length
 */
double TrojanMap::CalculatePathLength(const std::vector<std::string> &path) {
  // Do not change this function
  double sum = 0;
  for (int i = 0; i < int(path.size()) - 1; i++) {
    sum += CalculateDistance(path[i], path[i + 1]);
  }
  return sum;
}

/**
 * CalculateShortestPath_Dijkstra: Given 2 locations, return the shortest path
 * which is a list of id. Hint: Use priority queue.
 *
 * @param  {std::string} location1_name     : start
 * @param  {std::string} location2_name     : goal
 * @return {std::vector<std::string>}       : path
 */
std::vector<std::string> TrojanMap::CalculateShortestPath_Dijkstra(
    std::string location1_name, std::string location2_name) {
  std::vector<std::string> path;
  std::string start_id = GetID(location1_name);
  std::string end_id = GetID(location2_name);

  if(start_id.empty() || end_id.empty()) return path; //Invalid input

  //Priority queue to store (distance, node_id)
  std::priority_queue<std::pair<double, std::string>, std::vector<std::pair<double, std::string>>, std:: greater<>> pq;

  //Distance map to store the minimum distance to each node
  std::unordered_map<std::string, double> distance;
  //Parent map to reconstruct the path
  std::unordered_map<std::string, std::string> parent;

  //Initialize distances to infinity, except for the starting node
  for(auto &pair : data){
    distance[pair.first] = std::numeric_limits<double>::infinity();
  }
  distance[start_id] = 0;
  pq.push({0, start_id});

  while (!pq.empty()){
    auto [cur_dist, cur_node] = pq.top();
    pq.pop();

    // If we reach the target node, stop
    if (cur_node == end_id) break;
    // Skip nodes that have already been processed with a smaller distance
    if (cur_dist > distance[cur_node]) continue;

    // Relaxation step
    for (auto &neighbor_id : data[cur_node].neighbors){
      double new_dist = cur_dist + CalculateDistance(cur_node, neighbor_id);
      if (new_dist <distance[neighbor_id]){
        distance[neighbor_id] = new_dist;
        parent[neighbor_id] = cur_node;
        pq.push({new_dist, neighbor_id});
      }
    }
  }

  //Reconstruct the path from end to start
  if (distance[end_id] == std::numeric_limits<double>::infinity()) return {};

  for (std::string at = end_id; at != ""; at = parent[at]) {
    path.push_back(at);
  }
  std::reverse(path.begin(), path.end());
  return path;
}


/**
 * CalculateShortestPath_Bellman_Ford: Given 2 locations, return the shortest
 * path which is a list of id. Hint: Do the early termination when there is no
 * change on distance.
 *
 * @param  {std::string} location1_name     : start
 * @param  {std::string} location2_name     : goal
 * @return {std::vector<std::string>}       : path
 */
std::vector<std::string> TrojanMap::CalculateShortestPath_Bellman_Ford(
    std::string location1_name, std::string location2_name) {
  std::vector<std::string> path;
  std::string start_id = GetID(location1_name);
  std::string end_id = GetID(location2_name);

  if(start_id.empty() || end_id.empty()) return path; //Invalid input

  //Distance map to store the minimum distance to each node
  std::unordered_map<std::string, double> distance;
  //Parent map to reconstruct the path
  std::unordered_map<std::string, std::string> parent;

  //Initialize distances to infinity, except for the starting node
  for(auto &pair : data){
    distance[pair.first] = std::numeric_limits<double>::infinity();
  }
  distance[start_id] = 0;
  
  int n = data.size();

  // Relax all edges up to (n - 1) times
  for (int i = 0; i < n - 1; ++i){
    bool updated = false;
    
    for (auto &pair : data){
      std::string u = pair.first;
      if (distance[u] == std::numeric_limits<double>::infinity()) continue;
      for (auto &v : data[u].neighbors){
        double new_dist = distance[u] + CalculateDistance(u,v);
        
        if (new_dist < distance[v]){
          distance[v] = new_dist;
          parent[v] = u;
          updated = true;
        }
      }
    }

    if (!updated) break; // Early termination if no update in this iteration
  }

  // Check for negative weight cycles (not needed here since we don't have negative weights)

  // Reconstruct the path from end to start
  if(distance[end_id] == std::numeric_limits<double>::infinity()) return {}; 

  for (std::string at = end_id; at != ""; at = parent[at]){
    if (parent.find(at) == parent.end() && at != start_id) return {}; 
    path.push_back(at);
  }
  std::reverse(path.begin(),path.end());
  return path;
}

/**
 * Traveling salesman problem: Given a list of locations, return the shortest
 * path which visit all the places and back to the start point.
 *
 * @param  {std::vector<std::string>} input : a list of locations needs to visit
 * @return {std::pair<double, std::vector<std::vector<std::string>>} : a pair of total distance and the all the progress to get final path, 
 *                                                                      for example: {10.3, {{0, 1, 2, 3, 4, 0}, {0, 1, 2, 3, 4, 0}, {0, 4, 3, 2, 1, 0}}},
 *                                                                      where 10.3 is the total distance, 
 *                                                                      and the first vector is the path from 0 and travse all the nodes and back to 0,
 *                                                                      and the second vector is the path shorter than the first one,
 *                                                                      and the last vector is the shortest path.
 */
// Please use brute force to implement this function, ie. find all the permutations.
std::pair<double, std::vector<std::vector<std::string>>> TrojanMap::TravelingTrojan_Brute_force(
                                    std::vector<std::string> location_ids) {
  std::pair<double, std::vector<std::vector<std::string>>> records;
  return records;
}

// Please use backtracking to implement this function
std::pair<double, std::vector<std::vector<std::string>>> TrojanMap::TravelingTrojan_Backtracking(
                                    std::vector<std::string> location_ids) {
  std::pair<double, std::vector<std::vector<std::string>>> records;
  return records;
}

// Hint: https://en.wikipedia.org/wiki/2-opt
std::pair<double, std::vector<std::vector<std::string>>> TrojanMap::TravelingTrojan_2opt(
      std::vector<std::string> location_ids){
  std::pair<double, std::vector<std::vector<std::string>>> records;
  return records;
}

// This is optional
std::pair<double, std::vector<std::vector<std::string>>> TrojanMap::TravelingTrojan_3opt(
      std::vector<std::string> location_ids){
  std::pair<double, std::vector<std::vector<std::string>>> records;
  return records;
}

/**
 * Given CSV filename, it read and parse locations data from CSV file,
 * and return locations vector for topological sort problem.
 * We have provided the code for you. Please do not need to change this function.
 * Example: 
 *   Input: "topologicalsort_locations.csv"
 *   File content:
 *    Name
 *    Ralphs
 *    KFC
 *    Chick-fil-A
 *   Output: ['Ralphs', 'KFC', 'Chick-fil-A']
 * @param  {std::string} locations_filename     : locations_filename
 * @return {std::vector<std::string>}           : locations
 */
std::vector<std::string> TrojanMap::ReadLocationsFromCSVFile(
    std::string locations_filename) {
  std::vector<std::string> location_names_from_csv;
  std::fstream fin;
  fin.open(locations_filename, std::ios::in);
  std::string line, word;
  getline(fin, line);
  while (getline(fin, word)) {
    location_names_from_csv.push_back(word);
  }
  fin.close();
  return location_names_from_csv;
}

/**
 * Given CSV filenames, it read and parse dependencise data from CSV file,
 * and return dependencies vector for topological sort problem.
 * We have provided the code for you. Please do not need to change this function.
 * Example: 
 *   Input: "topologicalsort_dependencies.csv"
 *   File content:
 *     Source,Destination
 *     Ralphs,Chick-fil-A
 *     Ralphs,KFC
 *     Chick-fil-A,KFC
 *   Output: [['Ralphs', 'Chick-fil-A'], ['Ralphs', 'KFC'], ['Chick-fil-A', 'KFC']]
 * @param  {std::string} dependencies_filename     : dependencies_filename
 * @return {std::vector<std::vector<std::string>>} : dependencies
 */
std::vector<std::vector<std::string>> TrojanMap::ReadDependenciesFromCSVFile(
    std::string dependencies_filename) {
  std::vector<std::vector<std::string>> dependencies_from_csv;
  std::fstream fin;
  fin.open(dependencies_filename, std::ios::in);
  std::string line, word;
  getline(fin, line);
  while (getline(fin, line)) {
    std::stringstream s(line);
    std::vector<std::string> dependency;
    while (getline(s, word, ',')) {
      dependency.push_back(word);
    }
    dependencies_from_csv.push_back(dependency);
  }
  fin.close();
  return dependencies_from_csv;
}

/**
 * DeliveringTrojan: Given a vector of location names, it should return a
 * sorting of nodes that satisfies the given dependencies. If there is no way to
 * do it, return a empty vector.
 *
 * @param  {std::vector<std::string>} locations                     : locations
 * @param  {std::vector<std::vector<std::string>>} dependencies     : prerequisites
 * @return {std::vector<std::string>} results                       : results
 */
std::vector<std::string> TrojanMap::DeliveringTrojan(
    std::vector<std::string> &locations,
    std::vector<std::vector<std::string>> &dependencies) {
  std::vector<std::string> result;
  std::unordered_map<std::string, std::unordered_set<std::string>> graph;
  std::unordered_map<std::string, int> in_degree;

  // Initialize graph and in-degree map
  for (const auto &location : locations) {
    graph[location] = {};
    in_degree[location] = 0;
  }

  // Build graph and in-degree map based on dependencies
  for (const auto &dependency : dependencies) {
    if (dependency.size() != 2) continue;
    const std::string &from = dependency[0];
    const std::string &to = dependency[1];

    if (graph[from].find(to) == graph[from].end()) {
      graph[from].insert(to);
      in_degree[to]++;
    }
  }

  // Perform topological sort using Kahn's algorithm
  std::queue<std::string> zero_in_degree;

  // Find nodes with zero in-degree
  for (const auto &entry : in_degree) {
    if (entry.second == 0) {
      zero_in_degree.push(entry.first);
    }
  }

  // Process nodes with zero in-degree
  while (!zero_in_degree.empty()) {
    std::string current = zero_in_degree.front();
    zero_in_degree.pop();
    result.push_back(current);

    for (const auto &neighbor : graph[current]) {
      in_degree[neighbor]--;
      if (in_degree[neighbor] == 0) {
        zero_in_degree.push(neighbor);
      }
    }
  }

  // Check if there was a cycle (if result does not include all nodes)
  if (result.size() != locations.size()) {
    return {};
  }

  return result;    
}

/**
 * inSquare: Give a id retunr whether it is in square or not.
 *
 * @param  {std::string} id            : location id
 * @param  {std::vector<double>} square: four vertexes of the square area
 * @return {bool}                      : in square or not
 */
bool TrojanMap::inSquare(std::string id, std::vector<double> &square) {
  if (data.find(id) == data.end()) return false;
  Node node = data[id];
  double lat = node.lat;
  double lon = node.lon;
  if (lon >= square[0] && lon <= square[1] && lat <= square[2] && lat >= square[3]) return true;
  return false;
}

/**
 * GetSubgraph: Give four vertexes of the square area, return a list of location
 * ids in the squares
 *
 * @param  {std::vector<double>} square         : four vertexes of the square
 * area
 * @return {std::vector<std::string>} subgraph  : list of location ids in the
 * square
 */
std::vector<std::string> TrojanMap::GetSubgraph(std::vector<double> &square) {
  // include all the nodes in subgraph
  std::vector<std::string> subgraph;

  for (auto& pair : data){
    if (inSquare(pair.first, square)) subgraph.push_back(pair.first);
  }
  return subgraph;
}

/**
 * Cycle Detection: Given four points of the square-shape subgraph, return true
 * if there is a cycle path inside the square, false otherwise.
 *
 * @param {std::vector<std::string>} subgraph: list of location ids in the
 * square
 * @param {std::vector<double>} square: four vertexes of the square area
 * @return {bool}: whether there is a cycle or not
 */
bool TrojanMap::CycleDetection(std::vector<std::string> &subgraph, std::vector<double> &square) {
    // Step 1: Build adjacency list for the subgraph within the square
    std::unordered_map<std::string, std::vector<std::string>> adj;
    for (const auto &id : subgraph) {
        for (const auto &neighbor : data[id].neighbors) {
            if (inSquare(neighbor, square)) {
                adj[id].push_back(neighbor);
            }
        }
    }

    // Step 2: DFS function to detect cycles
    std::unordered_set<std::string> visited;

    auto dfs = [&](const std::string &node, const std::string &parent, auto &dfs_ref) -> bool {
        visited.insert(node);
        for (const auto &neighbor : adj[node]) {
            if (neighbor != parent) {  // Skip the edge that led to this node
                if (visited.count(neighbor) || dfs_ref(neighbor, node, dfs_ref)) {
                    return true;  // Cycle found
                }
            }
        }
        return false;
    };

    // Step 3: Check for cycles in the subgraph
    for (const auto &node : subgraph) {
        if (!visited.count(node)) {
            if (dfs(node, "", dfs)) {
                std::cout << "Cycle Path: " << node << std::endl;
                return true;
            }
        }
    }

    return false;  // No cycle found
}

/**
 * FindNearby: Given a class name C, a location name L and a number r,
 * find all locations in class C on the map near L with the range of r and
 * return a vector of string ids
 *
 * @param {std::string} className: the name of the class
 * @param {std::string} locationName: the name of the location
 * @param {double} r: search radius
 * @param {int} k: search numbers
 * @return {std::vector<std::string>}: location name that meets the requirements
 */
std::vector<std::string> TrojanMap::FindNearby(std::string attributesName, std::string name, double r, int k) {
  std::vector<std::string> res;
  return res;
}

/**
 * Shortest Path to Visit All Nodes: Given a list of locations, return the shortest
 * path which visit all the places and no need to go back to the start point.
 *
 * @param  {std::vector<std::string>} input : a list of locations needs to visit
 * @return {std::vector<std::string> }      : the shortest path
 */
std::vector<std::string> TrojanMap::TrojanPath(
      std::vector<std::string> &location_names) {
    std::vector<std::string> res;
    return res;
}

/**
 * Given a vector of queries, find whether there is a path between the two locations with the constraint of the gas tank.
 *
 * @param  {std::vector<std::pair<double, std::vector<std::string>>>} Q : a list of queries
 * @return {std::vector<bool> }      : existence of the path
 */
std::vector<bool> TrojanMap::Queries(const std::vector<std::pair<double, std::vector<std::string>>>& q) {
    std::vector<bool> ans(q.size());
    return ans;
}

/**
 * CreateGraphFromCSVFile: Read the map data from the csv file
 * We have provided the code for you. Please do not need to change this function.
 */
void TrojanMap::CreateGraphFromCSVFile() {
  // Do not change this function
  std::fstream fin;
  fin.open("src/lib/data.csv", std::ios::in);
  std::string line, word;

  getline(fin, line);
  while (getline(fin, line)) {
    std::stringstream s(line);

    Node n;
    int count = 0;
    while (getline(s, word, ',')) {
      word.erase(std::remove(word.begin(), word.end(), '\''), word.end());
      word.erase(std::remove(word.begin(), word.end(), '"'), word.end());
      word.erase(std::remove(word.begin(), word.end(), '{'), word.end());
      word.erase(std::remove(word.begin(), word.end(), '}'), word.end());
      if (count == 0)
        n.id = word;
      else if (count == 1)
        n.lat = stod(word);
      else if (count == 2)
        n.lon = stod(word);
      else if (count == 3)
        n.name = word;
      else {
        word.erase(std::remove(word.begin(), word.end(), ' '), word.end());
        if (isalpha(word[0])) n.attributes.insert(word);
        if (isdigit(word[0])) n.neighbors.push_back(word);
      }
      count++;
    }
    data[n.id] = n;
  }
  fin.close();
}
