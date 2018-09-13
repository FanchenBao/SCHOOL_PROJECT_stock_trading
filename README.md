# SchoolProject_stock_trading
Simulates stock trading processes with random stock price fluctuations, buy stock, sell stock, and keep a user portfolio

_School project from COP2220 in Summer 2018_

* User has five stocks to choose from, whether to buy and sell.
* User has $20,000 starting fund, and should try to maximize profit with trading.
* __project3main.c__ is the driver file. The rest are header files and their corresponding .c files.
* User has five options at the starting menu:
  * Check my portfolio: check the current state of portfolio, which should show what stocks user has bought and sold at what price, as well as the current profit acquired from each stock. If no porfolio is availabe, a notice will be printed to the console.
  * Check current stock price: each time user checks stock price, a random fluctuation is generated for the stocks such that each check can result in a new price. Details of how random fluctuation is achieved are in the simulator.c. In addition, user can choose to check specific stocks or all stocks. And each check displays the current new stock price, and user's buying price of certain stocks to aid decision making on when to sell.
  * Buy share: 
    * Purchase new shares of a certain stock (contingent on how much fund still available) at the price displayed in the last time user runs "Check current stock price". In other words, stock price does not change in the time user switches from check stock price to buy shares.
    * Only whole shares can be bought. User is notified the maximum number of shares he/she can buy based on the remaining fund.
    * After the buy, the current buy price of a certain stock is the weighted average of previous purchase and the current one. In other words, if a stock is bought previously at a high price, but now it is bought again at a lower price, the new buy price will be the weighted average of the two purchases (the new buy price is between the high and low prices)
    * User's portfolio is updated after each buy.
  * Sell share:
    * Sell shares (if user has not bought any share of a stock, error message will be displayed) at the current stock price.
    * Only whole shares can be sold. User is notified the maximum number of shares he/she can sell.
    * User's portfolio is updated after each sell.
  * Exit: exit the program.
* A new file called "portfolio.txt" will be created and updated when user uses the program. It stores the porfolio information. It is loaded to the program the next time user uses the program. However, if user wishes to start over, "portfolio.txt" should be deleted.
