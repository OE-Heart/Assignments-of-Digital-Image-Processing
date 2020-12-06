#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <cmath>

using namespace std;
 
#define Pi acos(-1)

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;

typedef struct tagIMAGEP  //每个像素
{
	BYTE blue;
	BYTE green;
	BYTE red;
}P;
 
typedef struct tagBITMAPINFOHEADER{
	DWORD biSize;                  //信息头大小
	DWORD biWidth;                 //图像宽度
	DWORD biHeight;                //图像高度
	WORD  biPlanes;                //位平面数，必须为1
	WORD  biBitCount;              //每像素位数
	DWORD biCompression;           //压缩类型
	DWORD biSizeImage;             //压缩图像大小字节数
	DWORD biXPelsPerMeter;         //水平分辨率
	DWORD biYPelsPerMeter;         //垂直分辨率
	DWORD biClrUsed;               //位图实际用到的色彩数
	DWORD biClrImportant;          //本位图中重要的色彩数
}BITMAPINFOHEADER;                 //位图信息头定义
 
typedef struct  tagBITMAPFILEHEADER{
	DWORD bfSize;                  //文件大小
	WORD  bfReserved1;             //保留字
	WORD  bfReserved2;             //保留字
	DWORD bfOffBits;               //从文件头到实际位图数据的偏移字节数
}BITMAPFILEHEADER;
 
BITMAPFILEHEADER strHead;
BITMAPINFOHEADER strInfo;
WORD bfType;
int h,w;    //图像的高度和宽度
int size;   //像素个数
int mW;   //取整的宽度
int cx,cy;  //中心宽度，高度坐标

void translation(const P* before, int x, int y);
void rotation(const P* before, double theta);
void scale(const P* before, float x, float y);
void shear(const P* before, double angle);
void mirror(const P* before);

int main()
{
	FILE* pic = fopen("./pic/pic.bmp","rb");
	if(pic == NULL)
	{
		printf("Open file failed!\n");
		return 0;
	}
	
	fread(&bfType, 1, sizeof(WORD), pic); //先读取文件类型
	if(bfType != 0x4d42) {
		printf("Please choose a BMP file!\n");
		return 0;
	}

	//读取bmp文件的文件头和信息头
	fread(&strHead,1,sizeof(tagBITMAPFILEHEADER),pic);
	fread(&strInfo,1,sizeof(tagBITMAPINFOHEADER),pic);
    h=strInfo.biHeight;
    w=strInfo.biWidth;

    if (w % 4 == 0) mW = w;
    else mW = (w/4+1)*4; //如果图像宽度不是4的倍数要取整

    size=strInfo.biSizeImage/3; //图像像素数
    //一个图像所有像素存在数组里
    P *imgP=new P[size];
    //读取bmp数据信息
	fread(imgP,1,sizeof(P)*size,pic);
	fclose(pic);
	cx = mW / 2;
	cy = h / 2;
	int x,y;
	//5个操作
	translation(imgP, 80, 60);
	rotation(imgP, 30.0/180*Pi);
	scale(imgP, 0.8, 1);
	shear(imgP, 30.0/180*Pi);
	mirror(imgP);
	delete[] imgP;

	return 0;
}
 
void translation(const P* before, int x, int y){
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
	//建立新的文件
	FILE *fpo1;
	fpo1=fopen("./pic/TranslationPic.bmp","wb");
	//新的文件头信息头
	BITMAPFILEHEADER nHead = strHead;
	BITMAPINFOHEADER newInfo = strInfo;
	//新的图像大小
	nHead.bfSize = (DWORD)(nHead.bfSize - size*3 + newSize*3);
	//新的图像高度和宽度
	newInfo.biHeight = (DWORD)nH;
	newInfo.biWidth = (DWORD)nW;
	newInfo.biSizeImage = (DWORD)(newSize * 3);
	fwrite(&bfType,1,sizeof(WORD),fpo1);
	fwrite(&nHead,1,sizeof(tagBITMAPFILEHEADER),fpo1);
	fwrite(&newInfo,1,sizeof(tagBITMAPINFOHEADER),fpo1);
	fwrite(Q,1,sizeof(P)*(newSize),fpo1);
	fclose(fpo1);
	//释放空间
	delete[] Q;
}
//坐标处理
double xAfterRot(int x, int y, double theta){
	x = x - cx;
	y = y - cy;
	double result = x * cos(theta) - y * sin(theta);
	return result += cx;
}
double yAfterRot(int x, int y, double theta){
	x = x - cx;
	y = y - cy;
	double result = x * sin(theta) + y * cos(theta);
	return result += cy;
}
 
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
 
void rotation(const P* before, double theta){
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
 
    //把新生成的数据写入文件
	FILE *fpo1;
	fpo1=fopen("./pic/RotationPic.bmp","wb");
	BITMAPFILEHEADER nHead = strHead;
	BITMAPINFOHEADER newInfo = strInfo;
	nHead.bfSize = (DWORD)(nHead.bfSize - size*3 + newSize*3);
	newInfo.biHeight = (DWORD)nH;
	newInfo.biWidth = (DWORD)nW;
	newInfo.biSizeImage = (DWORD)(newSize * 3);
	fwrite(&bfType,1,sizeof(WORD),fpo1);
	fwrite(&nHead,1,sizeof(tagBITMAPFILEHEADER),fpo1);
	fwrite(&newInfo,1,sizeof(tagBITMAPINFOHEADER),fpo1);
	fwrite(Q,1,sizeof(P)*(newSize),fpo1);
	fclose(fpo1);
	delete[] Q;
	delete[] tK;
}

void scale(const P* before, float x, float y){
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
    //新文件
	FILE *fpo1;
	fpo1=fopen("./pic/ScalePic.bmp","wb");
	BITMAPFILEHEADER nHead = strHead;
	BITMAPINFOHEADER newInfo = strInfo;
	nHead.bfSize = (DWORD)(nHead.bfSize - size*3 + newSize*3);
	newInfo.biHeight = (DWORD)nH;
	newInfo.biWidth = (DWORD)nW;
	newInfo.biSizeImage = (DWORD)(newSize * 3);
	fwrite(&bfType,1,sizeof(WORD),fpo1);
	fwrite(&nHead,1,sizeof(tagBITMAPFILEHEADER),fpo1);
	fwrite(&newInfo,1,sizeof(tagBITMAPINFOHEADER),fpo1);
	fwrite(Q,1,sizeof(P)*(newSize),fpo1);
	fclose(fpo1);
	delete[] Q;
	delete[] tK;
}
 
void shear(const P* before, double angle){
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
    //新文件
	FILE *fpo1;
	fpo1=fopen("./pic/ShearPic.bmp","wb");
	BITMAPFILEHEADER nHead = strHead;
	BITMAPINFOHEADER newInfo = strInfo;
	nHead.bfSize = (DWORD)(nHead.bfSize - size*3 + newSize*3);
	newInfo.biHeight = (DWORD)nH;
	newInfo.biWidth = (DWORD)nW;
	newInfo.biSizeImage = (DWORD)(newSize * 3);
	fwrite(&bfType,1,sizeof(WORD),fpo1);
	fwrite(&nHead,1,sizeof(tagBITMAPFILEHEADER),fpo1);
	fwrite(&newInfo,1,sizeof(tagBITMAPINFOHEADER),fpo1);
	fwrite(Q,1,sizeof(P)*(newSize),fpo1);
	fclose(fpo1);
	delete[] Q;
	delete[] tK;
}

void mirror(const P* before){
	P* Q = new P[size];
	memset(Q, 0, sizeof(P) * size);
	for (int i = 0; i < h; i++){
		for (int j = 0; j < mW; j++){
			Q[i*mW + j] = before[i*mW + mW - 1 - j];
		}
	}
    //建新文件
	FILE *fpo1;
	fpo1=fopen("./pic/MirrorPic.bmp","wb");
	fwrite(&bfType,1,sizeof(WORD),fpo1);
	fwrite(&strHead,1,sizeof(tagBITMAPFILEHEADER),fpo1);
	fwrite(&strInfo,1,sizeof(tagBITMAPINFOHEADER),fpo1);
	fwrite(Q,1,sizeof(P)*(size),fpo1);
	fclose(fpo1);
	delete[] Q;
}