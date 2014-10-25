
//#include <iostream>
//#include <string>

#include "AlphaEngine/Include/AlphaController.h"

#if WIN32
    #include <Windows.h>

    //INT WINAPI WinMain(HINSTANCE /*hInst*/, HINSTANCE, LPSTR /*strCmdLine*/, INT)
    int WINAPI wWinMain(_In_ HINSTANCE /*hInstance*/, _In_opt_ HINSTANCE /*hPrevInstance*/, _In_ LPWSTR /*lpCmdLine*/, _In_ int /*nCmdShow*/)
#else
    int main(int /*argc*/, char ** /*argv*/)
#endif
    {
        //UNREFERENCED_PARAMETER(nCmdShow);
        int error = alpha::InitiateAlpha();

        //std::string stuff;
        //std::cin >> stuff;

        return error;
    }
