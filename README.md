# Cloud Ghost Game Engine

## graduation_project for pbr

---
# 操作指南

> [移动]	鼠标右键 + WASD
> 
> [移动]	鼠标左键 + WASD
> 
> [移动]	鼠标左右键(同时按) + WASD
> 
> [环顾]	鼠标右键滑动
> 
> [环顾]	鼠标左键左右滑动
> 
> [开车]	鼠标左键前后滑动
> 
> [上天]	鼠标左右键向前滑动 or 鼠标按下+E
> 
> [入地]	鼠标左右键向后滑动 or 鼠标按下+Q
> 
> [说明]	可参考 unreal engine 4 基础操作

---
# 进度

- [清单]	已支持的功能

> [图元生成]
> 
> [资源导入]
> 
> [事件分发]
> 
> [数学库]
> 
> [BlinnPhong直接光照]
> 
> [拾取]
> 
> [后期处理]
> 
> [红蓝3D]
> 
> [骨骼动画]
> 
> [shadertoy]
> 
> [法线可视化]
> 
> [抗锯齿(MSAA)]
> 
> [gamma校正]
> 
> [HDR(tonemap)]
> 
> [天空盒]
> 
> [深度图]
> 
> [阴影(shadowmap)]

- [说明]	
	- 持续更新中, 有好的bug或者好的建议欢迎与本人联系, 共同学习, 共同进步。兴趣是最好的老师。

- [声明]	水平有限, 仅供学习和参考, 不得进行任何商业用途, 否则产生的一切后果将由使用者本人承担。

- [邮箱]	270389972@qq.com

- [以上]	\(^o^)/


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
