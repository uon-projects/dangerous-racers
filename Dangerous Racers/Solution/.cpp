#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

class Checkpoints
{
	static const int pointsLvl1 = 12, pointsLvl2 = 22, pointsLvl3 = 46;
	public: int pLvl1[pointsLvl1][2], pLvl2[pointsLvl2][2], pLvl3[pointsLvl3][2];
	
	static const int cpLvl1 = 7, cpLvl2 = 9, cpLvl3 = 13;
	public: int checkpointsLvl1[cpLvl1][4], checkpointsLvl2[cpLvl2][4], checkpointsLvl3[cpLvl3][4];

	public:
	  Checkpoints() {
		  int pLvl1[pointsLvl1][2] = {
			261, 825,
			396, 436,
			664, 235,
			899, 197,
			1153, 235,
			1449, 436,
			1580, 825,
			1580, 2324,
			1295, 2849,
			899, 2956,
			536, 2849,
			261, 2324
		  };

		  int pLvl2[pointsLvl2][2] = {
			221, 747,
			364, 323,
			584, 238,
			807, 196,
			1092, 228,
			1298, 342,
			1397, 572,
			1199, 972,
			1420, 1218,
			1687, 1192,
			1874, 1281,
			1973, 1464,
			1991, 1665,
			1977, 1950,
			1878, 2193,
			1491, 2323,
			1125, 2310,
			761, 2340,
			466, 2240,
			270, 2011,
			189, 1692,
			260, 1310
		  };

		  int pLvl3[pointsLvl3][2] = {
			1451, 1930,
			785, 2013,
			498, 1997,
			272, 1834,
			198, 1701,
			143, 1473,
			220, 1225,
			349, 1056,
			643, 991,
			1007, 1023,
			1373, 1134,
			1735, 1125,
			1985, 990,
			2182, 708,
			2249, 531,
			2355, 214,
			2537, 140,
			2700, 120,
			2962, 186,
			3087, 303,
			3276, 466,
			3716, 593,
			3919, 677,
			4003, 916,
			3934, 1113,
			3764, 1262,
			3536, 1285,
			3330, 1110,
			3079, 868,
			2890, 751,
			2651, 707,
			2452, 788,
			2352, 916,
			2328, 1092,
			2459, 1307,
			2655, 1315,
			2846, 1281,
			3154, 1381,
			3499, 1498,
			3644, 1654,
			3642, 1858,
			3494, 2001,
			3287, 2038,
			2995, 1956,
			2686, 1888,
			2203, 1925
		};
		  int checkpointsLvl1[cpLvl1][4] = {
			149, 802, 507, 802,
			915, 97, 915, 454,
			1323, 802, 1681, 802,
			1323, 2350, 1681, 2350,
			915, 2710, 915, 3069,
			149, 2366, 507, 2366,
			149, 1748, 507, 1748
		};

		  int checkpointsLvl2[cpLvl2][4] = {
			118, 759, 477, 759,
			800, 133, 800, 491,
			1118, 806, 1476, 806,
			1717, 1635, 2075, 1635,
			1712, 1864, 2071, 1864,
			1400, 2054, 1400, 2411,
			796, 2053, 796, 2411,
			118, 1669, 476, 1669,
			118, 1140, 476, 1140
		};

		  int checkpointsLvl3[cpLvl3][4] = {
			740, 1749, 740, 2105,
			88, 1473, 446, 1473,
			755, 847, 755, 1205,
			1454, 847, 1454, 1205,
			1980, 474, 2293, 666,
			2883, 479, 3012, 139,
			3550, 795, 3728, 477,
			3308, 1295, 3421, 947,
			2854, 1068, 3015, 740,
			2696, 1484, 2822, 1143,
			3149, 1657, 3269, 1313,
			3014, 1749, 3014, 2105,
			2118, 1749, 2118, 2105
		};

	  }

};
