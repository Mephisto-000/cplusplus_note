#include "Sort.h"
#include <stdlib.h>
#include <memory.h>
#include <limits>


/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: InsertionSort

  Summary:  ���J�ƧǪk

  Args:     
	int ayData[]
		���Ƨǰ}�C�C
	int iLen
		���Ƨǰ}�C���סC

  Returns:	None.
-----------------------------------------------------------------F-F*/
void InsertionSort (int ayData[], int iLen)
{
	for (int i = 1; i < iLen; i++)
	{
		int iValue = ayData[i];

		int j = i - 1;

		while (iValue < ayData[j] && j >= 0)
		{
			ayData[j + 1] = ayData[j];
			j--;
		}

		ayData[j + 1] = iValue;

	}
}

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: ShellSort

  Summary:  �º��ƧǪk

  Args:
	int ayData[]
		���Ƨǰ}�C�C
	int iLen
		���Ƨǰ}�C���סC

  Returns:	None.
-----------------------------------------------------------------F-F*/
void ShellSort (int ayData[], int iLen)
{
	for (int iGap = iLen / 2; iGap > 0; iGap /= 2)
	{
		for (int i = iGap; i < iLen; i += 1)
		{
			int iValue = ayData[i];
			int j;
			for (j = i; j >= iGap && ayData[j - iGap] > iValue; j -= iGap)
			{
				ayData[j] = ayData[j - iGap];
			}

			ayData[j] = iValue;

		}
	}
}

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: SelectionSort

  Summary:  ��ܱƧǪk

  Args:
	int ayData[]
		���Ƨǰ}�C�C
	int iLen
		���Ƨǰ}�C���סC

  Returns:	None.
-----------------------------------------------------------------F-F*/
void SelectionSort (int ayData[], int iLen)
{
	for (int i = 0; i < iLen - 1; i++)
	{
		int iMinimalIndex = i;
		for (int j = i + 1; j < iLen; j++)
		{
			if (ayData[j] < ayData[iMinimalIndex])
			{
				iMinimalIndex = j;
			}
		}

		int iTmp = ayData[iMinimalIndex];
		ayData[iMinimalIndex] = ayData[i];
		ayData[i] = iTmp;

	}
}

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: BubbleSort

  Summary:  �w�w�ƧǪk

  Args:
	int ayData[]
		���Ƨǰ}�C�C
	int iLen
		���Ƨǰ}�C���סC

  Returns:	None.
-----------------------------------------------------------------F-F*/
void BubbleSort (int ayData[], int iLen)
{
	for (int i = 0; i < iLen - 1; i++)
	{
		for (int j = 0; j < iLen - i - 1; j++)
		{
			if (ayData[j] > ayData[j + 1])
			{
				int iTmp = ayData[j];
				ayData[j] = ayData[j + 1];
				ayData[j + 1] = iTmp;
			}
		}
	}
}

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: ShakerSort

  Summary:  ���V�_�w�ƧǪk

  Args:
	int ayData[]
		���Ƨǰ}�C�C
	int iLen
		���Ƨǰ}�C���סC

  Returns:	None.
-----------------------------------------------------------------F-F*/
void ShakerSort(int ayData[], int iLen)
{
	int iLeft = 0;
	int iRight = iLen - 1;
	int iShift = 1;

	while (iLeft < iRight)
	{
		for (int i = iLeft; i < iRight; i++)
		{
			if (ayData[i] > ayData[i + 1])
			{
				int iTmp = ayData[i];
				ayData[i] = ayData[i + 1];
				ayData[i + 1] = iTmp;

				iShift = i;
			}
		}

		iRight = iShift;

		for (int i = iRight - 1; i >= iLeft; i--)
		{
			if (ayData[i] > ayData[i + 1])
			{
				int iTmp = ayData[i];
				ayData[i] = ayData[i + 1];
				ayData[i + 1] = iTmp;

				iShift = i + 1;
			}
		}

		iLeft = iShift;

	}
}

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: Merge

  Summary:  �X�֤w�ƧǦn���}�C

  Args:
	int ayData[]
		���k�U�b�Ƨǧ������}�C�C
	int iStart
		�ƾڰ_�l��m�C
	int iFinish
		�ƾڥ��ݦ�m�C
	int iMiddle
		�ƾڤ�����m

  Returns:	None.
-----------------------------------------------------------------F-F*/
void Merge(int ayData[], int iStart, int iFinish, int iMiddle)
{
	int iSizeLeft = iMiddle - iStart + 1;
	int iSizeRight = iFinish - iMiddle;

	int* iLeft = (int*)malloc(sizeof(int) * (iSizeLeft + 1));
	int* iRight = (int*)malloc(sizeof(int) * (iSizeRight + 1));

	// �ˬd�O������t�O�_���\
	if (!iLeft || !iRight) {
		free(iLeft);  // �T�O����w���t���O����
		free(iRight);
		return;  // �b������Τ����B�z���~�A�p�q�L��^���~�N�X
	}

	memcpy(iLeft, ayData + iStart, sizeof(int) * (iSizeLeft));
	memcpy(iRight, ayData + iMiddle + 1, sizeof(int) * (iSizeRight));

	iLeft[iSizeLeft] = INT_MAX;
	iRight[iSizeRight] = INT_MAX;

	int iLeftIndex = 0;
	int iRightIndex = 0;

	for (int i = iStart; i <= iFinish; i++)
	{
		if (iLeft[iLeftIndex] <= iRight[iRightIndex])
		{
			ayData[i] = iLeft[iLeftIndex];
			iLeftIndex++;
		}
		else
		{
			ayData[i] = iRight[iRightIndex];
			iRightIndex++;
		}
	}

	free(iLeft);
	free(iRight);

}

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: MergeSort

  Summary:  �X�ֱƧǪk

  Args:
	int ayData[]
		���Ƨǰ}�C�C
	int iStart
		�ƾڰ_�l��m�C
	int iFinish
		�ƾڥ��ݦ�m�C

  Returns:	None.
-----------------------------------------------------------------F-F*/
void MergeSort(int ayData[], int iStart, int iFinish)
{
	if (iFinish > iStart)
	{
		int iMiddle = (iFinish + iStart) / 2;
		MergeSort(ayData, iStart, iMiddle);
		MergeSort(ayData, iMiddle + 1, iFinish);
		Merge(ayData, iStart, iFinish, iMiddle);
	}
}

