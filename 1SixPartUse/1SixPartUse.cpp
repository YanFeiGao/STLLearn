// 1SixPartUse.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <cmath>
using namespace std;
namespace cppClassTest
{
	class A
	{
	public: 
		virtual void f() 
		{ 
			cout << "A::f() "; 
		}
		void f() const 
		{ 
			cout << "A::f() const "<<endl;
		}
	};
	class B : public A
	{
	public:
		void f() 
		{ 
			cout << "B::f() "; 
		}
		void f() const 
		{ 
			cout << "B::f() const "; 
		}
	};
	void g(const A* a) 
	{ 
		a->f(); 
	}
	int testPolymorphism() 
	{
		//多态调用
		A* p = new B(); 
		p->f(); 
		g(p); 
		delete(p); 
		return 0; 
	}

	class Foo 
	{

	public:Foo(int x) :value(x) {}

		   ~Foo(){}

	private:Foo() {}

		   int value = 0;//类内初始化 考点
	};

	struct date
	{
		int yy =0,mm = -1,dd = -1;
	};
	struct worklist
	{
		char name[20] = "llt test struct";
		char sex = '0';
		struct date birthday;
		date diyDay;
	}person,*pPerson;
	struct Student { //声明一个结构体类型Student
		int num = -1; //声明一个整形变量num
		char name[20]; //声明一个字符型数组name
		char sex; //声明一个字符型变量sex
		int age; //声明一个整形变量age
		float score; //声明一个单精度型变量
		char addr[30]; //声明一个字符型数组addr
	};
	struct Student0 { //声明一个结构体类型Student
		int num = 0; //声明一个整形变量num
		char name[20]; //声明一个字符型数组name
		char sex; //声明一个字符型变量sex
		int age; //声明一个整形变量age
		float score; //声明一个单精度型变量
		char addr[30]; //声明一个字符型数组addr
	}student3, student4;//声明变量student1和student2

	struct Student1 { //声明一个结构体类型Student
		int num; //声明一个整形变量num
		char name[20]; //声明一个字符型数组name
		char sex; //声明一个字符型变量sex
		int age; //声明一个整形变量age
		float score; //声明一个单精度型变量
		char addr[30]; //声明一个字符型数组addr
	} student7 = {
	10001,
	"qianshou",
	'm',
	19,
	100,
	"JiNan"
	};
	#define AA struct aa AA{int n;} tb1;//注意断句
	typedef struct bb
	{
		int n;
	}BB;//BB为别名，不再是变量，因为typedef
	int testStruct()
	{
		person.birthday.yy = 1958;
		pPerson = new worklist();
		pPerson->birthday.yy = 1968;

		Student student1, student2;// 定义结构体类型变量student1和student2
		cout << sizeof(Student) << endl;
		cout << sizeof(student1) << endl;
		cout << sizeof(student2) << endl;

		cout << sizeof(Student0) << endl;
		cout << sizeof(student3) << endl;
		cout << sizeof(student4) << endl;

		struct { //声明一个结构体类型Student
			int num = 1; //声明一个整形变量num
			char name[20]; //声明一个字符型数组name
			char sex; //声明一个字符型变量sex
			int age; //声明一个整形变量age
			float score; //声明一个单精度型变量
			char addr[30]; //声明一个字符型数组addr
		}student5, student6;//声明变量student1和student2
		cout << sizeof(student5) << endl;
		cout << sizeof(student6) << endl;
		Student1 student8 = {
	  10002,
	  "qianshou",
	  'm',
	  19,
	  100,
	  "JiNan"
		};
		return 1;
	}

	class MyClass
	{
	public:
		MyClass(int i = 0)
		{
			cout << i;
		}
		MyClass(const MyClass &x)
		{
			cout << 2;
		}
		MyClass &operator=(const MyClass &x)
		{
			cout << 3;
			return *this;
		}
		~MyClass()
		{
			cout << 4;
		}
	};

	void TestCopyConstructor()
	{
		MyClass obj1(1), obj2(2);
		MyClass obj3 = obj1;//编译器认为这是一个赋初始值操作,参看obj5
		MyClass obj5(obj1);
		MyClass obj4;
		obj4 = obj1;
		return ;
	}
}
namespace cppPointTest
{
	char* p0(char a[10]) //返回值为指针的函数
	{
		char* r =& a[0];
		return r; 
	}
	char(*p1)(char a[10]);//函数指针
	int testexp()
	{
		//#todo 函数指针  返回值为指针的函数
		int(*ng)[10];//数组指针
		int* n[10];//指针数组
		cout << "int(*ng)[10];数组指针大小为" << sizeof(ng) << endl;
		cout << "int* n[10];指针数组大小为" << sizeof(n) << endl;
		int k = 2;
		int *p1int, *p2int;
		p1int = &k;
		p2int = &k;
		k = *p1int + *p2int;
		p1int = p2int;
		k = *p1int**p2int;

		char a[] = "SF-TECH";
		char*p = a;
		p++;
		printf("%s\n", a);
		return 1;
	}
}
namespace cppExpressionTest
{
	void ptr()
	{
		for (int i = 5; i < 8; i++) printf("%c", '*');
		printf("\t");
	}
	void tests()
	{

		char stra[7] = "string";
		char*charp = stra;
		cout << charp + 3 << endl;

		long longa = 1, longb = 32;
		printf("%ld,%ld\n", longa << longb, 1 << 32);
		printf("%d,%d\n", 101010, 101010);//数据隐含右对齐
		printf("%-10d,%-10d\n", 101010, 101010);//加上-号，是左对齐
		printf("%10d,%10d\n", 101010, 101010);//数据隐含右对齐
		printf("%.3s,%.3s\n", "abcdef", "abcdef");//数据隐含右对齐,最多输出3个字符

		int inta = 12;
		inta &= (inta - 1);//等价 inta = inta &（inta-1）
		//scanf_s("%d", &inta);

		inta = printf("I see, Sea in C");
		inta = printf("%d\n", inta);
		float fround = round(11.5f);

		int aq = 666, bq = 888;
		printf("%d\n", aq, bq);

		char ca[][5] = { "A","BB","CCC" };
		char ca1[3][5] = { "A","BB","CCC" };
		//char *ca3[3] = { "A","BB","CCC" };

		int a = 1; int b = 2; int c = 2; int t;
		while (a < b &&b< c)
		{
			t = a; a = b; b = t; c--;
		}
		a = 1, b = 2, c = 2, t = -1;
		//while (a < b < c)//warning C4804: “<”: 在操作中使用类型“bool”不安全
		{
			t = a; a = b; b = t; c--;
		}
		printf("%d,%d,%d\n", a, b, c);
		int arrInt[3][4] = { 0,1,2,3,4,5,6,7,8,9,10,11 };
		int table[3][2] = { {1}, {3,4}, {5} };
		int (*aaa)[4] =arrInt;//数组指针
		int *p0 = arrInt[1]+1;
		int *p1  =*(arrInt + 1);
		int *p2 = (*(arrInt + 1) + 1);

		if (a)//if语句，非零就会进入
		{
			int aa = -2;
		}
		printf("%d\n", a = 0);//返回值为0
		while (a =0)
		{
			a++;
		}
		return;
	}
}


int main()
{
    //std::cout << "Hello World!\n"; 
	cppExpressionTest::tests();
	cppClassTest::testPolymorphism();
	cppPointTest::testexp();
	cppClassTest::TestCopyConstructor();
	cppClassTest::testStruct();
	return 0;
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
