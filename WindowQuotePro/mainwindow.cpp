#include<vector>
#include<map>
#include "mainwindow.h"
#include "resource.h"
#define DB_NAME "quotes.db"

// Used for pricing
// TO DO: Replace with third party service class
std::map<std::string, double> materialFactor = {
   { "Wood", 15.0 },
   { "Metal", 25.0 }
};

std::map<std::string, double> sizeFactor = {
   { "Small", 1.0 },
   { "Medium", 2.0 },
   { "Large", 3.0 }
};

double GetPrice(Window& window)
{
   // Dummy Pricer
   // Price based on material and size
   return materialFactor[window.material] * sizeFactor[window.size];
}


// Function to convert std::string to LPCWSTR
// To move to main if nothing else will use this
LPCWSTR ConvertToLPCWSTR(const std::string& narrowString)
{
   int len = MultiByteToWideChar(CP_UTF8, 0, narrowString.c_str(), -1, nullptr, 0);
   wchar_t* wideString = new wchar_t[len];
   MultiByteToWideChar(CP_UTF8, 0, narrowString.c_str(), -1, wideString, len);
   return wideString;
}

// Function to convert LPCWSTR to std::string
// To move to main if nothing else will use this
std::string ConvertToStdString(LPCWSTR wideString)
{
   int len = WideCharToMultiByte(CP_UTF8, 0, wideString, -1, nullptr, 0, nullptr, nullptr);
   char* narrowString = new char[len];
   WideCharToMultiByte(CP_UTF8, 0, wideString, -1, narrowString, len, nullptr, nullptr);
   std::string result(narrowString);
   delete[] narrowString;
   return result;
}

MainWindow::MainWindow()
{
   // Setup database
   manager = new QuoteManager(DB_NAME);
}

MainWindow::~MainWindow()
{
   delete manager;
}

LRESULT MainWindow::WndProcImpl(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   switch (message) {
      case WM_CREATE:
         EvWmCreate(hWnd);
         break;

      case WM_COMMAND:
         EvWmCommand(hWnd, wParam);
         break;

      case WM_DESTROY:
         PostQuitMessage(0);
         break;

      default:
         return DefWindowProc(hWnd, message, wParam, lParam);
         break;
   }

   return 0;
}

void MainWindow::EvWmCreate(HWND hWnd)
{
   // Layout constants
   const int iSpacerX = 10;
   const int iSpacerY = 5;
   const int iLabelX = 20;
   const int iLabelWidth = 120;
   const int iValueX = (iLabelX + iSpacerX) + iLabelWidth;
   const int iResourceWidth = 100;
   const int iResourceHeight = 25;
   int iY = 50;

   // Text labels and corresponding edit controls for Quote Name and Customer Name
   resourceMgr.AddText(hWnd, TEXT("Quote Name:"), ID_STATIC_QUOTE_NAME, iLabelX, iY, iLabelWidth, iResourceHeight);
   resourceMgr.AddEdit(hWnd, ID_EDIT_QUOTE_NAME, iValueX, iY, iResourceWidth, iResourceHeight);
   iY += iResourceHeight + iSpacerY;
   resourceMgr.AddText(hWnd, TEXT("Customer Name:"), ID_STATIC_QUOTE_NAME, iLabelX, iY, iLabelWidth, iResourceHeight);
   resourceMgr.AddEdit(hWnd, ID_EDIT_CUSTOMER_NAME, iValueX, iY, iResourceWidth, iResourceHeight);
   iY += iResourceHeight + iSpacerY;

   // Text labels and pdls for Window Material and Window Size
   resourceMgr.AddText(hWnd, TEXT("Window Material:"), ID_STATIC_DOOR_MATERIAL, iLabelX, iY, iLabelWidth, iResourceHeight);
   resourceMgr.AddPullDownList(hWnd, ID_PDL_DOOR_MATERIAL, iValueX, iY, iResourceWidth, iResourceHeight * 3);
   resourceMgr.AddPDLString(hWnd, ID_PDL_DOOR_MATERIAL, TEXT("Metal"));
   resourceMgr.AddPDLString(hWnd, ID_PDL_DOOR_MATERIAL, TEXT("Wood"));
   iY += iResourceHeight + iSpacerY;

   resourceMgr.AddText(hWnd, TEXT("Window Size:"), ID_STATIC_DOOR_SIZE, iLabelX, iY, iLabelWidth, iResourceHeight);
   resourceMgr.AddPullDownList(hWnd, ID_PDL_DOOR_SIZE, iValueX, iY, iResourceWidth, iResourceHeight * 4);
   resourceMgr.AddPDLString(hWnd, ID_PDL_DOOR_SIZE, TEXT("Small"));
   resourceMgr.AddPDLString(hWnd, ID_PDL_DOOR_SIZE, TEXT("Medium"));
   resourceMgr.AddPDLString(hWnd, ID_PDL_DOOR_SIZE, TEXT("Large"));
   iY += iResourceHeight + iSpacerY;

   // Text label and an empty label for displaying the price
   resourceMgr.AddText(hWnd, TEXT("Price:"), ID_STATIC_PRICE_LABEL, iLabelX, iY, iLabelWidth, iResourceHeight);
   resourceMgr.AddText(hWnd, TEXT(""), ID_STATIC_PRICE, iValueX, iY, iResourceWidth, iResourceHeight);
   iY += (iResourceHeight + iSpacerY) * 2;

   // Buttons for Create, View, Update, Delete, and Exit
   resourceMgr.AddButton(hWnd, TEXT("Create"), ID_BUTTON_CMD_CREATE, iValueX, iY, iResourceWidth, iResourceHeight);
   iY += iResourceHeight + iSpacerY;
   // Remove view button for now as we are showing Listview all the time
   //resourceMgr.AddButton(hWnd, TEXT("View"), ID_BUTTON_CMD_VIEW, iValueX, iY, iResourceWidth, iResourceHeight);
   //iY += iResourceHeight + iSpacerY;
   resourceMgr.AddButton(hWnd, TEXT("Update"), ID_BUTTON_CMD_UPDATE, iValueX, iY, iResourceWidth, iResourceHeight);
   iY += iResourceHeight + iSpacerY;
   resourceMgr.AddButton(hWnd, TEXT("Delete"), ID_BUTTON_CMD_DELETE, iValueX, iY, iResourceWidth, iResourceHeight);
   // Remove exit button for now
   //iY += iResourceHeight + iSpacerY;
   //resourceMgr.AddButton(hWnd, TEXT("Exit"), ID_BUTTON_CMD_EXIT, iValueX, iY, iResourceWidth, iResourceHeight);

   // Listview its column names
   hwndList = resourceMgr.AddList(hWnd, TEXT("LV"), ID_LISTVIEW, iValueX + iResourceWidth + iSpacerX, 20, 710, 420);
   resourceMgr.AddColumn(hwndList, TEXT("ROW_NUM"), 0, 80);
   resourceMgr.AddColumn(hwndList, TEXT("QUOTE"), 1, 140);
   resourceMgr.AddColumn(hwndList, TEXT("CUSTOMER"), 2, 140);
   resourceMgr.AddColumn(hwndList, TEXT("WINDOW_MATERIAL"), 3, 130);
   resourceMgr.AddColumn(hwndList, TEXT("WINDOW_SIZE"), 4, 130);
   resourceMgr.AddColumn(hwndList, TEXT("PRICE"), 5, 85);

   // Show updated Listview
   ViewAllQuotes(hWnd);
}

void MainWindow::EvWmCommand(HWND hWnd, WPARAM wParam)
{
   switch (LOWORD(wParam)) {
      case ID_PDL_DOOR_MATERIAL: // Intentional fall-through
      case ID_PDL_DOOR_SIZE: {
         // Handle only when selection changed
         if (HIWORD(wParam) == CBN_SELCHANGE) UpdatePrice(hWnd);
         break;
      }
   case ID_BUTTON_CMD_CREATE: 
      CreateNewQuote(hWnd);
    break;

   case ID_BUTTON_CMD_VIEW: 
      ViewAllQuotes(hWnd);
      break;

   case ID_BUTTON_CMD_UPDATE:
      UpdateQuote(hWnd);
      break;

   case ID_BUTTON_CMD_DELETE: 
      DeleteQuote(hWnd);
      break;

   case ID_BUTTON_CMD_EXIT:
      resourceMgr.ClearListView(hWnd, ID_LISTVIEW);
      break;

   default:
      break;
   }
}

void MainWindow::UpdatePrice(HWND hWnd)
{ 
   // Only called when pull-down list selection changed

   // Get values from pull-down list
   wchar_t windowMaterial[MAX_PATH];
   wchar_t windowSize[MAX_PATH];
   resourceMgr.GetText(hWnd, ID_PDL_DOOR_MATERIAL, windowMaterial);
   resourceMgr.GetText(hWnd, ID_PDL_DOOR_SIZE, windowSize);

   // Return when either material or size pull-down list is blank
   if ((std::wcscmp(windowMaterial, L"") == 0) || (std::wcscmp(windowSize, L"") == 0)) return;

   Window window(ConvertToStdString(windowMaterial), ConvertToStdString(windowSize));
   // Generate price from selection
   double price = GetPrice(window);

   // Update price control
   resourceMgr.AddText(hWnd, ConvertToLPCWSTR(std::to_string(price)), ID_STATIC_PRICE, 150, 170, 100, 25);
}

Quote MainWindow::ExtractQuoteFromInput(HWND hWnd)
{
   // Retrieve text from controls
   wchar_t name[MAX_PATH];
   wchar_t customer[MAX_PATH];
   wchar_t windowMaterial[MAX_PATH];
   wchar_t windowSize[MAX_PATH];

   resourceMgr.GetText(hWnd, ID_EDIT_QUOTE_NAME, name);
   resourceMgr.GetText(hWnd, ID_EDIT_CUSTOMER_NAME, customer);
   resourceMgr.GetText(hWnd, ID_PDL_DOOR_MATERIAL, windowMaterial);
   resourceMgr.GetText(hWnd, ID_PDL_DOOR_SIZE, windowSize);

   // Check if any field is missing
   if ((std::wcscmp(name, L"") == 0) || (std::wcscmp(customer, L"") == 0) ||
       (std::wcscmp(windowMaterial, L"") == 0) || (std::wcscmp(windowSize, L"") == 0)) {
      MessageBox(hWnd, L"Cannot add/modify quote with missing field/s.", L"Info", MB_OK);
      return Quote(); // Returning an empty Quote object
   }

   Window window(ConvertToStdString(windowMaterial), ConvertToStdString(windowSize));
   double price = GetPrice(window);
   return Quote(ConvertToStdString(name), ConvertToStdString(customer), window, price);
}

void MainWindow::CreateNewQuote(HWND hWnd)
{
   Quote newQuote = ExtractQuoteFromInput(hWnd);
   if (newQuote.GetName().empty()) {
      // Error handling, as newQuote is empty
      return;
   }
   // Create the new quote
   manager->CreateQuote(newQuote);

   //Update Listview
   ViewAllQuotes(hWnd);
}

void MainWindow::ViewAllQuotes(HWND hWnd)
{
   // Clear the list view
   resourceMgr.ClearListView(hWnd, ID_LISTVIEW);
   std::vector<std::vector<std::string>> results;
   manager->ViewAllQuotes(results);

   // Display all quotes in the list view
   for (int i = 0; i < results.size(); ++i) {
      for (int j = 0; j < results[i].size(); ++j) {
         resourceMgr.AddRow(hwndList, ConvertToLPCWSTR(results[i][j]), i, j);
      }
   }
}

void MainWindow::DeleteQuote(HWND hWnd)
{
   wchar_t name[MAX_PATH];
   resourceMgr.GetText(hWnd, ID_EDIT_QUOTE_NAME, name);
   // Delete quote based on quote name
   manager->DeleteQuote(ConvertToStdString(name));
   ViewAllQuotes(hWnd);
}

void MainWindow::UpdateQuote(HWND hWnd)
{
   Quote newQuote = ExtractQuoteFromInput(hWnd);
   // Update the quote
   manager->UpdateQuote(newQuote);

   //Update Listview
   ViewAllQuotes(hWnd);
}