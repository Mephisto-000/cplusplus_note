// �ĥΰ}�C�W�٪����Ъ�ܪk
#include <iostream>


///////////////////////////////////////
// �ϥΫ��Ъ�ܪk�ѦҤ��� : 
// lData[0], lData[1], lData[2], lData[3], ...
// = *lData, *(lData + 1), *(lData + 2), *(lData + 3), ...
///////////////////////////////////////


int main()
{
	long ayData[5] {};  // �ŧi�j�p�� 5 ���@���}�C

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