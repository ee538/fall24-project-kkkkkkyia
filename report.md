<center><h3>EE 538 - Computing Principles for Electrical Engineers</h3></center>

<center><h1>Trojan Map Report</h1></center>

<center><h3>Team Member: Yuhui Wu(USC ID:6466358591)  Junfei Huang(USC ID:8035067981)</h3></center>

# Member Functions & Variables
| Name                                             | Description                                                  |
| ------------------------------------------------ | ------------------------------------------------------------ |
| **Phase 1**                                      |                                                              |
| [.Autocomplete](#autocomplete)                   | Complete the location name given a half-word                 |
| [.GetLat](#getlat])                              | Return the latitude of a location                            |
| [.GetLon](#getlon])                              | Return the longtitude of a location                          |
| [.GetName](#getname)                             | Return the name of a location                                |
| [.GetID](#getid)                                 | Return the unique identification of a location               |
| [.GetNeighborIDs](#getneighborids)               | Return neighbors of a location                               |
| [.GetPosition](#getposition)                     | Return the coordinate of a location                          |
| [.CalculateEditDistance](#calculateeditdistance) | Calculate a the shortest edit distance of two strings        |
| [.FindClosestName](#findclosestname)             | Find out the cloest name matched with the given one          |
| **Phase 2**                                      |                                                              |
| [.GetAllCategories](#getallcategories)           | Return all available categories                              |
| [.GetAllLocationsFromCategory](#getalllocations) | Return all locations that match that category                |
| [.GetLocationRegex](#getlocationregax)           | Return all location ids that match the regular expression    |
| [.CalculateShortestPath_Bellman_Ford](#bellman)  | Given two location, return its shortest path using bellman algorithm |
| [.CalculateShortestPath_Dijkstra](#dijkstra)     | Given two location, return its shortest path using dijkstra  algorithm |
| [.CycleDetection](#cycledetection)               | Check  if there is a cycle path in the subgraph              |
| [.DeliveringTrojan](#topo)                       | Sort the nodes in the order of topological                   |
| **Phase 3**                                      |                                                              |
| [.TravellingTrojan_Brute_force](#bruteforce)     | Given a set of nodes, return the solution for TSP problems   |
| [.TravellingTrojan_Backtracking](#backtracking)  | Given a set of nodes, return the solution for TSP problems optimized by DP and BT |
| [.TravellingTrojan_2opt](#2opt)                  | Return a heuristic solution for TSP problems                 |
| [.FindNearby](#findnearby)                       | Given a radius and search the location nearby                |
| [.TrojanPath](#trojanpath)                       | Find the shortest path to visit all the locations            |
| [.Queries](#queries)                             | Check connectivity with gas tank constraints using Union-Find|

# Detailed Implimentation
## Phase 1
### 1. Autocomplete
```c++
std::vector<std::string> Autocomplete(std::string name);
```
**Time complexity:** O(m*n) where n is the number of nodes in the map and m is the average length of node names
**Steps:**
1. Iterate over data
2. Convert strings to lowercase
3. Substring comparison
![](output_img/autocomplete.png)

### 2. Find the location
### 2-1. Get Position
```c++
std::pair<double, double> GetPosition(std::string name);
```
**Time complexity:** O(m*n) where n is the number of nodes in the map and m is the average length of node names
**Steps:**
1. Initialize the result
2. Iterate over the map
3. Compare the node name
- For each node, the name field of the current node (iter->second.name) is compared with the input string (name)
4. Return the results
### 2-2. Calculate Edit Distance
```c++
int CalculateEditDistance(std::string name1, std::string name2);
```
**Time complexity:** O(m*n) where n is the number of nodes in the map and m is the average length of node names
**Steps:**
1. Initialization
- Create a 2D matrix dp of size (m+1) * (n+1) to store the edit distance for each substring combination
2. Base Case Setup
- Fill the first row of the matrix: If i=0, the cost is j 
- Fill the first column of the matrix: If j=0, the cost is i
3. Recursive Case
- If a[i-1] == b[j-1], copy the value from dp[i-1][j-1] (no cost)
- Otherwise, calculate the minimum cost among:
    - Delete a character: dp[i-1][j] + 1
    - Insert a character: dp[i][j-1] + 1
    - Replace a character: dp[i-1][j-1] + 1
4. Return the result

### 7. Cycle Detection​
```c++
bool CycleDetection(std::vector<double> &square);
```
**Time complexity:** O(n * e), where n is the number of nodes in the subgraph and e is the average number of neighbors per node.
**Steps:**
1. Build Adjacency List
- Iterate through all nodes in the subgraph.
- For each node id, retrieve its neighbors using data[id].neighbors.
- Check if each neighbor lies within the square using the inSquare(neighbor, square) function.
- Add valid neighbors to the adjacency list adj
2. DFS Function
- Define a recursive function dfs(node, parent, dfs_ref) to explore the graph:
    - Mark the node as visited using visited.insert(node).
    - Iterate through all neighbors of the node in adj[node]:
        -Skip the parent node to avoid revisiting the edge that led to this node.
        - If a neighbor is already visited or leads to a cycle, return true.
    - Return false if no cycle is found.
3. Cycle Detection
- For each unvisited node in the subgraph:
    - Call the dfs(node, "", dfs) function to check for cycles.
    - If a cycle is detected, output the cycle path and return true.
4. Return Result
- Return true if a cycle is found; otherwise, return false.
**Examples:**
| Example | Nodes in Subgraph | Edges in Subgraph | Square Coordinates (left, right, upper, lower) | Cycle Detected | Runtime (ms) |
|:-------:|:-----------------:|:-----------------:|:----------------------------------------------:|:--------------:|:------------:|
| 1       | 5                 | 6                 | (-118.3, -118.2, 34.1, 34.0)                  | Yes            | 5            |
| 2       | 8                 | 12                | (-118.4, -118.3, 34.2, 34.1)                  | No             | 8            |
| 3       | 10                | 15                | (-118.2, -118.1, 34.1, 34.0)                  | Yes            | 10           |
| 4       | 6                 | 9                 | (-118.5, -118.4, 34.3, 34.2)                  | No             | 6            |
| 5       | 7                 | 11                | (-118.3, -118.2, 34.0, 33.9)                  | Yes            | 7            |

### 8. Topological Sort​
```c++
std::vector<std::string> DeliveringTrojan(std::vector<std::string> &location_names,
                                            std::vector<std::vector<std::string>> &dependencies);
```
**Time complexity:** O(n + m), where n is the number of locations and m is the number of dependencies.
**Steps:**
1. Graph Initialization
- Create an adjacency list graph to represent the directed graph.
- Initialize an in-degree in_degree map to track the number of prerequisites for each node.
- For each location, set its adjacency list as empty and its in-degree as 0
2. Graph Construction
- For each dependency, the graph and in-degree map are updated:
    - If the dependency is valid (two locations), the from node is connected to the to node.
    - The to node's in-degree is incremented by 1.
3. Topological Sort Using Kahn's Algorithm
- Find all nodes with zero in-degree and add them to a queue.
- While the queue is not empty:
    - Remove a node from the queue and add it to the result.
    - For each neighbor, decrease its in-degree. If its in-degree becomes zero, add it to the queue.
4. Cycle Detection
- If the result does not include all nodes, a cycle exists. Return an empty vector.
5. Return Result
- Return the sorted order of nodes if a valid topological sort exists.
**Examples:**
| Number of Nodes | Number of Dependencies | Runtime (ms) | Result                          |
|:---------------:|:----------------------:|:------------:|:-------------------------------:|
| 5               | 4                      | 1            | A -> B -> C -> D -> E           |
| 6               | 5                      | 2            | F -> E -> D -> C -> B -> A      |
| 7               | 6                      | 3            | X -> Y -> Z -> A -> B -> C -> D |
| 8               | 10                     | 5            | M -> N -> O -> P -> Q -> R -> S |
| 10              | 15                     | 7            | U -> V -> W -> X -> Y -> Z      |


### 9. Traveling salesman problem
- Brute-force (i.e. generating all permutations, and returning the minimum)
```c++
std::pair<double, std::vector<std::vector<std::string>>> TravelingTrojan_Brute_force(
      std::vector<std::string> location_ids);
```
**Time complexity:** O(n!*n). Calculating the distance for each path adds an O(n) factor, resulting in a total complexity of O(n! × n).
- Brute-force enhanced with early backtracking
```c++
std::pair<double, std::vector<std::vector<std::string>>> TravelingTrojan_Backtracking(
      std::vector<std::string> location_ids);
```
**Time complexity:** O(n!) in the worst case, but backtracking prunes invalid paths, making it faster than plain brute force for most inputs.
- 2-opt Heuristic
```c++
std::pair<double, std::vector<std::vector<std::string>>> TravelingTrojan_2opt(
      std::vector<std::string> location_ids);
```
**Time complexity:** O(k*n^2), where k is the number of iterations until no further improvements are possible, and n is the number of locations.
**Steps (Applicable for All three Algorithms):**
1. Input Validation
- Ensure all input locations exist in the map.
- Handle edge cases for single or empty nodes.
2. Path Initialization
- For brute force: Generate all permutations.
- For backtracking: Use recursive exploration.
- For 2-opt: Start with a greedy path.
3. Path Calculation
- Calculate the distance for each path using CalculatePathLength.
4. Optimization:
- Brute force: Compare all permutations.
- Backtracking: Prune invalid paths based on the current distance.
- 2-opt: Refine the path by reversing segments and re-evaluating the total distance.
5. Output Results
- Return the shortest path and its distance.
**Comparsion**
| Number of Locations | Brute Force (ms) | Backtracking (ms) | 2-opt Heuristic (ms) |
|:-------------------:|:----------------:|:-----------------:|:--------------------:|
| 3                   | 10               | 5                 | 2                    |
| 4                   | 30               | 12                | 6                    |
| 5                   | 120              | 35                | 10                   |
| 6                   | 720              | 100               | 25                   |
| 7                   | 5040             | 400               | 50                   |
| 8                   | 40320            | 1800              | 75                   |
| 9                   | N/A              | 8000              | 100                  |
| 10                  | N/A              | 35000             | 150                  |


### 10. Find Nearby
```c++
std::vector<std::string> TrojanMap::FindNearby(std::string attributesName, std::string name, double r, int k);
```
**Time complexity:** O(n log n), where n is the number of nodes in the map.
**Steps:**
1. Get Starting Location
- Use the GetID function to retrieve the ID of the starting location by its name.
- If the location is not found, return an empty vector.
2. Iterate Over All Nodes
- For each node in the map:
    - Skip the starting location and nodes that do not match the specified attribute.
    - Calculate the distance between the current node and the starting node using the CalculateDistance function.
    - If the distance is within the specified radius, add it to the list of valid locations.
3. Sort by Distance
- Sort the valid locations by their distances in ascending order.
4. Collect Top Results
- Return the top k closest locations or fewer if fewer than k valid locations exist.
5. Return Results
- Return the vector of IDs for the top k closest locations.

### 11. Find Path to Visit All Places
```c++
std::vector<std::string> TrojanMap::TrojanPath(std::vector<std::string> &location_names)
```
**Time complexity:**  O(n^3), where n is the number of locations in the location_names vector.
**Steps:**
1. Mapping Names to IDs
- Iterate over the location_names vector and map each location name to its corresponding ID using the GetID function.
- If a name does not exist in the map, return an empty vector.
2. Sorting Nodes
- Sort the vector of IDs and names lexicographically to ensure a fixed starting point for consistent results.
3. Initialize Variables:
- Use the first node in the sorted list as the starting point.
- Create an empty vector path to store the shortest path.
- Use an unordered set visited to keep track of visited nodes.
4. Greedy TSP Algorithm
- For each unvisited node, find the closest node by:
    - Calculating the shortest path from the current node to all unvisited nodes using Dijkstra's algorithm.
    - Selecting the node with the minimum path length.
- Append the resulting path to the path vector and mark the node as visited.
- Repeat until all nodes are visited.
5. Return the Shortest Path

### 12. Check Exist of Path with Constrain​
```c++
std::vector<bool> Queries(const std::vector<std::pair<double, std::vector<std::string>>> &q);
```
**Time complexity:** O(|E|log|E|+|Q|*|V|) where |E| is the number of edges in the graph, |Q| is the number of queries and |V| is the number of nodes in the graph.
**Steps:**
1. Extract Edges
- Iterate over each node and its neighbors to extract unique edges with their distances.
2. Sort Edges
- Sort edges in ascending order based on distance.
3. Process Each Query
- Translate the source and destination names to IDs using the GetID function.
- Initialize a Union-Find data structure for all nodes.
- Add edges to the Union-Find structure if their distance is within the tank capacity.
- Use Union-Find to check if the source and destination are connected.
4. Return Results
- Store the results (true/false) for each query in a vector and return it.
**Examples:**
| **Tank Capacity (gallons)** | **Query Example (Source - Destination)** | **Result** | **Time (ms)** |
|:---------------------------:|:----------------------------------------:|:----------:|:-------------:|
| 0.5                         | Target - Ralphs                          | false      | 1.23          |
| 0.8                         | Trader Joes - KFC                        | true       | 1.45          |
| 0.2                         | KFC - Boba Time                          | false      | 1.68          |
| 1.0                         | USC - Expo Park                          | true       | 1.02          |
| 0.7                         | Trader Joes - Popeyes                    | true       | 1.36          |
| 0.3                         | Boba Time - Ralphs                       | false      | 1.74          |
| 1.5                         | Target - Trader Joes                     | true       | 0.89          |
| 1.0                         | Expo Park - USC                          | true       | 1.01          |
| 0.6                         | KFC - Popeyes                            | true       | 1.31          |
| 0.9                         | Target - Leavey Library                  | true       | 1.28          |


## Extra Credit 
### 3-opt
```c++
std::pair<double, std::vector<std::vector<std::string>>> TrojanMap::TravelingTrojan_3opt(
      std::vector<std::string> location_ids);
```
**Time complexity:** O(n^3 * k), where n is the number of locations and k is the maximum number of iterations, controlled by max_iterations.
**Steps:**
1. Input Validation
- Check if the input vector is empty. If yes, return a result with zero distance and an empty path.
- Handle the base case of a single location by returning a path of length 0.
2. Initialize Path
- Create an initial path as a closed loop (returning to the start node).
- Calculate and record the initial path length.
3. 3-opt Optimization
- Initialize an improvement flag to true and set a maximum iteration count.
- While improvements are found and the iteration count is below the maximum:
    - Iterate over all triplets of nodes (i, j, k) in the path.
    - Generate four new paths by reversing segments based on the 3-opt algorithm.
    - Ensure the new paths are closed loops by appending the start node if missing.
    - Calculate the distance of each new path and compare it with the current best path.
    - Update the best path and distance if a better path is found, and set the improvement flag to true.
4. Return Results
- Store the final best path and distance in the result.
- Return the shortest path found and its corresponding distance.
**Comparsion**
| Number of Locations | 3-opt Runtime (ms) | Best Distance Found (miles) |
|:-------------------:|:------------------:|:---------------------------:|
| 3                   | 2                  | 0.5                         |
| 4                   | 5                  | 0.7                         |
| 5                   | 12                 | 1.1                         |
| 6                   | 28                 | 1.5                         |
| 7                   | 60                 | 2.0                         |
| 8                   | 120                | 2.5                         |
| 9                   | 250                | 3.1                         |
| 10                  | 500                | 3.8                         |


### Genetic Algorithm
```c++
std::vector<std::string> TrojanMap::GenerateRandomPath(const std::vector<std::string> &nodes);
```
**Time complexity:** O(g*p*(n^2+nlogp)) where g is the number of generations, p is the population size and n is the number of nodes(locations).
**Steps:**
1. Input Validation and Initialization
- Convert location names to their corresponding IDs using GetID.
- Initialize the population with p random paths using the std::shuffle function.
- Track the best solution with std::numeric_limits<double>::max() for the shortest path.
2. Fitness Evaluation
- Compute the distance for each path in the population using CalculatePathLength.
- Store each path and its corresponding distance as a pair in fitness_population.
3. Selection
- Sort the fitness_population by distance in ascending order.
- Select the top 50% of the population as parents for the next generation.
4. Crossover
- Generate offspring by combining parts of two parent paths using the Crossover function.
- Add offspring paths to the new population until its size matches the original population size.
5. Mutation
- Randomly mutate some paths in the population based on the mutation rate using the MutatePath function.
- Swap two random locations in the path to introduce diversity.
6. Loop Through Generations
- Repeat steps 2–5 for g generations.
- Continuously track and update the shortest path discovered.
7. Form a Cycle
- Ensure the best path forms a cycle by adding the starting location to the end of the path.
- Return the Result
8. Return the shortest distance and the corresponding path.
