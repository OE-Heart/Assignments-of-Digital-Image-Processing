

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

### 3.1 平移

```c++
	int moveX, moveY;
	//对4取整
	if(x % 4 == 0)
		moveX = x;
	else
		moveX = x/4*4;
	moveY = y;
    //新坐标
	int nH = h + moveY;
	int nW = mW + moveX;
	//新像素数
	int newSize = nH * nW;
	P* Q = new P[newSize];
	memset(Q, 0, sizeof(P)*newSize);
	//处理每个像素
	for (int i = 0; i < nH; i++){
		for (int j = 0; j < nW; j++){
			if(i < moveY){
				Q[i*nW + j].red = Q[i*nW + j].blue = Q[i*nW + j].green = 0;
				continue;
			}
			if(j < moveX)
				Q[i*nW + j].red = Q[i*nW + j].blue = Q[i*nW + j].green = 0;
			else
				Q[i*nW + j] = before[(i- moveY)*mW + (j - moveX)];
		}
	}
```

​		在遍历时进行操作，将不在平移范围内的区域设置为0，即设置成黑色。在平移范围内的区域则关于x, y进行相关换算，最后得到相对应的平移后图片进行输出。

### 3.2 旋转

​		旋转部分操作比较复杂，采取几个函数复合实现。

```c++
double xAfterRot(int x, int y, double theta){
	x = x - cx;
	y = y - cy;
	double result = x * cos(theta) - y * sin(theta);
	return result += cx;
}
```

```c++
double yAfterRot(int x, int y, double theta){
	x = x - cx;
	y = y - cy;
	double result = x * sin(theta) + y * cos(theta);
	return result += cy;
}
```

​		xAfterRot和yAfterRot函数进行对点旋转后坐标的计算。

```c++
void tXY(int &x, int &y, double theta){
	double tmpX[4],tmpY[4];
	double minX = 999999, minY = 999999, maxX = 0, maxY = 0;
 
	tmpX[0] = xAfterRot(0, 0, theta);
	tmpY[0] = yAfterRot(0, 0, theta);
	tmpX[1] = xAfterRot(mW, 0, theta);
	tmpY[1] = yAfterRot(mW, 0, theta);
	tmpX[2] = xAfterRot(mW, h, theta);
	tmpY[2] = yAfterRot(mW, h, theta);
	tmpX[3] = xAfterRot(0, h, theta);
	tmpY[3] = yAfterRot(0, h, theta);
 
	for(int i = 0; i < 4; i++){
		minX = min(tmpX[i], minX);
		maxX = max(tmpX[i], maxX);
		minY = min(tmpY[i], minY);
		maxY = max(tmpY[i], maxY);
	}
	x = (int)(maxX - minX + 2);
	y = (int)(maxY - minY + 2);
}
```

​		tXY函数用于求出旋转后图片的宽度和高度。

```c++
	int nH;
	int nW;
	tXY(nW, nH, theta);
	if(nW % 4 !=0)
		nW = (nW / 4 + 1) * 4;
	int newSize = nH * nW;
	P* Q = new P[newSize];
	int* tK = new int[newSize];
	int* minIndexX = new int[nH];
	int* maxIndexX = new int[nH];
	int* minIndexY = new int[nW];
	int* maxIndexY = new int[nW];
 
	memset(Q, 0, sizeof(P)*newSize);
	for(int i = 0 ; i < nH; i++)
		minIndexX[i] = 999999;
	for(int i = 0; i < nW; i++)
		minIndexY[i] = 999999;
	memset(maxIndexX, 0, sizeof(int)*nH);
	memset(maxIndexY, 0, sizeof(int)*nW);
 
	for(int i = 0; i < newSize; i++)
		tK[i] = 1;
 
	int moveX = (nW - mW)/2;
	int moveY = (nH - h)/2;
 
	for(int i = 0; i < h; i++){
		for(int j = 0; j < mW; j++){
			int tmpX = (int)xAfterRot(j, i, theta) + moveX;
			int tmpY = (int)yAfterRot(j, i, theta) + moveY;
			Q[tmpY*nW + tmpX] = before[i*mW + j];
			tK[tmpY*nW + tmpX] = 0;
			minIndexX[tmpY] = min(tmpX,minIndexX[tmpY]);
			maxIndexX[tmpY] = max(tmpX,maxIndexX[tmpY]);
			minIndexY[tmpX] = min(tmpY,minIndexY[tmpX]);
			maxIndexY[tmpX] = max(tmpY,maxIndexY[tmpX]);
		}
	}
 
	//first do the horizontal interpolation
	for(int i = 0; i < nH; i++){
		int last = -1;
		for(int j = 0; j < nW; j++){
			if(j < minIndexX[i] || j > maxIndexX[i]){
				Q[i*nW + j].red = 255;
				Q[i*nW + j].green = 255;
				Q[i*nW + j].blue = 255;
				continue;
			}
			if(tK[i*nW + j] == 0){
				last = j;
				continue;
			}
			else{
				int k = j;
				//find next position which has value
				while(k < nW && tK[i*nW + k] == 1){
					k++;
				}
				if(k >= nW){			//in the last position of the line
					if(last == -1)
						break;
					Q[i*nW + j] = Q[i*nW + last];
				}
				else{
					if(last == -1){		//in the first position of the line
						Q[i*nW + j] = Q[i*nW + k];
					}
					else{				//in the middle
						float coe = (j-last)*1.0/(k-last);
						Q[i*nW + j].red = (BYTE)(((int)Q[i*nW + k].red - (int)Q[i*nW + last].red)*coe + (int)Q[i*nW + last].red);
						Q[i*nW + j].blue = (BYTE)(((int)Q[i*nW + k].blue - (int)Q[i*nW + last].blue)*coe + (int)Q[i*nW + last].blue);
						Q[i*nW + j].green = (BYTE)(((int)Q[i*nW + k].green - (int)Q[i*nW + last].green)*coe + (int)Q[i*nW + last].green);
					}
				}
			}
			tK[i*nW + j] = 0;
		}
	}
	//then do the vertical interpolation
	for(int i = 0; i < nW; i++){
		int last = -1;
		for(int j = 0; j < nH; j++){
			if(j < minIndexY[i] || j > maxIndexY[i]){
				continue;
			}
			if(tK[j*nW + i] == 0){
				last = j;
				continue;
			}
			else{
				int k = j;
				//find next position which has value
				while(k < nH && tK[k*nW + i] == 1){
					k++;
				}
				if(k >= nH){			//in the last position of the line
					if(last == -1)
						break;
					Q[j*nW + i] = Q[last*nW + i];
				}
				else{
					if(last == -1){		//in the first position of the line
						Q[j*nW + i] = Q[k*nW + i];
					}
					else{				//in the middle
						float coe = (j-last)*1.0/(k-last);
						Q[j*nW + i].red = (BYTE)(((int)Q[k*nW + i].red - (int)Q[last*nW + i].red)*coe + (int)Q[last*nW + i].red);
						Q[j*nW + i].blue = (BYTE)(((int)Q[k*nW + i].blue - (int)Q[last*nW + i].blue)*coe + (int)Q[last*nW + i].blue);
						Q[j*nW + i].green = (BYTE)(((int)Q[k*nW + i].green - (int)Q[last*nW + i].green)*coe + (int)Q[last*nW + i].green);
					}
				}
			}
			tK[j*nW + i] = 0;
		}
	}
```

​		分别在水平和竖直两个方向上进行操作，在遍历时判断下一个有值位置，按照公式对像素进行变换并对边界情况进行特殊处理，最后得到旋转后图像进行输出。

### 3.3 缩放

```c++
    //新的图像高度和宽度
	int nH = (int)(h * y);
	int tW = (int)(w * x);
	int nW;
	//对4取整
	if(tW % 4 == 0)
		nW = tW;
	else
		nW = (tW / 4 + 1) * 4;
	//新的图像大小
	int newSize = nH * nW;
	P* Q = new P[newSize];
	int* tK = new int[newSize];
	//先把整个图像置0
	memset(Q, 0, sizeof(P)*newSize);
	memset(tK, 0, sizeof(int)*newSize);
	//扫描需要的新图像
	for(int i = 0; i < nH; i++){
		for(int j = 0; j < nW; j++){
			float tX,tY;
			int intX,intY;
			//scale之后的坐标
			tX = (j*1.0/x);
			tY = (i*1.0/y);
			intX = (int)tX;
			intY = (int)tY;
 
			if(tX >= mW || tY >= h)
				Q[i*nW+j].red = Q[i*nW+j].green = Q[i*nW+j].blue = 255;
			else if(tX - intX > 0 || tY - intY > 0){
				Q[i*nW+j].red = Q[i*nW+j].green = Q[i*nW+j].blue = 255;
				tK[i*nW+j] = 1;
			}
			else
				Q[i*nW+j] = before[intY*mW + intX];
		}
	}
	//水平scale
	for(int i = 0; i < nH; i++){
		int last = -1;
		for(int j = 0; j < nW; j++){
			if(tK[i*nW + j] == 0){
				last = j;
				continue;
			}
			else{
				int k = j;
				//找到下一个有值的地方
				while(k < nW &&tK[i*nW + k] == 1 )
					k++;
				if(k >= nW){			//行最后
					if(last == -1)
						break;
					Q[i*nW + j] = Q[i*nW + last];
				}
				else{
					if(last == -1)	//行头
						Q[i*nW + j] = Q[i*nW + k];
					else{
						float coe = (j-last)*1.0/(k-last);
						Q[i*nW + j].red = (BYTE)(((int)Q[i*nW + k].red - (int)Q[i*nW + last].red)*coe + (int)Q[i*nW + last].red);
						Q[i*nW + j].blue = (BYTE)(((int)Q[i*nW + k].blue - (int)Q[i*nW + last].blue)*coe + (int)Q[i*nW + last].blue);
						Q[i*nW + j].green = (BYTE)(((int)Q[i*nW + k].green - (int)Q[i*nW + last].green)*coe + (int)Q[i*nW + last].green);
					}
				}
			}
			tK[i*nW + j] = 0;
		}
	}
	//竖直scale
	for(int i = 0; i < nW; i++){
		int last = -1;
		for(int j = 0; j < nH; j++){
			if(tK[j*nW + i] == 0){
				last = j;
				continue;
			}
			else{
				int k = j;
				//find next position which has value
				while(k < nH && tK[k*nW + i] == 1){
					k++;
				}
				if(k >= nH){			//行最后
					if(last == -1)
						break;
					Q[j*nW + i] = Q[last*nW + i];
				}
				else{
					if(last == -1){		//行头
						Q[j*nW + i] = Q[k*nW + i];
					}
					else{
						float coe = (j-last)*1.0/(k-last);
						Q[j*nW + i].red = (BYTE)(((int)Q[k*nW + i].red - (int)Q[last*nW + i].red)*coe + (int)Q[last*nW + i].red);
						Q[j*nW + i].blue = (BYTE)(((int)Q[k*nW + i].blue - (int)Q[last*nW + i].blue)*coe + (int)Q[last*nW + i].blue);
						Q[j*nW + i].green = (BYTE)(((int)Q[k*nW + i].green - (int)Q[last*nW + i].green)*coe + (int)Q[last*nW + i].green);
					}
				}
			}
			tK[j*nW + i] = 0;
		}
	}
```

​		利用放大倍数x,y进行整个画布大小的重构，并改写bmp结构中的头部分，使其大小适应newSize。先对原data中元素进行变换，此次选取最临近插值，对越界部分进行填充，其他部分进行对应坐标填充，最后得到缩放后图片进行输出。

### 3.4 错切

```c++
	int nH, nW ,newSize;
	P* Q;
	int* tK;
 
	nW = mW;
 
	nH = h + tan(angle)*mW;
	newSize = nW * nH;
	Q = new P[newSize];
	tK = new int[newSize];
    //判断所有像素是否已经有信息的标记数组
	for(int i = 0; i < newSize; i++)
		tK[i] = 1;
    //原来的
	for(int i = 0; i < h; i++){
		for(int j = 0; j < mW; j++){
            //求变换后的横纵坐标，并赋值对应的像素
			int tmpX = j;
			int tmpY = (int)(i + tan(angle)*j);
			Q[tmpY * nW + tmpX] = before[i*mW + j];
            //有信息后标记为0
			tK[tmpY * nW + tmpX] = 0;
		}
	}
    //没图像的位置都用白色填充
	for(int i = 0; i < newSize; i++)
		if(tK[i] == 1)
			Q[i].red = Q[i].green = Q[i].blue = 255;
```

​		错切操作与平移操作类似。利用一个数组来标记所有像素是否已经有信息。遍历图像，求出变换后的横纵坐标后赋值对应的像素并更新所有像素是否有信息的状态，最后遍历所有像素，没有信息的位置都用白色进行填充。

### 3.5 镜像

```c++
	P* Q = new P[size];
	memset(Q, 0, sizeof(P) * size);
	for (int i = 0; i < h; i++){
		for (int j = 0; j < mW; j++){
			Q[i*mW + j] = before[i*mW + mW - 1 - j];
		}
	}
```

​		镜像操作较为简单，在输出时倒序进行输出即可，例如对x方向镜像则j从大到小循环。

---



## 4 实验结果

### 4.1 原始图像

<img src="pic/pic.bmp" alt="pic" style="zoom: 50%;" />

### 4.2 平移结果

<img src="pic/TranslationPic.bmp" alt="TranslationPic" style="zoom:50%;" />

### 4.3 旋转结果

<img src="pic/RotationPic.bmp" alt="RotationPic" style="zoom:30%;" />

### 4.4 缩放结果

<img src="pic/ScalePic.bmp" alt="ScalePic" style="zoom:50%;" />

### 4.5 错切结果

<img src="pic/ShearPic.bmp" alt="ShearPic" style="zoom:50%;" />

### 4.6 镜像结果

<img src="pic/MirrorPic.bmp" alt="MirrorPic" style="zoom:50%;" />

---



##  5 心得体会

​		通过本次实验，我对BMP文件结构有了更深了解的同时还巩固了老师上课时讲到的一些简单几何变换操作。除此之外，对本次实验的深入思考也让我收获到很多知识，但在有些地方还有很大的改进可能空间，比如平移变换没有实现x，y是负值的情况，在插值的部分也可以进行效果更好的其它算法。