import matplotlib.pyplot as plt
import pandas as pd
import os

# File paths
herd_immunity_results_path = 'herd_immunity_results.csv'
disease_progression_path = 'disease_progression_50.csv'  # Example for 50% vaccination rate

# Function to create the disease progression plot
def plot_disease_progression(filepath, vaccination_rate):
    if os.path.exists(filepath):
        progression_data = pd.read_csv(filepath)

        plt.figure(figsize=(14, 8))
        plt.plot(progression_data['Day'], progression_data['Susceptible'], label='Susceptible', color='blue', linewidth=2)
        plt.plot(progression_data['Day'], progression_data['Infectious'], label='Infectious', color='orange', linewidth=2)
        plt.plot(progression_data['Day'], progression_data['Recovered'], label='Recovered', color='green', linewidth=2)
        plt.plot(progression_data['Day'], progression_data['Vaccinated'], label='Vaccinated', color='red', linewidth=2)

        plt.xlabel('Steps', fontsize=14)
        plt.ylabel('Number of Individuals', fontsize=14)
        plt.title(f'Disease Progression over Time for D = 3, C = 6, β = 0.4, V = {vaccination_rate}', fontsize=16)
        plt.legend(loc='upper right', fontsize=12)
        plt.grid(True)
        plt.ylim(0, progression_data[['Susceptible', 'Infectious', 'Recovered', 'Vaccinated']].max().max() * 1.1)
        plt.tight_layout()
        plt.savefig(f'disease_progression_{int(vaccination_rate*100)}_clean.png')
        plt.show()
    else:
        print(f"File not found: {filepath}")

# Function to create the recovered vs vaccination rate plot
def plot_recovered_vs_vaccination_rate(filepath):
    if os.path.exists(filepath):
        herd_data = pd.read_csv(filepath)
        plt.figure(figsize=(14, 8))
        plt.plot(herd_data['VaccinationRate'], herd_data['Recovered'], marker='o', linestyle='-', color='blue', linewidth=2, markersize=8)
        plt.xlabel('Vaccination Rate (%)', fontsize=14)
        plt.ylabel('Number of Recovered Individuals', fontsize=14)
        plt.title('Number of Recovered Individuals vs. Vaccination Rate', fontsize=16)
        plt.grid(True)
        plt.tight_layout()
        plt.savefig('recovered_vs_vaccination_rate_clean.png')
        plt.show()
    else:
        print(f"File not found: {filepath}")

# Plot the disease progression for 50% vaccination rate
plot_disease_progression(disease_progression_path, 0.5)

# Plot the recovered vs vaccination rate
plot_recovered_vs_vaccination_rate(herd_immunity_results_path)
