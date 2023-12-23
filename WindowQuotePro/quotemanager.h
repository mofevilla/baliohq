#ifndef quotemanager_h
#define quotemanager_h
#include <iostream>
#include <string>
#include "sqlite3.h"
#include "quote.h"
#include<vector>

class QuoteManager {
private:
   sqlite3* db;
   static int Callback(void* data, int argc, char** argv, char** azColName);

public:
   QuoteManager();
   ~QuoteManager();
   QuoteManager(const char* dbFile);
   QuoteManager(const QuoteManager& other);
   QuoteManager& operator=(const QuoteManager& other);

   void CreateQuote(const Quote& quote);
   void ViewAllQuotes(std::vector<std::vector<std::string>>& queryResults);
   void DeleteQuote(const std::string& name);
   void UpdateQuote(const Quote& quote);
   bool QuoteExists(std::string& name);
};


#endif //quotemanager_h