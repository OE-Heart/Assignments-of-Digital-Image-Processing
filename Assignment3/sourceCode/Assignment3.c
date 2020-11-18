#include<stdio.h>
#include<string.h>
#include<windows.h>
#include<math.h>

void Logarithmic(char* input, char* output);
void Equalization(char* input, char* output);

int main()
{
	Logarithmic("pic.bmp", "LogarithmicPic.bmp");
    //Equalization("pic.bmp", "EqualizationPic.bmp");

	return 0;
}

void Logarithmic(char* input, char* output)
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

    int maxY = 0; //最亮值

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
			if (Y > maxY) maxY = Y;
		}
	}

    for (i = 0; i < h; i++)
	{
		for (j = 0; j < w*3-skip; j += 3)
		{
			int t = i*bytesPerLine + j;
            
            int B = data[t];
			int G = data[t+1];
			int R = data[t+2];

			int Y = 0.299*R + 0.587*G + 0.114*B;  //真实亮度
            if (Y < 0 ) Y = 0;
			if (Y > 255) Y = 255;
			int U = -0.147*R - 0.289*G + 0.435*B;
			int V = 0.615*R - 0.515*G - 0.100*B;

			int d = 255*log10(Y+1)/log10(maxY+1);  //显示亮度

			B = d + 2.032*U;
			if (B < 0) B = 0;
			if (B > 255) B = 255;
			
			G = d -0.394*U - 0.581*V;
			if (G < 0) G = 0;
			if (G > 255) G = 255;
			
            R = d + 1.140*V;
			if (R < 0) R = 0;
			if (R > 255) R = 255;

			data[t] = (BYTE)B;
			data[t+1] = (BYTE)G;
			data[t+2] = (BYTE)R;
			
		}
	}

	FILE* pic2 = fopen(output, "wb");  //创建输出文件

    fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, pic2);  //写入位图文件头 
    fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, pic2);  //写入位图信息头
    fwrite(data, imageSize, 1, pic2);  //写入图像数据
    fclose(pic2);
    printf("Logarithmic succeeded!\n");
    free(data);
}

void Equalization(char* input, char* output)
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

    int maxY = 0;
    int minY = 255;
    int num[256];  //各灰度值的个数
    memset(num, 0, sizeof(int)*256);

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
			if (Y > maxY) maxY = Y;
            if (Y < minY) minY = Y;
		}
	}

	FILE* pic2 = fopen(output, "wb");  //创建输出文件

    fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, pic2);  //写入位图文件头 
    fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, pic2);  //写入位图信息头
    fwrite(data, imageSize, 1, pic2);  //写入图像数据
    fclose(pic2);
    printf("Equalization succeeded!\n");
    free(data);
}