#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GraphicsMathTests
{
	TEST_CLASS(M4_Test)
	{
	public:
		
		TEST_METHOD(Init_Scale)
		{
            M4 matrix = { 1.0f, 1.0f, 1.0f };
            float ref[16] = {
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f,
            };

            for (int i = 0; i < 16; ++i)
                Assert::AreEqual(ref[i], matrix.e[i], 0.00001f);
		}

        TEST_METHOD(Init_Scalar)
        {
            M4 matrix = { 1.0f };
            float ref[16] = {
                1.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f,
            };

            for (int i = 0; i < 16; ++i)
                Assert::AreEqual(ref[i], matrix.e[i], 0.00001f);
        }

        TEST_METHOD(Init_M4)
        {
            M4 source = {};
            source.v[0] = { 1.1f, 1.2f, 1.3f, 1.4f };
            source.v[1] = { 2.1f, 2.2f, 2.3f, 2.4f };
            source.v[2] = { 3.1f, 3.2f, 3.3f, 3.4f };
            source.v[3] = { 4.1f, 4.2f, 4.3f, 4.4f };
            M4 matrix(source);

            Assert::IsFalse(&source == &matrix);

            float ref[16] = {
                1.1f, 1.2f, 1.3f, 1.4f,
                2.1f, 2.2f, 2.3f, 2.4f,
                3.1f, 3.2f, 3.3f, 3.4f,
                4.1f, 4.2f, 4.3f, 4.4f,
            };

            for (int i = 0; i < 16; ++i)
                Assert::AreEqual(ref[i], matrix.e[i], 0.00001f);
        }

        TEST_METHOD(Init_V4Array)
        {
            V4 vector[4] = {
                {1.1f, 2.1f, 3.1f, 4.1f},
                {1.2f, 2.2f, 3.2f, 4.2f},
                {1.3f, 2.3f, 3.3f, 4.3f},
                {1.4f, 2.4f, 3.4f, 4.4f},
            };
            M4 matrix(vector);
            float ref[16] = {
                1.1f, 2.1f, 3.1f, 4.1f,
                1.2f, 2.2f, 3.2f, 4.2f,
                1.3f, 2.3f, 3.3f, 4.3f,
                1.4f, 2.4f, 3.4f, 4.4f,
            };

            for (int i = 0; i < 16; ++i)
                Assert::AreEqual(ref[i], matrix.e[i], 0.00001f);
        }

        TEST_METHOD(Init_V3)
        {
            V3 vector(1.1f, 2.2f, 3.3f);
            M4 matrix(vector);
            float ref[16] = {
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                1.1f, 2.2f, 3.3f, 1.0f,
            };

            for (int i = 0; i < 16; ++i)
                Assert::AreEqual(ref[i], matrix.e[i], 0.00001f);
        }

        TEST_METHOD(Init_float3)
        {
            M4 matrix(1.1f, 2.2f, 3.3f);
            float ref[16] = {
                1.1f, 0.0f, 0.0f, 0.0f,
                0.0f, 2.2f, 0.0f, 0.0f,
                0.0f, 0.0f, 3.3f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f,
            };

            for (int i = 0; i < 16; ++i)
                Assert::AreEqual(ref[i], matrix.e[i], 0.00001f);
        }

        TEST_METHOD(Multiply_V3)
        {
            V4 vec[4] = {
                { 1.0f, 2.0f, 3.0f, 4.0f },
                { 5.0f, 6.0f, 7.0f, 8.0f },
                { 9.0f, 10.0f, 11.0f, 12.0f },
                { 13.0f, 14.0f, 15.0f, 16.0f },
            };
            M4 matrix(vec);
            V4 mv4 = { 2.0f, 3.0f, 4.0f, 5.0f };
            V4 result = matrix * mv4;

            // check that matrix is not mutated
            for (int x = 0; x < 4; ++x)
                for (int y = 0; y < 4; ++y)
                    Assert::AreEqual(vec[x].e[y], matrix.e[x * 4 + y]);

            float ref[4] = {
                2.0f + 15.0f + 36.0f + 65.0f,
                4.0f + 18.0f + 40.0f + 70.0f,
                6.0f + 21.0f + 44.0f + 75.0f,
                8.0f + 24.0f + 48.0f + 80.0f,
            };

            for (int i = 0; i < 4; ++i)
                Assert::AreEqual(ref[i], result.e[i], 0.00001f);
        }
	};
}
