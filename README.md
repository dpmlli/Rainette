# Rainette

The ***Rainette*** project carried out as part of the Bachelor thesis at HEIG-VD. The
project involves designing an electronic board capable of collecting measurements
and controlling actuators to regulate an aquaponic greenhouse. It is an embedded
system that communicates wirelessly with a central unit and can operate on battery
power. The work includes the phases of preliminary analysis, hardware and software
design, as well as commissioning.

## Content

The repository contains 3 folders: `DOC`, `HW`, and `SW`.

- **DOC** : contains the project documentation. This includes :
    * Some elements specific to the *Rainette* PCB
    * The final report and the specifications
    * The logbook and planning documents
- **HW**  : contains the *Altium* projects for the *Rainette* and *STM32WB5MM-DK* boards.
- **SW**  : contains all the project code. This includes :
    * The firmware for the *Rainette* and *Disco (STM32WB5MM-DK)* boards
    * The programs for the *Raspberry Pi*
    * The simulation scripts

## Script instructions

### Mbed OS

To set up the code for the *Rainette* and *Disco* boards :
- Retrieve the contents of the `Mbed OS` folder and add the `mbed-os` folder from the following repository: https://github.com/ARMmbed/mbed-os. To reproduce the environment in which the project was developed, the following *VSCode* extensions must be installed : `C/C++ Tools`, `Task Runner`, `Cortex-Debug`, and `Copy file name`. You also need to install Mercurial, Mbed CLI, and the dependencies listed in the `requirements.txt` file. More details here : https://github.com/YannCharbon/vscode-mbed-env.  
- Then add the files from either the `Rainette` or `Disco` folders.
- Update the firmware of the Cortex-M0+ using the `STM32CubeProgrammer` software. Use the binary `stm32wb5x_BLE_HCILayer_fw.bin`, version 1.12.0 (additional instructions can be found in the report).
- Compile the program.
- For the *Disco* board, the binary can be flashed directly from *VSCode*. With the *Rainette* board, it must be done using the `STM32CubeProgrammer` software following these steps :
    1. Connect to the board
    2. *FUS* tab: Update the firmware using the following binary `stm32wb5x_BLE_HCILayer_fw.bin`, version 1.12.0 (`M0+ FW` folder)
    3. *FUS* tab: Click the `Start Wireless Stack` button
    4. *Erasing & Programming* tab: Start programming using the *Rainette* binary

### GUI

The `rainette_GUI.py` and `disco_GUI.py` scripts require the following Python libraries: `bleak` and `pyside6`. Python version 3.11 was used, but the scripts should work with earlier versions as well.

Once *advertising* is enabled by the *BLE* server, wait 2–3 seconds before running the script. The window will appear if the server is successfully detected by the client.