#include <iostream>
#include <random>

#include<vector>
#include<string>


int main()
{
	std::default_random_engine e(time(0));
	std::uniform_real_distribution<float> u(0,1);
	std::uniform_int_distribution<int> a(0,5);
	std::normal_distribution<> n(4, 1.5); 
	std::bernoulli_distribution b(.75); //probabilidad de true, por defecto está en 50%

	for (int i = 0; i < 10; i++)
	{
		//std::cout << e() << std::endl;
		std::cout << u(e) << std::endl;
		//std::cout << n(e) << std::endl;
	 
	}

	bool test = b(e);
	std::cout << (test ? "Test 1" : "test 2") << std::endl;




	//printar la distribución
	std::vector<unsigned> vals(9);
	for(int i = 0; i != 200; ++i)
	{
		unsigned v = lround(n(e));
		if (v < vals.size())
			++vals[v];
	}

	for (int i = 0; i != vals.size(); ++i)
	{
		std::cout << i << ": " << std::string(vals[i], '*') << std::endl;
	}

	return 0;
}