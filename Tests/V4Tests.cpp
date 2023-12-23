#include "graphics_math.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GraphicsMathTests
{
    TEST_CLASS(V4_Test)
    {
    public:

        TEST_METHOD(Init_Default)
        {
            V4 vec = {};
            float ref[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

            for (int i = 0; i < 4; ++i)
                Assert::AreEqual(ref[i], vec.e[i]);
        }

        TEST_METHOD(Init_float4)
        {
            V4 vec(1.1f, 2.2f, 3.3f, 4.4f);
            float ref[4] = { 1.1f, 2.2f, 3.3f, 4.4f };

            for (int i = 0; i < 4; ++i)
                Assert::AreEqual(ref[i], vec.e[i]);
        }

        TEST_METHOD(Init_V3)
        {
            V3 vec3(1.1f, 2.2f, 3.3f);
            V4 vec(vec3);
            float ref[4] = { 1.1f, 2.2f, 3.3f, 1.0f };

            for (int i = 0; i < 4; ++i)
                Assert::AreEqual(ref[i], vec.e[i]);
        }
    };
}
