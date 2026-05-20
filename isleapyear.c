#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Check the user passed a year argument
    if (argc < 2) {
        return 1; 
    }

    // Convert string command-line argument into numerical integer
    int year = atoi(argv[1]);

    // Gregorian calendar leap year validation check
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        printf("%d was a leap year\n", year);
    } else {
        printf("%d was not a leap year\n", year);
    }

    return 0;
}