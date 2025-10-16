#include "simulation.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include "INIReader.h"
#include <filesystem> // For C++17 filesystem support
#include <stdexcept>
// Constructor for Population
Population::Population(std::string name, int size, double vaccination_rate, bool patient_zero)
    : name(name), size(size), vaccination_rate(vaccination_rate), patient_zero(patient_zero) {
    if (vaccination_rate < 0.0 || vaccination_rate > 1.0) {
        throw std::invalid_argument("Vaccination rate must be between 0 and 1");
    }
    if (size < 0) {
        throw std::invalid_argument("Population size must be non-negative");
    }
    people.resize(size, Person(Status::Susceptible));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution dist(vaccination_rate);

    for (Person& person : people) {
        person.status = dist(gen) ? Status::Vaccinated : Status::Susceptible;
    }

    if (patient_zero) {
        for (Person& person : people) {
            if (person.status == Status::Susceptible) {
                person.status = Status::Infectious;
                break;
            }
        }
    }
}


// Constructor for Simulation
Simulation::Simulation(std::string in_file) : input_file(in_file) {
    // Print the current working directory
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
    
    // Check if the file exists before attempting to load
    if (!std::filesystem::exists(input_file)) {
        std::cerr << "File not found: " << input_file << std::endl;
        exit(1);
    }

    INIReader reader(input_file);
    if (reader.ParseError() < 0) {
        std::cerr << "Error loading INI file " << in_file << std::endl;
        exit(1);
    }
    std::cout << "Loaded INI file " << in_file << " successfully." << std::endl;

    // Initialize disease based on config file
    disease = Disease(reader.Get("disease", "name", "Unknown"),
                      reader.GetReal("disease", "transmissibility", 0.4),
                      reader.GetInteger("disease", "duration", 3));
    std::cout << "Disease parameters: Name = " << disease.name
              << ", Transmissibility = " << disease.transmissibility
              << ", Duration = " << disease.duration << std::endl;

    int num_populations = reader.GetInteger("global", "num_populations", 1);
    std::cout << "Number of populations: " << num_populations << std::endl;
    
    for (int i = 1; i <= num_populations; ++i) {
        std::string section = "population_" + std::to_string(i);
        std::string name = reader.Get(section, "name", "Unknown");
        int size = reader.GetInteger(section, "size", 1000);
        double vaccination_rate = reader.GetReal(section, "vaccination_rate", 0.0);
        bool patient_zero = reader.GetBoolean(section, "patient_zero", false);

        populations.push_back(Population(name, size, vaccination_rate, patient_zero));
        std::cout << "Loaded population " << name
                  << " with size " << size
                  << ", vaccination rate " << vaccination_rate
                  << ", patient zero " << (patient_zero ? "true" : "false") << std::endl;
    }
}

// Method to run herd immunity tests
void Simulation::runHerImmunityTest() {
    std::ofstream resultsFile("herd_immunity_results.csv");
    resultsFile << "VaccinationRate,Infected,Recovered,Susceptible\n";

    for (double vRate = 0.0; vRate <= 1.0; vRate += 0.1) {
        Population testPop("TestPopulation", 15000, vRate, true);
        std::ofstream detailFile("disease_progression_" + std::to_string(int(vRate * 100)) + ".csv");
        detailFile << "Day,Susceptible,Infectious,Recovered,Vaccinated\n";
        
        simulatePopulation(testPop, detailFile);
        outputResults(testPop); // Store or directly write results
        resultsFile << vRate * 100 << "%,"
                    << std::count_if(testPop.people.begin(), testPop.people.end(), [](const Person& p) { return p.status == Status::Infectious; }) << ","
                    << std::count_if(testPop.people.begin(), testPop.people.end(), [](const Person& p) { return p.status == Status::Recovered; }) << ","
                    << std::count_if(testPop.people.begin(), testPop.people.end(), [](const Person& p) { return p.status == Status::Susceptible; }) << "\n";
        detailFile.close();
    }

    resultsFile.close();
}

// Simulate disease spread within a population
void Simulation::simulatePopulation(Population& pop, std::ofstream& detailFile) {
    std::random_device rd;
    std::mt19937 gen(rd());

    int daysSimulated = 0;
    bool hasInfectious;

    do {
        hasInfectious = false;
        std::vector<int> newInfections;

        int susceptible = 0, infectious = 0, recovered = 0, vaccinated = 0;
        for (const Person& person : pop.people) {
            switch (person.status) {
                case Status::Susceptible: susceptible++; break;
                case Status::Infectious: infectious++; break;
                case Status::Recovered: recovered++; break;
                case Status::Vaccinated: vaccinated++; break;
            }
        }
        detailFile << daysSimulated << "," << susceptible << "," << infectious << "," << recovered << "," << vaccinated << "\n";

        for (size_t i = 0; i < pop.people.size(); ++i) {
            if (pop.people[i].status == Status::Infectious) {
                hasInfectious = true;
                pop.people[i].daysInfectious++;
                
                if (pop.people[i].daysInfectious >= disease.duration) {
                    pop.people[i].status = Status::Recovered;
                    continue;
                }
                
                // Infect other people
                for (int contact = 0; contact < 6; ++contact) {
                    int targetIndex = std::uniform_int_distribution<int>(0, pop.people.size() - 1)(gen);
                    if (pop.people[targetIndex].status == Status::Susceptible && std::bernoulli_distribution(disease.transmissibility)(gen)) {
                        newInfections.push_back(targetIndex);
                    }
                }
            }
        }

        // Update newly infected people's status
        for (int index : newInfections) {
            pop.people[index].status = Status::Infectious;
            pop.people[index].daysInfectious = 0;
        }

        daysSimulated++;
    } while (hasInfectious); // Continue until no more infectious people

    std::cout << "Simulation completed for " << pop.name << " in " << daysSimulated << " days." << std::endl;
}

// Output results of the simulation for a population
void Simulation::outputResults(const Population& pop) {
    int susceptible = 0, infectious = 0, recovered = 0, vaccinated = 0;
    for (const Person& person : pop.people) {
        switch (person.status) {
            case Status::Susceptible: susceptible++; break;
            case Status::Infectious: infectious++; break;
            case Status::Recovered: recovered++; break;
            case Status::Vaccinated: vaccinated++; break;
        }
    }
    std::cout << "Results for " << pop.name << ": "
              << "Susceptible: " << susceptible
              << ", Infectious: " << infectious
              << ", Recovered: " << recovered
              << ", Vaccinated: " << vaccinated << std::endl;
}
