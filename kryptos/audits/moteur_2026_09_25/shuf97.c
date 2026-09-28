/* shuf97 graine : K4 mélangé (même générateur que k4x : graine = seed0 + z) */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
int main(int c, char **v) { char s[] = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
  uint64_t r = strtoull(v[1], 0, 10) * 0x9E3779B97F4A7C15ULL; for (int i = 96; i > 0; i--) { r ^= r << 13; r ^= r >> 7; r ^= r << 17; int j = r % (i + 1); char t = s[i]; s[i] = s[j]; s[j] = t; }
  puts(s); return 0; }
