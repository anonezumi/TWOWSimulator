# TWOWSimulator by anonymouse

This project aims to simulate [Ten Words of Wisdom](https://www.youtube.com/watch?v=S64R-_LVHuY) rounds as quickly and accurately as possible. It is written in C++ without external libraries.

Do not redistribute the executable or its source code without including this file. Source code is provided so that you may alter it as you wish, but if you choose to redistribute it, please credit anonymouse#1643 as the original owner.

## HOWTO:
There are only 2 steps to calculating TWOW win chances and/or simulating TWOWs.

The first step is creating your data file. You should have a file named contestantdata.txt. This is the file in which to put the data concerning the contestants invloved in the simulation (obviously).

The three pieces of data the program needs about each contestant is their name, their average raw score, and the standard deviation of the set of their raw scores. The values must be formatted in a specific way for the program to parse them correctly. Here's how:

Name

Average raw score

Standard deviation of raw scores

Filler to separate entries

Or, as an example:

```
anonymouse
60.57
21.23
----------
```

You may add as many contestants as you want to the file. Make sure to keep track of the amount, though, as you will need to input it during step 2.

The second step is to run the program. You could choose to build the project yourself, using the visual studio project files provided, or run the executable in \Debug. The executable should be called TWOWSimulator.exe, and you run it as you would any program. The program will open a console window, and you will be asked some questions. Here are the questions in order:

### Type the current round number. 
This input should be an integer only. The program will not do what you want if it isn't. If you want to simulate starting at round 1, enter the number 1. If you want the simulation to start at round 4, type 4.

### Type the number of contestants.
This will typically be the same amount of contestants as there are in contestantdata.txt. If you want to exclude contestants, put them at the bottom of the file and enter the number of contestants you want to include. Once again, integers only.

### Type the number of simulations to perform.
If this number is 5 or less, the program will reveal results in more detail, showing the generated scores for each of the contestants. There is no guarantee these generated scores are technically possible in TWOW. This should also be an integer.

Note: The speed of the simulations is about 0.002c^2 + 0.04c ms/sim (if c is the number of contestants), so keep that in mind when choosing large numbers of simulations.