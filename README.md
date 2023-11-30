# GLIMS ASTER Gains

This program calculates gain settings for the ASTER instrument that are
optimized for snowy regions.  To compile:

  gcc -o testgain testgain.c gainutils.c -lm

or type

  make build

# Example Runs

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

This code is behind a web service (http://www.glims.org/tools/aster_gain/), which can be accessed through a browser form or via a program like `curl`.

# Background Reference

Raup, Bruce H., Hugh H. Kieffer, Trent M. Hare, and Jeffrey S. Kargel
(2000). "Generation of Data Acquisition Requests for the ASTER Satellite
Instrument for Monitoring a Globally Distributed Target: Glaciers." IEEE
Transactions On Geoscience and Remote Sensing 38:1105--1112.
http://cires.colorado.edu/~braup/pubs/xargenIEEE.pdf
