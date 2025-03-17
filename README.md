# GLIMS ASTER Gains

The main program in this repository, `glims_aster_gains.c`, calculates gain
settings for the ASTER instrument that are optimized for snowy regions.  To
compile:

  gcc -o testgain testgain.c gainutils.c -lm

or type

  make build

## Example Runs

```
# With no arguments, the program prints a usage summary
$ ./glims_aster_gains 
Usage:  ./glims_aster_gains  doy lat band eq_crossing_time_24

where
  doy                  is doy of year
  lat                  is latitude of target
  band                 is ASTER VNIR or SWIR band (1-9)
  eq_crossing_time_24  is the ascending node crossing time

# Run for a particular set of parameters
$ ./glims_aster_gains 180 39 3 22.5
Gain:  3.851765  (low1 gain at 85.18% saturation)

# Run for all bands, using the summer 2023 ascending node crossing time of 21:00
$ for b in 1 2 3 4 5 6 7 8 9 ; do echo -n "Band $b:  "; ./glims_aster_gains 180 39 $b 21; done
Band 1:  Gain:  3.793516  (low1 gain at 79.35% saturation)
Band 2:  Gain:  2.985390  (norm gain at 98.54% saturation)
Band 3:  Gain:  2.982356  (norm gain at 98.24% saturation)
Band 4:  Gain:  1.327237  (high gain at 32.72% saturation)
Band 5:  Gain:  1.234506  (high gain at 23.45% saturation)
Band 6:  Gain:  1.486456  (high gain at 48.65% saturation)
Band 7:  Gain:  1.701994  (high gain at 70.20% saturation)
Band 8:  Gain:  1.585633  (high gain at 58.56% saturation)
Band 9:  Gain:  1.550712  (high gain at 55.07% saturation)
```

# Web Service

The program `glims_aster_gains.c` is behind a web service (http://www.glims.org/tools/aster_gain/), which can be accessed through a browser form or via a program like `curl`.

# Adjustments to gains late in ASTER's mission

As of late 2023, ASTER's (Terra's) orbit has drifted such that the equator crossing time has changed from 10:30 (descending node) / 22:30 (ascending node) to
9:45 (descending node) / 21:45 (ascending node).  The Python program `create_new_gains_STAR.py` reads Science Team Acquisition Requests (STARs)
from the CSV file (see code for name) and writes another CSV file with gains adjusted.  These will go into effect for GLIMS image acquisitions
at the start of 2024.  `create_new_gains_STAR.py` is a standalone command-line program, but it uses the above-mentioned Web service to get the gains for each
STAR entry, based on latitude and the new equator crossing time.

# Script to call web service

The script `create_new_gains_STAR.py` repeatedly calls the above-described web
service to calculate gains for times and locations in an input spreadsheet.
This particular code does so for a spreadsheet provided by Jeff Kargel
to update the Data Acquisition Requests for the ASTER system.  The
gains are inserted into a new spreadsheet created as output.

# Background Reference

* Raup, Bruce H., Hugh H. Kieffer, Trent M. Hare, and Jeffrey S. Kargel
  (2000). "Generation of Data Acquisition Requests for the ASTER Satellite
  Instrument for Monitoring a Globally Distributed Target: Glaciers." IEEE
  Transactions On Geoscience and Remote Sensing 38:1105--1112.
  http://cires.colorado.edu/~braup/pubs/xargenIEEE.pdf

* ASTER Level-1 Data Processing ATBD (Algorithm Theoretical Basis Document), included in this repository.


