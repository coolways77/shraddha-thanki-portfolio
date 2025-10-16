#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../include/doctest.h"
#include "simulation.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include "INIReader.h"
#include <filesystem> // For C++17 filesystem support

// Integration Test: Full Simulation
TEST_CASE("Integration Test: Full Simulation") {
    // Create a configuration file for the test
    std::ofstream configFile("test_integration.ini");
    configFile << "[global]\n";
    configFile << "num_populations = 2\n";
    configFile << "[disease]\n";
    configFile << "name = Corona\n";
    configFile << "transmissibility = 0.4\n";
    configFile << "duration = 3\n";
    configFile << "[population_1]\n";
    configFile << "name = TestPop1\n";
    configFile << "size = 1000\n";
    configFile << "vaccination_rate = 0.5\n";
    configFile << "patient_zero = true\n";
    configFile << "[population_2]\n";
    configFile << "name = TestPop2\n";
    configFile << "size = 1000\n";
    configFile << "vaccination_rate = 0.3\n";
    configFile << "patient_zero = false\n";
    configFile.close();

    // Run the simulation
    Simulation sim("test_integration.ini");
    sim.runHerImmunityTest();

    // Check the results (modify this based on expected outcomes)
    std::ifstream resultFile("herd_immunity_results.csv");
    std::string line;
    std::vector<std::string> results;
    while (std::getline(resultFile, line)) {
        results.push_back(line);
    }
    resultFile.close();

    CHECK(results.size() > 1); // Ensure there are results
    CHECK(results[0] == "VaccinationRate,Infected,Recovered,Susceptible"); // Check header
}

// Integration Test: Edge Cases
TEST_CASE("Integration Test: Edge Cases") {
    // Create a configuration file for edge cases
    std::ofstream configFile("test_edge_cases.ini");
    configFile << "[global]\n";
    configFile << "num_populations = 1\n";
    configFile << "[disease]\n";
    configFile << "name = TestDisease\n";
    configFile << "transmissibility = 1.0\n";
    configFile << "duration = 1\n";
    configFile << "[population_1]\n";
    configFile << "name = EdgeCasePop\n";
    configFile << "size = 10\n";
    configFile << "vaccination_rate = 0.0\n";
    configFile << "patient_zero = true\n";
    configFile.close();

    // Run the simulation
    Simulation sim("test_edge_cases.ini");
    sim.runHerImmunityTest();

    // Check the results (modify this based on expected outcomes)
    std::ifstream resultFile("herd_immunity_results.csv");
    std::string line;
    std::vector<std::string> results;
    while (std::getline(resultFile, line)) {
        results.push_back(line);
    }
    resultFile.close();

    CHECK(results.size() > 1); // Ensure there are results
    CHECK(results[0] == "VaccinationRate,Infected,Recovered,Susceptible"); // Check header
   
}
