#include <stdio.h>
#include <stdlib.h>
#include "commuter.h"
#include "vehicle_owner.h"

void displayMenu() {
    int role;
  
    do {
        printf("\nPlease select your role:\n");
        printf("1. Commuter\n");
        printf("2. Vehicle Owner\n");
        printf("3. Exit\n");
        scanf("%d", &role);

        switch (role) {
            case 1:
                printf("\nYou have selected Commuter.\n");
                commuterMenu();
                break;
            case 2:
                printf("\nYou have selected Vehicle Owner.\n");
                ownerMenu();
                break;
            case 3:
                printf("\nThank you for using Viper Car Travelers!\n");
                exit(0);
            default:
                printf("\nPlease select an appropriate option.\n");
                break;
        }
    } while (role != 3);
}

int main() {
    printf("Hello! Welcome to Viper Car Travelers\n");

    // Call function to display menu
    displayMenu();

    return 0;
}
