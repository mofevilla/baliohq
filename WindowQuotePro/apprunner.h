#pragma once

#include "mainwindow.h"
#include "resource.h"

class Runner {
public:
   Runner();
   ~Runner();

   int Run(HINSTANCE hInstance, int nCmdShow);

   static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
   HWND hWnd;
};
