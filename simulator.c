/*
 * simulator.c
 *
 *  Created on: Jul 27, 2018
 *      Author: fanchen
 */

#include "simulator.h"


int readStockPrices(char* filename, MyStockPrices* pMyStockPrices){
	// read the previous stock price stored in a txt file, return the size of myStockPrices array
	FILE* pFile = 0;
	char line[MAX_CAPACITY] = {'\0'};
	int i = 0;

	pFile = fopen(filename, "r");
	if(pFile == NULL){
		printf("Open file %s failed. Please check your file directory\n", filename);
		exit(0);
	}

	while(fgets(line, MAX_CAPACITY - 1, pFile) != NULL){
		sscanf(line, " %s %lf", pMyStockPrices->myStockPrices[i].ticker, &pMyStockPrices->myStockPrices[i].price);
		i++;
		resetCharArray(line);
	}

	if (i > pMyStockPrices->myStocksCapacity){ // in case more than 5 stocks are stored in the txt file
		printf("More stocks present than expected. Please check the accuracy of %s.", filename);
		exit(0);
	}

	if(pFile)
		fclose(pFile);
	else{
		printf("An error has occurred while reading %s.\n", filename);
		exit(0);
	}

	return i;
}

void initializeSimulator(char* filename, MyStockPrices* pMyStockPrices){
    // initialize the struct myStockPrices
    pMyStockPrices->myStocksCapacity = MAX_MY_STOCKS;
    pMyStockPrices->myStocksSize = readStockPrices(filename, pMyStockPrices);
}

StockPrice* findStockPrice(MyStockPrices* pMyStockPrices, char* ticker){
	// find the StockPrice struct corresponding to the ticker, and return its address
	for (int i = 0; i < pMyStockPrices->myStocksSize; i++){
		if(!(strcmp(ticker, pMyStockPrices->myStockPrices[i].ticker))){ // found the ticker in myStockPrices
			return &(pMyStockPrices->myStockPrices[i]);
		}
	}

	return NULL; // ticker invalid

}

double randomMultiplier(double price){
    // generate a random multiplier for updating stock price
    double m = 0;
    double pertubation = 1 / price; // small price has larger potential to pertub than larger one
    m = 1 + pertubation * ((0.5 - ((double)(rand()) / RAND_MAX)) * 2 * MAX_FLUC) / 100; // generate a random pertubered multiplier (pertubation * a random number from -10 to 10) for price
    return m;
}

void priceSimulator(MyStockPrices* pMyStockPrices, char* ticker, int simAll){
    // simulate prices. Price will increase or decrease a random percentage (maximum 10%)
    // simAll = 1 simulate all stock, simAll = 0 simulate user-defined stocks

	StockPrice* pStockInPrice = 0;

    randomMultiplier(1); // must eliminate the first generated random number, since it is NOT random. Probably MacOS specific issue.

    if (simAll){ // all stock prices will be simulated
        for (int i = 0; i < pMyStockPrices->myStocksSize; i++)
            pMyStockPrices->myStockPrices[i].price *= randomMultiplier(pMyStockPrices->myStockPrices[i].price);
    }

    else{ // parsing tickers
    	pStockInPrice = findStockPrice(pMyStockPrices, ticker);
    	if (pStockInPrice)
    		pStockInPrice->price *= randomMultiplier(pStockInPrice->price); // simulate price
    	else
    		printf("\nWARNING: ticker '%s' invalid.\n", ticker);
    }
}
