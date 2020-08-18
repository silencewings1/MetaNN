# useful tool functions

## time

```c++
#include <iostream>
#include <chrono>

auto tic = []() {
	return std::chrono::system_clock::now();
};

void toc(std::chrono::system_clock::time_point start)
{
	auto end = std::chrono::system_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << "cost "
		<< double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den
		<< "s";
};

void toc(std::chrono::system_clock::time_point start, std::string msg)
{
	std::cout << '[' << msg << "]\t";
	toc(start);
};
```



## random

````c++
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<type> dist(min_num, max_num - 1);

auto num = dist(gen);
````

