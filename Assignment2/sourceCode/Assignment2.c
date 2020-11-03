#include<stdio.h>
#include<windows.h>

void Binarization(char* input, char* output);
void Erosion(char* input, char* output);
void Delation(char* input, char* output);
void Opening(char* input, char* output);
void Closing(char* input, char* output);

int main()
{
	Binarization("pic.bmp", "BinarizationPic.bmp");
    // Erosion("pic.bmp", "ErosionPic.bmp");
    // Delation("pic.bmp", "DelationPic.bmp");
    // Opening("pic.bmp", "OpeningPic.bmp");
    // Closing("pic.bmp", "ClosingPic.bmp");

	return 0;
}

void Binarization(char* input, char* output)
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
			if (Y < 128) Y = 0;
			else Y = 255;
            
			data[t] = data[t+1] = data[t+2] = (BYTE)Y;
		}
	}

	FILE* pic2 = fopen(output, "wb");  //创建输出文件

    fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, pic2);  //写入位图文件头 
    fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, pic2);  //写入位图信息头
    fwrite(data, imageSize, 1, pic2);  //写入图像数据
    fclose(pic2);
    printf("Binarization suceeded!\n");
    free(data);
}

void Erosion(char* input, char* output)
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

	// int i, j;
	// for (i = 0; i < h; i++)
	// {
	// 	for (j = 0; j < w*3-skip; j += 3)
	// 	{
	// 		int t = i*bytesPerLine + j;

	// 		int B = data[t];
	// 		int G = data[t+1];
	// 		int R = data[t+2];

	// 		int Y = 0.299*R + 0.587*G + 0.114*B;
	// 		if (Y < 128) Y = 0;
	// 		else Y = 255;
            
	// 		data[t] = data[t+1] = data[t+2] = (BYTE)Y;
	// 	}
	// }

	FILE* pic2 = fopen(output, "wb");  //创建输出文件

    fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, pic2);  //写入位图文件头 
    fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, pic2);  //写入位图信息头
    fwrite(data, imageSize, 1, pic2);  //写入图像数据
    fclose(pic2);
    printf("Erosion suceeded!\n");
    free(data);
}

void Delation(char* input, char* output)
{

}

void Opening(char* input, char* output)
{

}

void Closing(char* input, char* output)
{

}