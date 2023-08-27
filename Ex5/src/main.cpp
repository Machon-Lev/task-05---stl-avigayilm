﻿#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <map>
using namespace std;

// City structure to store city information
struct City {
    string name;
    double y;
    double x;
};

// Function to calculate distances using different norms
double calculateDistance(const City& city1, const City& city2, int norm) {
    double dx = abs(city1.x - city2.x);
    double dy = abs(city1.y - city2.y);

    if (norm == 0) return std::sqrt(dx * dx + dy * dy);      // Euclidean distance
    else if (norm == 1) return std::max(dx, dy);              // Chebyshev distance
    else if (norm == 2) return dx + dy;                       // Manhattan distance
    else throw std::invalid_argument("Invalid norm value.");
}

vector<City> readData(string file_name)
{
    vector<City> cities;
    ifstream inputFile(file_name);
    if (!inputFile) {
        std::cerr << "Error: Could not open input file." << std::endl;
        return cities;
    }
    string line;

    while (std::getline(inputFile, line)) {
        City city;
        city.name = line;

        if (std::getline(inputFile, line)) {
            std::istringstream iss(line);
            iss >> city.y; // Read y coordinate
            iss.ignore(); // Ignore the space
            iss.ignore(); // Ignore the hyphen
            iss.ignore(); // Ignore the space
            iss >> city.x; // Read x coordinate
            cities.push_back(city);
        }
    }
    return cities;
}

int main()
{
    vector<City> cities;
    cities = readData("data.txt");

    std::cout << "Welcome to the city search program!" << std::endl;

    while (true) {
        cout << "Please enter selected city name (with line break after it):" << std::endl;
        string cityName;
        getline(cin, cityName);

        if (cityName == "0") {
            std::cout << "Bye" << std::endl;
            break;
        }

        auto cityIterator = std::find_if(cities.begin(), cities.end(),
            [&](const City& city) { return city.name == cityName; });

        if (cityIterator == cities.end()) {
            std::cout << "ERROR: \"" << cityName << "\" isn't found in the city list. Please try again." << std::endl;
            continue;
        }

        City selectedCity = *cityIterator;

        std::cout << "Please enter the wanted radius:" << std::endl;
        double radius;
        std::cin >> radius;

        if (radius < 0) {
            std::cout << "ERROR: Invalid radius. Please try again." << std::endl;
            continue;
        }

        std::cout << "Please enter the desired norm (0 – L2, 1 – Linf, 2 – L1):" << std::endl;
        int norm;
        std::cin >> norm;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear newline from input

        // Create a multimap to store distances and corresponding cities
        multimap<double, string> distancesToCities;

        // Calculate and insert distances into the multimap
        for (const City& city : cities) {
            if (city.name != selectedCity.name) {
                double distance = calculateDistance(selectedCity, city, norm);
                if (distance <= radius) {
                    distancesToCities.insert(std::make_pair(distance, city.name));
                }
            }
        }

        // Output cities within the radius and norm using multimap
        std::cout << "Search result:" << std::endl;
        std::cout << distancesToCities.size() << " city/cities found in the given radius." << std::endl;

        int northCities = 0;
        for (const auto& entry : distancesToCities) {
            const City& city = *std::find_if(cities.begin(), cities.end(),
                [&](const City& c) { return c.name == entry.second; });

            if (city.y < selectedCity.y) {
                northCities++;
            }
        }

        std::cout << northCities << " cities are to the north of the selected city." << std::endl;

        std::cout << "City list:" << std::endl;
        for (const auto& entry : distancesToCities) {
            std::cout << entry.second << std::endl;
        }
    }

    return 0;
}
