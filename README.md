# Traffic Sim 🚦

gif / images

Simulate traffic through a lighted junction, controlling arrival rates and how many cars each light lets through in turn.

# Usage
Built to run on windows

To use, run `./traffic.exe` (i.e. open cmd navigate tot he project directory and run `.\traffic.exe`) then enter the relevant inputs. You will be asked for:

<ul>
<li>"The number of entries" - The number of roads that are within this light controlled junction.</li>
<li>"The number of depature traffic" - The number of cars that leave from an exit within it turn.</li>
<li>"The number of arrival traffic" - The number of cars that arrive at an entrance within a 1000 frames of time.</li>
</ul>

Once inputted the simulation animations will start up.  
To modify how fast it runs change the sleep() found on  line 144 of traffic-animation/roads.c

## Compiling

You will need a 64-bit Windows C compiler and this library `libwinpthread-1.dll`.  

For Windows, in the project root directory run:  
`gcc.exe traffic.c traffic-animation/cars.c traffic-animation/constants.c traffic-animation/overflow.c traffic-animation/roads.c -o traffic.exe -lgdi32 -luser32`

# Differential equations + solver

To be added atm !

This is a seperate included C file/.exe which will take the rate of flows inputs and calculate hte best configuration for how many cars should leave from each light to keep the wait time most optimised.