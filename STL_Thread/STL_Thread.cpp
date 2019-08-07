// STL_Thread.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include <vector>
#include <map>
#include <thread>
#include <list>
#include <string>
#include <mutex>
#include <future>
#include <windows.h>
using namespace std;
namespace Stl_Thread_join_detach
{
	void mythreadFun()
	{
		cout << "mythreadFun is Run   " << "thread_ID is  "<<this_thread::get_id() <<endl;
	}
	void join_Test()
	{
		thread myThread(mythreadFun);
		myThread.join();
		cout << "STL_THREAD01::join_Test is Run   " << "thread_ID is  " << this_thread::get_id() << endl;
	}
	void detach_Test()
	{
		thread myThread(mythreadFun);
		myThread.detach();
		cout << "STL_THREAD01::detach_Test is Run   " << "thread_ID is  " << this_thread::get_id() << endl;
	}
}
namespace Stl_Thread_protect
{
	//创建多个线程、数据共享问题分析、案例代码

	vector <int> g_v = { 1,2,3 };//共享数据，只读
	//线程入口函数
	void myprint(int inum)
	{
		cout << "mtprint函数开始执行了，线程编号=" << inum << endl;
		//....
		cout << "mtprint函数结束执行了，线程编号=" << inum << endl;
		return;
	}
	void myprint2(int inum)
	{
		cout << "ID为" << this_thread::get_id() << "的线程,打印g_v值" << g_v[0] << g_v[1] << g_v[2] << endl;
		return;
	}
	void Thread_01()
	{
		//一：创建和等待多个线程
		vector<thread> mythreads;
		//创建10个线程，线程入口函数统一使用myprint
		//a）多个线程执行顺序是乱的，跟操作系统内部对线程的运行调度记住有关
		//b）主线程等待所有子线程运行结束，最后主线程结束，推荐这种join写法，更容易写出来稳定程序。
		//c）thread对象放入到容器中管理，看起来像一个thread对象数组，这对我们一次创建出来大量线程，
		//并对大量线程进行管理比较好
		for (int i =0;i<10;i++)
		{
			//创建10个线程，同时这10个线程开始执行
			mythreads.push_back(std::thread(myprint, i));
		}
		for (auto iter = mythreads.begin();iter !=mythreads.end();++iter)
		{
			iter->join();//等待10个线程都返回
		}
		cout << "I Love China" << endl;
	}
	class A
	{
	public:
		void inMsgRecvQueue()
		{
			for (int i =0;i<100000;i++)
			{
				cout << "inMsgRecvQueue()执行，插入一个元素" << i <<"\n"<< endl;
				msgRecvQueue.push_back(i);//假定数字i就是收到的命令，直接放到消息队列中
			}
		}
		//把数据从消息队列中取出来的线程
		void outMsgRecvQueue()
		{
			for (int i = 0; i < 100000; i++)
			{
				if (!msgRecvQueue.empty())
				{
					//消息不是空的
					int command = msgRecvQueue.front();//返回第一个数据，但是不检查元素存在否
					msgRecvQueue.pop_front();//移除第一个元素，但是不返回
					//处理数据......
				}
				else
				{
					cout << "outMsgRecvQueue()执行，但是当前消息队列中是空的" << i << "\n" << endl;
				}
			}
			cout << "end" << endl;
		}
	private:
		list<int> msgRecvQueue;//容器，专门用来代表玩家发送来的命令
	};
	class A_withMutex
	{
	public:
		void inMsgRecvQueue()
		{
			for (int i = 0; i < 100000; i++)
			{
				cout << "inMsgRecvQueue()执行，插入一个元素" << i << endl;
				my_mutex.lock();
				msgRecvQueue.push_back(i);//假定数字i就是收到的命令，直接放到消息队列中
				my_mutex.unlock();
			}
		}
		bool outMsgLULProc(int &command)
		{
			my_mutex.lock();
			if (!msgRecvQueue.empty())
			{
				//消息不是空的
				int command = msgRecvQueue.front();//返回第一个数据，但是不检查元素存在否
				msgRecvQueue.pop_front();//移除第一个元素，但是不返回
				//处理数据......
				my_mutex.unlock();
				return true;
			}
			my_mutex.unlock();
			return false;
		}
		bool outMsgLULProc_lockguard(int &command)
		{
			lock_guard<mutex> sbguard(my_mutex);//构造的时候执行了mutex::lock 析构时候unlock
			if (!msgRecvQueue.empty())
			{
				//消息不是空的
				int command = msgRecvQueue.front();//返回第一个数据，但是不检查元素存在否
				msgRecvQueue.pop_front();//移除第一个元素，但是不返回
				//处理数据......
				return true;
			}
			return false;
		}
		//把数据从消息队列中取出来的线程
		void outMsgRecvQueue()
		{
			int command = 0;
			for (int i = 0; i < 100000; i++)
			{
				//两者等价
				bool result1 = outMsgLULProc_lockguard(command);
				bool result = outMsgLULProc(command);
				
				if (result)
				{
					cout << "outMsgRecvQueue()执行，取出来一个元素" << command << endl;
				}
				else
				{
					cout << "outMsgRecvQueue()执行，但是当前消息队列中是空的" << i << endl;
				}
			}
			cout << "end" << endl;
		}
	private:
		list<int> msgRecvQueue;//容器，专门用来代表玩家发送来的命令
		mutex my_mutex;
	};
	class A_withSiLock
	{
	public:
		void inMsgRecvQueue()
		{
			for (int i = 0; i < 100000; i++)
			{
				cout << "inMsgRecvQueue()执行，插入一个元素" << i << endl;
				my_mutex1.lock();
				my_mutex2.lock();
				msgRecvQueue.push_back(i);//假定数字i就是收到的命令，直接放到消息队列中
				my_mutex1.unlock();
				my_mutex2.unlock();
			}
		}
		bool outMsgLULProc(int &command)
		{
			my_mutex2.lock();
			my_mutex1.lock();
			if (!msgRecvQueue.empty())
			{
				//消息不是空的
				int command = msgRecvQueue.front();//返回第一个数据，但是不检查元素存在否
				msgRecvQueue.pop_front();//移除第一个元素，但是不返回
				//处理数据......
				my_mutex2.unlock();
				my_mutex1.unlock();
				return true;
			}
			my_mutex2.unlock();
			my_mutex1.unlock();
			return false;
		}
		//把数据从消息队列中取出来的线程
		void outMsgRecvQueue()
		{
			int command = 0;
			for (int i = 0; i < 100000; i++)
			{
				//两者等价
				bool result = outMsgLULProc(command);

				if (result)
				{
					cout << "outMsgRecvQueue()执行，取出来一个元素" << command << endl;
				}
				else
				{
					cout << "outMsgRecvQueue()执行，但是当前消息队列中是空的" << i << endl;
				}
			}
			cout << "end" << endl;
		}
	private:
		list<int> msgRecvQueue;//容器，专门用来代表玩家发送来的命令
		mutex my_mutex1;
		mutex my_mutex2;
	};

	void Thread_02()
	{
		//二：数据共享问题分析
		//（2.1）只读的数据,是安全稳定的，不需要特别的处理手段。直接读取即可。
		//（2.2）有读有写的数据：2个线程写，8个线程读，如果代码没有特别的处理，那么程序肯定崩溃;
		//最简单的不崩溃处理，读的时候不能写，写的时候不能读。2个线程不能同时写，8个线程不能同时读。
		//写的动作分成10步;由于任务切换，导致各种诡异的事情发生（崩溃之类）
		//（2.3）其他案例 数据共享（订票）
		
		vector<thread> mythreads;
		for (int i = 0; i < 10; i++)
		{
			mythreads.push_back(std::thread(myprint2, i));
		}
		for (auto iter = mythreads.begin(); iter != mythreads.end(); ++iter)
		{
			iter->join();//等待10个线程都返回
		}
		cout << "I Love China" << endl;
	}
	void Thread_03()
	{
		//三：共享数据的保护案例代码
		//网络游戏服务器。两个自己创建的线程，一个线程收集玩家命令，并把命令数据写到一个队列中。
		//另外一个线程，从队列中取出来玩家发送的命令，解析，然后执行玩家需要的动作;
		
		//准备使用成员函数作为线程函数的方法来写线程
		A myobj;
		thread myoutMsgobj(&A::outMsgRecvQueue, &myobj);
		thread myinMsgobj(&A::inMsgRecvQueue, &myobj);
		myoutMsgobj.join();
		myinMsgobj.join();
	}
	void Thread_04()
	{
		//保护共享数据，操作时，某个线程用代码把共享数据锁住，操作数据、解锁，其他想操作共享数据的线程必须等待解锁。
		//互斥量 这是个类对象。理解成一把锁，多个线程尝试使用lock()成员函数来加锁这把锁头，只有一个线程能够锁定成功
		//成功的有返回值。如果没有成功，那么流程卡在lock这里不断尝试去加锁这把锁头。
		//mutex的使用要小心，不能保护的多也不能少，多了效率低少了不安全
		//(2.1)lock（） unlock（） 先lock，操作共享数据，unlock
		//要成对使用，有lock必然要有unlock。也不要有非对称数量的指令
		//为了防止忘记unlock（），引入一个叫做std：：lock——guard的模板类：自己会替你unlock（）;
		//智能指针（unique_ptr<>）:你忘记释放没关系，我给你释放
		//(2.2)std::lock_guard类模板 直接取代了lock unlock
		A_withMutex myobj;
		thread myoutMsgobj(&A_withMutex::outMsgRecvQueue, &myobj);
		thread myinMsgobj(&A_withMutex::inMsgRecvQueue, &myobj);
		myoutMsgobj.join();
		myinMsgobj.join();
	}
	void Thread_05()
	{
		//
		//三 死锁
		//张三 在北京等李四，不走;李四在深圳等张三 不走
		//C++中，有两把锁（mutex）
		//两个线程AB
		//（1）线程A执行的时候，这个线程先锁住了金锁，把金锁lock()成功了，然后它去lock银锁
		//（2）出现上下问切换
		//（3）线程B执行了，这个线程先锁住了银锁，因为银锁没有锁，所以银锁成功，线程b要去锁金锁了
		//此时此刻，死锁出现
		//
		//(3.1)死锁演示
		//（3.2）死锁一般解决方案  按照两个地方都按照顺序调用mutex,将两个函数中互斥量的lock顺序一致
		A_withSiLock myobj;
		thread myoutMsgobj(&A_withSiLock::outMsgRecvQueue, &myobj);
		thread myinMsgobj(&A_withSiLock::inMsgRecvQueue, &myobj);
		myoutMsgobj.join();
		myinMsgobj.join();
		
		
		//(3.3)std::lock()函数模板：用来处理两个以上的互斥量（至少两个，多了不限）
		//能力：一次性锁住两个或者两个以上的互斥量。这解决了多个线程中，因为锁的顺序问题导致的死锁风险问题
		//如果互斥量中有一个没有锁住，他就等在那里，等所有的互斥量都锁住了，它再往下走;
		//要么两个互斥量都锁住，要么都没有没有锁住。如果只锁住了一个，另外一个没有成功，则它立即把已经锁住的解锁。
		
		//（3.4）std::lock_guard的std::adopt_lock参数
		//std::adopt_lock是个结构体参数，起一个标记作用：作用就是表示这个互斥量已经lock了
		//不需要在std::lock_guard<std::mutex>构造函数里面对mutex对象进行lock了;
		//总结：std::lock() 一次锁定多个互斥量;谨慎使用（建议一个一个锁）
	}

}
namespace stl_Thread_condition
{
	class A
	{
	public:
		void inMsgRecvQueue()
		{
			for (int i = 0; i < 100000; i++)
			{
				cout << "inMsgRecvQueue()执行，插入一个元素" << i << endl;
				my_mutex.lock();
				msgRecvQueue.push_back(i);//假定数字i就是收到的命令，直接放到消息队列中
				my_cond.notify_one();//我们尝试把wait的线程唤醒，那么outMsgRecvQueue中的线程机会起来
				my_mutex.unlock();
			}
		}
		bool outMsgLULProc(int &command)
		{
			//wait()用来等一个东西
			//如果第二个参数lambda表达式返回值为TRUE，那么wait直接返回
			//如果第二个参数lambda表达式返回值是FALSE，那么wait将会解锁互斥量
			//并将线程堵塞到本行，直到其他某个线程调用notify_one()成员函数。
			//
			command = 0;
			std::unique_lock<std::mutex> sbguard(my_mutex);

			while (true)
			{
				my_cond.wait(sbguard, [this] {
					if (!msgRecvQueue.empty())
						return true;
					return false;
				});
			}
			
			if (!msgRecvQueue.empty())
			{
				//消息不是空的
				command = msgRecvQueue.front();//返回第一个数据，但是不检查元素存在否
				msgRecvQueue.pop_front();//移除第一个元素，但是不返回
				//处理数据......
				my_mutex.unlock();
				return true;
			}
			return false;
		}
		bool outMsgLULProc_lockguard(int &command)
		{
			lock_guard<mutex> sbguard(my_mutex);//构造的时候执行了mutex::lock 析构时候unlock
			if (!msgRecvQueue.empty())
			{
				//消息不是空的
				int command = msgRecvQueue.front();//返回第一个数据，但是不检查元素存在否
				msgRecvQueue.pop_front();//移除第一个元素，但是不返回
				//处理数据......
				return true;
			}
			return false;
		}
		//把数据从消息队列中取出来的线程
		void outMsgRecvQueue()
		{
			int command = 0;
			for (int i = 0; i < 100000; i++)
			{
				//两者等价
				//bool result1 = outMsgLULProc_lockguard(command);
				bool result = outMsgLULProc(command);

				if (result)
				{
					cout << "outMsgRecvQueue()执行，取出来一个元素" << command << endl;
				}
				else
				{
					cout << "outMsgRecvQueue()执行，但是当前消息队列中是空的" << i << endl;
				}
			}
			cout << "end" << endl;
		}
	private:
		list<int> msgRecvQueue;//容器，专门用来代表玩家发送来的命令
		mutex my_mutex;
		condition_variable my_cond;
	};
	//一条件变量std::condition_variable wait() notify_one()
	//线程A：等待一个条件满足
	//线程B:专门往消息队列中扔消息
	//
	//
	//
	//
	void condition_Test()
	{
		A myobj;
		thread myoutMsgobj(&A::outMsgRecvQueue, &myobj);
		thread myinMsgobj(&A::inMsgRecvQueue, &myobj);
		myoutMsgobj.join();
		myinMsgobj.join();
	}
	
}
namespace stl_Thread_future
{
	//int mythread(int mypar)
	//{
	//	cout << "mythread() start" << " threadid = " << std::this_thread::get_id() << endl;
	//	std::chrono::milliseconds dura(5000); //1秒 = 1000毫秒，所以5000毫秒 = 5秒
	//	std::this_thread::sleep_for(dura); //休息一定的时长
	//	return 5;
	//}
	//
	////void mythread2(std::future<int> &tmpf) //注意参数
	//void mythread2(std::shared_future<int> &tmpf) //注意参数
	//{
	//	cout << "mythread2() start" << " threadid = " << std::this_thread::get_id() << endl;
	//	auto result = tmpf.get(); //获取值，只能get一次否则会报异常
	//	                           //为什么第二次get这个future我们会得到一个异常;主要是因为get函数的设计，是一个移动语义
	//	cout << "mythread2 result = " << result << endl;
	//	return;
	//}

	//int g_mycout = 0; //定义一个全局量
	//std::atomic<int> g_mycout = 0; //我们封装了一个类型为int的 对象（值）;我们可以象操作一个int类型变量一样来操作这个g_mycout

	//std::mutex g_my_mutex; //互斥量
	//
	//void mythread() //线程入口函数
	//{
	//	
	//	for (int i = 0; i < 10000000; i++)
	//	{
	//		//g_my_mutex.lock(); //7秒钟实现了2000万次加锁和解锁;
	//		//g_mycout++;
	//		//...
	//		//...
	//		//g_my_mutex.unlock();
	//		g_mycout++; //对应的的操作是原子操作（不会被打断）
	//	}
	//	
	//	return;
	//}

	std::atomic<bool> g_ifend = false;  //线程退出标记，这里是原子操作，防止读和写乱套;

	void mythread()
	{
		std::chrono::milliseconds dura(1000); //1秒钟
		while (g_ifend == false)
		{
			//系统没要求线程退出，所以本线程可以干自己想干的事情
			cout << "thread id = " << std::this_thread::get_id() << " 运行中...." << endl;
			std::this_thread::sleep_for(dura);
		}
		cout << "thread id = " << std::this_thread::get_id() << " 运行结束...." << endl;
		return;
	}
	int future_test()
	{
		//一：std::future的其他成员函数,get()函数转移数据
		//二：std::shared_future：也是个类模板,get()函数复制数据	

		//cout << "main" << " threadid = " << std::this_thread::get_id() << endl;
		//std::packaged_task<int(int)> mypt(mythread);  //我们把函数mythread通过packaged_task包装起来;
		//std::thread t1(std::ref(mypt), 1);  //线程直接开始执行，第二个参数作为线程入口函数的参数;
		//t1.join(); //我们可以调用这个等待线程执行完毕,不调用这个不太行，会崩溃

		////std::future<int> result = mypt.get_future();  //std::future大家不陌生了，这个对象里含有线程入口函
		////std::shared_future<int> result_s(std::move(result)); 
		////bool ifcanget = result.valid();
		////std::shared_future<int> result_s(result.share()); //执行完毕后result_s里有值，而result里空了
		////ifcanget = result.valid();
		//std::shared_future<int> result_s(mypt.get_future()); //通过get_future返回值直接构造了一个shared_future对象。
		//auto mythreadresult = result_s.get();
		//mythreadresult = result_s.get();
		//
		////std::thread t2(mythread2, std::ref(result));
		//std::thread t2(mythread2, std::ref(result_s));
		//t2.join(); //等线程执行完毕
		//cout << "I Love China!" << endl;

		//三：原子操作std::atomic
		//（3.1）原子操作概念引出范例
		//互斥量：多线程编程中 保护共享数据：  锁，操作共享数据，开锁
		//有两个线程，对一个变量进行操作，这个线程读该变量值，另一个线程往这个变量中写值。
		//int atomvalue = 5;
		////读线程A
		//int tmpvalue = atomvalue;  //这里这个atoomvalue代表的是多个线程之间要共享的变量;	
		////写线程B
		//atomvalue = 6; //汇编代码的话;

		//大家可以把原子操作理解成一种：不需要用到互斥量加锁（无锁）技术的多线程并发编程方式
		//原子操作: 是在多线程中 不会被打断的 程序执行片段;原子操作，比互斥量效率上更胜一筹。
		//互斥量的加锁一般是针对一个代码段（几行代码），而原子操作针对的一般都是一个变量，而不是一个代码段;

		//原子操作，一般都是指“不可分割的操作”;也就是说这种操作状态要么是完成的，要么是没完成的，不可能出现半完成状态;
		//std::atomic来代表原子操作 ,std::atomic是个类模板。其实std::atomic这个东西是用来封装某个类型的值的;

		//（3.2）基本的std::atomic用法范例
		//（3.3）老师的心得，一般用于计数或者统计（累计发送出去了多少个数据包，累计接收到了多少个数据包;）

		/*thread mytobj1(mythread);
		thread mytobj2(mythread);
		mytobj1.join();
		mytobj2.join();

		cout << "两个线程执行完毕，最终的g_mycount的结果是：" << g_mycout << endl;

		*/
		thread mytobj1(mythread);
		thread mytobj2(mythread);
		std::chrono::milliseconds dura(5000); //5秒钟
		std::this_thread::sleep_for(dura);
		g_ifend = true; //对原子对象的写操作，让线程自行运行结束;
		mytobj1.join();
		mytobj2.join();
		cout << "程序执行完毕，退出" << endl;
		return 0;
	}
}
namespace stl_atomic_asnc
{
	//std::atomic<int> g_mycout = 0;  //这是个原子整型类型变量;可以向使用整型变量一样使用
//void mythread()
//{
//	for (int i = 0; i < 10000000; i++)
//	{
//		g_mycout++; //对应的操作就是原子操作，不会被打断;
//		g_mycout += 1;
//		//g_mycout = g_mycout + 1; //结果不对
//
//	}
//	return;
//}

	int mythread()
	{
		cout << "mythread() start" << " threadid = " << std::this_thread::get_id() << endl;

		std::chrono::milliseconds dura(5000); //1秒 = 1000毫秒，所以5000毫秒 = 5秒
		std::this_thread::sleep_for(dura); //休息一定的时长

		return 1;

	}
	int atomic_asnc_test()
	{
		//一：原子操作std::atomic续谈
		//一般atomic原子操作，针对++,--,+=,&=,|=,^=是支持的。其他的可能不支持。
		/*thread mytobj1(mythread);
		thread mytobj2(mythread);
		mytobj1.join();
		mytobj2.join();
		cout << "两个线程都执行完毕，最终的g_mycout的结果是" << g_mycout << endl;*/

		//二：std::async深入谈
		//（2.1）std::async参数详述 ,async用来创建 一个异步任务;
		//cout << "main start" << " threadid = " << std::this_thread::get_id() << endl;
		////std::future<int> result = std::async(mythread); 
		////std::future<int> result = std::async(std::launch::deferred,mythread); //deferred延迟调用，并且不创建新线程，延迟到future对象调用.get()或者.wait的时候才执行mythread()
		////std::future<int> result = std::async(std::launch::async, mythread);
		////std::future<int> result = std::async(std::launch::async| std::launch::deferred, mythread);
		//std::future<int> result = std::async(mythread);
		//cout << result.get() << endl;

		//老师讲解过:参数 std::launch::deferred【延迟调用】 ，以及std::launch::async【强制创建一个线程】
		//std::thread() 如果系统资源紧张，那么可能创建线程就会失败，那么执行std::thread()时整个程序可能崩溃。
		//std::async()我们一般不叫创建线程（解释async能够创建线程），我们一般叫它创建 一个异步任务。
		 //std::async和std::thread最明显的不同，就是async有时候并不创建新线程。
		//a)如果你用std::launch::deferred来调用async会怎么 样？
		//std::launch::deferred延迟调用，并且不创建新线程，延迟到future对象调用.get()或者.wait的时候才执行mythread()，如果没有调用get或者wait，那么这个mythread()不会执行。
		//b)std::launch::async：强制这个异步任务在新线程上执行，这 意味着，系统必须要给我创建出新线程来运行mythread();
		//c)std::launch::async | std::launch::deferred
		//这里这个 | ：意味着调用async的行为可能是 “ 创建新线程并立即执行”  或者 
						//没有创建新线程并且延迟到调用 result.get()才开始执行任务入口函数，    两者居其一;

		//d)我们不带额外参数;只给async函数一个 入口函数名;
		//第九节老师长生了一点小错误， 其实，默认值应该是std::launch::async | std::launch::deferred;和c)效果完全一致。
		   //换句话说：系统会自行决定是异步(创建新线程)还是同步（不创建新线程）方式运行。

		//自行决定是啥意思?系统如何决定是 异步(创建新线程)还是同步（不创建新线程）方式运行 
		//（2.2）std::async和std::thread的区别
		//std::thread创建线程，如果系统资源紧张，创建线程失败，那么整个程序就会报异常崩溃(有脾气)
		//int mythread(){return 1;}
		//std::thread mytobj(mythread);
		//mytobj.join();
		//std::thread创建线程的方式，如果线程返回值，你想拿到这个值也不容易;
		//std::async创建异步任务。可能创建也可能不创建线程。并且async调用方法很容易拿到线程入口函数的返回值;
		//由于系统资源限制：
		//(1)如果用std::thread创建的线程太多，则可能创建失败，系统报告异常，崩溃。
		//(2)如果用std::async，一般就不会报异常不会崩溃，因为 如果系统资源紧张导致无法创建新线程的时候，
			 //std::async这种不加额外参数的调用 就不会创建新线程。而是后续谁调用了result.get()来请求结果，
				//那么这个异步任务mythread就运行在执行这条get()语句所在的线程上。
		//如果你强制std::async一定 要创建新线程，那么就必须使用  std::launch::async。承受的代价就是系统资源紧张时，程序崩溃。
		//(3)经验：一个程序里，线程数量不宜超过100-200，时间片。

		//（2.3）std::async不确定性问题的解决
		//不加额外参数的std::async调用 ，让系统自行决定是否创建新线程。
		//问题焦点在于  std::future<int> result = std::async(mythread); 写法
		//这个异步任务到底有没有被推迟执行，（std::launch::async还是std::launch::deferred）
		//std::future对象的wait_for函数，第10节讲过。

		cout << "main start" << " threadid = " << std::this_thread::get_id() << endl;
		std::future<int>  result = std::async(mythread); //想判断async到底有没有创建新线程立即执行还是延迟（没创建新线程）执行。

		std::future_status status = result.wait_for(0s);   //(std::chrono::seconds(0));
		if (status == std::future_status::deferred)
		{
			//线程被延迟执行了(系统资源紧张了，它给我采用std::launch::deferred策略了)
			cout << result.get() << endl; //这个时侯才去调用了mythread（）;
		}
		else
		{
			//任务没有被推迟，已经开始运行了被，线程被创建了;
			if (status == std::future_status::ready)
			{
				//线程成功返回
				cout << "线程成功执行完毕并返回!" << endl;
				cout << result.get() << endl;
			}
			else if (status == std::future_status::timeout)
			{
				//超时线程还没执行完
				cout << "超时线程没执行完!" << endl;
				cout << result.get() << endl;
			}

		}


		return 0;
	}
}
namespace stl_mutex_eg_critical
{
	#define __WINDOWSJQ_ 
	//本类用于自动释放windows下的临界区，防止忘记LeaveCriticalSection导致死锁情况的发生，类似于c++11中的std::lock_guard<std::mutex>功能
	class CWinLock  //叫RAII类（Resource Acquisition is initialization）中文“资源获取即初始化”; 
						  //容器，智能指针这种类，都属于RAII类;
	{
	public:
		CWinLock(CRITICAL_SECTION *pCritmp) //构造函数
		{
			m_pCritical = pCritmp;
			EnterCriticalSection(m_pCritical);
		}
		~CWinLock() //析构函数
		{
			LeaveCriticalSection(m_pCritical);
		}
	private:
		CRITICAL_SECTION *m_pCritical;
	};

	class A
	{
	public:
		//把收到的消息入到队列的线程
		void inMsgRecvQueue()
		{
			for (int i = 0; i < 100000; i++)
			{
				cout << "inMsgRecvQueue()执行，插入一个元素" << i << endl;

#ifdef __WINDOWSJQ_
				//EnterCriticalSection(&my_winsec); //进入临界区（加锁）
				//EnterCriticalSection(&my_winsec);
				CWinLock wlock(&my_winsec);  //wlock,wlock2 都属于RAII对象。
				CWinLock wlock2(&my_winsec); //调用多次也没问题;
				msgRecvQueue.push_back(i);
				//LeaveCriticalSection(&my_winsec); //离开临界区（解锁）
				//LeaveCriticalSection(&my_winsec);
#else
				//my_mutex.lock();
				//my_mutex.lock(); //报异常，和windows有区别;
				//std::lock_guard<std::recursive_mutex> sbguard(my_mutex);   
				//testfunc1();//加了三次锁，报异常;(只要lock超过1次就报异常)
				//std::lock_guard<std::mutex> sbguard1(my_mutex);

				std::chrono::milliseconds timeout(100); //100毫秒
				//if (my_mutex.try_lock_for(timeout)) //等待100毫秒来尝试 获取锁
				if (my_mutex.try_lock_until(chrono::steady_clock::now() + timeout))
				{
					//在这100毫秒之内拿到了锁
					msgRecvQueue.push_back(i); //假设这个数字就是我收到的命令，我直接弄到消息队列里来
					my_mutex.unlock();  //用完了要解锁;
				}
				else
				{
					//这次没拿到锁头
					std::chrono::microseconds  sleeptime(100);
					std::this_thread::sleep_for(sleeptime);
				}

				//my_mutex.unlock();
				//my_mutex.unlock();
#endif
			}
		}

		bool outMsgLULProc(int &command)
		{
#ifdef __WINDOWSJQ_
			EnterCriticalSection(&my_winsec);
			if (!msgRecvQueue.empty())
			{
				int command = msgRecvQueue.front();//返回第一个元素但不检查元素存在与否
				msgRecvQueue.pop_front();
				LeaveCriticalSection(&my_winsec);
				return true;
			}
			LeaveCriticalSection(&my_winsec);
#else
			my_mutex.lock();

			//std::chrono::microseconds  sleeptime(10000000000);
			//std::this_thread::sleep_for(sleeptime);

			if (!msgRecvQueue.empty())
			{
				int command = msgRecvQueue.front();//返回第一个元素但不检查元素存在与否
				msgRecvQueue.pop_front();
				my_mutex.unlock();
				return true;
			}
			my_mutex.unlock();
#endif
			return false;
		}


		void outMsgRecvQueue()
		{
			int command = 0;
			for (int i = 0; i < 100000; i++)
			{
				bool result = outMsgLULProc(command);
				if (result == true)
				{
					cout << "outMsgRecvQueue()执行，取出一个元素" << command << endl;
					//这里可以考虑处理数据
					//......			
				}
				else
				{
					cout << "outMsgRecvQueue()执行，但目前还是空元素" << i << endl;
				}
			}
			cout << "end" << endl;
		}

		A()
		{
#ifdef __WINDOWSJQ_
			InitializeCriticalSection(&my_winsec); //用临界区之前要先初始化
#endif
		}

		//void testfunc1()
		//{
		//	std::lock_guard<std::recursive_mutex> sbguard(my_mutex);
		//	//.....干各种事情
		//	testfunc2(); //悲剧了，崩溃;
		//}

		//void testfunc2()
		//{
		//	std::lock_guard<std::recursive_mutex> sbguard(my_mutex);
		//	/*std::lock_guard<std::recursive_mutex> sbguard2(my_mutex);
		//	std::lock_guard<std::recursive_mutex> sbguard3(my_mutex);
		//	std::lock_guard<std::recursive_mutex> sbguard4(my_mutex);
		//	std::lock_guard<std::recursive_mutex> sbguard5(my_mutex);
		//	*/

		//	//.....干各种另外一些事情
		//}

	private:
		std::list<int>  msgRecvQueue; //容器，专门用于代表玩家给咱们发送过来的命令
		//std::mutex my_mutex; //创建独占互斥量
		//std::recursive_mutex my_mutex; //递归独占互斥量
		std::recursive_timed_mutex my_mutex; //带超时功能的独占互斥量;

#ifdef __WINDOWSJQ_
		CRITICAL_SECTION my_winsec; //windows中的临界区，非常类似于c++11中的mutex
#endif

	};
	int critical_test()
	{
		//一：windows临界区
		//二：多次进入临界区试验
		//在“同一个线程”（不同线程就会卡住等待）中，windows中的“相同临界区变量”代表的临界区的进入（EnterCriticalSection)可以被多次调用
		 //但是你调用了几次EnterCriticalSection，你就得调用几次LeaveCriticalSection(&my_winsec); .
		 //而在c++11中，不允许 同一个线程中lock同一个互斥量多次，否则报异常

		//三：自动析构技术
		//std::lock_guard<std::mutext>

		//四：recursive_mutex递归的独占互斥量
		//std::mutex： 独占互斥量,自己lock时别人lock不了;
		//std::recursive_mutex：递归的独占互斥量:允许同一个线程，同一个互斥量多次被.lock()，效率上比mutex要差一些;
			 //recursive_mutex也有lock,也有unlock();
				  //考虑代码是否有优化空间。
		   //递归次数据说有限制，递归太多次可能报异常。

		//五：带超时的互斥量std::timed_mutex和			std::recursive_timed_mutex
		//std::timed_mutex：是带超时功能的独占互斥量;
				//try_lock_for()  ：参数是一段时间，是等待一段时间。如果我拿到了锁，或者等待超过时间没拿到锁，就走下来;
				//try_lock_until() ：参数是一个未来的时间点，在这个未来的时间没到的时间内，如果拿到了锁，那么就走下来;
														//如果时间到了，没拿到锁，程序流程也走下来;
		//std::recursive_timed_mutex：带超时功能的递归独占互斥量(允许同一个线程多次获取这个互斥量)



		A  myobja;
		std::thread myOutnMsgObj(&A::outMsgRecvQueue, &myobja);  //注意这里第二个参数必须是引用，才能保证线程里用的是同一个对象（这个咱们在上节课详细分析过了）
		std::thread myInMsgObj(&A::inMsgRecvQueue, &myobja);
		myInMsgObj.join();
		myOutnMsgObj.join();
		return 0;
	}
}
namespace stl_Threadpool
{
	class A {
	public:
		atomic<int> atm;
		A()
		{
			atm = 0;
			//auto atm2 = atm; //这种定义时初始化操作不允许，显示 “尝试引用已删除的函数”编译器内部肯定把拷贝构造函数给干掉了用 = delete
			//atomic<int> atm3 = atm;
			//atomic<int> atm2;
			//atm2 = atm; //尝试引用已删除的函数，拷贝赋值运算符也不让用
			//load()：以原子方式读atomic对象的值
			atomic<int> atm2(atm.load()); //读
			auto atm3(atm.load());
			//store()以原子方式写入内容
			atm2.store(12);
			atm2 = 12;

		}

		void outMsgRecvQueue()
		{
			//int command = 0;
			//while (true)
			//{
			//	std::unique_lock<std::mutex> sbguard1(my_mutex1); //临界进去
			//	my_cond.wait(sbguard1, [this] {  //this，可以参考 未归类知识点，第八节
			//		if (!msgRecvQueue.empty())
			//			return true; // 该lambda返回true，则wait就返回，流程走下来，互斥锁被本线程拿到。
			//		return false; //解锁并休眠，卡在wait等待被再次唤醒
			//	});
			//	//现在互斥锁锁着，流程走下来了，队列里有数据;
			//	command = msgRecvQueue.front(); //返回第一个元素，但不检查元素是否存在;
			//	msgRecvQueue.pop_front();  //移除第一个元素，但不返回;
			//	sbguard1.unlock(); //因为unique_lock的灵活性，我们可以随时unlock解锁，以免锁住太长时间
			//	cout << "outMsgRecvQueue()执行，取出一个元素" << command << endl;
			//} //end while
			while (true)
			{
				cout << atm << endl; //读atm是个原子操作，但是整个这一行代码并不是个原子操作;
			}
		}

		void inMsgRecvQueue() //unlock()
		{
			//for (int i = 0; i < 100000; ++i)
			//{
			//	cout << "inMsgRecvQueue()执行，插入一个元素" << i << endl;
			//	std::unique_lock<std::mutex> sbguard1(my_mutex1);
			//	msgRecvQueue.push_back(i); //假设这个数字i就是我收到的命令，我直接弄到消息队列里边来;
			//	my_cond.notify_one();   //我们尝试把wait()的线程唤醒，其实现在outMsgRecvQueue()中的my_cond.wait()已经醒了,但光醒了没有用，你这里要是不把互斥量撒开，醒了他也要堵在另外一个线程的wait()那里;
			//}
			for (int i = 0; i < 1000000; ++i)
			{
				atm += 1; //原子操作
				//atm = atm + 1; //不是原子操作
			}
			return;
		}

	private:
		std::list<int> msgRecvQueue; //容器(消息队列)，专门用于代表玩家给咱们发送过来的命令。
		std::mutex my_mutex1; //创建了一个互斥量 （一把锁头）	
		std::condition_variable my_cond; //生成一个条件对象

	};

	int pool_test()
	{
		//一：补充一些知识点
		//（1.1）虚假唤醒:wait中要有第二参数(lambda)并且这个lambda中要正确判断要处理的公共数据是否存在;
		//wait(),notify_one(),notify_all()

		//（1.2）atomic ,10,11节都有介绍

		//二：浅谈线程池
		//（2.1）场景设想
		//服务器程序，--》客户端， 每来 一个客户端，就创建 一个新线程为该客户提供服务。
		//a)网络游戏，2万玩家不可能给每个玩家创建个新线程,此程序写法在这种场景下不通;
		//b)程序稳定性问题：编写的代码中，偶尔创建一个线程这种代码，这种写法，就让人感到不安;
		//线程池：把一堆线程弄到一起，统一管理。这种统一管理调度，循环利用线程的方式，就叫线程池;
		//（2.2）实现方式
		//在程序启时，我一次性的创建好一定数量的线程。10，8，100-200,更让人放心，觉得程序代码更稳定;

		//三：线程创建数量谈
		//（3.1）线程开的数量极限问题，2000个线程基本就是极限;再创建线程就崩溃;
		//（3.2）线程创建数量建议
		//a)采用某些技术开发程序;api接口提供商建议你 创建线程数量 = cpu数量，cpu *2 ,cpu *2 +2，遵照专业建议和指示来，专业意见确保程序高效率执行
		//b)创建多线程完成业务; 一个线程等于一条执行通路; 100要堵塞充值，我们这里开110个线程，那是很合适的;
		//c)1800个线程，建议，线程数量尽量不要超过500个，能控制在200个之内;


		//四：c++11多线程总结
		//windows,linux;

		A myobja;
		std::thread myOutnMsgObj(&A::outMsgRecvQueue, &myobja); //第二个参数是 引用，才能保证线程里 用的是同一个对象。
		std::thread myInMsgObj(&A::inMsgRecvQueue, &myobja);
		std::thread myInMsgObj2(&A::inMsgRecvQueue, &myobja);
		myInMsgObj.join();
		myOutnMsgObj.join();
		myInMsgObj2.join();


		//int abc = 10'00; c++新标准允许这么写
		return 0;
	}
}
namespace stl_move_cotr
{
#include <iostream>
	using namespace std;

	class A {
	public:
		int x;
		A(int x) : x(x)
		{
			cout << "Constructor" << endl;
		}
		A(A& a) : x(a.x)
		{
			cout << "Copy Constructor" << endl;
		}
		A& operator=(A& a)
		{
			x = a.x;
			cout << "Copy Assignment operator" << endl;
			return *this;
		}
		A(A&& a) : x(a.x)
		{
			cout << "Move Constructor" << endl;
		}
		A& operator=(A&& a)
		{
			x = a.x;
			cout << "Move Assignment operator" << endl;
			return *this;
		}
	};

	A GetA()
	{
		return A(1);
	}

	A&& MoveA()
	{
		return A(1);
	}

	int cotr_test()
	{
		cout << "-------------------------1-------------------------" << endl;
		A a(1);
		cout << "-------------------------2-------------------------" << endl;
		A b = a;
		cout << "-------------------------3-------------------------" << endl;
		A c(a);
		cout << "-------------------------4-------------------------" << endl;
		b = a;
		cout << "-------------------------5-------------------------" << endl;
		A d = A(1);
		cout << "-------------------------6-------------------------" << endl;
		A e = std::move(a);
		cout << "-------------------------7-------------------------" << endl;
		A f = GetA();
		cout << "-------------------------8-------------------------" << endl;
		A&& g = MoveA();
		cout << "-------------------------9-------------------------" << endl;
		d = A(1);
		return 1;
	}
}
int main()
{
	//Stl_Thread_join_detach::join_Test();
	//Stl_Thread_join_detach::detach_Test();
	//Stl_Thread_protect::Thread_01();
	//Stl_Thread_protect::Thread_02();
	//Stl_Thread_protect::Thread_03();
	//Stl_Thread_protect::Thread_04();
	//Stl_Thread_protect::Thread_05();
	//stl_move_cotr::cotr_test();
	//stl_Thread_future::future_test();
	//stl_atomic_asnc::atomic_asnc_test();
	//stl_mutex_eg_critical::critical_test();
	//stl_Threadpool::pool_test();
	stl_Thread_condition::condition_Test();
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
