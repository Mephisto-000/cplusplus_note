// 採用陣列名稱的指標表示法
#include <iostream>


///////////////////////////////////////
// 使用指標表示法參考元素 : 
// lData[0], lData[1], lData[2], lData[3], ...
// = *lData, *(lData + 1), *(lData + 2), *(lData + 3), ...
///////////////////////////////////////


int main()
{
	long ayData[5] {};  // 宣告大小為 5 的一維陣列

	for (size_t i {}; i < std::size(ayData); i++)
	{
		*(ayData + i) = 2 * (i + 1);
	}


	std::cout << "ayData's elements : " << std::endl;
	for (size_t i{}; i < std::size(ayData); i++)
	{
		std::cout << *(ayData + i) << ", ";
	}


	return 0;
}