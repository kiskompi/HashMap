#include "../HashMap/HashMap.h"
#include <iostream>


int main(int argc, char* argv[]) {
	std::cout << "Kezdes"<<std::endl;
	{
	HashMap<int, std::string> hmap = HashMap<int, std::string>();
	//hmap.put(1, "Anna");
	//hmap.put(2, "Janos");
	//hmap.put(3, "Bela");
	//hmap.put(4, "Kecske");
	//hmap.put(5, "Gergo");
	//hmap.put(6, "Eszter");
	//hmap.put(7, "Jancsi");

	for (int i = 0; i < 5000; i++)
	{
		hmap.put(i, std::to_string(i));
	}

	for (int i = 2000; i < 2090; i++)
	{
		std::cout << i << " : " << *(hmap.get(i)) << std::endl;
	}

	std::cout << "M�ret: " << hmap.size() << std::endl;
	}
	std::cout << "Lefutott";
	std::cin.get();

	return 0;
}