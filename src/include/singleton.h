#pragma once

// 继承式单例类 -- 所有继承此抽象类的类都是单例类
// 缺点是只能继承一次， 子类不能被再次继承

// 所有单例类都用此语句声明
#define SINGLETON_CLASS(name) class name : public Singleton<name>

#define SINGLETON(name)						\
public:										\
	name(token) { _init(); }				\
	name(const name&) = delete;				\
	name operator = (const name&) = delete; \
protected:									\
	name() = default;						\
private:

// 简化调用 - 两种方式
#define SINGLETON_X(name) inline name& name##_ins() { return name::get_instance(); }	
template <class T> T& instance() { return T::get_instance(); }

// =================================================================

template<class T>
class Singleton {
public:
	// 子类有唯一公开的构造函数 -- 带一个token参数 -- 所以外界不能构造
	static T& get_instance() { static T instance{ token() }; return instance; }

	Singleton(const Singleton&) = delete;
	Singleton operator = (const Singleton&) = delete;
	virtual ~Singleton() = default;
protected:
	struct token {};					// 辅助公有化子类的构造但同时不对外界公有
	Singleton() = default;
	virtual void _init() {}				// 需要初始化一些值时可重写此函数
};

// 子类单例示例 -- CRTP(奇异递归模板模式 curiously recurring template pattern) + 实现上面定义的宏
/*
#include <string>
SINGLETON_CLASS(SingleTest) {
	SINGLETON(SingleTest)
public:
	std::string t;
	std::string test() { return t; }
	virtual void _init() override { t = "hehe"; }	// 初始化一些值
};
SINGLETON_X(SingleTest)

c_debug() << SingleTest::get_instance().test();
c_debug() << instance<SingleTest>().test();
c_debug() << SingleTest_ins().test();
*/

