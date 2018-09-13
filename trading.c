/*
 * trading.c
 *
 *  Created on: Jul 27, 2018
 *      Author: fanchen
 */

#include "trading.h"

int readPortfolio(char* filename, Portfolio* pMyPortfolio){
    // read in the portfolio file, and return its size or 0 (indicating portfolio doesn't exit)
    FILE* pFile = 0;
    char line[MAX_CAPACITY] = {'\0'};
    char *pLine;
    int lineNum = 1, offset = 0, i = 0;
    int portfolioSize = 0;

    pFile = fopen(filename, "r");

    if(pFile == NULL){
        return 0; // indicating portfolio has not been created yet.
    }
    else{
        while(fgets(line, MAX_CAPACITY - 1, pFile) != NULL){
            pLine = line;
            switch(lineNum){
                case 1:
                    while(sscanf(pLine, " %s%n", pMyPortfolio->myStocks[i].ticker, &offset) > 0){
                        pLine += offset;
                        i++;
                    }
                    break;
                case 2:
                    sscanf(pLine, "%*[^:]:%n", &offset); // consume the non-number bytes
                    pLine += offset;
                    while(sscanf(pLine, " %lf%n", &pMyPortfolio->myStocks[i].buyPrice, &offset) > 0){
                        pLine += offset;
                        i++;
                    }
                    break;
                case 3:
                    sscanf(pLine, "%*[^:]:%n", &offset); // consume the non-number bytes
                    pLine += offset;
                    while(sscanf(pLine, " %d%n", &pMyPortfolio->myStocks[i].shares, &offset) > 0){
                        pLine += offset;
                        i++;
                    }
                    break;
                case 4:
                    sscanf(pLine, "%*[^:]:%n", &offset); // consume the non-number bytes
                    pLine += offset;
                    while(sscanf(pLine, " %lf%n", &pMyPortfolio->myStocks[i].profits, &offset) > 0){
                        pLine += offset;
                        i++;
                    }
                    portfolioSize = i; // record portfolio size
                    break;
                case 5:
                    sscanf(pLine, "%*[^:]:%lf", &pMyPortfolio->balance);
                    break;
                default:
                    printf("ERROR: check the integrity of %s.\n", filename);
                    return 1; // return to main menu
            }
            i = 0;
            offset = 0;
            lineNum++;
            resetCharArray(line);
        }

        if (portfolioSize > pMyPortfolio->myStocksCapacity){ // in case more than 5 stocks are stored in the txt file
            printf("More stocks present than expected. Please check the accuracy of %s.", filename);
            exit(0);
        }
    }

    if(pFile){
        fclose(pFile);
    }
    else{
        printf("An error has occurred while reading %s.\n", filename);
        exit(0);
    }
    return portfolioSize; // read in portfolio successful, return its size
}

int initializePortfolio(char* filename, Portfolio* pMyPortfolio, double balance){
	// initialize portfolio. If one exists already, read the info into myPortfolio struct, otherwise initialize everything to 0
	// return 1 if portfolio already exits, 0 not exists.
	pMyPortfolio->myStocksCapacity = MAX_MY_STOCKS;
	pMyPortfolio->balance = balance;

	int returnRP = readPortfolio(filename, pMyPortfolio);

	if (returnRP)
		pMyPortfolio->myStocksSize = returnRP;
	else{ // portfolio doesn't exist
		for(int i = 0; i < pMyPortfolio->myStocksCapacity; i++){
			for(int j = 0; j < MAX_TICKER_SIZE; j++)
				pMyPortfolio->myStocks[i].ticker[j] = '\0';
			pMyPortfolio->myStocks[i].buyPrice = 0;
			pMyPortfolio->myStocks[i].shares = 0;
			pMyPortfolio->myStocks[i].profits = 0;
		}
		pMyPortfolio->myStocksSize = 0;
		return 0;
	}
	return 1;
}

void saveStockPrice(char* filename, MyStockPrices* pMyStockPrices){
	// save the new stock prices in the mystocks.txt file.
	FILE* pFile = 0;
	int i = 0;
	pFile = fopen(filename, "w");

	for(i = 0; i < pMyStockPrices->myStocksSize; i++)
		fprintf(pFile, "%s %.2lf\n", pMyStockPrices->myStockPrices[i].ticker, pMyStockPrices->myStockPrices[i].price);

	if(pFile)
		fclose(pFile);

	return;
}

void savePortfolio(char* filename, Portfolio* pMyPortfolio){
    // upon user exit, save the current portfolio in the txt file.
    FILE* pFile = 0;
    int i = 0;
    int size = pMyPortfolio->myStocksSize;

    pFile = fopen(filename, "w");

    fprintf(pFile, "\t\t");
    for (i = 0; i < size; i++){
        fprintf(pFile, "%s\t\t", pMyPortfolio->myStocks[i].ticker);
    }

    fprintf(pFile, "\nBuy Price:\t");
    for (i = 0; i < size; i++){
        fprintf(pFile, "%.2lf\t\t", pMyPortfolio->myStocks[i].buyPrice);
    }
    fprintf(pFile, "\nShares:\t\t");
    for (i = 0; i < size; i++){
        if ((int)(log10(pMyPortfolio->myStocks[i].shares)) + 1 >= 8){
            fprintf(pFile, "%d\t", pMyPortfolio->myStocks[i].shares);
        }
        else{
            fprintf(pFile, "%d\t\t", pMyPortfolio->myStocks[i].shares);
        }
    }
    fprintf(pFile, "\nProfits:\t");
    for (i = 0; i < size; i++){
        if(pMyPortfolio->myStocks[i].profits >= 0){
            if ((int)(log10(pMyPortfolio->myStocks[i].profits)) + 1 + 3 >= 8){
                fprintf(pFile, "%.2lf\t", pMyPortfolio->myStocks[i].profits);
            }
            else{
                fprintf(pFile, "%.2lf\t\t", pMyPortfolio->myStocks[i].profits);
            }
        }
        else{
            if ((int)(log10(-pMyPortfolio->myStocks[i].profits)) + 1 + 4 >= 8){
                fprintf(pFile, "%.2lf\t", pMyPortfolio->myStocks[i].profits);
            }
            else{
                fprintf(pFile, "%.2lf\t\t", pMyPortfolio->myStocks[i].profits);
            }
        }
    }

    fprintf(pFile, "\nCurrent Balance: %.2lf\n", getBalance(pMyPortfolio));

    printf("\nYour portfolio has been updated.\n");
    if(pFile){
        fclose(pFile);
    }
}

Stock* findStock(Portfolio* pMyPortfolio, char* ticker){
	// find the Stock struct corresponding to the ticker, and return its address
	for (int i = 0; i < pMyPortfolio->myStocksSize; i++){
		if(!(strcmp(ticker, pMyPortfolio->myStocks[i].ticker))){ // found the ticker in myStocks
			return &(pMyPortfolio->myStocks[i]);
		}
	}

	return NULL; // ticker invalid

}

void printStocks(MyStockPrices* pMyStockPrices, Portfolio* pMyPortfolio, char* ticker, int printAll){
    // print current stock price, but also juxtapose the user's buy price for easier comparison.
    // used to print all stocks or only user-defined stocks
    double buyPrice = 0;
    Stock* pStockInPortfolio = 0;
    StockPrice* pStockInPrice = 0;

    if(printAll){
    	for(int i = 0; i < pMyStockPrices->myStocksSize; i++){
    		pStockInPortfolio = findStock(pMyPortfolio, pMyStockPrices->myStockPrices[i].ticker);
    		if(pStockInPortfolio)
    			buyPrice = pStockInPortfolio->buyPrice;
    		else
    			buyPrice = 0;

    		if(buyPrice != 0 && buyPrice < pMyStockPrices->myStockPrices[i].price) // put an asterik ahead of the ticker to notify user the selling the stock is profitable now
    			printf("*%s:\t%4c%.2lf\t%4c%.2lf\n", pMyStockPrices->myStockPrices[i].ticker, '$', pMyStockPrices->myStockPrices[i].price, '$', buyPrice);
    		else
    			printf("%s:\t%4c%.2lf\t%4c%.2lf\n", pMyStockPrices->myStockPrices[i].ticker, '$', pMyStockPrices->myStockPrices[i].price, '$', buyPrice);
    	}
    }

    else{
    	pStockInPrice = findStockPrice(pMyStockPrices, ticker);
    	if(pStockInPrice){ // ticker exists in myStockPrices
    		pStockInPortfolio = findStock(pMyPortfolio, pStockInPrice->ticker);
    		if(pStockInPortfolio)
    			buyPrice = pStockInPortfolio->buyPrice;
    		else
    			buyPrice = 0;

    		if(buyPrice != 0 && buyPrice < pStockInPrice->price) // put an asterik ahead of the ticker to notify user the selling the stock is profitable now
    			printf("*%s:\t%4c%.2lf\t%4c%.2lf\n", pStockInPrice->ticker, '$', pStockInPrice->price, '$', buyPrice);
    		else
    			printf("%s:\t%4c%.2lf\t%4c%.2lf\n", pStockInPrice->ticker, '$', pStockInPrice->price, '$', buyPrice);
    	}
    }

}

void printPortfolio(Portfolio* pMyPortfolio){
	// print portfolio based on the current myPortfolio struct
	int i = 0;
	int size = pMyPortfolio->myStocksSize;

	printf("\nSummary of Portfolio\n");

	for (i = 0; i <= 16 * (1 + size); i++){
		printf("-");
	}
	printf("\n\t\t");

	for (i = 0; i < size; i++){
		printf("%s\t\t", pMyPortfolio->myStocks[i].ticker);
	}

	printf("\n");
	for (i = 0; i <= 16 * (1 + size); i++){
		printf("-");
	}

	printf("\nBuy Price:\t");
	for (i = 0; i < size; i++){
		printf("%.2lf\t\t", pMyPortfolio->myStocks[i].buyPrice);
	}
	printf("\nShares:\t\t");
	for (i = 0; i < size; i++){
		if ((int)(log10(pMyPortfolio->myStocks[i].shares)) + 1 >= 8){
			printf("%d\t", pMyPortfolio->myStocks[i].shares);
		}
		else{
			printf("%d\t\t", pMyPortfolio->myStocks[i].shares);
		}
	}
	printf("\nProfits:\t");
	for (i = 0; i < size; i++){
		if(pMyPortfolio->myStocks[i].profits >= 0){
			if ((int)(log10(pMyPortfolio->myStocks[i].profits)) + 1 + 3 >= 8){
				printf("%.2lf\t", pMyPortfolio->myStocks[i].profits);
			}
			else{
				printf("%.2lf\t\t", pMyPortfolio->myStocks[i].profits);
			}
		}
		else{
			if ((int)(log10(-pMyPortfolio->myStocks[i].profits)) + 1 + 4 >= 8){
				printf("%.2lf\t", pMyPortfolio->myStocks[i].profits);
			}
			else{
				printf("%.2lf\t\t", pMyPortfolio->myStocks[i].profits);
			}
		}
	}
	printf("\n");

	for (i = 0; i <= 16 * (1 + size); i++){
		printf("-");
	}

	printf("\n\nCurrent Balance: %.2lf\n", getBalance(pMyPortfolio));
}

int priceCheck(MyStockPrices* pMyStockPrices, Portfolio* pMyPortfolio, int userChoice, char* userTickers){
    // allow user to check price for all stocks or user-defined stocks
	char* pTickers = userTickers; // a pointer to the unparsed tickers
	char chosenTic[MAX_TICKER_SIZE] = {'\0'}; // store each chosen ticker in the tickers
	int offset = 0;

	switch(userChoice){
        case 1:
            priceSimulator(pMyStockPrices, "", 1);
            printf("\n\tCurrent Price\tBuy Price\n");
            printStocks(pMyStockPrices, pMyPortfolio, "", 1);
            break;
        case 2:
            printf("\n\tCurrent Price\tBuy Price\n");
            while(sscanf(pTickers, " %s%n", chosenTic, &offset) > 0){
            	priceSimulator(pMyStockPrices, chosenTic, 0);
            	printStocks(pMyStockPrices, pMyPortfolio, chosenTic, 0);
            	resetCharArray(chosenTic);
            	pTickers += offset;
            }

            break;
        case 3:
            return 1; // return to main menu
        case 0:
            return 0; // exit program
        default:
            printf("\nThe program encounters a fatal error in price check. Program terminated");
            exit(0);

    }
    return 2; // another round of price check
}

void buy(MyStockPrices* pMyStockPrices, Portfolio* pMyPortfolio, char* portfolioFile, char* buyTicker, int buyShare){
    // allow user to buy one stock at a time. user can press C to cancel at any time

    // buy-related
	StockPrice* pStockInPrice = findStockPrice(pMyStockPrices, buyTicker);
    Stock* pStockInPortfolio = findStock(pMyPortfolio, buyTicker);
	double price = pStockInPrice->price; // price at which user buys
    double cost = buyShare * price; // cost of the purchase

    //make the purchase
    pMyPortfolio->balance -= cost;
    if (pStockInPortfolio){ // ticker already exists in portfolio
        pStockInPortfolio->profits -= cost;

        // new buy price is the average between the current buy price and previous buy price of the stock
        pStockInPortfolio->buyPrice = ((pStockInPortfolio->buyPrice * pStockInPortfolio->shares) + cost) / (pStockInPortfolio->shares + buyShare);

        pStockInPortfolio->shares += buyShare;
    }
    else{ // ticker needs to be added
        strcpy(pMyPortfolio->myStocks[pMyPortfolio->myStocksSize].ticker, buyTicker);
        pMyPortfolio->myStocks[pMyPortfolio->myStocksSize].shares += buyShare;
        pMyPortfolio->myStocks[pMyPortfolio->myStocksSize].profits -= cost;
        pMyPortfolio->myStocks[pMyPortfolio->myStocksSize].buyPrice = price;
        (pMyPortfolio->myStocksSize)++;
    }

    printf("\n...Buying %d shares of '%s'\n", buyShare, buyTicker);

    printf("\n+-----------------------------------+\n");
    printf("|         BUY Successful!           |\n");
    printf("+-----------------------------------+\n");
    savePortfolio(portfolioFile, pMyPortfolio);
    return; // back to main menu
}

void sell(MyStockPrices* pMyStockPrices, Portfolio* pMyPortfolio, char* portfolioFile, char* sellTicker, int sellShare){
    // allow user to sell one stock at a time, user can press C to cancel
	// sell-related
	StockPrice* pStockInPrice = findStockPrice(pMyStockPrices, sellTicker);
	Stock* pStockInPortfolio = findStock(pMyPortfolio, sellTicker);
    double price = pStockInPrice->price; // price at which user sells
    double revenue = sellShare * price; // money gathered after the sell

    //make the sell
    pMyPortfolio->balance += revenue;
    pStockInPortfolio->shares -= sellShare;
    if (pStockInPortfolio->shares == 0) // when current share is 0, set the buy price to 0
    	pStockInPortfolio->buyPrice = 0;
    pStockInPortfolio->profits += revenue;

    printf("\n...Selling %d shares of '%s'\n", sellShare, sellTicker);

    printf("\n+-----------------------------------+\n");
    printf("|        SELL Successful!           |\n");
    printf("+-----------------------------------+\n");
    savePortfolio(portfolioFile, pMyPortfolio);
    return; // return to main menu
}

double getBalance(Portfolio* pMyPortfolio){
	return pMyPortfolio->balance;
}



