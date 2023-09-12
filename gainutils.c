#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gainutils.h"

/**************************************************************************
This file contains C routines to calculate ASTER gains.

These routines calculate ASTER gains, radiances, etc. based on the
assumption that the reflected solar radiance from the ground can be
expressed as:
  L(band) = (1/pi)*S(band)*r(band)*P(i, eps, theta),
where
  L       is the reflected radiance (W/m^2/um/sr)
  S       is the solar irradiance, integrated over the bandwidth (W/m^2/um)
  r       is the ground reflectance, as a function of band (unitless)
  P       is the phase function, which gives the variation of reflectance as
          a function of i, eps, and theta (unitless)
  i       is the solar incidence angle, measure from vertical
  eps     is the look angle (measured from vertical)
  theta   is the azimuthal angle between the sun and the observer

In this case, we assume a Lambertian surface, so that P = cos(i).  (See
Raup's ASTER document entitled "Calculation of ASTER Gain Settings
for the GLIMS Project")
**************************************************************************/

/*******************************************************************
                   getastergainnumb
*******************************************************************/
float getastergainnumb( float doy, float lat, short band, short node ) {
  float l, inputrad, lsatur, percent, rtnval;
  int i, result;
  float *gains;

  /* specified saturation radiances (W/m^2/sr/um) for the ASTER bands */
  /* See page 9 of the ASTER Level-1 Data Processing (Ver 3.0) ATBD */
  static float input_rads[] =
     {427.0, 358.0, 218.0, 55.0, 17.6, 15.8, 15.1, 10.55, 8.04};

  /* gain settings for bands 1 through 9 */
  static float gain_options1[] = { 2.5, 1.0, 0.75, -1 }; /* yes, it's 2.5 */
  static float gain_options2[] = { 2.0, 1.0, 0.75, -1 };
  static float gain_options3[] = { 2.0, 1.0, 0.75, -1 };
  static float gain_options4[] = { 2.0, 1.0, 0.75, 0.75 };
  static float gain_options5[] = { 2.0, 1.0, 0.75, 0.17 };
  static float gain_options6[] = { 2.0, 1.0, 0.75, 0.16 };
  static float gain_options7[] = { 2.0, 1.0, 0.75, 0.18 };
  static float gain_options8[] = { 2.0, 1.0, 0.75, 0.17 };
  static float gain_options9[] = { 2.0, 1.0, 0.75, 0.12 };

  static float answers[] = { 1.0, 2.0, 3.0, 4.0, 5.0, 0.0 };
  /* corresponds to ['high', 'norm', 'low1', 'low2', 'bryt', 'dark'] */

  l = getsolarirrad(band) * getsnowref(band) * getcosi(doy,lat,node) / PI;
  switch (band) {
    case 1: gains = gain_options1; break;
    case 2: gains = gain_options2; break;
    case 3: gains = gain_options3; break;
    case 4: gains = gain_options4; break;
    case 5: gains = gain_options5; break;
    case 6: gains = gain_options6; break;
    case 7: gains = gain_options7; break;
    case 8: gains = gain_options8; break;
    case 9: gains = gain_options9; break;
    default:
      fprintf( stderr, "getastergainnumb:  Band must be >= 1 and <= 9\n" );
      exit( -1 );
  }
  inputrad = input_rads[band-1];

  for (i=0; i < NUMGAINLEVELS; ++i) {
    lsatur = inputrad / gains[i];
    if (l < lsatur) {
      result = i;
      break;
    } else {
      result = -1;
    }
  }

  /*
  result = where(l lt lsatur)
  result = result(0)
  */
  percent = l/lsatur;

  if (result == -1) result = 4;
  else if (l == 0.0) result = 5;

  if (result > 3) rtnval = answers[result];
  else rtnval = answers[result] + percent;

  return( rtnval );
}


/*******************************************************************
                   getdec
*******************************************************************/
/*
 This functions returns the declination of the sun (the latitude
 of the sun) on the given day of year.
 This ignores the eccentricity of the earth's orbit.  The
 error is on the order of 1 degree or less (?)

 Input:  doy    day of year (Jan1 = 1, etc)  (can be fractional)
 Output: declination (latitude), in degrees
*/
float getdec( float doy ) {
  float twopi;

  twopi = 2.0 * PI;

  return( -23.4417*cos(twopi*(doy + 10.0)/365.0) );
}

/*******************************************************************
                   localtime
*******************************************************************/
/*
 This function returns the local time when a sun-synchronous
 satellite crosses a given latitude and the anomaly at that point.
 Inputs:
   inc        the inclination of the orbit.  EOS AM-1: 98.3 degrees

   nodetime   time (in decimal hours) when satellite crosses ascending
              node

   node       +1 for ascending node; -1 for descending node

   lat        latitude at which time of crossing is desired

 Returns:
   local time, in decimal hours (through function name)
   anomaly of satellite at crossing (by reference)

 limitations:  the equation of time is ignored.  This causes an
               error from -14 to +16 minutes (?)

 Author:  Bruce Raup
 Date:    3/7/1996
 Translated to C 8/15/1996

*/
float localtime( float inc, float nodetime, int node, float lat, float *anom ) {
  float check, signinc, twopi, deg2rad, pi2, piminusanom, localtime, result;
  float cosdellon, dellon, signlat;
  short wnode;

/* First make sure that the input latitude is valid.  Return -1 if not. */
  if (inc > 90.0) {
    check = 180.0 - inc;
    signinc = 1.0;
  } else {
    check = inc;
    signinc = -1.0;
  }

  if (fabs(lat) > check) {
    *anom = -1.0;
    return( -1.0 );
  }

  pi2 = PI / 2.0;
  twopi = 2.0 * PI;
  deg2rad = PI / 180.0;

  inc *= deg2rad;
  lat *= deg2rad;

  if (lat >= 0.0)
    signlat = 1.0;
  else
    signlat = -1.0;

  /* from spherical law of sines */
  piminusanom = asin( sin( lat ) / sin(PI - inc) );
  *anom = PI - piminusanom;
  if (node == 1) *anom -= signlat*2.0*(*anom - pi2);

  /* dellon is delta-longitude */
  cosdellon = cos( piminusanom ) / cos( lat );
  dellon = acos( cosdellon );

  wnode = (-node + 1) / 2;  /* wnode = +1 for descending; 0 for ascending */

  localtime = nodetime - wnode*12.0 - signinc*node*signlat*24.0*dellon/twopi;
  if (localtime < 0.0) localtime += 24.0;

  *anom /= deg2rad;
  return( localtime );
}


/*******************************************************************
                   getsnowref
*******************************************************************/
float getsnowref( short band ) {
  /* snow reflectances in the ASTER VNIR and SWIR bands,
     taken from graphs of Steve Warren (Univ. of Washington)
  */
  static float ref[] =
       { 0.95, 0.88, 0.75, 0.15, 0.10, 0.20, 0.30, 0.20, 0.15 };

  if (band < 1 || band > 9) {
    fprintf( stderr, "getsnowref:  Band must be >= 1 and <= 9\n" );
    exit( -1 );
  } else {
    return( ref[band-1] );
  }
}


/*******************************************************************
                   getcosi
*******************************************************************/
/***
This function returns the cosine of the solar incidence angle.

Inputs:  doy       day of year
         lat       latitude of target
         node      ascending or descending (+1 or -1, respectively.)

This routine includes calculation of ASTER's orbit to find the time
of day when the satellite crosses the input latitude.
***/
float getcosi( float doy, float lat, short node ) {
  float deg2rad, radlat, dec, ltime, timeangle, cosi, anom;

  deg2rad = PI / 180.0;

  radlat = lat * deg2rad;

  dec = deg2rad * getdec( doy );
  ltime = localtime( ORBIT_DEC, ASC_NODE_TIME, node, lat, &anom );

  timeangle = PI * (ltime/12.0 - 1.0);

  /* cosi is the cosine of the solar incidence angle at the given lat */
  cosi = cos(dec)*cos(radlat)*cos(timeangle) + sin(dec)*sin(radlat);

  return( cosi );
}


/*******************************************************************
                   getsolarirrad
*******************************************************************/
float getsolarirrad( short band ) {

  /*
  spectral solar irradiance for the ASTER bands.
  */
  static float solar_irr[] =
      { 1845.78, 1555.93, 1108.27, 232.855, 80.0975, 74.58,
        68.5710, 59.9514, 57.2850 };

  if (band < 1 || band > 9) {
    fprintf( stderr, "getsolarirrad:  Band must be >= 1 and <= 9\n" );
    exit( -1 );
  } else {
    return( solar_irr[band-1] );
  }
}
