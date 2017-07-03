#include "stdafx.h"
#include "../HashMap/HashMap.h"
#include <iostream>
using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace	Microsoft::VisualStudio::TestTools::UnitTesting;

namespace UnitTests
{
	[TestClass]
	public ref class UnitTest1
	{
	public: 
		[TestMethod]
		void TestMethod1()
		{
			HashMap<int, std::string> *hmap = new HashMap<int, std::string>();
			hmap->put(1, "Anna");
			hmap->put(2, "Janos");
			hmap->put(3, "Bela");
			hmap->put(4, "Kecske");
			hmap->put(5, "Gergo");
			hmap->put(6, "Eszter");
			hmap->put(7, "Jancsi");

			int i = 6;
			int j = 7;
			hmap->get(i);
			hmap->get(j);

			hmap->del(j);
			hmap->get(j);
			std::cout << "Lefutott\n";
		}
	};
}
