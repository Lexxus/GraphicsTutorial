#include "graphics_math.h"

class Camera
{
public:
    Camera();

    void HandleMouse(V2 mousePosistion, bool isMouseDown);
    M4 Move(float frameTime);

    bool isMoveForward;
    bool isMoveBackward;
    bool isMoveLeft;
    bool isMoveRight;

private:
    bool mPrevMouseDown;
    V2 mPrevMousePos;

    float mYaw;
    float mPitch;

    V3 mPos;
};
