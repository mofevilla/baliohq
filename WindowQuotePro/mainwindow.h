#pragma once

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <CommCtrl.h>
#include "quotemanager.h"

#include "resource.h"

// Forward declarations of functions included in this code module:
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

class MainWindow {
public:
   MainWindow();
   ~MainWindow();
   QuoteManager* manager;

   LRESULT WndProcImpl(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
protected:
   virtual void EvWmCreate(HWND hWnd);
   virtual void EvWmCommand(HWND hWnd, WPARAM wParam);
   Quote MainWindow::ExtractQuoteFromInput(HWND hWnd);
   void CreateNewQuote(HWND hWnd);
   void ViewAllQuotes(HWND hWnd);
   void DeleteQuote(HWND hWnd);
   void UpdateQuote(HWND hWnd);
   void UpdatePrice(HWND hWnd);

private:
   ResourceManager resourceMgr; // shared_ptr
   HWND hwndList;
};
