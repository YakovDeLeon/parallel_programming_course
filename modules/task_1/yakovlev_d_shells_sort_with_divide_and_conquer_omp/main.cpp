#include <string>
#include <iostream>
#include <memory>
#include <ctime>
#include <cassert>
#include <chrono>

using namespace std;
using namespace std::chrono;
#pragma warning (disable: 4703)

template<typename Type>
void FillArray(Type* pd2dArray, uint32_t unSizeArr, Type Begin, Type End);

template<typename Type>
void ShowArray(Type* pdArray, uint32_t unSizeArr);

template <typename Type>
void ShellMerge(const Type* pArr, Type* pResArr, uint32_t nSizeArr);

template <typename BaseType>
void ShellsSort(BaseType *A, uint32_t N);



int main(int argc, char** argv)
{
	using time = chrono::steady_clock::time_point;
	int nSizeArr;

	if (argc == 5)
	{
		nSizeArr = atoi(argv[1]);
	}
	else
	{
		nSizeArr = 10;
	}

	double * pdArr = new double[nSizeArr] {0};
	double * pdCopyArr = new double[nSizeArr] {0};
	double * pdResArr = new double[nSizeArr] {0};

	if (argc == 5)
	{
		FillArray(pdArr, nSizeArr, atof(argv[2]), atof(argv[3]));
	}
	else
	{
		FillArray(pdArr, nSizeArr, 0.0, 10.0);
	}

	memcpy(pdCopyArr, pdArr, nSizeArr * sizeof(double));

	time startTime = chrono::high_resolution_clock::now();
	std::cout << "Computer has been beginning a solving for merge version!" << std::endl;
	ShellMerge(pdArr, pdResArr, nSizeArr);
	time endTime = chrono::high_resolution_clock::now();
	auto WorkTimeMerge = chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count();
	cout << "Time of work the merge version: " << (double)WorkTimeMerge << " ns" << endl;

	startTime = chrono::high_resolution_clock::now();
	std::cout << "Computer has been beginning a solving for serial version!" << std::endl;
	ShellsSort(pdCopyArr, nSizeArr);
	endTime = chrono::high_resolution_clock::now();
	auto WorkTimeSerial = chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count();
	cout << "Time of work serial version: " << (double)WorkTimeSerial << " ns" << endl;
	
	string sLess = (WorkTimeMerge < WorkTimeSerial ? "faster" : "slower");
	cout << "Merge version " << sLess << " serial version!" << std::endl;
	for (size_t i = 0; i < nSizeArr; i++)
	{
		if (pdCopyArr[i] != pdResArr[i])
		{
			cout << "Serial and parallel versions are not matching!" << endl;
			break;
		}
	}
	delete[] pdArr, pdResArr;
	return 0;
}

template<typename Type>
void FillArray(Type* pArr, uint32_t unSizeArr, Type Begin, Type End)
{
	srand(time(nullptr));
	for (uint32_t i = 0; i < unSizeArr; ++i)
	{
		pArr[i] = rand() % (int)(End - Begin) + Begin + double(rand()) / 1000000.f;
	}
}

template<typename Type>
void ShowArray(Type* pdArray, uint32_t unSizeArr)
{
	for (uint32_t i = 0; i < unSizeArr; ++i)
	{
		std::cout << "Arr[" << i << "] = " << pdArray[i] << std::endl;
	}
}


template <typename BaseType>
void ShellsSort(BaseType * pArr, uint32_t N)
{
	assert(pArr);
	uint32_t i, j, k;
	BaseType t;
	for (k = N / 2; k > 0; k /= 2)
		for (i = k; i < N; i++)
		{
			t = pArr[i];
			for (j = i; j >= k; j -= k)
			{
				if (t < pArr[j - k])
					pArr[j] = pArr[j - k];
				else
					break;
			}
			pArr[j] = t;
		}
}

template<typename T>
void Merge(T* pArr1, T* pArr2, T* pResArr, uint32_t unSize1, uint32_t unSize2)
{
	uint32_t a = 0;
	uint32_t b = 0;
	uint32_t i = 0;
	while ((a != unSize1) && (b != unSize2)) 
	{
		if (pArr1[a] <= pArr2[b])
		{
			pResArr[i] = pArr1[a];
			a++;
		}
		else
		{
			pResArr[i] = pArr2[b];
			b++;
		}
		i++;
	}
	if (a == unSize1)
	{
		uint32_t j = b;
		for (; j<unSize2; j++, i++)
			pResArr[i] = pArr2[j];
	}
	else
	{
		uint32_t j = a;
		for (; j<unSize1; j++, i++)
			pResArr[i] = pArr1[j];
	}
}


template<typename T>
void MergeArrays(T* pArr1, T* pArr2, uint32_t unSize1, uint32_t unSize2, T* pResArr)
{
	uint32_t nMedIndex = BinSearch(pArr2, 0, unSize2, (pArr1[unSize1 / 2]));
	uint32_t nTmp1Size = nMedIndex + unSize1 / 2;
	T* pTmp1 = new T[nTmp1Size];
	
	uint32_t nTmp2Size = unSize1 + unSize2 - nTmp1Size;
	T* pTmp2 = new T[nTmp2Size];
	
	Merge(pArr1, pArr2, pTmp1, unSize1 / 2, nMedIndex);
	Merge(pArr1 + unSize1 / 2, pArr2 + nMedIndex, pTmp2, unSize1 - unSize1 / 2, unSize2 - (nMedIndex));
	
	memcpy(pResArr, pTmp1, nTmp1Size * sizeof(T));
	memcpy(pResArr + nTmp1Size, pTmp2, nTmp2Size * sizeof(T));
	delete[] pTmp1, pTmp2;
}

template<typename T>
uint32_t BinSearch(T *pArr, uint32_t l, uint32_t r, T x)
{
	if (l == r)
		return l;
	if (l + 1 == r)
		if (x < pArr[l])
			return l;
		else
			return r;
	uint32_t m = (l + r) / 2;
	if (x < pArr[m])
		r = m;
	else
		if (x > pArr[m])
			l = m;
		else
			return m;
	return BinSearch(pArr, l, r, x);
}


template <typename Type>
void ShellMerge(const Type *pArr, Type *pResArr, uint32_t nSizeArr)
{
	assert(pArr);
	assert(pResArr);

	uint32_t nTmpArr1Size = nSizeArr / 2;
	Type* pTmpArr1 = new Type[nTmpArr1Size];
	memcpy(pTmpArr1, pArr, nTmpArr1Size * sizeof(Type));
	
	uint32_t nTmpArr2Size = nSizeArr - nSizeArr / 2;
	Type* pTmpArr2 = new Type[nTmpArr2Size];
	memcpy(pTmpArr2, pArr + nTmpArr1Size, nTmpArr2Size * sizeof(Type));

	ShellsSort(pTmpArr1, nTmpArr1Size);
	ShellsSort(pTmpArr2, nTmpArr2Size);
	
	MergeArrays(pTmpArr1, pTmpArr2, nTmpArr1Size, nTmpArr2Size, pResArr);
	delete[] pTmpArr1, pTmpArr2;
}