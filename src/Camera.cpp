#include "Camera.h"

Camera::Camera()
    : mPitch{0.0f}, mYaw{0.0f},
    isMoveForward{ false }, isMoveBackward { false }, isMoveLeft{ false }, isMoveRight{ false },
    mPrevMouseDown{ false }
{
    mPos = {};
    mPrevMousePos = {};
}

void Camera::HandleMouse(V2 mousePos, bool mouseDown)
{
    if (mouseDown)
    {
        if (!mPrevMouseDown)
        {
            mPrevMousePos = mousePos;
        }
        V2 mouseDelta = mousePos - mPrevMousePos;
        mPitch += mouseDelta.y;
        mYaw += mouseDelta.x;

        mPrevMousePos = mousePos;
    }
    mPrevMouseDown = mouseDown;
}

M4 Camera::Move(float frameTime)
{
    M4 axisTransform = M4::Identity().Rotate(0, mYaw, 0) * M4::Identity().Rotate(mPitch, 0, 0);

    V3 forward = (axisTransform * V4(0, 0, 1, 0)).xyz.Normalize();
    V3 up = (axisTransform * V4(0, 1, 0, 0)).xyz.Normalize();
    V3 right = (axisTransform * V4(1, 0, 0, 0)).xyz.Normalize();

    M4 viewTransform = M4::Identity();

    viewTransform.v[0].x = right.x;
    viewTransform.v[1].x = right.y;
    viewTransform.v[2].x = right.z;

    viewTransform.v[0].y = up.x;
    viewTransform.v[1].y = up.y;
    viewTransform.v[2].y = up.z;

    viewTransform.v[0].z = forward.x;
    viewTransform.v[1].z = forward.y;
    viewTransform.v[2].z = forward.z;

    if (isMoveForward)
    {
        mPos += forward * frameTime;
    }
    if (isMoveBackward)
    {
        mPos -= forward * frameTime;
    }
    if (isMoveLeft)
    {
        mPos -= right * frameTime;
    }
    if (isMoveRight)
    {
        mPos += right * frameTime;
    }

    return viewTransform * M4::Identity().SetTranslation(-mPos);
}
