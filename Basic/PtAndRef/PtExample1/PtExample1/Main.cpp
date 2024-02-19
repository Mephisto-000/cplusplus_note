// �Q�Ϋ��Эp����

#include <iostream>
#include <iomanip>

int main()
{
	const size_t cMax { 100 };    // ��ƪ��q
	long ayPrimes[cMax] { 2L };   // �w�q�Ĥ@�ӽ��
	size_t cCount{ 1 };			  // �ثe�����쪺��Ƽƶq
	long lTrial{ 3L };            // ���ˬd���ƭ�


	while (cCount < cMax)
	{
		bool bIsPrime{ true };    // �P�_�O�_����� 

		for (size_t i {}; i < cCount && bIsPrime; i++)
		{	// �̥ثe��쪺��ƥh��U�@�ӽ��
			bIsPrime = lTrial % *(ayPrimes + i) > 0;
		}

		
		if (bIsPrime)
		{	// �T�{�O��ƫ�A�x�s���ư}�C��
			*(ayPrimes + cCount++) = lTrial;
		}

		lTrial += 2;              // �ˬd�U�@�Ӽƭ�

	}


	// �C 10 �ӽ�ƦL���@�C
	std::cout << "The first " << cMax << " primes are : " << std::endl;
	for (size_t i {}; i < cMax; i++)
	{
		std::cout << std::setw(7) << *(ayPrimes + i);
		if ((i + 1) % 10 == 0) 
		{	// �C 10 �ӽ�Ƥ�����s���@�C
			std::cout << std::endl;
		}
	}

	return 0;
}