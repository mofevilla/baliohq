#include "quotemanager.h"

// Currently uses std::cerr for errors.
// TO DO: Throw esceptions and handle in main


QuoteManager::QuoteManager()
{
}

QuoteManager::~QuoteManager()
{
   sqlite3_close(db);
}

QuoteManager::QuoteManager(const QuoteManager& other)
{
   // TO DO: Do not allow copying of database
}

QuoteManager& QuoteManager::operator = (const QuoteManager& other)
{
   // TO DO: Do not allow copying of database
   return *this;
}

QuoteManager::QuoteManager(const char* dbFile)
{
   // Setup DB
   char* errMsg = nullptr;
   int rc = sqlite3_open(dbFile, &db);
   if (rc) std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;

   // Create the table if it doesn't exist
   std::string createTableQuery = "CREATE TABLE IF NOT EXISTS quotes ("
      "QUOTE TEXT PRIMARY KEY UNIQUE,"
      "CUSTOMER TEXT NOT NULL,"
      "WINDOW_MATERIAL TEXT NOT NULL,"
      "WINDOW_SIZE TEXT NOT NULL,"
      "PRICE REAL NOT NULL"
      ");";

   rc = sqlite3_exec(db, createTableQuery.c_str(), nullptr, 0, &errMsg);
   // Check query execution result
   if (rc != SQLITE_OK) {
      std::cerr << "SQL error: " << errMsg << std::endl;
      sqlite3_free(errMsg);
      sqlite3_close(db);
   } 

}

int QuoteManager::Callback(void* data, int argc, char** argv, char** azColname)
{
   std::vector<std::vector<std::string>>* results = static_cast<std::vector<std::vector<std::string>>*>(data);
   std::vector<std::string> row;

   for (int i = 0; i < argc; i++) {
      row.push_back(argv[i] ? argv[i] : "NULL");
   }

  results->push_back(row);

   return 0;
}

void QuoteManager::CreateQuote(const Quote& quote)
{
   std::string sql;
   sql = "INSERT OR IGNORE INTO quotes (QUOTE, CUSTOMER, WINDOW_MATERIAL, WINDOW_SIZE, PRICE) VALUES ('" +
      quote.GetName() + "', '" + quote.GetCustomer() + "', '" + quote.GetWindow().material +
      "', '" + quote.GetWindow().size + "', " + std::to_string(quote.GetPrice()) + ");";

   char* errMsg = 0;
   int rc = sqlite3_exec(db, sql.c_str(), Callback, 0, &errMsg);
   if (rc != SQLITE_OK) {
      std::cerr << "SQL error: " << errMsg << std::endl;
      sqlite3_free(errMsg);
   }

}

void QuoteManager::ViewAllQuotes(std::vector<std::vector<std::string>>& queryResults)
{
   std::string sql = "SELECT ROW_NUMBER() OVER() AS NoId, QUOTE, CUSTOMER, WINDOW_MATERIAL, WINDOW_SIZE, PRICE FROM quotes;";
   //std::string sql = "SELECT * FROM quotes;";

   char* errMsg = nullptr;
   int rc;

   rc = sqlite3_exec(db, sql.c_str(), Callback, &queryResults, &errMsg);

   if (rc != SQLITE_OK) {
      std::cerr << "SQL error: " << errMsg << std::endl;
      sqlite3_free(errMsg);
   }

}
void QuoteManager::UpdateQuote(const Quote& quote)
{
   std::string sql = "UPDATE quotes SET CUSTOMER = '" + quote.GetCustomer() + "', WINDOW_MATERIAL = '"
      + quote.GetWindow().material + "', WINDOW_SIZE = '" +
      quote.GetWindow().size + "', PRICE = '" + std::to_string(quote.GetPrice()) + "' WHERE QUOTE = '" + quote.GetName() + "';";

   char* errMsg = 0;
   int rc = sqlite3_exec(db, sql.c_str(), Callback, 0, &errMsg);
   if (rc != SQLITE_OK) {
      std::cerr << "SQL error: " << errMsg << std::endl;
      sqlite3_free(errMsg);
   }
}

void QuoteManager::DeleteQuote(const std::string& nameOfQuote)
{
   std::string sql = "DELETE FROM quotes WHERE QUOTE = '" + nameOfQuote + "';";
   char* errMsg = 0;
   int rc = sqlite3_exec(db, sql.c_str(), Callback, 0, &errMsg);
   if (rc != SQLITE_OK) {
      std::cerr << "SQL error: " << errMsg << std::endl;
      sqlite3_free(errMsg);
   } 

   std::string sql1 = "REINDEX quotes; ";
   rc = sqlite3_exec(db, sql.c_str(), Callback, 0, &errMsg);
   if (rc != SQLITE_OK) {
      std::cerr << "SQL error: " << errMsg << std::endl;
      sqlite3_free(errMsg);
   } 
}

bool QuoteManager::QuoteExists(std::string& name)
{
   sqlite3_stmt* stmt;
   std::string sql = "SELECT COUNT(*) FROM quotes WHERE QUOTE = '" + name + "';";

   int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
   if (rc != SQLITE_OK) {
      std::cerr << "Failed to prepare query: " << sqlite3_errmsg(db) << std::endl;
      sqlite3_close(db);
      return false;
   }

   rc = sqlite3_step(stmt);
   if (rc == SQLITE_ROW) {
      int count = sqlite3_column_int(stmt, 0);
      if (count > 0) return true;
   } 
   sqlite3_finalize(stmt);
   return false;

}
