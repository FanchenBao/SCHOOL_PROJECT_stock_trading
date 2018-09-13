/*
* Stock Market Simulator
* project3.c
* AUTHOR: Fanchen Bao
* DATE: July 26-27, 2018
*/

#define _CRT_SECURE_NO_WARNINGS
#include "simulator.h"
#include "trading.h"
#include "utility.h"
#include "tradingApp.h"

void test3(){
	// structs
	MyStockPrices myStockPrices;
	Portfolio myPortfolio;
	StockPrice* testSP;
	Stock* testStock;

	// user input
//	int priceCheckChoice = 0;
//	char priceCheckTickers[MAX_CAPACITY] = {'\0'};
//	int buyShare = 0;
//	char buyTicker[MAX_CAPACITY] = {'\0'};
//	int sellShare = 0;
//	char sellTicker[MAX_CAPACITY] = {'\0'};

	double balance = 20000;
	double testMultiplier = 0;

	// file read and save
	char* priceFile = "mystocks.txt";
	char* portfolioFile = "portfolio.txt";

	srand(time(0)); // seed random

	// test banner()
	banner(1, balance);

	// test initializeSimulator() and printStocks()
	initializeSimulator(priceFile, &myStockPrices);
	printStocks(&myStockPrices, &myPortfolio, "", 1);

	// test readStockPrices() and printStocks()
	readStockPrices(priceFile, &myStockPrices);
	printStocks(&myStockPrices, &myPortfolio, "", 1);

	// test findStockPrice()
	testSP = findStockPrice(&myStockPrices, "ROSE");
	printf("Ticker: %s, Price: %lf\n", testSP->ticker, testSP->price);

	// test randomMultiplier()
	testMultiplier = randomMultiplier(8.01);
	printf("testMultiplier is %lf\n", testMultiplier);

	// test priceSimulator() for simulating all stocks, and printStocks()
	priceSimulator(&myStockPrices, "", 1);
	printStocks(&myStockPrices, &myPortfolio, "", 1);

	// test priceSimulator() for simulating one stock, and printStocks()
	for (int j = 0; j < 5; j++){
		priceSimulator(&myStockPrices, "ROSE", 0);
		printf("\n");
		printStocks(&myStockPrices, &myPortfolio, "", 1);
	}

	// test initializePortfolio() and printPortfolio()
	initializePortfolio(portfolioFile, &myPortfolio, balance);
	printPortfolio(&myPortfolio);

	// test getBalance()
	printf("Current balance is: %lf\n", getBalance(&myPortfolio));

	// test findStock()
	testStock = findStock(&myPortfolio, "BABY");
	if(testStock)
		printf("Ticker: %s, Buy Price: %lf, Shares: %d, Profits: %lf\n", testStock->ticker, testStock->buyPrice, testStock->shares, testStock->profits);
	else
		printf("Stock is not in your portfolio yet.\n");

	// test getPriceCheckChoice()
//	priceCheckChoice = getPriceCheckChoice();
//	printf("Price check choice is %d\n", priceCheckChoice);

	// test getPriceCheckTickers
//	getPriceCheckTickers(2, priceCheckTickers);
//	printf("The ticker you input is '%s'", priceCheckTickers);

	// test getBuyTicker()
//	getBuyTicker(&myStockPrices, buyTicker);
//	printf("The ticker you input is '%s'\n", buyTicker);

	// test getBuyShare()
//	buyShare = getBuyShare(&myStockPrices, &myPortfolio, "HONE");
//	printf("The share you want to buy is: %d\n", buyShare);

	// test getSellTicker()
//	getSellTicker(&myStockPrices, &myPortfolio, sellTicker);
//	printf("The ticker you input is '%s'\n", sellTicker);

	// test getSellShare()
//	sellShare = getSellShare(&myPortfolio, "MUDS");
//	printf("The share you want to sell is: %d\n", sellShare);


	// test priceCheck()
	priceCheck(&myStockPrices, &myPortfolio, 2, "ROSE");

	// test buy()
	buy(&myStockPrices, &myPortfolio, portfolioFile, "MUDS", 10);

	// test sell()
	sell(&myStockPrices, &myPortfolio, portfolioFile, "MUDS", 10);

	// test findStock() again
	testStock = findStock(&myPortfolio, "MUDS");
	if(testStock)
		printf("Ticker: %s, Buy Price: %lf, Shares: %d, Profits: %lf\n", testStock->ticker, testStock->buyPrice, testStock->shares, testStock->profits);
	else
		printf("Stock is not in your portfolio yet.\n");

	// test savePortfolio()
	savePortfolio(portfolioFile, &myPortfolio);

	// test saveStockPrice()
	saveStockPrice(priceFile, &myStockPrices);


}

void test4(){
    // structs
    MyStockPrices myStockPrices;
    Portfolio myPortfolio;

    // user input
    int priceCheckChoice = 0;
    char priceCheckTickers[MAX_CAPACITY] = {'\0'};
    int mainMenuChoice = 0;
//    char exitConfirm = 0;
    int buyShare = 0;
    char buyTicker[MAX_CAPACITY] = {'\0'};
    int sellShare = 0;
    char sellTicker[MAX_CAPACITY] = {'\0'};

    // function return values
    int returnMM = 1; // return from mainMenu(), initialize to 1 such that the app won't quit when user just wants to get back to main menu
//    int returnCE = 0; // return from confirmExit()
    int hasPortfolio = 0;

    // file read and save
    char* priceFile = "mystocks.txt";
    char* portfolioFile = "portfolio.txt";

    // initial balance
    double balance = 20000;


    srand(time(0)); // seed random

    initializeSimulator(priceFile, &myStockPrices);
    hasPortfolio = initializePortfolio(portfolioFile, &myPortfolio, balance);


    // test getMainMenuChoice()
//    mainMenuChoice = getMainMenuChoice();
//    printf("Main menu choice is %d\n", mainMenuChoice);


    // test getExitConfirm()
//    exitConfirm = getExitConfirm();
//    printf("The exit confirm value you input is '%c'", exitConfirm);

    // test confirmExit()
    confirmExit(&myStockPrices, &myPortfolio, priceFile, portfolioFile, 'y');

    // test getTradingInputs() and mainMenu(), i.e. testing the main loop of the app
    mainMenuChoice = 1;
    while(1){
    	if(!getTradingInputs(&myStockPrices, &myPortfolio, mainMenuChoice, &priceCheckChoice, priceCheckTickers, buyTicker, &buyShare, sellTicker, &sellShare, &hasPortfolio))
    		break; // return to main menu

    	returnMM = mainMenu(&myStockPrices, &myPortfolio, priceFile, portfolioFile, mainMenuChoice, priceCheckChoice, priceCheckTickers, buyTicker, buyShare, sellTicker, sellShare);
    	if (returnMM != 2)
    		break; // return to main menu
    }


}

int main() {
//    test3();
//    test4();
    process(); // main app function from tradingApp.c
    return 0;
}
