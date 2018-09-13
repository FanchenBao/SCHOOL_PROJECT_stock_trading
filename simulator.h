/*
 * simulator.h
 *
 *  Created on: Jul 26, 2018
 *      Author: fanchen
 */

#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include "utility.h"

#define MAX_TICKER_SIZE 10 //maximum size to store a ticker
#define MAX_MY_STOCKS 5 // maximum 5 stocks to be traded
#define MAX_CAPACITY 200 // max length for a char array to read in a line
#define MAX_FLUC 10 // maximum fluctuation of stock price is 10%

typedef struct{
    char ticker[MAX_TICKER_SIZE]; // record ticker
    double price; // current price. modified each time priceSimulate() is called
} StockPrice;

typedef struct{
	StockPrice myStockPrices[MAX_MY_STOCKS];
	int myStocksSize;
	int myStocksCapacity;
} MyStockPrices;

int readStockPrices(char* filename, MyStockPrices* pMyStockPrices);
void initializeSimulator(char* filename, MyStockPrices* pMyStockPrices);
StockPrice* findStockPrice(MyStockPrices* pMyStockPrices, char* ticker);
double randomMultiplier(double price);
void priceSimulator(MyStockPrices* pMyStockPrices, char* tickers, int simAll);




#endif /* SIMULATOR_H_ */
