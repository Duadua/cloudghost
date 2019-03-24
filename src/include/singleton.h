#pragma once

// 继承式单例类 -- 所有继承此抽象类的类都是单例类

// 所有单例类都用此语句声明
#define SINGLETON_CLASS(name) class name : public Singleton<name>

#define SINGLETON(name)						\
public:										\
	name(token) {}							\
	name(const name&) = delete;				\
	name operator = (const name&) = delete; \
protected:									\
	name() = default;						\
private:
	

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
};

// 子类单例示例 -- CRTP(奇异递归模板模式 curiously recurring template pattern) + 实现上面定义的宏
/*
SINGLETON_CLASS(SingleTest) {
	SINGLETON(SingleTest)
public:
	std::string test() { return "singleton yep"; }
};
*/


