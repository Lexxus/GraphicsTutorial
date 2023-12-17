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

    bool isW = false;
    bool isA = false;
    bool isS = false;
    bool isD = false;
    MSG msg = {};

    while (GlobalState.isRunning)
    {
        Assert(QueryPerformanceCounter(&endTime));
        f32 frameTime = f32(endTime.QuadPart - beginTime.QuadPart) / f32(timerFrequency.QuadPart);
        beginTime = endTime;

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
                            isW = isDown;
                            break;
                        case 'A':
                            isA = isDown;
                            break;
                        case 'S':
                            isS = isDown;
                            break;
                        case 'D':
                            isD = isDown;
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
        M4 cameraTransform = M4::Identity();
        {
            Camera* camera = &GlobalState.camera;
            bool mouseDown = false;
            V2 mousePos = {};
            if (GetActiveWindow() == GlobalState.windowHandle)
            {
                POINT winMousePos = {};
                Assert(GetCursorPos(&winMousePos));
                Assert(ScreenToClient(GlobalState.windowHandle, &winMousePos));

                RECT clientRect = {};
                Assert(GetClientRect(GlobalState.windowHandle, &clientRect));
                winMousePos.y = clientRect.bottom - winMousePos.y;
                mousePos.x = float(winMousePos.x) / float(clientRect.right - clientRect.left);
                mousePos.y = float(winMousePos.y) / float(clientRect.bottom - clientRect.top);

                mouseDown = (GetKeyState(VK_LBUTTON) & 0x80) != 0;
            }

            if (mouseDown)
            {
                if (!camera->prevMouseDown)
                {
                    camera->prevMousePos = mousePos;
                }
                V2 mouseDelta = mousePos - camera->prevMousePos;
                camera->pitch += mouseDelta.y;
                camera->yaw += mouseDelta.x;

                camera->prevMousePos = mousePos;
            }
            camera->prevMouseDown = mouseDown;

            M4 cameraAxisTransform = M4::Identity().Rotate(0, camera->yaw, 0) * M4::Identity().Rotate(camera->pitch, 0, 0);

            V3 forward = (cameraAxisTransform * V4(0, 0, 1, 0)).xyz.Normalize();
            V3 up = (cameraAxisTransform * V4(0, 1, 0, 0)).xyz.Normalize();
            V3 right = (cameraAxisTransform * V4(1, 0, 0, 0)).xyz.Normalize();

            M4 cameraViewTransform = M4::Identity();

            cameraViewTransform.v[0].x = right.x;
            cameraViewTransform.v[1].x = right.y;
            cameraViewTransform.v[2].x = right.z;

            cameraViewTransform.v[0].y = up.x;
            cameraViewTransform.v[1].y = up.y;
            cameraViewTransform.v[2].y = up.z;

            cameraViewTransform.v[0].z = forward.x;
            cameraViewTransform.v[1].z = forward.y;
            cameraViewTransform.v[2].z = forward.z;

            if (isW)
            {
                camera->pos += forward * frameTime;
            }
            if (isS)
            {
                camera->pos -= forward * frameTime;
            }
            if (isA)
            {
                camera->pos -= right * frameTime;
            }
            if (isD)
            {
                camera->pos += right * frameTime;
            }

            cameraTransform = cameraViewTransform * cameraTransform.SetTranslation(-camera->pos);
        }

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
        V3 modelColors[] =
        {
            V3(1, 0, 0),
            V3(1, 1, 0),
            V3(1, 1, 0),
            V3(0, 1, 0),

            V3(0, 0, 1),
            V3(0, 1, 1),
            V3(0, 1, 1),
            V3(1, 0, 1),
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
        M4 transform = cameraTransform
            * M4(V3(0, 0, 2.0f))
            * M4::Identity().Rotate(currAngle, 0.0f, currAngle);

        for (int i = 0; i < 36; i += 3)
        {
            u32 index0 = modelIndices[i];
            u32 index1 = modelIndices[i + 1];
            u32 index2 = modelIndices[i + 2];
            fb.DrawTriangle(modelVertices[index0], modelVertices[index1], modelVertices[index2],
                modelColors[index0], modelColors[index1], modelColors[index2], transform);
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
    GlobalState.camera = {};

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

