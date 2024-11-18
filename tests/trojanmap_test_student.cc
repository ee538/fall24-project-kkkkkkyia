#include <vector>
#include <unordered_set>
#include "gtest/gtest.h"
#include "src/lib/trojanmap.h"

// Defined nodes
TrojanMap InitializeMap() {
    TrojanMap map;
    // Create nodes
    Node n1, n2, n3, n4, n5, n6;
    
    n1.id = "1";
    n1.name = "Chick-fil-A";
    n1.lat = 34.0167334;
    n1.lon = -118.2825307;
    n1.attributes = {"restaurant", "fast food"};
    n1.neighbors = {"2", "5"};

    n2.id = "2";
    n2.name = "Chipotle";
    n2.lat = 34.067;
    n2.lon = -118.444;
    n2.attributes = {"restaurant", "mexican"};
    n2.neighbors = {"4", "1"};

    n3.id = "3";
    n3.name = "Chinese Street Food";
    n3.lat = 34.0219;
    n3.lon = -118.4814;
    n3.attributes = {"restaurant", "asian"};
    n3.neighbors = {"4"};

    n4.id = "4";
    n4.name = "Burger King";
    n4.lat = 34.0522;
    n4.lon = -118.2437;
    n4.attributes = {"restaurant", "fast food"};
    n4.neighbors = {"2", "5"};

    n5.id = "5";
    n5.name = "Target";
    n5.lat = 34.0257016;
    n5.lon = -118.2843512;
    n5.attributes = {"retail", "shopping"};
    n5.neighbors = {"4", "1"};

    n6.id = "6";
    n6.name = "Starbucks";
    n6.lat = 34.0162;
    n6.lon = -118.2827;
    n6.attributes = {"cafe", "coffee"};
    n6.neighbors = {};

    // Add nodes to map
    map.data = {
        {"1", n1},
        {"2", n2},
        {"3", n3},
        {"4", n4},
        {"5", n5},
        {"6", n6}
    };
    return map;
}


// -----------------Phase 1--------------------------------------------------------------------------
// Autocomplete Tests
// Case 1: Empty Input
TEST(TrojanMapStudentTest, AutocompleteEmptyInput) {
    TrojanMap map;
    auto result = map.Autocomplete("");
    EXPECT_TRUE(result.empty());  // Expect empty result for empty input
}

// Case 2: Prefix Matching Multiple Locations
TEST(TrojanMapStudentTest, AutocompletePrefixMatchingMultipleLocations) {
    TrojanMap map;
    // Create nodes
    Node n1, n2, n3;
    n1.id = "1";
    n1.name = "Chick-fil-A";
    n1.lat = 34.0167334;
    n1.lon = -118.2825307;

    n2.id = "2";
    n2.name = "Chipotle";
    n2.lat = 34.067;
    n2.lon = -118.444;

    n3.id = "3";
    n3.name = "Chinese Street Food";
    n3.lat = 34.0219;
    n3.lon = -118.4814;

    // Add nodes to map
    map.data = {
        {"1", n1},
        {"2", n2},
        {"3", n3}
    };

    auto result = map.Autocomplete("Chi");
    std::vector<std::string> expected = {"Chick-fil-A", "Chipotle", "Chinese Street Food"};
    
    // Sort results to avoid order dependency in comparison
    std::sort(result.begin(), result.end());
    std::sort(expected.begin(), expected.end());

    EXPECT_EQ(result, expected);  // Expect these three results
}

// Case 3: Prefix Matching Single Location
TEST(TrojanMapStudentTest, AutocompletePrefixMatchingSingleLocation) {
    TrojanMap map;
    // Create nodes
    Node n1, n2;
    n1.id = "1";
    n1.name = "Pizza Hut";
    n1.lat = 34.052235;
    n1.lon = -118.243683;

    n2.id = "2";
    n2.name = "Panda Express";
    n2.lat = 34.0522;
    n2.lon = -118.2437;

    // Add nodes to map
    map.data = {
        {"1", n1},
        {"2", n2}
    };

    auto result = map.Autocomplete("Pizza");
    std::vector<std::string> expected = {"Pizza Hut"};
    EXPECT_EQ(result, expected);  // Expect only "Pizza Hut"
}

// Case 4: Prefix Not Matching Any Location
TEST(TrojanMapStudentTest, AutocompletePrefixNotMatchingAnyLocation) {
    TrojanMap map;
    // Create nodes
    Node n1, n2;
    n1.id = "1";
    n1.name = "Chick-fil-A";
    n1.lat = 34.0167334;
    n1.lon = -118.2825307;

    n2.id = "2";
    n2.name = "Chipotle";
    n2.lat = 34.067;
    n2.lon = -118.444;

    // Add nodes to map
    map.data = {
        {"1", n1},
        {"2", n2}
    };

    auto result = map.Autocomplete("Sushi");
    EXPECT_TRUE(result.empty());  // Expect no results for "Sushi"
}

// Case 5: Case Insensitivity
TEST(TrojanMapStudentTest, AutocompleteCaseInsensitivity) {
    TrojanMap map;
    // Create nodes
    Node n1, n2, n3;
    n1.id = "1";
    n1.name = "Chick-fil-A";
    n1.lat = 34.0167334;
    n1.lon = -118.2825307;

    n2.id = "2";
    n2.name = "Chipotle";
    n2.lat = 34.067;
    n2.lon = -118.444;

    n3.id = "3";
    n3.name = "Chinese Street Food";
    n3.lat = 34.0219;
    n3.lon = -118.4814;

    // Add nodes to map
    map.data = {
        {"1", n1},
        {"2", n2},
        {"3", n3}
    };

    auto result = map.Autocomplete("chi");
    std::vector<std::string> expected = {"Chick-fil-A", "Chipotle", "Chinese Street Food"};

    // Sort results to avoid order dependency in comparison
    std::sort(result.begin(), result.end());
    std::sort(expected.begin(), expected.end());

    EXPECT_EQ(result, expected);  // Case-insensitive match for "chi"
}

// GetPosition Tests

// Case 1: Location Exists
TEST(TrojanMapStudentTest, GetPositionLocationExists) {
    TrojanMap map;
    // Create nodes
    Node n1;
    n1.id = "1";
    n1.name = "Target";
    n1.lat = 34.0257016;
    n1.lon = -118.2843512;

    // Add nodes to map
    map.data = {
        {"1", n1}
    };

    auto result = map.GetPosition("Target");
    std::pair<double, double> expected = {34.0257016, -118.2843512};
    EXPECT_EQ(result, expected);  // Expect correct latitude and longitude for "Target"
}

// Case 2: Location Does Not Exist
TEST(TrojanMapStudentTest, GetPositionLocationDoesNotExist) {
    TrojanMap map;
    // Create nodes
    Node n1;
    n1.id = "1";
    n1.name = "Chick-fil-A";
    n1.lat = 34.0167334;
    n1.lon = -118.2825307;

    // Add nodes to map
    map.data = {
        {"1", n1}
    };

    auto result = map.GetPosition("Nonexistent");
    std::pair<double, double> expected = {-1, -1};
    EXPECT_EQ(result, expected);  // Expect (-1, -1) for a location that doesn't exist
}

// Case 3: Case Sensitivity
TEST(TrojanMapStudentTest, GetPositionCaseSensitivity) {
    TrojanMap map;
    // Create nodes
    Node n1;
    n1.id = "1";
    n1.name = "Target";
    n1.lat = 34.0257016;
    n1.lon = -118.2843512;

    // Add nodes to map
    map.data = {
        {"1", n1}
    };

    auto result = map.GetPosition("target");  // Different case
    std::pair<double, double> expected = {-1, -1};
    EXPECT_EQ(result, expected);  // Expect (-1, -1) for lowercase input when the name is case-sensitive

    result = map.GetPosition("Target");  // Correct case
    expected = {34.0257016, -118.2843512};
    EXPECT_EQ(result, expected);  // Expect correct latitude and longitude for "Target"
}

// Case 4: Exact Match
TEST(TrojanMapStudentTest, GetPositionExactMatch) {
    TrojanMap map;
    // Create nodes
    Node n1;
    n1.id = "1";
    n1.name = "Pizza Hut";
    n1.lat = 34.052235;
    n1.lon = -118.243683;

    // Add nodes to map
    map.data = {
        {"1", n1}
    };

    auto result = map.GetPosition("Pizza Hut");
    std::pair<double, double> expected = {34.052235, -118.243683};
    EXPECT_EQ(result, expected);  // Exact match for "Pizza Hut"
}

// Case 5: Empty Input
TEST(TrojanMapStudentTest, GetPositionEmptyInput) {
    TrojanMap map;
    // Create nodes
    Node n1;
    n1.id = "1";
    n1.name = "Chick-fil-A";
    n1.lat = 34.0167334;
    n1.lon = -118.2825307;

    // Add nodes to map
    map.data = {
        {"1", n1}
    };

    auto result = map.GetPosition("");
    std::pair<double, double> expected = {-1, -1};
    EXPECT_EQ(result, expected);  // Expect (-1, -1) for an empty input string
}
// ------------------------------Phase 2-------------------------------------------------------------
// item 2-2: calculate edit distance / find closest name
// Test for CalculateEditDistance function
TEST(TrojanMapTest, CalculateEditDistance) {
  TrojanMap map;
  EXPECT_EQ(map.CalculateEditDistance("horse", "ros"), 3);
  EXPECT_EQ(map.CalculateEditDistance("ralphs", "rolphs"), 1);
}
// Test for FindClosestName function
TEST(TrojanMapTest, FindClosestName) {
  TrojanMap map;
  EXPECT_EQ(map.FindClosestName("Rolphs"), "Ralphs");
  EXPECT_EQ(map.FindClosestName("Targeety"), "Target");
  EXPECT_EQ(map.FindClosestName("Dulle"), "Dulce");
}

// item 3: get all categories
// Case 1: empty map
TEST(TrojanMapTest, GetAllCategories_EmptyMap) {
    TrojanMap map;
    // Clear preloaded data
    map.data.clear();
    auto categories = map.GetAllCategories();
    EXPECT_TRUE(categories.empty());
}
// Case 2: single node with one category
TEST(TrojanMapTest, GetAllCategories_SingleNodeOneCategory) {
    TrojanMap map;
    map.data.clear();
    Node node;
    node.attributes = {"Food"};
    map.data["1"] = node;

    auto categories = map.GetAllCategories();
    EXPECT_EQ(categories.size(), 1);
    EXPECT_EQ(categories[0], "Food");
}
// Case 3: Multiple Nodes with Unique Categories
TEST(TrojanMapTest, GetAllCategories_MultipleNodesWithUniqueCategories) {
    TrojanMap map;
    map.data.clear();

    Node node1;
    node1.attributes = {"Grocery"};
    map.data["1"] = node1;

    Node node2;
    node2.attributes = {"Pharmacy"};
    map.data["2"] = node2;

    Node node3;
    node3.attributes = {"Gym"};
    map.data["3"] = node3;

    auto categories = map.GetAllCategories();
    EXPECT_EQ(categories.size(), 3);
    EXPECT_TRUE(std::find(categories.begin(), categories.end(), "Grocery") != categories.end());
    EXPECT_TRUE(std::find(categories.begin(), categories.end(), "Pharmacy") != categories.end());
    EXPECT_TRUE(std::find(categories.begin(), categories.end(), "Gym") != categories.end());
}

// Item 4: GetAllLocationsFromCategory Tests
// Case 1: Category Exists with Multiple Matches
TEST(TrojanMapStudentTest, GetAllLocationsFromCategoryMultipleMatches) {
    TrojanMap map = InitializeMap();
    auto result = map.GetAllLocationsFromCategory("restaurant");
    std::vector<std::string> expected = {"1", "2", "3", "4"};
    
    // Sort result to avoid order dependency in comparison
    std::sort(result.begin(), result.end());
    std::sort(expected.begin(), expected.end());

    EXPECT_EQ(result, expected);  // Expect multiple restaurant locations
}

// Case 2: Category Exists with a Single Match
TEST(TrojanMapStudentTest, GetAllLocationsFromCategorySingleMatch) {
    TrojanMap map = InitializeMap();
    auto result = map.GetAllLocationsFromCategory("shopping");
    std::vector<std::string> expected = {"5"};
    EXPECT_EQ(result, expected);  // Expect only "5" to be returned
}

// Case 3: Category Does Not Exist
TEST(TrojanMapStudentTest, GetAllLocationsFromCategoryNonExistent) {
    TrojanMap map = InitializeMap();
    auto result = map.GetAllLocationsFromCategory("bank");
    EXPECT_TRUE(result.empty());  // Expect no results for non-existent category "bank"
}

// Item 5: GetLocationRegex Tests
// Case 1: Valid regex that matches multiple locations
TEST(TrojanMapStudentTest, GetLocationRegexMultipleMatches) {
    TrojanMap map = InitializeMap();
    std::regex reg1("Chi.*");
    auto result = map.GetLocationRegex(reg1);
    std::vector<std::string> expected = {"1", "2", "3"};
    // Sort result to avoid order dependency in comparison
    std::sort(result.begin(), result.end());
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(result, expected);  // Expect IDs of locations starting with "Chi"
}
// Case 2: Valid regex that matches no locations
TEST(TrojanMapStudentTest, GetLocationRegexNoMatch) {
    TrojanMap map = InitializeMap();
    std::regex reg2("Taco.*");
    auto result = map.GetLocationRegex(reg2);
    EXPECT_TRUE(result.empty());  // Expect no results for "Taco.*"
}
// Case 3: Case insensitive matching
TEST(TrojanMapStudentTest, GetLocationRegexCaseInsensitive) {
    TrojanMap map = InitializeMap();
    std::regex reg3("burger king", std::regex_constants::icase);
    auto result = map.GetLocationRegex(reg3);
    std::vector<std::string> expected = {"4"};
    EXPECT_EQ(result, expected);  // Case-insensitive match for "Burger King"
}

// Item 6: CalculateShortestPath Tests
// Helper function to print paths for debugging
void PrintPath(const std::string& algorithm, const std::vector<std::string>& path) {
    std::cout << algorithm << " path: ";
    for (const auto& node : path) {
        std::cout << node << " ";
    }
    std::cout << std::endl;
}
// Case 1: Shortest path
TEST(TrojanMapTest, CalculateShortestPathDijkstra) {
    TrojanMap map = InitializeMap();

    // Test Dijkstra's Algorithm
    auto dijkstra_path = map.CalculateShortestPath_Dijkstra("Chick-fil-A", "Burger King");
    std::vector<std::string> expected_dijkstra_path_1 = {"1", "2", "4"};
    std::vector<std::string> expected_dijkstra_path_2 = {"1", "5", "4"};
    // Debugging output
    PrintPath("Dijkstra", dijkstra_path);
    EXPECT_TRUE(dijkstra_path == expected_dijkstra_path_1 || dijkstra_path == expected_dijkstra_path_2);

    // Test Bellman-Ford Algorithm
    auto bellman_ford_path = map.CalculateShortestPath_Bellman_Ford("Chick-fil-A", "Burger King");
    // Debugging output
    PrintPath("Bellman-Ford", bellman_ford_path);
    EXPECT_TRUE(bellman_ford_path == expected_dijkstra_path_1 || bellman_ford_path == expected_dijkstra_path_2);
}
// Case 2: No path available
TEST(TrojanMapTest, NoPathAvailable) {
    TrojanMap map = InitializeMap();

    // Remove connections to make no path available
    map.data["1"].neighbors = {};
    map.data["2"].neighbors = {};
    map.data["3"].neighbors = {};

    // Test Dijkstra's Algorithm
    auto path_dijkstra = map.CalculateShortestPath_Dijkstra("Chick-fil-A", "Burger King");
    EXPECT_TRUE(path_dijkstra.empty()); 

    // Test Bellman-Ford Algorithm
    auto path_bellman_ford = map.CalculateShortestPath_Bellman_Ford("Chick-fil-A", "Burger King");
    EXPECT_TRUE(path_bellman_ford.empty());
}
// Case 3: Invalid input locations
TEST(TrojanMapTest, InvalidLocations) {
    TrojanMap map = InitializeMap();

    // Test Dijkstra's Algorithm with invalid location
    auto path_dijkstra = map.CalculateShortestPath_Dijkstra("Invalid", "Burger King");
    EXPECT_TRUE(path_dijkstra.empty());

    // Test Bellman-Ford Algorithm with invalid location
    auto path_bellman_ford = map.CalculateShortestPath_Bellman_Ford("Chick-fil-A", "Invalid");
    EXPECT_TRUE(path_bellman_ford.empty());  
}
// Case 4: Different valid paths
TEST(TrojanMapTest, DifferentValidPaths) {
    TrojanMap map = InitializeMap();

    // Test Dijkstra's Algorithm
    auto path_dijkstra = map.CalculateShortestPath_Dijkstra("Chick-fil-A", "Burger King");
    std::vector<std::string> expected_path_1 = {"1", "2", "4"};
    std::vector<std::string> expected_path_2 = {"1", "5", "4"};

    EXPECT_TRUE(path_dijkstra == expected_path_1 || path_dijkstra == expected_path_2);

    // Test Bellman-Ford Algorithm
    auto path_bellman_ford = map.CalculateShortestPath_Bellman_Ford("Chick-fil-A", "Burger King");
    EXPECT_TRUE(path_bellman_ford == expected_path_1 || path_bellman_ford == expected_path_2);
}
// Test consistency between Dijkstra and Bellman-Ford
TEST(TrojanMapTest, CompareDijkstraAndBellmanFord) {
    TrojanMap map = InitializeMap();

    // Calculate paths using both algorithms
    auto dijkstra_path = map.CalculateShortestPath_Dijkstra("Chick-fil-A", "Burger King");
    auto bellman_ford_path = map.CalculateShortestPath_Bellman_Ford("Chick-fil-A", "Burger King");
    // Debugging output
    PrintPath("Dijkstra", dijkstra_path);
    PrintPath("Bellman-Ford", bellman_ford_path);
    // Both algorithms should find the same path
    EXPECT_EQ(dijkstra_path, bellman_ford_path);
}


// item 7: cycle detection
TEST(TrojanMapTest, CycleDetection) {
  TrojanMap map;
  
  // Test case 1
  std::vector<double> square1 = {-118.299, -118.264, 34.032, 34.011};
  auto sub1 = map.GetSubgraph(square1);
  bool result1 = map.CycleDetection(sub1, square1);
  EXPECT_EQ(result1, true);

  // Test case 2
  std::vector<double> square2 = {-118.290, -118.289, 34.030, 34.020};
  auto sub2 = map.GetSubgraph(square2);
  bool result2 = map.CycleDetection(sub2, square2);
  EXPECT_EQ(result2, false);

  // Test case 3
  std::vector<double> square3 = {-118.290, -117.289, 34.130, 34.020};
  auto sub3 = map.GetSubgraph(square3);
  bool result3 = map.CycleDetection(sub3, square3);
  EXPECT_EQ(result3, true);
}

//item 8: topological sort
// Case 1: Valid topological sort
TEST(TrojanMapTest, TopologicalSortValid) {
    TrojanMap map = InitializeMap();
    std::vector<std::string> location_names = {"Ralphs", "Chick-fil-A", "KFC"};
    std::vector<std::vector<std::string>> dependencies = {{"Ralphs", "KFC"}, {"Ralphs", "Chick-fil-A"}, {"Chick-fil-A", "KFC"}};
    auto result = map.DeliveringTrojan(location_names, dependencies);
    std::vector<std::string> expected = {"Ralphs", "Chick-fil-A", "KFC"};
    EXPECT_EQ(result, expected);
}
// Case 2: Larger valid topological sort
TEST(TrojanMapTest, TopologicalSortLargerValid) {
    TrojanMap map = InitializeMap();
    std::vector<std::string> location_names = {"Ralphs", "Chick-fil-A", "KFC", "Target", "Walmart"};
    std::vector<std::vector<std::string>> dependencies = {
        {"Ralphs", "KFC"}, {"Ralphs", "Chick-fil-A"}, {"Chick-fil-A", "KFC"}, {"KFC", "Target"}, {"Target", "Walmart"}};
    auto result = map.DeliveringTrojan(location_names, dependencies);
    std::vector<std::string> expected = {"Ralphs", "Chick-fil-A", "KFC", "Target", "Walmart"};
    EXPECT_EQ(result, expected);
}
// Case 3: Graph with a cycle
TEST(TrojanMapTest, TopologicalSortCycle) {
    TrojanMap map = InitializeMap();
    std::vector<std::string> location_names = {"Ralphs", "Chick-fil-A", "KFC"};
    std::vector<std::vector<std::string>> dependencies = {{"Ralphs", "KFC"}, {"KFC", "Chick-fil-A"}, {"Chick-fil-A", "Ralphs"}};
    auto result = map.DeliveringTrojan(location_names, dependencies);
    EXPECT_TRUE(result.empty());  // Expect empty vector since there's a cycle
}
// Case 4: Independent nodes
TEST(TrojanMapTest, TopologicalSortIndependentNodes) {
    TrojanMap map = InitializeMap();
    std::vector<std::string> location_names = {"Ralphs", "Chick-fil-A", "KFC"};
    std::vector<std::vector<std::string>> dependencies = {};  // No dependencies
    auto result = map.DeliveringTrojan(location_names, dependencies);
    // Any permutation of the nodes is valid
    std::sort(result.begin(), result.end());
    std::vector<std::string> expected = {"Ralphs", "Chick-fil-A", "KFC"};
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(result, expected);
}
// Case 5: Node depending on itself
TEST(TrojanMapTest, TopologicalSortSelfDependency) {
    TrojanMap map = InitializeMap();
    std::vector<std::string> location_names = {"Ralphs", "Chick-fil-A", "KFC"};
    std::vector<std::vector<std::string>> dependencies = {{"Ralphs", "Ralphs"}};
    auto result = map.DeliveringTrojan(location_names, dependencies);
    EXPECT_TRUE(result.empty());  // Expect empty vector since a node depends on itself
}
// Case 6: Multiple valid orders
TEST(TrojanMapTest, TopologicalSortMultipleValidOrders) {
    TrojanMap map = InitializeMap();
    std::vector<std::string> location_names = {"Ralphs", "Chick-fil-A", "KFC", "Target"};
    std::vector<std::vector<std::string>> dependencies = {{"Ralphs", "KFC"}, {"Chick-fil-A", "Target"}};
    auto result = map.DeliveringTrojan(location_names, dependencies);
    // Check if the result contains the correct order
    auto ralphs_pos = std::find(result.begin(), result.end(), "Ralphs");
    auto kfc_pos = std::find(result.begin(), result.end(), "KFC");
    auto chickfila_pos = std::find(result.begin(), result.end(), "Chick-fil-A");
    auto target_pos = std::find(result.begin(), result.end(), "Target");
    EXPECT_TRUE(ralphs_pos != result.end());
    EXPECT_TRUE(kfc_pos != result.end());
    EXPECT_TRUE(chickfila_pos != result.end());
    EXPECT_TRUE(target_pos != result.end());
    EXPECT_LT(ralphs_pos, kfc_pos);  // Ralphs should be before KFC
    EXPECT_LT(chickfila_pos, target_pos);  // Chick-fil-A should be before Target
}
// Case 7: Empty input
TEST(TrojanMapTest, TopologicalSortEmptyInput) {
    TrojanMap map = InitializeMap();
    std::vector<std::string> location_names = {};
    std::vector<std::vector<std::string>> dependencies = {};
    auto result = map.DeliveringTrojan(location_names, dependencies);
    EXPECT_TRUE(result.empty());  // Expect empty vector since input is empty
}

