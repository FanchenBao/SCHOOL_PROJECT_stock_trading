/*
 * utility.c
 *
 *  Created on: Jul 27, 2018
 *      Author: fanchen
 */

#include "utility.h"

void banner(int isStart, double balance){
    if (isStart){
        printf("#################################################\n");
        printf("#\tWelcome to Stock Market Simulator!\t#\n");
        printf("#\tYou have $");
        printComma(balance);
        printf(" as starting fund.\t#\n#\tGOOD LUCK!\t\t\t\t#\n");
        printf("#################################################\n");
    }
    else{
        printf("\n#####################################################\n");
        printf("#   Thank you for using Stock Market Simulator!     #\n");
        printf("#   Live Long and Prosper \\\\//                      #\n");
        printf("#####################################################\n");
    }
}

void printComma(int n){
    // print comma in big number
    if (n < 100){
        printf("%d", n);
        return;
    }
    else{
        printComma(n / 1000);
        printf(",%03d", n % 1000);
        return;
    }
}

void resetCharArray(char* array){
    // reset the char array to all 0 for accepting the next line in a file
    for (int i = 0; i < strlen(array); i++)
        array[i] = '\0';
}

