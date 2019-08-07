#pragma once
class Account {
public:
	static double m_rate;
	static void set_rate(const double& x) {
		m_rate = x;
	}
};
//类模板 class template
template<typename T> class Template_Complex
{
public:
	Template_Complex(T r = 0, T i = 0)//（0，0默认实参）
		:re(r), im(i)//初始值列
	{
		//不良书写方式，如果不进行初始值列，而在这里赋值
		//cpp等于说会先进行一次初始，再将其赋值
		//re = r; im = i;
	}
	//下面这一句和上面的构造函数冲突
	//Template_Complex() :re(0), im(0){}
	//不带指针的函数多半不用写析构函数
	//inline function  如果函数太复杂，编译器就没有能力把它inline
	T real() const { return re; }
	T imag() const { return im; }

	void real(T r) { re = r; } // overloading
	//overload后编译器根据函数名称 参数等生成一个编译器自己看的懂的函数名字。

	bool operator< (const Template_Complex& rhs) const
	{
		return re < rhs.re;
	}
private:
	T re, im;
};
//函数模板 function template
template<typename T> inline const T& min(const T&a, const T&b)
 {
	 return b < a ? b : a;
 }


