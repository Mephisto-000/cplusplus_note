// 利用指標計算質數

#include <iostream>
#include <iomanip>

int main()
{
	const size_t cMax { 100 };    // 質數的量
	long ayPrimes[cMax] { 2L };   // 定義第一個質數
	size_t cCount{ 1 };			  // 目前為止找到的質數數量
	long lTrial{ 3L };            // 待檢查的數值


	while (cCount < cMax)
	{
		bool bIsPrime{ true };    // 判斷是否為質數 

		for (size_t i {}; i < cCount && bIsPrime; i++)
		{	// 依目前找到的質數去找下一個質數
			bIsPrime = lTrial % *(ayPrimes + i) > 0;
		}

		
		if (bIsPrime)
		{	// 確認是質數後，儲存到質數陣列內
			*(ayPrimes + cCount++) = lTrial;
		}

		lTrial += 2;              // 檢查下一個數值

	}


	// 每 10 個質數印成一列
	std::cout << "The first " << cMax << " primes are : " << std::endl;
	for (size_t i {}; i < cMax; i++)
	{
		std::cout << std::setw(7) << *(ayPrimes + i);
		if ((i + 1) % 10 == 0) 
		{	// 每 10 個質數之後跳新的一列
			std::cout << std::endl;
		}
	}

	return 0;
}