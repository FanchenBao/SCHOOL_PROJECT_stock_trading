/*
 * trading.h
 *
 *  Created on: Jul 27, 2018
 *      Author: fanchen
 */

#ifndef TRADING_H_
#define TRADING_H_

#define _CRT_SECURE_NO_WARNINGS
#include <math.h> // log10()
#include "simulator.h"
#include "utility.h"

#define MAX_TICKER_SIZE 10 //maximum size to store a ticker
#define MAX_MY_STOCKS 5 // maximum 5 stocks to be traded
#define MAX_CAPACITY 200 // max length for a char array to read in a line

typedef struct{
	char ticker[MAX_TICKER_SIZE];
	int shares;
	double buyPrice;
	double profits;
} Stock;

typedef struct{
	Stock myStocks[MAX_MY_STOCKS]; // MAX_MY_STOCKS is five.
	double balance;
	int myStocksSize;
	int myStocksCapacity;  //Should be set to MAX_MY_STOCKS
} Portfolio;

int readPortfolio(char* filename, Portfolio* pMyPortfolio);
int initializePortfolio(char* filename, Portfolio* pMyPortfolio, double balance);
void saveStockPrice(char* filename, MyStockPrices* pMyStockPrices);
void savePortfolio(char* filename, Portfolio* pMyPortfolio);
void printStocks(MyStockPrices* pMyStockPrices, Portfolio* pMyPortfolio, char* ticker, int printAll);
void printPortfolio(Portfolio* pMyPortfolio);
Stock* findStock(Portfolio* pMyPortfolio, char* ticker);
int priceCheck(MyStockPrices* pMyStockPrices, Portfolio* pMyPortfolio, int userChoice, char* userTickers);
void buy(MyStockPrices* pMyStockPrices, Portfolio* pMyPortfolio, char* portfolioFile, char* buyTicker, int buyShare);
void sell(MyStockPrices* pMyStockPrices, Portfolio* pMyPortfolio, char* portfolioFile, char* sellTicker, int sellShare);
double getBalance(Portfolio* pMyPortfolio);

#endif /* TRADING_H_ */
