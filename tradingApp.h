/*
 * tradingApp.h
 *
 *  Created on: Jul 27, 2018
 *      Author: fanchen
 */

#ifndef TRADINGAPP_H_
#define TRADINGAPP_H_

#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h> // tolower()
#include <time.h>
#include "simulator.h"
#include "trading.h"
#include "utility.h"


int getMainMenuChoice();
int getTradingInputs(MyStockPrices* pMyStockPrices, Portfolio *pMyPortfolio, int mainMenuChoice, int* pPriceCheckChoice, char* priceCheckTickers, char* buyTicker, int* pBuyShare, char* sellTicker, int* pSellShare, int* pHasPortfolio);
int getPriceCheckChoice();
void getPriceCheckTickers(int priceCheckChoice, char* priceCheckTickers);
int getBuyTicker(MyStockPrices* pMyStockPrices, char* buyTicker);
int getBuyShare(MyStockPrices* pMyStockPrices, Portfolio* pMyPortfolio, char* buyTicker);
int getSellTicker(MyStockPrices* pMyStockPrices, Portfolio* pMyPortfolio, char* sellTicker);
int getSellShare(Portfolio* pMyPortfolio, char* sellTicker);
char getExitConfirm();
int mainMenu(MyStockPrices* pMyStockPrices, Portfolio *pMyPortfolio, char* priceFile, char* portfolioFile, int mainMenuChoice, int priceCheckChoice, char* priceCheckTickers, char* buyTicker, int buyShare, char* sellTicker, int sellShare);
int confirmExit(MyStockPrices* pMyStockPrices, Portfolio *pMyPortfolio, char* priceFile, char* portfolioFile, char exitConfirm);
void process();



#endif /* TRADINGAPP_H_ */
