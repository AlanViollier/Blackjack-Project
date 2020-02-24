// BlackJack.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PLAYERS 20
#define MAX_NAME_LENGTH 100

//Program Prototypes
void deal(int arr[], int arr2[]);
void hit(int arr[]);
void Comphit(int arr[]);
int checkValue(int arr[]);

int main() {
    srand(time(NULL));
    char names[MAX_PLAYERS][MAX_NAME_LENGTH] = {};
    int moneys[MAX_PLAYERS];
    int currentPlayerCount = 0;
    FILE *pfPtr;
    if ((pfPtr = fopen("players.txt", "r")) == NULL) {
        puts("Creating new players file.");
        pfPtr = fopen("players.txt", "w");
        fclose(pfPtr);
        pfPtr = fopen("players.txt", "r");
    }
    unsigned int counter = 0;
    while (!feof(pfPtr)) {
        char name[MAX_NAME_LENGTH];
        fscanf(pfPtr, "%s", &name);
        strcpy(names[counter], name);
        fscanf(pfPtr, "%d", &moneys[counter]);
        counter++;
    }
    fclose(pfPtr);
    
    
    char name[MAX_NAME_LENGTH];
    puts("What is your name?");
    scanf("%s", &name);
    
    
    counter = 0;
    while (moneys[counter] != 0) {
        if (strcmp(name, names[counter]) == 0) {
            currentPlayerCount = counter;
            printf("Welcome back %s!\n", names[currentPlayerCount]);
            break;
        }
        counter++;
    }
    if (moneys[counter] == 0) {
        currentPlayerCount = counter;
        printf("Welcome to Black Jack, %s!\nSince you are new you will start with 1000$\n", name);
        strcpy(names[currentPlayerCount], name);
        moneys[currentPlayerCount] = 1000;
    }
    
    int pInput = 0;
    while (pInput != 3) {
        int dealerHand[11] = { 0 };
        int playerHand[11] = { 0 };
        int bet = 0;
        
        do {
            printf("Your current balance is %d$\n", moneys[currentPlayerCount]);
            printf("Please enter a valid bet (0 < bet <= Your money): ");
            scanf("%d", &bet);
        } while(bet<0&&bet<=moneys[currentPlayerCount]);
        
        //The inital dealing fo the cards
        deal(dealerHand, playerHand);
        do {
            printf("You hand is worth %d\n", checkValue(playerHand));
            printf("Would you like to hit (Enter 1)\nOr would you like to stay? (Enter 2): ");
            scanf("%d", &pInput);
        } while (pInput > 2 || pInput < 1);
        
        //If the player wants more cards
        if (pInput == 1) {
            do {
                hit(playerHand);
                if (checkValue(playerHand) > 21) {
                    pInput = 4;
                }
                else {
                    printf("You hand is worth %d\n", checkValue(playerHand));
                    printf("Would you like to hit (Enter 1)\nOr would you like to stay? (Enter 2): ");
                    scanf("%d", &pInput);
                }
            } while (pInput == 1);
        }
        
        //Gives the computer more cards
        do {
            Comphit(dealerHand);
        } while (checkValue(dealerHand) < 16);
        
        //If the player wants to show their cards
        if (pInput == 2) {
            printf("You hand is worth %d\n", checkValue(playerHand));
            printf("The dealer has %d\n", checkValue(dealerHand));
            if (checkValue(playerHand) == checkValue(dealerHand)) {
                printf("You Lose\n");
                printf("You pay the dealer %d\n", bet);
                moneys[currentPlayerCount] -= bet;
            }
            else if (checkValue(dealerHand) > 21) {
                printf("You Win!\n");
                printf("The dealer pays you %d\n", bet);
                moneys[currentPlayerCount] += bet;
            }
            else if (checkValue(playerHand) > checkValue(dealerHand)) {
                printf("You Win!\n");
                printf("The dealer pays you %d\n", bet);
                moneys[currentPlayerCount] += bet;
            }
            else {
                printf("You Lose\n");
                printf("You pay the dealer %d\n", bet);
                moneys[currentPlayerCount] -= bet;
            }
        }
        
        //If the player hits and goes over 21
        if (pInput == 4) {
            printf("You have %d\n", checkValue(playerHand));
            printf("You Lose!\n");
            printf("You pay the dealer %d\n", bet);
            moneys[currentPlayerCount] -= bet;
        }
        if(moneys[currentPlayerCount]<=0)
        {
            puts("You have lost all your moneys, but this is a game so you get 50 more dollars.");
            moneys[currentPlayerCount] = 50;
        }
        printf("You now have %d$\n", moneys[currentPlayerCount]);
        printf("Play again? (Enter 1)\nLeave game? (Enter 3): ");
        scanf("%d", &pInput);
        puts("");
        
        //To end program
        if (pInput == 3) {
            puts("End Program");
        }
    }
    pfPtr = fopen("players.txt", "w");
    counter = 0;
    while (moneys[counter] != 0) {
        fprintf(pfPtr, "%s %d", names[counter], moneys[counter]);
        counter++;
    }
    puts("All player data saved");
    fclose(pfPtr);
}

//Gives the player and dealer 2 cards
void deal(int arr[], int arr2[]) {
    hit(arr2);
    hit(arr2);
    Comphit(arr);
    Comphit(arr);
}

//Gives the player 1 card
void hit(int arr[]) {
    int i = 0;
    while (arr[i] != 0) {
        i++;
    }
    int r2 = (rand() % 13) + 2;
    int input;
    switch (r2)
    {
        case 11:
            printf("You get a Jack\n");
            arr[i] = 10;
            break;
        case 12:
            printf("You get a Queen\n");
            arr[i] = 10;
            break;
        case 13:
            printf("You get a King\n");
            arr[i] = 10;
            break;
        case 14:
            do {
                printf("Your hand is currently worth %d\n", checkValue(arr));
                printf("You get an Ace\n");
                printf("Would you like the Ace to be worth 1 (Enter 1) or 11 (Enter 2)?: ");
                scanf("%d", &input);
            } while (input < 1 || input > 2);
            if (input == 1) {
                arr[i] = input;
            }
            else {
                arr[i] = 11;
            }
            break;
        default:
            printf("You get a %d\n", r2);
            arr[i] = r2;
            break;
    }
}

//Gives the dealer 1 card
void Comphit(int arr[]) {
    int i = 0;
    while (arr[i] != 0) {
        i++;
    }
    int r2 = (rand() % 13) + 2;
    switch (r2)
    {
        case 11:
            arr[i] = 10;
            break;
        case 12:
            arr[i] = 10;
            break;
        case 13:
            arr[i] = 10;
            break;
        case 14:
            if (checkValue(arr) > 10) {
                arr[i] = 1;
            }
            else {
                arr[i] = 11;
            }
            break;
        default:
            arr[i] = r2;
            break;
    }
}

//Checks the value of a given hand
int checkValue(int arr[]) {
    int count = 0;
    for (int i = 0; i < 6; i++) {
        count += arr[i];
    }
    return count;
}
