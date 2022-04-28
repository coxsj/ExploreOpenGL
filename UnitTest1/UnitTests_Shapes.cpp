#include "pch.h"
#include "CppUnitTest.h"
#include "../src/shapes.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_Geometry {
	TEST_CLASS(NormalCorrect){
	public:
		TEST_METHOD(NormalXOk){
			NewShape s;
			Assert::IsTrue(Geometry::normalCorrect(Point{ 1.0 }, Point(1.0f, 0.0f, 0.0f), Point{ 0.0 }));
		}
		TEST_METHOD(NormalXNotCorrect) {
			NewShape s;
			Assert::IsFalse(Geometry::normalCorrect(Point{ 1.0 }, Point(-1.0f, 0.0f, 0.0f), Point{ 0.0 }));
		}
		TEST_METHOD(NormalXSameSideAsRef) {
			NewShape s;
			Assert::IsFalse(Geometry::normalCorrect(Point{ -1.0 }, Point(1.0f, 0.0f, 0.0f), Point{ 0.0 }));
		}
		TEST_METHOD(NormalYOk) {
			NewShape s;
			Assert::IsTrue(Geometry::normalCorrect(Point{ 0.0f, 0.5f, 0.0f }, Point(0.0f, 1.0f, 0.0f), Point{ 0.0 }));
		}

	};
	TEST_CLASS(ExtractTriangles) {
	public:
		TEST_METHOD(PointsSameFail) {
			Triangle a{};
			Triangle b{};
			Assert::IsFalse(Geometry::extractTrianglesFromRectangle(
				Point{ 0,0,0 }, Point{ 0,0,0 }, Point{ 1,0,0 }, Point{ 0,1,0 }, a, b, Point{ 0,0,0 }));
		}
	};
}
