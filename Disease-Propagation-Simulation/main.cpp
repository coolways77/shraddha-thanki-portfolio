#include <iostream>
#include "simulation.h"

int main() {
    std::cout << "Loading Disease Simulation..." << std::endl;
    Simulation sim("./configExample/disease_in.ini");
    sim.runHerImmunityTest();
    return 0;
}
