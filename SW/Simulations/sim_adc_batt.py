#  sim_adc_batt.py
#  project : Rainette
#  author : Damien Pomelli 
#  last modified : 2025-07-24

# Simulation of ADC voltage in battery mode

import numpy as np
import matplotlib.pyplot as plt

batt_number = 4
batt_max_voltage = 1.5
batt_min_voltage = 0.9

batt_range = np.linspace(batt_min_voltage, batt_max_voltage, 100)

Vf = 0.35 # Schottky
Vz = 3.6 # Zener
R1 = 22e3
R2 = 82e3

Vbatt = batt_number * batt_range
Vadc = ((Vbatt-Vf) * R2) / (R1 + R2)

Vadc[Vadc > Vz] = Vz

plt.figure()
plt.plot(Vbatt,Vadc)
plt.xlabel("Tension de la pile [V]")
plt.ylabel("Tension de l'ADC [V]")
plt.title("V$_{adc}$(V$_{batt}$)")
plt.grid(True)
plt.savefig("Vadc_vs_Vbatt.png")
plt.show()