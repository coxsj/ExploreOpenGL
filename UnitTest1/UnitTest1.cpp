#include "pch.h"
#include "CppUnitTest.h"
#include "../src/shapes.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(NormalIsCorrect){
	public:
		TEST_METHOD(NormalXOk){
			NewShape s;
			Assert::IsTrue(s.normalCorrect(Point{ 1.0 }, Point(1.0f, 0.0f, 0.0f), Point{ 0.0 }));
		}
		TEST_METHOD(NormalXNotCorrect) {
			NewShape s;
			Assert::IsFalse(s.normalCorrect(Point{ 1.0 }, Point(-1.0f, 0.0f, 0.0f), Point{ 0.0 }));
		}
		TEST_METHOD(NormalXSameSideAsRef) {
			NewShape s;
			Assert::IsFalse(s.normalCorrect(Point{ -1.0 }, Point(1.0f, 0.0f, 0.0f), Point{ 0.0 }));
		}
		TEST_METHOD(NormalYOk) {
			NewShape s;
			Assert::IsTrue(s.normalCorrect(Point{ 0.0f, 0.5f, 0.0f }, Point(0.0f, 1.0f, 0.0f), Point{ 0.0 }));
		}

	};
}
