// DataStructCpp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//本工程进行数据结构的C++版本具体实现
#include "pch.h"
#include <iostream>
using namespace std;
namespace sort
{
	void InsertSortUp()
	{
		std::cout << "**************************start InsertSortUp**************************\n";
		int array0[10] = {0,9,8,7,6,5,4,3,2,1};
		for (auto i : array0)
		{
			std::cout <<i << ",";
		}
		std::cout << " " << endl;
		int i = 0, j = 0;
		for (i =2;i<10;++i)
		{
			if (array0[i] < array0[i - 1])
			{
				array0[0] = array0[i];
				j = i - 1;
				do
				{
					array0[j + 1] = array0[j];
					j--;
				} while (array0[j] > array0[0]);
				array0[j + 1] = array0[0];
			}
#ifdef _DEBUG //debug版本默认预处理器 ，release版本为 NDEBUG
			for (auto k : array0)
			{
				std::cout << k << ",";
			}
			std::cout << " " << endl;
#endif // _DEBUG
			
		}
		std::cout << "**************************end InsertSortUp**************************\n";
	}
	void InsertSortDown()
	{
		std::cout << "**************************start InsertSortDown**************************\n";
		int array0[10] = { 0,1,2,3,4,5,6,7,8,9 };
		for (auto i : array0)
		{
			std::cout << i << ",";
		}
		std::cout << " " << endl;
		int i = 0, j = 0;
		for (i = 2; i < 10; ++i)
		{
			if (array0[i]>array0[i-1])//降序需要前面大后面小，这里如果前面小，就开始排序
			{
				array0[0] = array0[i];
				j = i - 1;
				do 
				{
					array0[j + 1] = array0[j];
					j--;
				} while (array0[0]>array0[j]);
				array0[j+1] = array0[0];
			}
#ifdef _DEBUG //debug版本默认预处理器 ，release版本为 NDEBUG
			for (auto k : array0)
			{
				std::cout << k << ",";
			}
			std::cout << " " << endl;
#endif // _DEBUG
		}
		std::cout << "**************************end InsertSortDown**************************\n";
	}
	void sortTest() 
	{
		InsertSortUp();
		std::cout << " " << endl;
		InsertSortDown();
		std::cout << " " << endl;
	}
}
int main()
{
	sort::sortTest();
    std::cout << "Hello World!\n"; 
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
