#include <cstddef>
#include <windows.h>

#include "main.h"
#include "FrameBuffer.h"

global global_state GlobalState;

ATOM InitApplication(HINSTANCE hInstance, LPCSTR className);
bool InitInstance(HINSTANCE hInstance, int nShowCmd, LPCSTR className);
LRESULT WindowCallBack(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    GlobalState.isRunning = TRUE;

    LARGE_INTEGER timerFrequency = {};
    Assert(QueryPerformanceFrequency(&timerFrequency));

    const LPCSTR className = "GraphicsTutorial";

    if (!InitApplication(hInstance, className))
    {
        InvalidCodePath;
    }

    if (!InitInstance(hInstance, nShowCmd, className))
    {
        InvalidCodePath;
    }

    FrameBuffer fb = FrameBuffer(GlobalState.windowHandle, GlobalState.deviceContext);

    const f32 speed = 200.0f;
    f32 currOffset = 0.0f;

    LARGE_INTEGER beginTime = {};
    LARGE_INTEGER endTime = {};
    Assert(QueryPerformanceCounter(&beginTime));

    while (GlobalState.isRunning)
    {
        Assert(QueryPerformanceCounter(&endTime));
        f32 frameTime = f32(endTime.QuadPart - beginTime.QuadPart) / f32(timerFrequency.QuadPart);
        beginTime = endTime;

        MSG msg = {};

        while (PeekMessageA(&msg, GlobalState.windowHandle, 0, 0, PM_REMOVE))
        {
            switch (msg.message)
            {
                case WM_QUIT:
                    GlobalState.isRunning = FALSE;
                break;

                default:
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                break;
            }
        }

        currOffset += speed * frameTime;

        fb.Fill(currOffset);
        fb.Render();
    }

    return 0;
}

ATOM InitApplication(HINSTANCE hInstance, LPCSTR className)
{
    WNDCLASSA WindowClass = {};
    WindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    WindowClass.lpfnWndProc = WindowCallBack;
    WindowClass.hInstance = hInstance;
    // WindowClass.hIcon = LoadIconA(NULL, IDI_APPLICATION);
    WindowClass.hCursor = LoadCursorA(NULL, (LPCSTR)IDC_ARROW);
    WindowClass.lpszClassName = className;

    return RegisterClassA(&WindowClass);
}

bool InitInstance(HINSTANCE hInstance, int nShowCmd, LPCSTR className)
{
    GlobalState.windowHandle = CreateWindowExA(
        0,  // DWORD dwExStyle,
        className,  // LPCSTR lpClassName,
        "Graphics Tutorial",  // LPCSTR lpWindowName,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, // DWORD dwStyle,
        CW_USEDEFAULT,  // int X,
        CW_USEDEFAULT, // int Y,
        1240, // int nWidth,
        720, // int nHeight,
        NULL, // HWND hWndParent,
        NULL, // HMENU hMenu,
        hInstance, // HINSTANCE hInstance,
        NULL// LPVOID lpParam
    );

    if (!GlobalState.windowHandle)
        return FALSE;

    // ShowWindow(GlobalState.windowHandle, nShowCmd);
    // UpdateWindow(GlobalState.windowHandle);

    GlobalState.deviceContext = GetDC(GlobalState.windowHandle);

    return TRUE;
}

LRESULT WindowCallBack(
    HWND windowHandle,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
)
{
    LRESULT result = {};

    switch (message) {
        case WM_DESTROY:
        case WM_CLOSE:
            GlobalState.isRunning = FALSE;
        break;

        default:
            result = DefWindowProcA(windowHandle, message, wParam, lParam);
        break;
    }

    return result;
}

