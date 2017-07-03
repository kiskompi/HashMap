#include "stdafx.h"
#include <iostream>
#include "../HashMap/HashMap.h"
using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

namespace UnitTests
{
	[TestClass]
	public ref class UnitTest
	{
	private:
		TestContext^ testContextInstance;

	public: 
		/// <summary>
		///Gets or sets the test context which provides
		///information about and functionality for the current test run.
		///</summary>
		property Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ TestContext
		{
			Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ get()
			{
				return testContextInstance;
			}
			System::Void set(Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ value)
			{
				testContextInstance = value;
			}
		};

		#pragma region Additional test attributes
		//
		//You can use the following additional attributes as you write your tests:
		//
		//Use ClassInitialize to run code before running the first test in the class
		//[ClassInitialize()]
		//static void MyClassInitialize(TestContext^ testContext) {};
		//
		//Use ClassCleanup to run code after all tests in a class have run
		//[ClassCleanup()]
		//static void MyClassCleanup() {};
		//
		//Use TestInitialize to run code before running each test
		//[TestInitialize()]
		//void MyTestInitialize() {};
	

		//Use TestCleanup to run code after each test has run
		//[TestCleanup()]
		//void MyTestCleanup() {};
		//
		#pragma endregion 
		
		[TestMethod]
		void TestMethod1()
		{
			
			HashMap<int, std::string> *hmap = new HashMap<int, std::string>();
			hmap->put(1, "Anna");
			hmap->put(2, "János");
			hmap->put(3, "Bála");
			hmap->put(4, "Kecske");
			hmap->put(5, "Gergő");
			hmap->put(6, "Eszter");
			hmap->put(7, "Jancsi");

			int i = 6;
			int j = 7;
			hmap->get(i);
			hmap->get(j);

			hmap->del(j);
			hmap->get(j);
			std::cout << "Lefutott\n";
		};
	};
}
