// OOProgramDesgin.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <string>
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
		return 0;
	}
}
namespace String_Test
{
#include "String.h"
	void String_Test_main()
	{
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
	}
}
namespace OOP_Test
{
#include "oop-demo.h"
	double Account::m_rate = 8.0;
	
	void OOP_Test_main()
	{
		//调用static函数的两种方法，1 通过obj调用 2 通过class name调用
		Account::set_rate(5.0);//单件模式，就是这样访问的
		Account a;
		a.set_rate(7.0);
		//总结：当一个类A有包含另外一个类B时（复合关系），A的构造函数会
		//先执行B的默认构造函数，再执行自己的。A的析构函数会先执行自己的
		//再调用B的析构函数
		//继承关系也是一样的

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
		Fraction f(3, 5);
		double sum = 4 + f;
		cout << "sum(4 + f) = " << sum << endl;
		Fraction_B f1(3, 5);
		Fraction_B d = f1 + 5;
		cout << "sum(f+5) = " << (string)d << endl;
		string str = f;
		cout << "Result is: " << str << endl; //Result is: 3/5
		//cout << "Result is: "<< endl; //Result is: 3/5
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
int main()
{
	Complex_Test::Complex_Test_main();
	String_Test::String_Test_main();
	OOP_Test::OOP_Test_main();
	OOP_Test::conversion_fun();
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
