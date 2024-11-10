#include "gtest/gtest.h"
#include "src/lib/trojanmap.h"

TEST(TrojanMapStudentTest, Test1) {
  EXPECT_EQ(true, true);
}


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