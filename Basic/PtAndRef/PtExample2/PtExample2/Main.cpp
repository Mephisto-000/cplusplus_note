// �Q�Ϋ��Эp����
// �ϥΰʺA�t�m����k

#include <iostream>
#include <iomanip>
#include <cmath>         // for square root function

int main()
{
	size_t cMax;         // �Q�n����Ƽƶq

	std::cout << "How many primes would you like ? ";
	std::cin >> cMax;    // ��J�Q�o�쪺��Ƽƶq

	if (cMax <= 0)
	{	// �n�O��J�p�󵥩� 0 ���ơA�{����������
		return 0;
	}


	// ���@�� cMax �� unsigned ���A�Ȱ}�C�t�m�O����A�ñN���}�s�b ptPrimes ��
	unsigned* ptPrimes{ new unsigned[cMax] };

	size_t cCount{ 1 };     // �ثe�����쪺��Ƽƶq
	ptPrimes[0] = 2;        // ���x�s�Ĥ@�ӽ��
	unsigned uiTrial{ 3 };  // ���ˬd���ƭ�

	while (cCount < cMax)
	{
		bool bIsPrime{ true };    // �P�_�O�_����� 

		// �ˬd���ˬd�ƭȥ���ڤ�����ơA�O�_�i�H�㰣���ˬd�ƭ�
		const double dLimit = static_cast<unsigned>(std::sqrt(uiTrial));
		for (size_t i{}; ptPrimes[i] <= dLimit && bIsPrime; i++)
		{
			bIsPrime = uiTrial % ptPrimes[i] > 0;
		}


		if (bIsPrime)
		{	// �T�{�O��ƫ�A�x�s���ư}�C��
			ptPrimes[cCount++] = uiTrial;
		}


		uiTrial += 2;              // �ˬd�U�@�Ӽƭ�

	}


	// �C 10 �ӽ�ƦL���@�C
	std::cout << "The first " << cMax << " primes are : " << std::endl;
	for (size_t i{}; i < cMax; i++)
	{
		std::cout << std::setw(10) << ptPrimes[i];
		if ((i + 1) % 10 == 0)
		{	// �C 10 �ӽ�Ƥ�����s���@�C
			std::cout << std::endl;
		}
	}

	delete[] ptPrimes;  // ���� ptPrimes �e�Ϊ��O����
	ptPrimes = nullptr; // ���]����

	return 0;
}