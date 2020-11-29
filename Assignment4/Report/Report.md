

<img src="pic/1.png" alt="1" style="zoom:120%;" />



<center><font face="黑体" size="20">图像的简单几何变换实验报告</font><font face="黑体" size="20"></font>



<img src="pic/2.png" alt="2" style="zoom:150%;" />



<center>
    <font face="楷体" size="5">姓名：欧翌昕</font>
</center>

<center>
    <font face="楷体" size="5">专业：软件工程</font>
</center>

<center>
    <font face="楷体" size="5">学号：3190104783</font>
</center>

<center>
    <font face="楷体" size="5">课程名称：图像信息处理</font>
</center>

<center>
    <font face="楷体" size="5">指导老师：宋明黎</font>



<center>
    </font><font face="黑体" size="5">2020~2021秋冬学期 2020 年 12 月 6 日</font>
</center>




---



## 1 实验目的和要求

  		1. 平移
  		2. 旋转
  		3. 缩放
  		4. 错切
  		5. 镜像

---



## 2 实验内容和原理

### 2.1 几何变换的概念

​		几何变换是利用变换函数改变图像中像素的位置，从而产生新图像的过程。 几何变换不改变像素值，而是改变像素所在的位置。 

​		变换公式：
$$
g(x,y)=f(x',y')=f[a(x,y),b(x,y)]
$$
​		其中$f(x,y)$表示输入图像， $g(x,y)$表示输出图像。函数$a(x,y)$和$b(x,y)$唯一地描述了空间变换。 

​		几何变换根据难易程度通常可以归结为两类:

- 简单变换：变换过程（各个像素变换前后的位置）以及变换参数可知时的变换，如图像的平移、镜像、转置、旋转、缩放、错切变换等。
- 一般变换：变换过程不是一目了然，变换参数难以测量时的变换。通常情况下，对图像畸变进行校正时，需要用到较为复杂的变换公式。 

### 2.2 平移

​		平移是将图像沿水平和竖直方向移动，从而产生新图像的过程。

​		变换公式：
$$
\begin{bmatrix}
x'\\
y'\\
1\\
\end{bmatrix}
=
 \begin{bmatrix}
1 & 0 & x_0 \\
0 & 1 & y_0 \\
0 & 0 & 1 \\
\end{bmatrix}
 \begin{bmatrix}
x\\
y\\
1\\
\end{bmatrix}
$$
​		原图像中的每个像素沿$x$方向平移$x_0$，沿$y$方向平移$y_0$。 

​		平移后的景物与原图像相同，但“画布”一定是扩大了，否则就会丢失信息。

### 2.3 旋转

​		旋转是将图像绕原点旋转$\theta$角，从而得到新图像的过程。

​		变换公式：
$$
\begin{bmatrix}
x'\\
y'\\
1\\
\end{bmatrix}
=
 \begin{bmatrix}
\cos\theta & -\sin\theta & 0 \\
\sin\theta & \cos\theta & 0 \\
0 & 0 & 1 \\
\end{bmatrix}
 \begin{bmatrix}
x\\
y\\
1\\
\end{bmatrix}
$$
​		图像经过旋转变换以后，新图像中会出现许多空洞，可以使用插值方法填补。

​		行插值：按顺序寻找每一行中的空洞像素，设置其像素值与同一行中前一个像素的像素值相同。

### 2.4 缩放

​		缩放是将图像乘以一定系数，从而产生新图像的过程。

​		变换公式：
$$
\begin{bmatrix}
x'\\
y'\\
1\\
\end{bmatrix}
=
 \begin{bmatrix}
c & 0 & 0 \\
0 & d & 0 \\
0 & 0 & 1 \\
\end{bmatrix}
 \begin{bmatrix}
x\\
y\\
1\\
\end{bmatrix}
$$
​		沿*x*轴方向缩放$c$倍（$c>1$时为放大，$0<c<1$时为缩小）；沿$y$轴方向缩放$d$倍（$d>1$时为放大，$0<d<1$时为缩小）。 

​		当$c=d$时，图像等比缩放；否则为非等比缩放，导致图像变形。

​		缩小：按一定间隔选取某些行和列的像素构成缩小后的新图像。

​		放大：新图像出现空行和空列，可采用插值的方法加以填补，但存在“马赛克”现象。

### 2.5 错切

​		图像的错切变换实际上是景物在平面上的非垂直投影效果。

​		变换公式：
$$
\left\{ 
\begin{array}{c}
a(x,y)=x+d_xy \\ 
b(x,y)=y
\end{array}
\right.\\
\left\{ 
\begin{array}{c}
a(x,y)=x \\ 
b(x,y)=y+d_yx
\end{array}
\right.
$$

### 2.6 镜像

​		镜像是将图像绕$x$轴或$y$轴翻转，从而产生与原图像对称的新图像的过程。

​		变换公式：
$$
\begin{bmatrix}
x'\\
y'\\
1\\
\end{bmatrix}
=
 \begin{bmatrix}
S_x & 0 & 0 \\
0 & S_y & 1 \\
0 & 0 & 1 \\
\end{bmatrix}
 \begin{bmatrix}
x\\
y\\
1\\
\end{bmatrix}
$$
​		当$S_x = 1$，且$S_y = -1$时实现绕$x$轴的镜像变换；当$S_x = -1$，且$S_y = 1$时实现绕$y$轴的镜像变换。

---



## 3 实验步骤和分析



---



## 4 实验结果



---



##  5 心得体会