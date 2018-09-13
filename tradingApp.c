/*
 * tradingApp.c
 *
 *  Created on: Jul 27, 2018
 *      Author: fanchen
 */

#include "tradingApp.h"

int getPriceCheckChoice(){
    // get an integer choice 0-3 from user for priceCheck function
    char inputStr[MAX_CAPACITY] = {'\0'};
    char* pInputStr = 0;
    int choice = 0, offset = 0, cycle = 0;

    while(1){
        resetCharArray(inputStr);
        cycle = 0;
        offset = 0;
        printf("\n=============================================\n");
        printf("Input option for checking stock prices\n");
        printf("1: All stocks (profitable trade noted by *)\n2: User-defined stocks (profitable trade noted by *)\n3: Return to main menu\n\n0: Exit\n");
        if(fgets(inputStr, MAX_CAPACITY - 1, stdin) == NULL){ // if input failed
            printf("\nERROR! Please input a value\n");
        }
        else{
            pInputStr = inputStr;
            while(sscanf(pInputStr, "%d%n", &choice, &offset) > 0){
                cycle++; // sscanf() should only execute once. Use cycle to see how many iterations sscanf has been executed
                pInputStr += offset;
            }

            if (cycle == 1){
                if(choice >=0 && choice <= 3)
                    return choice; // user-input choice is valid
                else
                    printf("\nERROR: the option %d is invalid\n", choice);
            }
            else // more than one cycle of sscanf executed, meaning inputStr contains more than one number
                printf("\nERROR! Please input a valid option\n");
        }
    }
}

void getPriceCheckTickers(int priceCheckChoice, char* priceCheckTickers){
    // get a tickr string from user when they want to check price for specific stocks

    if (priceCheckChoice == 2){ // indicating user chooses option 2 in priceCheck menu
        while(1){
            resetCharArray(priceCheckTickers);
            printf("\n---------------------------------------------------------\n");
            printf("Input the tickers of the stocks (case sensitive), separated by empty space\n");
            if(fgets(priceCheckTickers, MAX_CAPACITY - 1, stdin) == NULL){ // if input failed
                printf("\nERROR! Please input only tickers\n");
                continue;
            }
            else
                return;
        }
    }
}

int getBuyTicker(MyStockPrices* pMyStockPrices, char* buyTicker){
    // get user-input stock ticker to buy shares.
    char inputStr[MAX_CAPACITY] = {'\0'};
    char* pInputStr = 0;
    StockPrice* pStockInPrice = 0;

    // housekeeping
    int count = 0; // count how many items are input after each prompt. Should be ONLY 1.
    int offset = 0;

    while(1){
        resetCharArray(inputStr);
        resetCharArray(buyTicker);
        count = 0;
        offset = 0;
        printf("\n==================================================\n");
        printf("Input the stock ticker (UPPERCASE) you wish to BUY\n(press 'C' to cancel and return to main menu)\n");
        if(fgets(inputStr, MAX_CAPACITY - 1, stdin) == NULL){
            printf("\nERROR! Please input a ticker\n");
            continue;
        }
        else{
            if ((tolower((inputStr[0])) == 'c') && strlen(inputStr) == 2){ // chec whether user cancel trade. str length has to be 2 because must take the new line character into consideration
                return -1; // back to main menu
            }
            pInputStr = inputStr;
            while(sscanf(pInputStr, " %s%n", buyTicker, &offset) > 0){
                pInputStr += offset;
                count++;
            }
        }
        if(count == 1){
            pStockInPrice = findStockPrice(pMyStockPrices, buyTicker);
            if(pStockInPrice){
            	printf("\nCurrent price of %s:\t\t$%.2lf\n", buyTicker, pStockInPrice->price);
            	return 1; // ticker input successful
            }
            else
            	printf("\nERROR: ticker '%s' invalid.\n", buyTicker);
        }
        else // more than one cycle of sscanf executed (or no sscanf not executed), meaning inputStr contains more than one elements
            printf("\nERROR! Please input ONE ticker\n");
    }
}

int getBuyShare(MyStockPrices* pMyStockPrices, Portfolio* pMyPortfolio, char* buyTicker){
    // get user-defined number of shares to buy
	StockPrice* pStockInPrice = findStockPrice(pMyStockPrices, buyTicker);
	int maxShare = 0;
    int share = 0;

    char inputStr[MAX_CAPACITY] = {'\0'};
    char extra[MAX_CAPACITY] = {'\0'}; // check for input situation such as "10abc"
    char* pInputStr = 0;

    // housekeeping
    int count = 0; // count how many items are input after each prompt. Should be ONLY 1.
    int offset = 0;

    // calculate max shares that can be bought based on current balance
    maxShare = (int)(getBalance(pMyPortfolio) / pStockInPrice->price); // share in whole number
    printf("Maximum shares affordable:\t%d shares\n", maxShare);

    while(1){
        resetCharArray(inputStr);
        count = 0;
        offset = 0;
        printf("\n--------------------------------------------------\n");
        printf("How many shares (whole number) you want to BUY?\n(press 'C' to cancel and return to main menu)\n");
        if(fgets(inputStr, MAX_CAPACITY - 1, stdin) == NULL){
            printf("\nERROR! Please input a value\n");
        }
        else{
            if ((tolower((inputStr[0])) == 'c') && strlen(inputStr) == 2){ // chec whether user cancel trade
                return -1; // back to main menu
            }
            pInputStr = inputStr;
            while(sscanf(pInputStr, " %d%n %s", &share, &offset, extra) == 1){
                pInputStr += offset;
                count++;
            }

            if(count == 1){
                if (share <= maxShare && share >=0)
                    return share;
                else
                    printf("\nERROR: You cannot buy negative or more than %d shares.\n", maxShare);
            }
            else
                printf("\nERROR! Please input ONE share in whole number, or 'C' to cancel.\n");
        }
    }
}



int getSellTicker(MyStockPrices* pMyStockPrices, Portfolio* pMyPortfolio, char* sellTicker){
    // get user-input stock ticker to sell shares. When user input correct ticker, also return the stock's index
    StockPrice* pStockInPrice = 0;
    Stock* pStockInPortfolio = 0;
	char inputStr[MAX_CAPACITY] = {'\0'};
    char* pInputStr = 0;

    // housekeeping
    int count = 0; // count how many items are input after each prompt. Should be ONLY 1.
    int offset = 0;

    while(1){
        resetCharArray(inputStr);
        resetCharArray(sellTicker);
        count = 0;
        offset = 0;
        printf("\n==================================================\n");
        printf("Input the stock ticker (UPPERCASE) you wish to SELL\n(press 'C' to cancel and return to main menu)\n");
        if(fgets(inputStr, MAX_CAPACITY - 1, stdin) == NULL){
            printf("\nERROR! Please input a ticker\n");
            continue;
        }
        else{
            if ((tolower((inputStr[0])) == 'c') && strlen(inputStr) == 2){ // chec whether user cancel trade. str length has to be 2 because must take the new line character into consideration
                return -1; // back to main menu
            }
            pInputStr = inputStr;
            while(sscanf(pInputStr, " %s%n", sellTicker, &offset) > 0){
                pInputStr += offset;
                count++;
            }
        }
        if(count == 1){
        	pStockInPrice = findStockPrice(pMyStockPrices, sellTicker);
        	if(pStockInPrice){
        		pStockInPortfolio = findStock(pMyPortfolio, sellTicker);
        		if(pStockInPortfolio){
        			printf("\nCurrent price of %s:\t$%.2lf\n", sellTicker, pStockInPrice->price);
        			return 1; // ticker input successful
        		}
        		else{
        			printf("\nWARNING: Stock '%s' is not in your portfolio.\nMake a purchase first before you can sell.\n", sellTicker);
        			return -1;
        		}

        	}
        	else
        		printf("\nERROR: ticker '%s' invalid.\n", sellTicker);
        }
        else // more than one cycle of sscanf executed (or no sscanf not executed), meaning inputStr contains more than one elements
            printf("\nERROR! Please input ONE ticker\n");
    }
}

int getSellShare(Portfolio* pMyPortfolio, char* sellTicker){
    // get user-defined number of shares to buy
	Stock* pStockInPortfolio = findStock(pMyPortfolio, sellTicker);
    int maxShare = 0; // max share can be sold
    int share = 0; // user-defined shares to sell

    char inputStr[MAX_CAPACITY] = {'\0'};
    char extra[MAX_CAPACITY] = {'\0'}; // check for input situation such as "10abc"
    char* pInputStr = 0;

    // housekeeping
    int count = 0; // count how many items are input after each prompt. Should be ONLY 1.
    int offset = 0;

    // find max shares that can be sold based on the amount of shares currently owned
    printf("Buy price of %s:\t$%.2lf\n", sellTicker, pStockInPortfolio->buyPrice);
    maxShare = pStockInPortfolio->shares;
    printf("Maximum shares to sell:\t%d shares\n", maxShare);
    if (maxShare == 0){
        printf("\nWARNING: You cannot sell any share of '%s'.\n", sellTicker);
        return -1;  // back to main menu
    }

    while(1){
        resetCharArray(inputStr);
        count = 0;
        offset = 0;
        printf("\n--------------------------------------------------\n");
        printf("How many shares (whole number) you want to SELL?\n(press 'C' to cancel and return to main menu)\n");
        if(fgets(inputStr, MAX_CAPACITY - 1, stdin) == NULL){
            printf("\nERROR! Please input a value\n");
        }
        else{
            if ((tolower((inputStr[0])) == 'c') && strlen(inputStr) == 2){ // chec whether user cancel trade
                return -1; // back to main menu
            }
            pInputStr = inputStr;
            while(sscanf(pInputStr, " %d%n %s", &share, &offset, extra) == 1){
                pInputStr += offset;
                count++;
            }

            if(count == 1){
                if (share <= maxShare && share >=0)
                    return share;
                else
                    printf("\nERROR: You cannot sell negative or more than %d shares.\n", maxShare);
            }
            else
                printf("\nERROR! Please input ONE share in whole number, or 'C' to cancel.\n");
        }
    }
}


int getMainMenuChoice(){
    // get an integer choice 0-3 from user for priceCheck function
    char inputStr[MAX_CAPACITY] = {'\0'};
    char* pInputStr = 0;
    int choice = 0, offset = 0, cycle = 0;

    while(1){
        resetCharArray(inputStr);
        cycle = 0;
        offset = 0;
        printf("\n*****************************************************\n");
        printf("*   What do you want to do next (input an option)?  *\n");
        printf("*   1: Check my portfolio                           *\n");
        printf("*   2: Check current stock price                    *\n");
        printf("*   3: Buy Share                                    *\n");
        printf("*   4: Sell Share                                   *\n");
        printf("*                                                   *\n");
        printf("*   0: Exit                                         *\n");
        printf("*****************************************************\n");
        if(fgets(inputStr, MAX_CAPACITY - 1, stdin) == NULL){ // if input failed
            printf("\nERROR! Please input a value\n");
        }
        else{
            pInputStr = inputStr;
            while(sscanf(pInputStr, "%d%n", &choice, &offset) > 0){
                cycle++; // sscanf() should only execute once. Use cycle to see how many iterations sscanf has been executed
                pInputStr += offset;
            }

            if (cycle == 1){
                if(choice >=0 && choice <= 4)
                    return choice; // user-input choice is valid
                else
                    printf("\nERROR: the option %d is invalid\n", choice);
            }
            else // more than one cycle of sscanf executed, meaning inputStr contains more than one number
                printf("\nERROR! Please input a valid option\n");
        }
    }
}

char getExitConfirm(){
    // get a Y or N confirmation when user decides to exit
    char inputStr[MAX_CAPACITY] = {'\0'};
    int cycle = 0, offset = 0;

    while(1){
        resetCharArray(inputStr);
        offset = 0;
        cycle = 0;
        printf("\nSure to exit the program? Y/N\n");
        if(fgets(inputStr, MAX_CAPACITY - 1, stdin) == NULL){ // if input failed
            printf("\nERROR! Please input an option\n");
        }
        else{
            if (strlen(inputStr) == 2){
                if (tolower((inputStr[0])) == 'y') // user wants really to quit
                    return 'y'; // quit program.
                if (tolower((inputStr[0])) == 'n') // user changes mind and doesn't want to quit
                    return 'n';
            }
            printf("\nERROR! Please input Y or N\n");
        }
    }
}

int mainMenu(MyStockPrices* pMyStockPrices, Portfolio *pMyPortfolio, char* priceFile, char* portfolioFile, int mainMenuChoice, int priceCheckChoice, char* priceCheckTickers, char* buyTicker, int buyShare, char* sellTicker, int sellShare){
    // main mennu of the program

    int returnPC = 0; // return value of priceCheck(), initialize to 0

    switch(mainMenuChoice){
        case 1:
            if (readPortfolio(portfolioFile, pMyPortfolio)){
                printPortfolio(pMyPortfolio);
            }
            else{
                printf("\nYou have not created a portfolio yet.\nPortfolio will be created after making a trade.\n");
            }
            break;
        case 2:
            returnPC = priceCheck(pMyStockPrices, pMyPortfolio, priceCheckChoice, priceCheckTickers);
            if (!returnPC) // user wants to quit
                return 0;
            else if (returnPC == 2) // user wants another round of price check (doesn't want to return to main menu)
                return 2;
            else // user wants to return to main menu
                break;
        case 3:
            buy(pMyStockPrices, pMyPortfolio, portfolioFile, buyTicker, buyShare);
            break;
        case 4:
            sell(pMyStockPrices, pMyPortfolio, portfolioFile, sellTicker, sellShare);
            break;
        case 0:
            return 0; // user wants to exit
        default:
            printf("\nThe program encounters a fatal error in main menu. Program terminated");
            exit(0);
    }
    return 1; // user not exiting yet
}

int confirmExit(MyStockPrices* pMyStockPrices, Portfolio *pMyPortfolio, char* priceFile, char* portfolioFile, char exitConfirm){
    // confirm a second time user wants to quit
    if (exitConfirm == 'y'){
        saveStockPrice(priceFile, pMyStockPrices);
        if (readPortfolio(portfolioFile, pMyPortfolio))
            savePortfolio(portfolioFile, pMyPortfolio);
        return 0; // quit program.
    }
    else
        return 1; // user changes mind and not exiting yet
}

int getTradingInputs(MyStockPrices* pMyStockPrices, Portfolio *pMyPortfolio, int mainMenuChoice, int* pPriceCheckChoice, char* priceCheckTickers, char* buyTicker, int* pBuyShare, char* sellTicker, int* pSellShare, int* pHasPortfolio){
    // get all user inputs during trading, after the main menu options
    switch(mainMenuChoice){
        case 0:
            break;
        case 1:
            break;
        case 2:
            // price check inputs
            *pPriceCheckChoice = getPriceCheckChoice();
            getPriceCheckTickers(*pPriceCheckChoice, priceCheckTickers);
            break;
        case 3:
            // buy function inputs
            if (getBuyTicker(pMyStockPrices, buyTicker) == -1) // user cancel buy, return to main menu
                return 0;
            *pBuyShare = getBuyShare(pMyStockPrices, pMyPortfolio, buyTicker);
            if (*pBuyShare == -1) // user cancel buy
                return 0;
			*pHasPortfolio = 1; // user made a buy, thus has a portfolio now.
			break;
        case 4:
            // sell function inputs
        	if(*pHasPortfolio){
        		if (getSellTicker(pMyStockPrices, pMyPortfolio, sellTicker) == -1) // user cancel sell or stock not in portfolio yet
        			return 0;
        		*pSellShare = getSellShare(pMyPortfolio, sellTicker);
        		if (*pSellShare == -1) // user cancel sell
        			return 0;
        	}
        	else{
        		printf("\nError: Unable to sell because you do not have a portfolio yet.\n");
        		return 0;
        	}
            break;
        default:
            printf("\nThe program encounters a fatal error in main menu. Program terminated");
            exit(0);
    }
    return 1; // do not return to main menu but proceeds in the menu
}

void process(){
	// the main app function that combines everything together
	// structs
	MyStockPrices myStockPrices;
	Portfolio myPortfolio;

	// user input
	int priceCheckChoice = 0;
	char priceCheckTickers[MAX_CAPACITY] = {'\0'};
	int mainMenuChoice = 0;
	char exitConfirm = 0;
	int buyShare = 0;
	char buyTicker[MAX_CAPACITY] = {'\0'};
	int sellShare = 0;
	char sellTicker[MAX_CAPACITY] = {'\0'};

	// function return values
	int returnMM = 1; // return from mainMenu(), initialize to 1 such that the app won't quit when user just wants to get back to main menu
	int returnCE = 0; // return from confirmExit()
	int hasPortfolio = 0; // return from initializePortfolio()

	// file read and save
	char* priceFile = "mystocks.txt";
	char* portfolioFile = "portfolio.txt";

	// initial balance
	double balance = 20000;


	srand(time(0)); // seed random

	banner(1, balance);
	hasPortfolio = initializePortfolio(portfolioFile, &myPortfolio, balance); // read in or initialize portfolio
	initializeSimulator(priceFile, &myStockPrices); // read in and initialize stock prices

	// main application loop
	while(1){
		// main menu inputs
		mainMenuChoice = getMainMenuChoice();
		while(1){
			if(!getTradingInputs(&myStockPrices, &myPortfolio, mainMenuChoice, &priceCheckChoice, priceCheckTickers, buyTicker, &buyShare, sellTicker, &sellShare, &hasPortfolio))
				break; // return to main menu

			returnMM = mainMenu(&myStockPrices, &myPortfolio, priceFile, portfolioFile, mainMenuChoice, priceCheckChoice, priceCheckTickers, buyTicker, buyShare, sellTicker, sellShare);
			if (returnMM != 2)
				break; // return to main menu
		}

		if(!returnMM){
			exitConfirm = getExitConfirm();
			returnCE = confirmExit(&myStockPrices, &myPortfolio, priceFile, portfolioFile, exitConfirm);
			if(!returnCE)
				break;
			else
				returnMM = 1; // reset returnMM value from 0 (user wants to quit) to 1 (user changes mind)
		}
	}
	banner(0,0);
}

