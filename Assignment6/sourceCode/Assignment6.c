#include<stdio.h>
#include<string.h>
#include<windows.h>
#include<math.h>

void RGBToYUV(char* input, char* output);
void BilateralFiltering(char* input, char* output);

int main()
{
    RGBToYUV("./pic/pic.bmp", "./pic/picInYUV.bmp");
	BilateralFiltering("./pic/picInYUV.bmp", "./pic/BilateralFilteringPic.bmp");

	return 0;
}

void RGBToYUV(char* input, char* output)
{
	BITMAPFILEHEADER fileHeader;  //位图文件头
    BITMAPINFOHEADER infoHeader;  //位图信息头
    FILE* pic1= fopen(input, "rb");  //打开输入文件
    if (pic1 == NULL)
	{
    	printf("Open file failed!\n");
    	exit(-1);
    }
 
    fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, pic1);  //读取文件头
    fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, pic1);  //读取信息头
    WORD bitCount = infoHeader.biBitCount;  //颜色位数
    if (bitCount != 24)
    {
        printf("Only 24 bit color map is allowed!\n");
    	exit(-1);
    }

    LONG w = infoHeader.biWidth;    //图像数据的宽度
    LONG h = infoHeader.biHeight;   //图像数据的高度
    
    int bytesPerLine = ((w * bitCount+31)>>5)<<2;  //行数据大小
    int imageSize = bytesPerLine*h;  //图像数据大小
    int skip = 4-((w * bitCount)>>3)&3;  //需跳过的字节
    
    BYTE* data = (BYTE*)malloc(imageSize);  //申请存储图像数据的空间
    fread(data, imageSize, 1, pic1);  //记录图像数据

	int i, j;
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w*3-skip; j += 3)
		{
			int t = i*bytesPerLine + j;

			int B = data[t];
			int G = data[t+1];
			int R = data[t+2];

			int Y = 0.299*R + 0.587*G + 0.114*B;
			if (Y < 0 ) Y = 0;
			if (Y > 255) Y = 255;
            
			data[t] = data[t+1] = data[t+2] = (BYTE)Y;
		}
	}

	FILE* pic2 = fopen(output, "wb");  //创建输出文件

    fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, pic2);  //写入位图文件头 
    fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, pic2);  //写入位图信息头
    fwrite(data, imageSize, 1, pic2);  //写入图像数据
    fclose(pic2);
    printf("RGBToYUV suceeded!\n");
    free(data);
}

void BilateralFiltering(char* input, char* output)
{
    BITMAPFILEHEADER fileHeader;  //位图文件头
    BITMAPINFOHEADER infoHeader;  //位图信息头
    FILE* pic1= fopen(input, "rb");  //打开输入文件
    if (pic1 == NULL)
	{
    	printf("Open file failed!\n");
    	exit(-1);
    }
 
    fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, pic1);  //读取文件头
    fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, pic1);  //读取信息头
    WORD bitCount = infoHeader.biBitCount;  //颜色位数
    if (bitCount != 24)
    {
        printf("Only 24 bit color map is allowed!\n");
    	exit(-1);
    }

    LONG w = infoHeader.biWidth;    //图像数据的宽度
    LONG h = infoHeader.biHeight;   //图像数据的高度
    
    int bytesPerLine = ((w * bitCount+31)>>5)<<2;  //行数据大小
    int imageSize = bytesPerLine*h;  //图像数据大小
    int skip = 4-((w * bitCount)>>3)&3;  //需跳过的字节
    
    BYTE* data = (BYTE*)malloc(imageSize);  //申请存储图像数据的空间
    fread(data, imageSize, 1, pic1);  //记录图像数据

    int r = 7;
 
    double sigma_s = 1000;  //空间函数方差
    double sigma_r = 10;    //相似函数方差
    double s_coef = 0.5/(sigma_s*sigma_s);
    double r_coef = 0.5/(sigma_r*sigma_r);
 
    double s_w[200][200];  //空间权重
    double r_w[256];       //相似权重

    int i, j;

    //计算空间权重
    for(i = -r; i <= r; i++)
    {
        for(j = -r; j <= r; j++)
        {
            int x = i+r;
            int y = j+r;
            s_w[x][y] = exp(-(i*i+j*j)*s_coef);
        }
    }

    //计算相似权重
    for(i = 0; i < 256; i++)
    {
        r_w[i] = exp(-i*i*r_coef);
    }

    //双边滤波
    int k, m, n;
    for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
		    for (k = 0; k < 3; k++)
            {
                double weightSum = 0, dataSum = 0;
 
                for ( m = -r; m <= r; m++)
                {
                    for (n = -r; n <= r; n++)
                    {
                        if (m * m + n * n > r * r) continue;
                        
                        int H = i + m;
                        int W = j + n;

                        if (H < 0) H = 0;
                        if (H > h-1) H = h-1;
                        if (W < 0) W = 0;
                        if (W > w-1) W = w-1;

                        int d = abs(data[H*3*w+W*3+k]-data[i*3*w+j*3+k]);

                        double weight = s_w[m+r][n+r]*r_w[d];   //复合权重
                        dataSum += data[H*3*w+3*W+k]*weight;
                        weightSum += weight;
                    }
                }
                double final = dataSum/weightSum;
                data[i*3*w+j*3+k] = (BYTE)final;
		    }
		}
	}

	FILE* pic2 = fopen(output, "wb");  //创建输出文件

    fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, pic2);  //写入位图文件头 
    fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, pic2);  //写入位图信息头
    fwrite(data, imageSize, 1, pic2);  //写入图像数据
    fclose(pic2);
    printf("BilateralFiltering succeeded!\n");
    free(data);
}