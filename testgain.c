#include <stdio.h>
#include <stdlib.h>
float getastergainnumb( float doy, float lat, short band, short node );
float getsolarirrad( short band );
float getsnowref( short band );
float getcosi( float doy, float lat, short node );

int main( int argc, char *argv[] ) {
  float doy, lat, gain;
  float frc;
  int intpart;
  short band, node = -1;
  static char *words[] = { "dark", "high", "norm", "low1", "low2", "bright" };

  if (argc != 4) {
    printf( "Usage:  %s  doy lat band\n", argv[0] );
    exit( -1 );
  }
  doy = atof( argv[1] );
  lat = atof( argv[2] );
  band = atoi( argv[3] );
  // fprintf( stderr, "Args:  doy=%f, lat=%f, band=%d\n", doy,lat,band);
  gain = getastergainnumb( doy, lat, band, node );
  // fprintf( stderr, "%s: returned from getastergainnumb\n", argv[0] );
  // fprintf( stderr, "%s: gain = %f\n", argv[0],gain );
  intpart = (int) gain;
  frc = 100.0*(gain - (float)intpart);
//printf( "1: high  2: norm  3: low1  4: low2  5: bright  6: dark\n" );
  printf( "Gain:  %f  (%s gain at %.2f%% saturation)\n",gain,words[intpart],frc );
  return 0;
}
