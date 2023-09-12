# GLIMS ASTER Gains

This program calculates gain settings for the ASTER instrument that are
optimized for snowy regions.  To compile:

  gcc -o testgain testgain.c gainutils.c -lm

or type

  make build

For more information about these calculations, see

Raup, Bruce H., Hugh H. Kieffer, Trent M. Hare, and Jeffrey S. Kargel
(2000). "Generation of Data Acquisition Requests for the ASTER Satellite
Instrument for Monitoring a Globally Distributed Target: Glaciers." IEEE
Transactions On Geoscience and Remote Sensing 38:1105--1112.
http://cires.colorado.edu/~braup/pubs/xargenIEEE.pdf
