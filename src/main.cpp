//#include <cstddef>
#include "pch.h"

#include "main.h"
#include "graphics_math.h"
#include "FrameBuffer.h"
#include "Camera.h"

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
    Camera camera = {};

    LARGE_INTEGER beginTime = {};
    LARGE_INTEGER endTime = {};
    Assert(QueryPerformanceCounter(&beginTime));

    float currAngle = 0.0f;
    const float pi2 = PI * 2.0f;
    Texture checkBoardTexture(32, 32);

    for (u32 y = 0; y < checkBoardTexture.height(); ++y)
        for (u32 x = 0; x < checkBoardTexture.width(); ++x)
        {
            u32 texColor = (x + (y % 2)) % 2 == 0 ? 0xFF000000 : 0xFFFFFFFF;
            checkBoardTexture.setTexel(x, y, texColor);
        }

    MSG msg = {};

    while (GlobalState.isRunning)
    {
        Assert(QueryPerformanceCounter(&endTime));
        float frameTime = float(endTime.QuadPart - beginTime.QuadPart) / float(timerFrequency.QuadPart);
        beginTime = endTime;

        RECT clientRect = {};
        Assert(GetClientRect(GlobalState.windowHandle, &clientRect));
        u32 clientWidth = clientRect.right - clientRect.left;
        u32 clientHeight = clientRect.bottom - clientRect.top;
        float aspectRatio = float(clientWidth) / float(clientHeight);

        while (PeekMessageA(&msg, GlobalState.windowHandle, 0, 0, PM_REMOVE))
        {
            switch (msg.message)
            {
                case WM_QUIT:
                    GlobalState.isRunning = FALSE;
                break;

                case WM_KEYUP:
                case WM_KEYDOWN:
                {
                    u32 vCode = msg.wParam;
                    bool isDown = !((msg.lParam >> 31) & 0x1);

                    switch (vCode)
                    {
                        case 'W':
                            camera.isMoveForward = isDown;
                            break;
                        case 'A':
                            camera.isMoveLeft = isDown;
                            break;
                        case 'S':
                            camera.isMoveBackward = isDown;
                            break;
                        case 'D':
                            camera.isMoveRight = isDown;
                            break;
                    }
                } break;

                default:
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                break;
            }
        }

        // calculate the camera
        {
            bool mouseDown = false;
            V2 mousePos = {};
            if (GetActiveWindow() == GlobalState.windowHandle)
            {
                POINT winMousePos = {};
                Assert(GetCursorPos(&winMousePos));
                Assert(ScreenToClient(GlobalState.windowHandle, &winMousePos));

                winMousePos.y = clientRect.bottom - winMousePos.y;
                mousePos.x = float(winMousePos.x) / float(clientWidth);
                mousePos.y = float(winMousePos.y) / float(clientHeight);

                mouseDown = (GetKeyState(VK_LBUTTON) & 0x80) != 0;
            }

            camera.HandleMouse(mousePos, mouseDown);
        }
        M4 cameraTransform = camera.Move(frameTime);

        // clear the canvas
        fb.Fill(0xFF000000);

        // Cube model
        V3 modelVertices[] =
        {
            // front face
            V3(-0.5f, -0.5f, -0.5f),
            V3(-0.5f, 0.5f, -0.5f),
            V3(0.5f, 0.5f, -0.5f),
            V3(0.5f, -0.5f, -0.5f),
            // back face
            V3(-0.5f, -0.5f, 0.5f),
            V3(-0.5f, 0.5f, 0.5f),
            V3(0.5f, 0.5f, 0.5f),
            V3(0.5f, -0.5f, 0.5f),
        };
        V2 modelUvs[] =
        {
            V2(0, 0),
            V2(1, 0),
            V2(1, 1),
            V2(0, 1),

            V2(0, 0),
            V2(1, 0),
            V2(1, 1),
            V2(0, 1),
        };
        u32 modelIndices[] =
        {
            // front face
            0, 1, 2,
            2, 3, 0,
            // back face
            6, 5, 4,
            4, 7, 6,
            // left face
            4, 5, 1,
            1, 0, 4,
            // right face
            3, 2, 6,
            6, 7, 3,
            // top face
            1, 5, 6,
            6, 2, 1,
            // bottom face
            4, 0, 3,
            3, 7, 4,
        };

        //float offset = abs(sinf(currAngle));
        M4 transform = M4::Perspective(60.0f, aspectRatio, 0.01f, 1000.0f)
            * cameraTransform
            * M4(V3(0, 0, 2.0f))
            * M4::Identity().Rotate(currAngle, 0.0f, currAngle);

        for (int i = 0; i < 36; i += 3)
        {
            u32 index0 = modelIndices[i];
            u32 index1 = modelIndices[i + 1];
            u32 index2 = modelIndices[i + 2];
            fb.DrawTriangle(modelVertices[index0], modelVertices[index1], modelVertices[index2],
                modelUvs[index0], modelUvs[index1], modelUvs[index2], transform, checkBoardTexture);
        }

#if 0
        // colors at the corners of triangles
        V3 colors[] =
        {
            V3(1, 0, 0),
            V3(0, 1, 0),
            V3(0, 0, 1)
        };
        // move triangles every frame
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
#endif

        fb.Render(clientWidth, clientHeight);

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

