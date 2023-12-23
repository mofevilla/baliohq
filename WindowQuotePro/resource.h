#pragma once

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <CommCtrl.h>

// Stored instance handle for use in Win32 API calls such as FindResource
static HINSTANCE g_hInst;
// The main window class name.
static TCHAR szWindowClass[] = _T("WindowQuotePro");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Window Quote Pro");


enum {
   ID_BUTTON_CMD_CREATE = 1,
   ID_BUTTON_CMD_VIEW,
   ID_BUTTON_CMD_UPDATE,
   ID_BUTTON_CMD_DELETE,
   ID_BUTTON_CMD_EXIT,
   ID_LISTVIEW,
   ID_STATIC_QUOTE_NAME,
   ID_EDIT_QUOTE_NAME,
   ID_STATIC_CUSTOMER_NAME,
   ID_EDIT_CUSTOMER_NAME,
   ID_STATIC_DOOR_MATERIAL,
   ID_PDL_DOOR_MATERIAL,
   ID_STATIC_DOOR_SIZE,
   ID_PDL_DOOR_SIZE,
   ID_STATIC_PRICE_LABEL,
   ID_STATIC_PRICE,
};

class ResourceManager {
public:
   HWND AddButton(HWND hWnd, LPCWSTR lpLabel, int iID, int iX, int iY, int iWidth, int iHeight);
   HWND AddList(HWND hWnd, LPCWSTR lpLabel, int iID, int iX, int iY, int iWidth, int iHeight);
   void AddColumn(HWND hwndList, LPCWSTR lpLabel, int iCol, int iWidth);
   void AddRow(HWND hwndList, LPCWSTR lpText, int iRow, int iCol);
   HWND AddPullDownList(HWND hWnd, int iID, int iX, int iY, int iWidth, int iHeight);
   int AddPDLString(HWND hWnd, int iID, LPCWSTR szString);
   HWND AddEdit(HWND hWnd, int iID, int iX, int iY, int iWidth, int iHeight);
   HWND AddText(HWND hWnd, LPCWSTR lpLabel, int iID, int iX, int iY, int iWidth, int iHeight);
   void ClearListView(HWND hWnd, int iID);
   void DeleteListViewItem(HWND hWnd, int iID, int iRow);
   void SetListViewColumnString(HWND hWnd, int iID, int iRow, int iCol, LPCWSTR lpText);
   void GetText(HWND hWnd, int iID, LPWSTR lpText);
};
