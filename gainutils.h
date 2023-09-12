#ifndef PI
#define PI 3.141592653589793238462643383279502884197
#endif
#define NUMGAINLEVELS     4
#define ORBIT_DEC         98.3
#define ASC_NODE_TIME     22.5
/* prototypes */
float getastergainnumb( float doy, float lat, short band, short node );
float getsolarirrad( short band );
float getsnowref( short band );
float getcosi( float doy, float lat, short node );
float localtime( float inc, float nodetime, int node, float lat, float *anom );
