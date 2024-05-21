/*              includes                */
#include <stdio.h>
#include "sumarUno.c"



/**
 * @brief Adds one to a given floating-point number.
 * @return The result of adding one to the given number.
 */
int main() {
    float numero;
    int resultado;

    printf("Ingrese un número flotante: ");
    scanf("%f", &numero);

    resultado = sumar_uno(numero);

    printf("Resultado: %d\n", resultado);

    return 0;
}
