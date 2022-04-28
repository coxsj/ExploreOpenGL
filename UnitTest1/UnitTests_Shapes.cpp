#include "pch.h"
#include "CppUnitTest.h"
#include "../src/shapes.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_Geometry {
	TEST_CLASS(NormalCorrect){
	public:
		TEST_METHOD(NormalXOk){
			NewShape s;
			Assert::IsTrue(Geometry::normalCorrect(Point{ 0.5f }, Point(1.0f, 0.0f, 0.0f), Point{ 0.0 }));
		}
		TEST_METHOD(NormalXNotCorrect) {
			NewShape s;
			Assert::IsFalse(Geometry::normalCorrect(Point{ 0.5f }, Point(-0.1f, 0.0f, 0.0f), Point{ 0.0 }));
		}
		TEST_METHOD(NormalXSameSideAsRef) {
			NewShape s;
			Assert::IsFalse(Geometry::normalCorrect(Point{ -0.5f }, Point(1.0f, 0.0f, 0.0f), Point{ 0.0 }));
		}
		TEST_METHOD(NormalYOk) {
			NewShape s;
			Assert::IsTrue(Geometry::normalCorrect(Point{ 0.0f, 0.5f, 0.0f }, Point(0.0f, 1.0f, 0.0f), Point{ 0.0 }));
		}
	};
	TEST_CLASS(ExtractTrianglesFromRectangle) {
	public:
		TEST_METHOD(AllPointsNotUniqueFail) {
			Triangle a{};
			Triangle b{};
			Assert::IsFalse(Geometry::extractTrianglesFromRectangle(
				Point{ 0.0f, 0.0f, 0.0f }, Point{ 0.0f, 0.0f, 0.0f }, 
				Point{ 0.5f, 0.0f, 0.0f }, Point{ 0.0f, 0.5f, 0.0f }, a, b));
		}
		TEST_METHOD(OnePointNotSamePlaneFail) {
			Triangle a{};
			Triangle b{};
			Assert::IsFalse(Geometry::extractTrianglesFromRectangle(
				Point{ 0.0f, 0.0f, 0.0f }, Point{ 0.0f,0.5f,0.5f }, 
				Point{ 0.5f, 0.0f, 0.5f }, Point{ 0.5f,0.5f,0.5f }, a, b));
		}
		TEST_METHOD(ValidRectanglePass) {
			Triangle a{ Point{}, Point{}, Point{} };
			Triangle b{ Point{}, Point{}, Point{} };
			Assert::IsTrue(Geometry::extractTrianglesFromRectangle(
				Point{ 0.0f, 0.0f, 0.5f }, Point{ 0.0f, 0.5f, 0.5f }, 
				Point{ 0.5f, 0.0f, 0.5f }, Point{ 0.5f, 0.5f, 0.5f }, a, b));
		}
	};
	TEST_CLASS(VerifyRectangle) {
	public:
		TEST_METHOD(AllAnglesNot90Fail) {
			Assert::IsFalse(Geometry::verifyRectangle(
				Point{ 0.0f, 0.0f, 0.5f }, Point{ 0.0f, 0.5f, 0.5f }, 
				Point{ 0.5f, 0.0f, 0.5f }, Point{ 0.5f, 0.25f, 0.0f }));
		}
		TEST_METHOD(AllAngles90Pass) {
			Assert::IsTrue(Geometry::verifyRectangle(
				Point{ 0.0f, 0.0f, 0.5f }, Point{ 0.0f, 0.5f, 0.5f }, 
				Point{ 0.5f, 0.0f, 0.5f }, Point{ 0.5f, 0.5f, 0.5f }));
		}
	};
	TEST_CLASS(AllPointsUnique) {
	public:
		TEST_METHOD(AllPointsDifferent) {
			Assert::IsTrue(Geometry::allPointsUnique(
				Point{ 0.0f, 0.0f, 0.5f }, Point{ 0.0f, 0.5f, 0.5f }, 
				Point{ 0.5f, 0.0f, 0.5f }, Point{ 0.5f, 0.25f, 0.0f }));
		}
		TEST_METHOD(OnePointSame) {
			Assert::IsFalse(Geometry::allPointsUnique(
				Point{ 0.0f, 0.0f, 0.5f }, Point{ 0.0f, 0.5f, 0.5f }, 
				Point{ 0.5f, 0.0f, 0.5f }, Point{ 0.5f, 0.0f, 0.5f }));
		}
	};
	TEST_CLASS(GetNormal) {
	public:
		TEST_METHOD(GetNormalThreePts) {
			glm::vec3 normal = Geometry::getNormal(Point{ 0.0f, 0.0f, 0.0f },
				Point{ 0.0f, 0.5f, 0.0f }, Point{ 0.5f, 0.0f, 0.0f });
			Assert::IsTrue(normal == glm::vec3{0.0, 0.0, 1.0} || normal == glm::vec3{ 0.0, 0.0, -1.0 });
		}		
	};
	TEST_CLASS(AllPointsSamePlane) {
	public:
		TEST_METHOD(FourPointsSameXYPlane) {
			Assert::IsTrue(Geometry::allPointsSamePlane(
				Point{ 0.0f, 0.0f, 0.0f }, Point{ 0.0f, 0.5f, 0.0f }, 
				Point{ 0.5f, 0.0f, 0.0f }, Point{ 0.5f, 0.5f, 0.0f }));
		}
		TEST_METHOD(FourPointsSameXZPlane) {
			Assert::IsTrue(Geometry::allPointsSamePlane(
				Point{ 0.0f, 0.0f, 0.0f }, Point{ 0.0f, 0.0f, 0.5f }, 
				Point{ 0.5f, 0.0f, 0.0f }, Point{ 0.5f, 0.0f, 0.5f }));
		}
		TEST_METHOD(FourPointsSameYZPlane) {
			Assert::IsTrue(Geometry::allPointsSamePlane(
				Point{ 0.0f, 0.0f, 0.0f }, Point{ 0.0f, 0.0f, 0.5f },
				Point{ 0.0f, 0.5f, 0.0f }, Point{ 0.0f, 0.5f, 0.5f }));
		}
		TEST_METHOD(FourPointsNotSameXYPlane) {
			Assert::IsFalse(Geometry::allPointsSamePlane(
				Point{ 0.5f, 0.5f, 0.5f }, Point{ 0.0f, 0.5f, 0.0f },
				Point{ 0.5f, 0.0f, 0.0f }, Point{ 0.5f, 0.5f, 0.0f }));
		}
		TEST_METHOD(FourPointsNotSameXZPlane) {
			Assert::IsFalse(Geometry::allPointsSamePlane(
				Point{ 0.5f, 0.5f, 0.5f }, Point{ 0.0f, 0.0f, 0.5f }, 
				Point{ 0.5f, 0.0f, 0.0f }, Point{ 0.5f, 0.0f, 0.5f }));
		}
		TEST_METHOD(FourPointsNotSameYZPlane) {
			Assert::IsFalse(Geometry::allPointsSamePlane(
				Point{ 0.5f, 0.5f, 0.5f }, Point{ 0.0f, 0.0f, 0.5f }, 
				Point{ 0.0f, 0.5f, 0.0f }, Point{ 0.0f, 0.5f, 0.5f }));
		}
	};
}
