# Cloud Ghost Game Engine

## graduation_project for pbr

---
# 临时笔记

---
## 协变 -- 不能用于 shared_ptr

```c++
class A {
public:
	A() {}
	virtual ~A() {}

	A(const A& a) { x = a.get_x(); }
	A& operator = (const A& a) { x = a.x; return *this; }

	int get_x() const { return x; }
protected:
	int x;

};
class B : public A {
public:
	B() {}
	virtual ~B() {}

	B(const B& b) : A(b) { y = b.get_y(); }
	B& operator = (const B& b) {
		A::operator=(b);
		y = b.get_y();
		return *this;
	}

	int get_y() const { return y; }
	void set_y(int t) { y = t; }
protected:
	int y;
};

using SPTR_A = std::shared_ptr<A>;
using SPTR_B = std::shared_ptr<B>;
class P {
public:
	P() {}
	~P() {}
	virtual A* get_v() = 0;
protected:
	 A* v;
};
class C : public P {
public:
	C() { v = new B(); }
	~C() {}
	virtual B* get_v() override {
		return (B*)v;
	}
};

int main() {
	
	C c;
	c.get_v()->set_y(1);
	qDebug() << c.get_v()->get_y();
	
	return 0;
}
```
