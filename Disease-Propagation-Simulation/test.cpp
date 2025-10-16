#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../include/doctest.h"
#include "simulation.h"

// Test cases for the Disease class
TEST_CASE("Test Disease class") {
    Disease disease("Corona", 0.4, 3);
    CHECK(disease.name == "Corona");
    CHECK(disease.duration == 3);
    CHECK(disease.transmissibility == 0.4);
}

// Test cases for the Person class
TEST_CASE("Test Person class") {
    Person person(Status::Susceptible);
    CHECK(person.status == Status::Susceptible);
    person.status = Status::Infectious;
    CHECK(person.status == Status::Infectious);
    CHECK(person.daysInfectious == 0);
}

// Test cases for the Population class
TEST_CASE("Test Population class") {
    Population population("TestPopulation", 1000, 0.5, true);
    CHECK(population.name == "TestPopulation");
    CHECK(population.size == 1000);
    CHECK(population.vaccination_rate == 0.5);
    CHECK(population.patient_zero == true);
    
    int vaccinated_count = 0;
    for (const auto& person : population.people) {
        if (person.status == Status::Vaccinated) {
            vaccinated_count++;
        }
    }
    CHECK(vaccinated_count > 0);  // Ensure some people are vaccinated
}

// Test cases for the Simulation class
TEST_CASE("Test Simulation class") {
    Simulation sim("configExample/disease_in.ini");
    CHECK(sim.input_file == "configExample/disease_in.ini");
    sim.disease = Disease("Corona", 0.4, 3);
    CHECK(sim.disease.name == "Corona");
    CHECK(sim.disease.duration == 3);
    CHECK(sim.disease.transmissibility == 0.4);
    CHECK(sim.populations.size() > 0);
}

// Additional Tests for edge cases

// Test Disease with zero transmissibility
TEST_CASE("Test Disease with zero transmissibility") {
    Disease disease("Corona", 0.0, 3);
    CHECK(disease.name == "Corona");
    CHECK(disease.duration == 3);
    CHECK(disease.transmissibility == 0.0);
}

// Test Disease with maximum transmissibility
TEST_CASE("Test Disease with maximum transmissibility") {
    Disease disease("Corona", 1.0, 3);
    CHECK(disease.name == "Corona");
    CHECK(disease.duration == 3);
    CHECK(disease.transmissibility == 1.0);
}

// Test Population with zero size
TEST_CASE("Test Population with zero size") {
    Population population("EmptyPopulation", 0, 0.5, true);
    CHECK(population.name == "EmptyPopulation");
    CHECK(population.size == 0);
    CHECK(population.people.size() == 0);
}

// Test Population with 100% vaccination rate
TEST_CASE("Test Population with 100% vaccination rate") {
    Population population("VaccinatedPopulation", 1000, 1.0, false);
    CHECK(population.name == "VaccinatedPopulation");
    int vaccinated_count = 0;
    for (const auto& person : population.people) {
        if (person.status == Status::Vaccinated) {
            vaccinated_count++;
        }
    }
    CHECK(vaccinated_count == 1000);
}

// Test Population with 0% vaccination rate
TEST_CASE("Test Population with 0% vaccination rate") {
    Population population("NonVaccinatedPopulation", 1000, 0.0, false);
    CHECK(population.name == "NonVaccinatedPopulation");
    int vaccinated_count = 0;
    for (const auto& person : population.people) {
        if (person.status == Status::Vaccinated) {
            vaccinated_count++;
        }
    }
    CHECK(vaccinated_count == 0);
}

