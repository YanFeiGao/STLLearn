// OOProgramDesgin.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <new>
#include <cassert>
using namespace std;

namespace Complex_Test
{
#include "complex.h"
	//这个<<符号特殊，只能进行非成员函数重载
	//因为符号都是看左操作数类型判定符号属于哪个类的
	ostream& operator << (ostream& os, const complex& x)
	{
		return os << '(' << real(x) << ',' << imag(x) << ')';
	}

	int Complex_Test_main()
	{
		std::cout << "**************************start Complex_Test_main**************************\n";
		complex c1(2, 1);
		complex c2(4, 0);

		cout << c1 << endl;
		cout << c2 << endl;

		//三种+的重载
		cout << c1 - c2 << endl;
		cout << (c2 - 2) << endl;
		cout << (5 - c2) << endl;

		cout << c1 + c2 << endl;
		cout << (5 + c2) << endl;
		cout << (c2 + 4) << endl;
		//正反
		cout << +c2 << endl;
		cout << -c2 << endl;

		cout << c1 * c2 << endl;
		cout << c1 / 2 << endl;
		

		cout << conj(c1) << endl;
		cout << norm(c1) << endl;
		cout << polar(10, 4) << endl;
		//碰到+=，会先看c1是不是有重载+=，有就调用
		cout << (c1 += c2) << endl;
		//如果+=的返回值是void 那么这种连续+=就不行
		cout << (c1 += c1 += c2) << endl;

		cout << (c1 == c2) << endl;
		cout << (c1 != c2) << endl;
		
		//注意这个，是否打破了封装
		c2.func(c1);

		complex * pc = new complex(2, 1);
		delete pc;
		//等价于下面三部

		void *mem = operator new(sizeof(complex));//分配内存
		complex *pc1 = static_cast<complex*>(mem);//转型
		pc1->complex::complex(1, 2);//构造函数 等于complex::complex(pc1,1, 2);
		//基本都这样，但是开了优化之后，可能会先返回一个指针，再分配内存、转型、构造
		//这就会带来问题了。
		delete mem;
		//析构的时候，先析构函数再释放内存
		//00：24：50 高能开始  8 堆栈内存管理
		std::cout << "**************************end Complex_Test_main**************************\n";
		return 0;
		
	}
}
namespace String_Test
{
#include "String.h"
	void String_Test_main()
	{
		std::cout << "**************************start String_Test_main**************************\n";
		(String)0;
		String s1("hello");
		String s2("world");
		//带有指针的类必须有拷贝构造与拷贝赋值
		//如果没有那么就一定会浅拷贝
		String s3(s2);//拷贝构造
		cout << s3 << endl;

		s3 = s1;//拷贝赋值
		cout << s3 << endl;
		cout << s2 << endl;
		cout << s1 << endl;
		std::cout << "**************************end String_Test_main**************************\n";
	}
}
namespace OOP_Test
{
#include "oop-demo.h"
	double Account::m_rate = 8.0;
	
	void OOP_Test_main()
	{
		std::cout << "**************************start OOP_Test_main**************************\n";
		//调用static函数的两种方法，1 通过obj调用 2 通过class name调用
		Account::set_rate(5.0);//单件模式，就是这样访问的
		Account a;
		a.set_rate(7.0);
		//总结：当一个类A有包含另外一个类B时（复合关系），A的构造函数会
		//先执行B的默认构造函数，再执行自己的。A的析构函数会先执行自己的
		//再调用B的析构函数
		//继承关系也是一样的
		std::cout << "**************************end OOP_Test_main**************************\n";

	}
	class Fraction
	{
		//conversion_fun
	public:
		Fraction(int num, int den = 1)
			:m_numerator(num), m_denominator(den)
		{}
		operator double() const {//const很重要，一定要加
			return (double)m_numerator / m_denominator;
		}
		//转换函数
		operator string() const
		{
			return std::to_string(m_numerator) + "/" + std::to_string(m_denominator);
		}
	protected:
	private:
		int m_numerator;//分子
		int m_denominator;//分母
	};
	class Fraction_B
	{
		//non-explicit one argument constructor 
	public:
		Fraction_B(int num, int den = 1)
			:m_numerator(num), m_denominator(den)
		{}
		Fraction_B operator+ (const Fraction_B &f) {
			return Fraction_B(m_numerator + f.m_numerator, m_denominator + f.m_denominator);
		}
		//转换函数
		operator string() const
		{
			return std::to_string(m_numerator) + "/" + std::to_string(m_denominator);
		}
	protected:
	private:
		int m_numerator;//分子
		int m_denominator;//分母
	};
	class Fraction_explicit
	{
	public:
		//明确告诉构造函数 不要将普通int值转换为本类
		explicit Fraction_explicit(int num, int den = 1)
			:m_numerator(num), m_denominator(den)
		{}
		operator double() const {//const很重要，一定要加
			return (double)m_numerator / m_denominator;
		}
		Fraction_explicit operator+ (const Fraction_explicit &f) {
			return Fraction_explicit(m_numerator + f.m_numerator, m_denominator + f.m_denominator);
		}
		//转换函数
		operator string() const
		{
			return std::to_string(m_numerator) + "/" + std::to_string(m_denominator);
		}
	protected:
	private:
		int m_numerator;//分子
		int m_denominator;//分母
	};
	void conversion_fun()
	{
		std::cout << "**************************start conversion_fun**************************\n";
		Fraction f(3, 5);
		double sum = 4 + f;
		cout << "sum(4 + f) = " << sum << endl;
		Fraction_B f1(3, 5);
		Fraction_B d = f1 + 5;
		cout << "sum(f+5) = " << (string)d << endl;
		string str = f;
		cout << "Result is: " << str << endl; //Result is: 3/5
		//cout << "Result is: "<< endl; //Result is: 3/5
		std::cout << "**************************end conversion_fun**************************\n";
	}
	//智能指针 pointer——like classes
	//仿函数 function——like calsses
	template<class T> struct identity 
	{
		const T&
			operator() (const T& x) const
		{
			return x;
		}
	};
}
namespace Smart_Test
{
	void shared_ptr_Test()
	{
		std::cout << "**************************start shared_ptr_Test**************************\n";
		//STL中存在两种智能指针，shared_ptr unique_ptr
		//weak_ptr是一种弱引用来指向shared_ptr所管理的对象
		shared_ptr<string> p1;
		shared_ptr<list<int>> p2;
		if (p1&&p1->empty())
		{
			*p1 = "hi";
		}
		string* pstr = p1.get();//get用于获取智能指针管理的指针
		bool us = p1.unique();//判断是否是共享指针
		long count = p1.use_count();//获取引用计数

		shared_ptr<double>  pdouble(new double(12.0));//显示使用double* 来创建智能指针
		shared_ptr<double > p3 = make_shared<double>(42.0);
		double* dvalue = p3.get();
		us = p1.unique();
		count = p3.use_count();

		p3.reset(new double(11.0));

		auto p4 = make_shared<list<int>>();
		auto p5(p3);
		count = p5.use_count();

		int *pint1 = new int;//如果分配失败，new返回一个std::bad_alloc
		int *pint2 = new (nothrow) int;//如果分配失败，new返回一个空指针 palcement new

		const int *pci = new const int(1024);
		delete pci;
		std::cout << "**************************end shared_ptr_Test**************************\n";

	}
	struct Foo {
		Foo() { std::cout << "Foo\n"; }
		~Foo() { std::cout << "~Foo\n"; }
	};
	struct D
	{
		void bar() { std::cout << "Call deleter D::bar()...\n"; }
		void operator()(Foo* p) const
		{
			std::cout << "Call delete for Foo object...\n";
			delete p;
		}
	};
	void unique_ptr_Test()
	{
		std::cout << "**************************start unique_ptr_Test**************************\n";
		unique_ptr<double> p1;
		unique_ptr<int> p2(new int(42));
		unique_ptr<string>str1(new string("char"));
		//unique_ptr<string> str2(str1);//不支持拷贝
		unique_ptr<string> str3;
		//str3 = str1;//不支持赋值

		unique_ptr<int[]> up0(new int[1000000]);
		int* pint = up0.release();//并没有按照书中所说的，自动清理掉内存

		delete[]pint;

		std::cout << "Creating new Foo...\n";
		std::unique_ptr<Foo> up(new Foo());

		std::cout << "About to release Foo...\n";
		Foo* fp = up.release();

		assert(up.get() == nullptr);
		std::cout << "Foo is no longer owned by unique_ptr...\n";

		delete fp;

		std::unique_ptr<Foo, D> up1(new Foo(), D());
		D& del = up1.get_deleter();
		del.bar();

		std::cout << "Creating new Foo...\n";
		std::unique_ptr<Foo, D> up2(new Foo(), D());  // up 占有 Foo 指针（删除器 D ）

		std::cout << "Replace owned Foo with a new Foo...\n";
		up2.reset(new Foo());  // 调用旧者的删除器

		std::cout << "Release and delete the owned Foo...\n";
		up2.reset(nullptr);
		std::cout << "**************************end unique_ptr_Test**************************\n";

	}

	void observe(std::weak_ptr<int> weak)
	{
		if (auto observe = weak.lock()) {
			std::cout << "\tobserve() able to lock weak_ptr<>, value=" << *observe << "\n";
		}
		else {
			std::cout << "\tobserve() unable to lock weak_ptr<>\n";
		}
	}
	void weak_ptr_Test()
	{
		std::cout << "**************************start weak_ptr_Test**************************\n";
		auto p = make_shared<int>(42);
		weak_ptr<int> wp(p);//弱引用，所以引用计数不会增长
		if (shared_ptr<int> np = wp.lock())//如果np不为空则条件成立
		{
			//在if中np与p共享对象
		}

		std::weak_ptr<int> weak;
		std::cout << "weak_ptr<> not yet initialized\n";
		observe(weak);
		{
			auto shared = std::make_shared<int>(42);
			weak = shared;
			std::cout << "weak_ptr<> initialized with shared_ptr.\n";
			observe(weak);
		}

		std::cout << "shared_ptr<> has been destructed due to scope exit.\n";
		observe(weak);
		std::cout << "**************************end weak_ptr_Test**************************\n";
		
	}
	void Smart_Test_main()
	{
		shared_ptr_Test();
		unique_ptr_Test();
		weak_ptr_Test();
		return;
	}
}
namespace Allocator_Test
{
	struct Foo {
		Foo():a(-1) { std::cout << "Foo\n"; }
		Foo(int b ) :a(b) { std::cout << "Foo\n"; }
		~Foo() { std::cout << "~Foo\n"; }
		int a;
	};
	void allocator_Test()
	{
		std::cout << "**************************start allocator_Test**************************\n";
		int n = 10;
		allocator<Foo> allocFoo;//可以分配FOO的allocator对象
		auto const p = allocFoo.allocate(n);//分配n个未初始化的FOO
		auto q = p;
		int value = 100;
		for (int i =0;i<n;i++)
		{
			allocFoo.construct(q, value);
			q ++ ;
		}
	
		while (p!=q)
		{
			allocFoo.destroy(q--);//并没有释放内存，只是调用了析构函数
		}
		
		allocFoo.deallocate(p,n);
		n = 0;

		std::vector<int> vec{ 0,1,2,3,4,5,6,7,8,9 };
		allocator<int> allocInt;
		//分配比vec中元素多两倍的元素空间
		auto pInt = allocInt.allocate(vec.size() * 2);
		//通过拷贝vec中的元素来构造从pInt开始的元素
		auto qInt = uninitialized_copy(vec.begin(), vec.end(), pInt);
		//将剩余元素初始化为42
		uninitialized_fill_n(qInt, vec.size(), 42);
		std::cout << "**************************end allocator_Test**************************\n";
	}
}
int main()
{
	Complex_Test::Complex_Test_main();
	String_Test::String_Test_main();
	OOP_Test::OOP_Test_main();
	OOP_Test::conversion_fun();
	Smart_Test::Smart_Test_main();
	Allocator_Test::allocator_Test();
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
