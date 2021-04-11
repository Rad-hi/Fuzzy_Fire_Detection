# Test file for the temperature data logger

The data logger allows for writing data to the flash and parse it in a specific manner.

The concept it:

- If it's not the last reading of the hour: Wrtie Temp_value + ','

- If it's the last reasing of the hour: Write Temp_value + ',' + '#'

Then the parser reads in the file byte per byte recurcively and with the help of a state machine, parse-in each temp value and for each hour's readings produce:

- The max Temp_value of the that hour

- The min Temp_value of that hour

- The mean of all Temp_value(s) of that hour

Then the final output of the parser is a Json file containing these three values for each hour of the day {H0 ... H23}, resulting in 96 total values.

**PS**: After each parsing, the file is erased.
