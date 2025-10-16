#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <string>
#include <random>
#include <fstream>

// Enum to represent the status of a person
enum class Status {
    Susceptible, Infectious, Recovered, Vaccinated
};

// Class representing a person in the population
class Person {
public:
    Status status;
    int daysInfectious = 0;

    Person(Status status = Status::Susceptible) : status(status) {}
};

// Class representing the disease characteristics
class Disease {
public:
    std::string name;
    double transmissibility;
    int duration;

    Disease(std::string name = "Unnamed", double trans = 0.1, int dur = 5)
        : name(name), transmissibility(trans), duration(dur) {}
};

// Class representing a population affected by the disease
class Population {
public:
    std::string name;
    std::vector<Person> people;
    int size;
    double vaccination_rate;
    bool patient_zero;

    Population(std::string name, int size, double vaccination_rate, bool patient_zero);
};

// Class to simulate the spread of the disease in multiple populations
class Simulation {
public:
    std::vector<Population> populations;
    Disease disease;
    std::string input_file;

    Simulation(std::string in_file = "disease_in.ini");
    void runHerImmunityTest();
    void simulatePopulation(Population& pop, std::ofstream& detailFile);
    void outputResults(const Population& pop);
};

#endif // SIMULATION_H
