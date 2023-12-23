#include "resource.h"

HWND ResourceManager::AddButton(HWND hWnd, LPCWSTR lpLabel, int iID, int iX, int iY, int iWidth, int iHeight)
{
   return CreateWindow(TEXT("button"), lpLabel,
      WS_VISIBLE | WS_CHILD,
      iX, iY, iWidth, iHeight,
      hWnd, (HMENU)iID, NULL, NULL);
}

HWND ResourceManager::AddList(HWND hWnd, LPCWSTR lpLabel, int iID, int iX, int iY, int iWidth, int iHeight)
{
   return CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, lpLabel,
      WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_EDITLABELS | LVS_EX_GRIDLINES,
      iX, iY, iWidth, iHeight,
      hWnd, (HMENU)ID_LISTVIEW, g_hInst, NULL);
}

void ResourceManager::AddColumn(HWND hwndList, LPCWSTR lpLabel, int iCol, int iWidth)
{
   LVCOLUMN lvc;
   lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
   lvc.fmt = LVCFMT_LEFT;
   lvc.cx = iWidth;
   lvc.pszText = LPWSTR(lpLabel);
   lvc.iSubItem = iCol;
   ListView_InsertColumn(hwndList, iCol, &lvc);
}

void ResourceManager::AddRow(HWND hwndList, LPCWSTR lpText, int iRow, int iCol)
{

   LV_ITEM lvi;
   lvi.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
   lvi.iItem = ListView_GetItemCount(hwndList);
   lvi.iSubItem = 0;
   lvi.state = 0;
   lvi.stateMask = 0;
   lvi.pszText = LPWSTR(lpText);
   lvi.lParam = lvi.iItem;

   if (lvi.iItem > iRow) {
      lvi.mask = LVIF_TEXT;
      lvi.iItem = iRow;
      lvi.iSubItem = iCol;
      SendMessage(hwndList, LVM_SETITEM, 0, (LPARAM)(const LV_ITEM*)&lvi);
   } else {
      SendMessage(hwndList, LVM_INSERTITEM, 0, (LPARAM)(const LV_ITEM*)&lvi);
   }
}

HWND ResourceManager::AddPullDownList(HWND hWnd, int iID, int iX, int iY, int iWidth, int iHeight)
{
   const DWORD dwFlags = CBS_DROPDOWNLIST | CBS_SIMPLE | WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP;
   return CreateWindow(TEXT("ComboBox"), TEXT(""),
      dwFlags,
      iX, iY, iWidth, iHeight,
      hWnd, (HMENU)iID, NULL, NULL);
}

int ResourceManager::AddPDLString(HWND hWnd, int iID, LPCWSTR szString)
{
   return int(SendDlgItemMessage(hWnd, iID, CB_ADDSTRING, 0, (LPARAM)szString));
}

HWND ResourceManager::AddEdit(HWND hWnd, int iID, int iX, int iY, int iWidth, int iHeight)
{
   return CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""),
      ES_MULTILINE | ES_WANTRETURN | ES_LEFT | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
      iX, iY, iWidth, iHeight,
      hWnd, (HMENU)iID, NULL, NULL);
}

HWND ResourceManager::AddText(HWND hWnd, LPCWSTR lpLabel, int iID, int iX, int iY, int iWidth, int iHeight)
{
   return CreateWindow(TEXT("Static"), lpLabel,
      SS_LEFT | WS_CHILD | WS_VISIBLE,
      iX, iY, iWidth, iHeight,
      hWnd, (HMENU)iID, NULL, NULL);
}

void ResourceManager::ClearListView(HWND hWnd, int iID)
{
   SendDlgItemMessage(hWnd, iID, LVM_DELETEALLITEMS, 0, 0);
}

void ResourceManager::DeleteListViewItem(HWND hWnd, int iID, int iRow)
{
   ListView_DeleteItem(GetDlgItem(hWnd, iID), iRow);
}

void ResourceManager::SetListViewColumnString(HWND hWnd, int iID, int iRow, int iCol, LPCWSTR lpText)
{
   LV_ITEM lvi;
   lvi.iSubItem = iCol;
   lvi.pszText = LPWSTR(lpText);
   SendDlgItemMessage(hWnd, iID, LVM_SETITEMTEXT, iRow, (LPARAM)(const LV_ITEM*)&lvi);
}

void ResourceManager::GetText(HWND hWnd, int iID, LPWSTR lpText)
{
   GetDlgItemText(hWnd, iID, lpText, 20);
}
