#include <stdio.h>
#include "sumarTo.c"

int main() {
    float number;
    int result;

    printf("Enter a float number: ");
    scanf("%f", &number);

    result = add_one(number);

    printf("Result: %d\n", result);

	    return 0;
}
