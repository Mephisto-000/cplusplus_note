// 利用指標計算質數
// 使用動態配置的方法

#include <iostream>
#include <iomanip>
#include <cmath>         // for square root function

int main()
{
	size_t cMax;         // 想要的質數數量

	std::cout << "How many primes would you like ? ";
	std::cin >> cMax;    // 輸入想得到的質數數量

	if (cMax <= 0)
	{	// 要是輸入小於等於 0 的數，程式直接結束
		return 0;
	}


	// 為一個 cMax 個 unsigned 型態值陣列配置記憶體，並將其位址存在 ptPrimes 中
	unsigned* ptPrimes{ new unsigned[cMax] };

	size_t cCount{ 1 };     // 目前為止找到的質數數量
	ptPrimes[0] = 2;        // 先儲存第一個質數
	unsigned uiTrial{ 3 };  // 待檢查的數值

	while (cCount < cMax)
	{
		bool bIsPrime{ true };    // 判斷是否為質數 

		// 檢查待檢查數值平方根內的質數，是否可以整除待檢查數值
		const double dLimit = static_cast<unsigned>(std::sqrt(uiTrial));
		for (size_t i{}; ptPrimes[i] <= dLimit && bIsPrime; i++)
		{
			bIsPrime = uiTrial % ptPrimes[i] > 0;
		}


		if (bIsPrime)
		{	// 確認是質數後，儲存到質數陣列內
			ptPrimes[cCount++] = uiTrial;
		}


		uiTrial += 2;              // 檢查下一個數值

	}


	// 每 10 個質數印成一列
	std::cout << "The first " << cMax << " primes are : " << std::endl;
	for (size_t i{}; i < cMax; i++)
	{
		std::cout << std::setw(10) << ptPrimes[i];
		if ((i + 1) % 10 == 0)
		{	// 每 10 個質數之後跳新的一列
			std::cout << std::endl;
		}
	}

	delete[] ptPrimes;  // 釋放 ptPrimes 占用的記憶體
	ptPrimes = nullptr; // 重設指標

	return 0;
}