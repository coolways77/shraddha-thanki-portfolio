
# Disease Propagation Simulation (C++)

This project simulates the spread of a disease through a population using the **SIR (Susceptible–Infected–Recovered)** model.  
It examines how varying vaccination rates impact the spread and illustrates when herd immunity is attained.

---

## 🧩 Overview
- Each individual in the population is modeled and updated daily.  
- Infection probability, duration, and vaccination rate are configurable.  
- Outputs show how infections and recoveries evolve over time.  
- The simulation compares results with theoretical herd immunity thresholds.

---

## ⚙️ Technologies
- **Language:** C++  
- **Concepts:** Loops, arrays, randomization, and condition-based state transitions  
- **Visualization:** Data exported and plotted in Python/Matplotlib (optional)

---

## ▶️ Run Instructions
Compile and run the simulation using:
```bash
g++ Main.cpp -o disease_simulation
./disease_simulation
