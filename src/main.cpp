#include <cmath>
#include <cstddef>
#include <windows.h>

#include "main.h"
#include "graphics_math.h"
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

    LARGE_INTEGER beginTime = {};
    LARGE_INTEGER endTime = {};
    Assert(QueryPerformanceCounter(&beginTime));

    float currAngle = 0.0f;
    const float pi2 = PI * 2.0f;

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

        fb.Fill(0xFF000000);

        V3 colors[] =
        {
            V3(1, 0, 0),
            V3(0, 1, 0),
            V3(0, 0, 1)
        };
        V3 shift = V3(cosf(currAngle), sinf(currAngle), 0);

        for (u32 n = 0; n < 5; ++n)
        {
            float depth = powf(2, n + 1);
            V3 points[3] =
            {
                V3(-1.5f, -0.5f, depth),
                V3(0.0f, 0.5f, depth),
                V3(1.8f, -0.5f, depth)
            };

            for (u32 i = 0; i < 3; ++i)
            {
                points[i] += shift;
            }
            fb.DrawTriangle(points, colors);
        }

        fb.Render();

        currAngle += frameTime;
        if (currAngle >= pi2)
            currAngle -= pi2;
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
        WS_OVERLAPPEDWINDOW, // | WS_VISIBLE, // DWORD dwStyle,
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

    ShowWindow(GlobalState.windowHandle, nShowCmd);
    UpdateWindow(GlobalState.windowHandle);

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

