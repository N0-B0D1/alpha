/**
Copyright 2014-2015 Jason R. Wendlandt

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "Graphics/GraphicsWindow.h"

namespace alpha
{
    HWND g_hWnd = nullptr;

    GraphicsWindow::GraphicsWindow()
        : m_hInstance(nullptr)
    { }
    GraphicsWindow::~GraphicsWindow() { }

    bool GraphicsWindow::Initialize()
    {
        m_hInstance = GetModuleHandle(NULL);

        // Register class
        WNDCLASSEX wcex = {};
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = GraphicsWindow::WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = m_hInstance;
        wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = "ALPHAClass";
        wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
        DWORD err = RegisterClassEx(&wcex);
        if (!err)
        {
            DWORD err = GetLastError();
            UNREFERENCED_PARAMETER(err);
            return false;
        }

        // Create window
        RECT rc = { 0, 0, 800, 600 };
        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
        g_hWnd = CreateWindow("ALPHAClass", "ALPHA Engine", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, m_hInstance, nullptr);
        if (!g_hWnd)
        {
            return false;
        }

        ShowWindow(g_hWnd, 10); // nCmdShow);

        return true;
    }

    bool GraphicsWindow::Update(double /*currentTime*/, double /*elapsedTime*/)
    {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (msg.message == WM_QUIT)
        {
            //SendMessage(m_hWnd, WM_CLOSE, 0, 0);
            return false;
        }
        return true;
    }

    bool GraphicsWindow::Shutdown()
    {
        CloseWindow(g_hWnd);
        return true;
    }

    HWND GraphicsWindow::GetHWND() const
    {
        return g_hWnd;
    }

    LRESULT CALLBACK GraphicsWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        PAINTSTRUCT ps;
        HDC hdc;

        switch (message)
        {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        return 0;
    }
}
