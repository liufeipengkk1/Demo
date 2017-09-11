# Easy Engine(version 1.0)
 基于Optix写的光线跟踪引擎，写这份源代码的目的是国内对真实感渲染的资料非常少，该简单引擎最后将会集成网上能够找到的不同的渲染模型，采样都会使用重要性采样，积分也都会
使用蒙特卡洛积分法，供大家参考学习吧。Optix设计上有一些个人认为不合理的地方，经过一周的Coding，基本熟悉所有Optix的工作模式，并封装成简单光线跟踪渲染引擎，
整理出一共五种shader，为了开发的方便性，很多引擎概念上的东西都被我舍弃，由于时间仓促，没有在我自己原来的引擎上去拓展(某公司在用>.<就不公布了),将来有机会再去做拓展。 由于时间仓促，整个架构极其简单，本来打算采用entity-component架构，很多东西都被我舍弃了。
	 
## 1.testDemo
模块完成所有测试，放心使用
![Alt text](https://github.com/liufeipengkk1/Demo/raw/master/Image/TestImage.jpg)

## 2.Engine Introduction
#### 2.1 5种shader：这里的shader是基于函数的，所以指定一份文件，同时也要指定一个函数作为入口。cameraShader:光线发射函数；backGroundSHader: 背景shader;exceptionShader: 异常shader，出现异常的时候表现的函数
GeometryShader:由BoundingBox和Intesect共同构成，Intesect这个函数不多说了，具体看我写的Triangle_mesh.cu函数就能明白是咋回事了，一个完整IntesectShader应;该
包含材质指定入口，以及法线和uv的处理。BoundingBoxShader:指定一个包围盒，这个就不说了，场景加速使用的，bvh算法更不用多说了，很成熟的一棵树了。
MaterialShader: 可以由三种Shader构成，对应三个类别，着色，透明，阴影。
#### 2.3 How To Use?
看demo.使用非常简单，处理流程一般是：创建上下文----> 创建各类Shader--->创建一个场景---->模型加载---->创建一个Camera---->创建一个View用来绘制---->激活一个Camera->渲染 。
#### 2.4 不得吐槽
吐槽我现在把Transform作为场景的入口，真实悔死了，Optix自带的Transform有问题，问题在于：只会对网格做旋转，法线的操作需要你自己去操作，当时的感受是这个东西为何要存在？
#### 2.5
## 3.Light Model
#### 3.1.Microfacet Cook-Torrance
#### 3.2.Glossy reflection Model

## 4.Water Light Simulation Result using different Model