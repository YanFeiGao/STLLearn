// 1SixPartUse.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <cmath>
#include <list>
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

		   void fun(int *i) {};//等价void fun(int *const i) {};
		   //void fun(int *const i) {};
		   void fun(const int *i) {};
		   
		   void fun(int i) {};
		   //void fun(const int i) {};//等价于void fun(int const i) {};
		   //void fun(int const i) {};
		   
		   
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

		size_t size = sizeof(student2);
		size = sizeof(Student1);
		vector<Student1> vec;
		cout << "empty vector size: " << sizeof(vec) << endl;
		vec.push_back(student7);
		cout << "push 1 vector size: " << sizeof(vec) << endl;
		vec.push_back(student7);
		cout << "push 2 vector size: " << sizeof(vec) << endl;
		vec.push_back(student7);
		vec.push_back(student7);
		cout << "push 4 vector size: " << sizeof(vec) << endl;
		vec.push_back(student7);
		vec.push_back(student7);
		vec.push_back(student7);
		vec.push_back(student7);
		cout << "push 8 vector size: " << sizeof(vec) << endl;
		vec.push_back(student7);
		vec.push_back(student7);
		vec.push_back(student7);
		vec.push_back(student7);
		vec.push_back(student7);
		vec.push_back(student7);
		vec.push_back(student7);
		vec.push_back(student7);
		cout << "push 16 vector size: " << sizeof(vec) << endl;
		cout << "only calculate vector contain solid part, not elements"<< endl;
		vector<Student1>::size_type vecsize = vec.size();
		size = sizeof(student2);
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
namespace cppPointerTest
{
	char* p0(char a[10]) //返回值为指针的函数
	{
		char* r =& a[0];
		return r; 
	}
	char(*p1)(char a[10]);//函数指针
	int testpointer()
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
	void unFormatTest()
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
		while (a < b &&b < c)
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
		int(*aaa)[4] = arrInt;//数组指针
		int *p0 = arrInt[1] + 1;
		int *p1 = *(arrInt + 1);
		int *p2 = (*(arrInt + 1) + 1);

		if (a)//if语句，非零就会进入
		{
			int aa = -2;
		}
		printf("%d\n", a = 0);//返回值为0
		while (a = 0)
		{
			a++;
		}

		//cout << a < 60 ? "pass" : "fail";
		// <<返回值为basic_ostream
		//无法编译，等价于(cout << a) < 60 ? "pass" : "fail";
		cout << (a < 60) ? "pass" : "fail";//error
		cout << ((a < 60) ? "pass" : "fail");//success

		//int &pq = fround;//定义pq就是为了用它来修改fround的值，但是实际上pq却绑定到了一个临时量上
		// 这肯定是不希望的，所以说，语句被定义为非法
		
		//常量对象的地址必须由常量指针管理，常量对象的引用也必须是常量引用
		const int temp = fround;
		const int* pt = &a;
		return;
	}
	void PrimeTest()
	{
		//1. cast 
		//1.1 static_cast
		int i = 1, j = 2;
		int itemp = i / j;// output 0 
		double dtemp = i / j;// output 0.0
		//static_cast等于告诉编译器不需要在意精度的损失，所以说这在一定程度上。可以关闭一些因为精度丢失所产生的警告
		//只要不包含底层const，都可以使用static_cast
		double static_cast_value = static_cast<double>(i) / j;// output 0.500
		void* pVoid = &dtemp;
		double* pdtemp = static_cast<double*>(pVoid);
		//1.1const_cast
		const char* pCon_char = "Test const char";
		//语法上是正确的，但是通过char*指针来写值会产生未定义的结果
		char* pchar0 = const_cast<char*>(pCon_char);
		//1.2 
		*pdtemp = 12;
		char* pcharValue = reinterpret_cast<char*>(pdtemp);
		string str(pcharValue);

		//2.优先级与结合律
		// C++ Operator Precedence and Associativity
		// The highest precedence level is at the top of the table.
		//+------------------+-----------------------------------------+---------------+
		//| Operator         | Name or Meaning                         | Associativity |
		//+------------------+-----------------------------------------+---------------+
		//| ::               | Scope resolution                        | None          |
		//| ::               | Global                                  | None          |
		//| [ ]              | Array subscript                         | Left to right |
		//| ( )              | Function call                           | Left to right |
		//| ( )              | Conversion                              | None          |
		//| .                | Member selection (object)               | Left to right |
		//| ->               | Member selection (pointer)              | Left to right |
		//| ++               | Postfix increment                       | None          |
		//| --               | Postfix decrement                       | None          |
		//| new              | Allocate object                         | None          |
		//| delete           | Deallocate object                       | None          |
		//| delete[ ]        | Deallocate object                       | None          |
		//| ++               | Prefix increment                        | None          |
		//| --               | Prefix decrement                        | None          |
		//| *                | Dereference                             | None          |
		//| &                | Address-of                              | None          |
		//| +                | Unary plus                              | None          |
		//| -                | Arithmetic negation (unary)             | None          |
		//| !                | Logical NOT                             | None          |
		//| ~                | Bitwise complement                      | None          |
		//| sizeof           | Size of object                          | None          |
		//| sizeof ( )       | Size of type                            | None          |
		//| typeid( )        | type name                               | None          |
		//| (type)           | Type cast (conversion)                  | Right to left |
		//| const_cast       | Type cast (conversion)                  | None          |
		//| dynamic_cast     | Type cast (conversion)                  | None          |
		//| reinterpret_cast | Type cast (conversion)                  | None          |
		//| static_cast      | Type cast (conversion)                  | None          |
		//| .*               | Apply pointer to class member (objects) | Left to right |
		//| ->*              | Dereference pointer to class member     | Left to right |
		//| *                | Multiplication                          | Left to right |
		//| /                | Division                                | Left to right |
		//| %                | Remainder (modulus)                     | Left to right |
		//| +                | Addition                                | Left to right |
		//| -                | Subtraction                             | Left to right |
		//| <<               | Left shift                              | Left to right |
		//| >>               | Right shift                             | Left to right |
		//| <                | Less than                               | Left to right |
		//| >                | Greater than                            | Left to right |
		//| <=               | Less than or equal to                   | Left to right |
		//| >=               | Greater than or equal to                | Left to right |
		//| ==               | Equality                                | Left to right |
		//| !=               | Inequality                              | Left to right |
		//| &                | Bitwise AND                             | Left to right |
		//| ^                | Bitwise exclusive OR                    | Left to right |
		//| |                | Bitwise OR                              | Left to right |
		//| &&               | Logical AND                             | Left to right |
		//| ||               | Logical OR                              | Left to right |
		//| e1?e2:e3         | Conditional                             | Right to left |
		//| =                | Assignment                              | Right to left |
		//| *=               | Multiplication assignment               | Right to left |
		//| /=               | Division assignment                     | Right to left |
		//| %=               | Modulus assignment                      | Right to left |
		//| +=               | Addition assignment                     | Right to left |
		//| -=               | Subtraction assignment                  | Right to left |
		//| <<=              | Left-shift assignment                   | Right to left |
		//| >>=              | Right-shift assignment                  | Right to left |
		//| &=               | Bitwise AND assignment                  | Right to left |
		//| |=               | Bitwise inclusive OR assignment         | Right to left |
		//| ^=               | Bitwise exclusive OR assignment         | Right to left |
		//| ,                | Comma                                   | Left to right |
		//+------------------+-----------------------------------------+---------------+
		//*(解引用)运算符优先级优于+(加号)运算符
		int a[3][3] = { 1,3,5,7,9,11,13,15,17 };
		cout << "a[2]+1:" << a[2] + 1 << endl;//生成a[2][1]的地址，二维数组中a[2]代表一个一维数组的首地址
		cout << "*a[2]+1:" << *a[2] + 1 << endl;//先对a[2][0]取值，再加1
		cout << "*(a[2]+1):" << *(a[2] + 1) << endl;//生成a[2][1]的地址，再取值
		cout << "(*a[2])+1:" << (*a[2]) + 1 << endl;//先对a[2][0]取值，再加1

		//*(解引用)运算符优先级与+(一元正号)运算符相同，皆为右结合律
		cout << "+*a[2]：" << +*a[2] << endl;


		//*(解引用)运算符的优先级低于后置递增运算符(++),但是结果为1
		//这是一个坑点，因为后置++返回的是原对象的副本，并不会将其递增的结果返回
		int c[3] = { 1,3,5 };
		vector<int> b(c, c + 3);
		auto p = b.begin();
		cout << "*p++:" << *p++ << endl;

		cin.get();
		return;
	}
}
namespace cppTemplateTest
{
	// general version
	template<class T>
	class Compare
	{
	public:
		static bool IsEqual(const T& lh, const T& rh)
		{
			return lh == rh;
		}
	};
	// specialize for float
	template<>
	class Compare<float>
	{
	public:
		static bool IsEqual(const float& lh, const float& rh)
		{
			return abs(lh - rh) < 10e-3;
		}
	};

	// specialize for double
	template<>
	class Compare<double>
	{
	public:
		static bool IsEqual(const double& lh, const double& rh)
		{
			return abs(lh - rh) < 10e-6;
		}
	};


	//类模板
	template<typename T, typename Q>
	class ClassTemplate
	{
	public:
		ClassTemplate(T r = 0,Q t = 0) :i(r),j(t)
		{
		};
		~ClassTemplate()
		{};
		ClassTemplate& operator += (const ClassTemplate&)
		{
		};
		size_t operator() (T)const { return T; };
		
	private:
		T i;
		Q j;
	};
	//特化
	template<> class ClassTemplate<char,char>
	{
	public:
		ClassTemplate& operator += (const ClassTemplate&)
		{
		};
	};
	//偏特化 “个数上的偏”
	template< typename Q> class ClassTemplate<char, typename Q>
	{
	public:
		ClassTemplate& operator += (const ClassTemplate&)
		{
		};
	};
	//偏特化 “范围上的偏”
	template<typename T, typename Q>
	class ClassTemplate<T*,Q*>
	{
	};

	//函数模板
	template<typename T> inline const T& min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	//成员模板
	template<typename T>
	class MemberTemplate
	{
	public:
		MemberTemplate(T r = 0) :i(r)
		{
		};
		~MemberTemplate()
		{};
		MemberTemplate& operator += (const MemberTemplate&)
		{
		};
		template<typename U>
		MemberTemplate(const MemberTemplate<U>& p):i(p.i)
		{
		}
	private:
		T i;
	};

	//模板模板参数
	template<typename T,
			template<typename T> class container
	>
	class Xscl
	{
	private:
		container<T> c;
	};
	template<typename T>
	using	Lst = list<T, allocator<T>>;//其实容器是需要分配器的

	void templateTest()
	{
		ClassTemplate<int,int> c1(1,2);
		const int a = min(int(1), int(2));
		MemberTemplate<int> c2 = MemberTemplate<int>(1);

		//Xscl<string, list> myLst1;//存在错误
		Xscl<string, Lst> myLst2;
	}
}

int main()
{
	cppExpressionTest::unFormatTest();
	cppExpressionTest::PrimeTest();

	cppClassTest::testPolymorphism();
	cppClassTest::TestCopyConstructor();
	cppClassTest::testStruct();

	cppPointerTest::testpointer();

	cppTemplateTest::templateTest();
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
