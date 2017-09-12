# Easy Engine(version 1.0)
 基于Optix写的光线跟踪引擎，写这份源代码的目的是因为国内对真实感渲染的资料非常少，该简单引擎最后会集成大多数主流的渲染模型，采样都会对不同光照模型使用重要性采样，积分也都会
使用蒙特卡洛积分法，供大家参考学习吧。引擎架构非常简单，本来想搭载到自己的引擎上边去的，由于时间以及公司正在使用不便公开就不整了。
不懂可以联系我:liufeipengkk1@163.com
	 
## 1.testDemo
模块完成所有测试，请放心使用
![Alt text](https://github.com/liufeipengkk1/Demo/raw/master/Image/TestImage.jpg)

## 2.介绍
	非常简单，看看demo，读读代码就明白了。

## 3.光照模型
#### 3.1.Microfacet Cook-Torrance 
#### 3.2.Glossy reflection Model

## 4.水面彩带真实模拟

## 5.参考资料
	1.physical based Rendering ，RealTime Rendering 以及部分sigraph文献
	