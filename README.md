# opengl_demo

## 介绍
opengl 学习

## 软件架构
c++

cmake

## 项目内容

### QiJinOpengl

奇境-opengl

#### demo001

glfw - glad 创建窗口

#### demo002

vao - vbo - shader 显示三角形

#### demo003

vbo - vbo2 显示三角形

源码没有使用 vao 导致顶点数据使用最后一次配置来实现顶点传递，即便它绑定了最开始的 vbo

#### demo004

使用两个 vao 管理 2 个三角形的顶点传递，并且实现绘制

#### demo005

传递 shader uniform 变量值

#### demo006

增加透明处理

