#include <stdio.h>

// C interface to the Fortran code
#include "calculator.h"
#include "version.h"

int main()
{
    printf("VERSION: v%d.%d.%d\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
    printf("GIT_HASH:%s\n", GIT_HASH);


    printf("2 + 3 = %i\n", add_integers(2, 3));
    printf("2 - 3 = %i\n", subtract_integers(2, 3));

    return 0;
}
