#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

STRUCT(test,
  int, a,
  char, b,
  double, c
)

int main(void) {
  STRUCT_MALLOC(test, test_struct);
  STRUCT_SET(test, test_struct, a, 10);
  STRUCT_SET(test, test_struct, b, 'H');
  STRUCT_SET(test, test_struct, c, 3.141592653589793238462643383);

  printf("a = %d\nb = '%c'\nc = %.16F\n",
	 STRUCT_GET(test, test_struct, a),
	 STRUCT_GET(test, test_struct, b),
	 STRUCT_GET(test, test_struct, c));
}
