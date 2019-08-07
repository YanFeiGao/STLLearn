#pragma once
class Account {
public:
	static double m_rate;
	static void set_rate(const double& x) {
		m_rate = x;
	}
};
//��ģ�� class template
template<typename T> class Template_Complex
{
public:
	Template_Complex(T r = 0, T i = 0)//��0��0Ĭ��ʵ�Σ�
		:re(r), im(i)//��ʼֵ��
	{
		//������д��ʽ����������г�ʼֵ�У��������︳ֵ
		//cpp����˵���Ƚ���һ�γ�ʼ���ٽ��丳ֵ
		//re = r; im = i;
	}
	//������һ�������Ĺ��캯����ͻ
	//Template_Complex() :re(0), im(0){}
	//����ָ��ĺ�����벻��д��������
	//inline function  �������̫���ӣ���������û����������inline
	T real() const { return re; }
	T imag() const { return im; }

	void real(T r) { re = r; } // overloading
	//overload����������ݺ������� ����������һ���������Լ����Ķ��ĺ������֡�

	bool operator< (const Template_Complex& rhs) const
	{
		return re < rhs.re;
	}
private:
	T re, im;
};
//����ģ�� function template
template<typename T> inline const T& min(const T&a, const T&b)
 {
	 return b < a ? b : a;
 }


