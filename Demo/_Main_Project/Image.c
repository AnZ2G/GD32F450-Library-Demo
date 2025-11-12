/*********************************************************************************************************************
* @file				Image.c
* @author			胡籍
* @version			1.0
* @Target core		GD32F450ZIT6
* @board			Main_Board_V1.0
* @date				2021-9-9
********************************************************************************************************************/


#include "Image.h"
#include "oled.h"
#include "headfile.h"
#include "Track.h"
#include "math.h"

extern uint8_t IMG_zip[60][ColumnMax];

uint8_t my_adapt_threshold_otso(uint8_t tmImage[RowMax][ColumnMax], uint16_t col, uint16_t row)   //注意计算阈值的一定要是原图像
{
   #define GrayScale 256
    static uint8_t last_threshold=0;
    uint16_t width = col;
    uint16_t height = row;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height/4;
    uint8_t threshold = 0,Gray_max,Gray_min;
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    uint32_t gray_sum=0;
    //统计灰度级中每个像素在整幅图像中的个数
    for (i = 0; i < height; i+=2)
    {
        for (j = 0; j < width; j+=2)
        {
            pixelCount[tmImage[j][i]]++;  //将当前的点的像素值作为计数数组的下标
            gray_sum+=(int)tmImage[j][i];       //灰度值总和
					  Gray_max=MAX(tmImage[j][i],Gray_max);
					  Gray_min=MIN(tmImage[j][i],Gray_min);
        }
    }
    //计算每个像素值的点在整幅图像中的比例
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;

    }

    //遍历灰度级[0,255]
       float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
				j= MAX(last_threshold-20,Gray_min);
        for (; j < Gray_max; j++)
        {

                w0 += pixelPro[j];  //背景部分每个灰度值的像素点所占比例之和   即背景部分的比例
                u0tmp += j * pixelPro[j];  //背景部分 每个灰度值的点的比例 *灰度值
                 w1=1-w0;
                u1tmp=gray_sum/pixelSum-u0tmp;
                u0 = u0tmp / w0;              //背景平均灰度
                u1 = u1tmp / w1;              //前景平均灰度
                u = u0tmp + u1tmp;            //全局平均灰度
                deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
                if (deltaTmp > deltaMax)
                {
                    deltaMax = deltaTmp;
                    threshold = j;
                }
                if (deltaTmp < deltaMax)
                {
                break;
                }

         }
    return threshold;
}

uint8_t ostu1(void) //注意计算阈值的一定要是原图像
{
#define GrayScale 256

#define Width ColumnMax
#define Height RowMax

    int pixelCount[GrayScale] = { 0 };
    float pixelProportion[GrayScale] = { 0 };

    const int pixelSum = Width * Height;

    //统计每个灰度级中像素的个数
    for (int i = 0; i < Height; i += 2) {
        for (int j = 0; j < Width; j += 2) {
            int position=(unsigned char)mt9v03x_image[i][j];
            pixelCount[position]++;
            //pixelCount[(unsigned char)mt9v03x_image[i * Width + (j + 1)]]++;
            //pixelCount[(unsigned char)mt9v03x_image[(i + 1) * Width + j]]++;
            //pixelCount[(unsigned char)mt9v03x_image[(i + 1) * Width + (j + 1)]]++;
        }
    }

    float averageGrayScale = 0; //averageGrayScale = u
    //计算每个像素值的点在整幅图像中的比例
    for (int i = 0; i < GrayScale; i++) {
        pixelProportion[i] = (float)pixelCount[i] / (pixelSum/4);
        averageGrayScale += i * pixelProportion[i]; //整幅图像的平均灰度
    }

    int threshold = 0;
    float maxVariance = 0;
    float w0 = 0, avgValue = 0;
    for (int i = 0; i < 256; i++) {
        w0 += pixelProportion[i]; //假设当前灰度i为阈值, 0~i 灰度像素所占整幅图像的比例即前景比例
        avgValue += i * pixelProportion[i]; // 平均值

        float t = avgValue / w0 - averageGrayScale; //avgValue/w0 = u0, t=u0-u
        float variance = t * t * w0 / (1 - w0);

        if (variance > maxVariance) {
            maxVariance = variance;
            threshold = i;
        } else if(variance < maxVariance){
            break;
        }
    }

    return threshold;
}

void ImRez(void)
{
    uint8_t i,j;
    for(i=0;i<56;i+=8)// 56行 
    {
        for(j=0;j<188;j+=2) 
        { 
            IMG_zip[i+0][j/2]=(Img[(0+i)*2][j]+Img[(0+i)*2+1][j]+Img[(0+i)*2][j+1]+Img[(0+i)*2+1][j+1])/4;
            IMG_zip[i+1][j/2]=(Img[(1+i)*2][j]+Img[(1+i)*2+1][j]+Img[(1+i)*2][j+1]+Img[(1+i)*2+1][j+1])/4;
            IMG_zip[i+2][j/2]=(Img[(2+i)*2][j]+Img[(2+i)*2+1][j]+Img[(2+i)*2][j+1]+Img[(2+i)*2+1][j+1])/4;
            IMG_zip[i+3][j/2]=(Img[(3+i)*2][j]+Img[(3+i)*2+1][j]+Img[(3+i)*2][j+1]+Img[(3+i)*2+1][j+1])/4;
            IMG_zip[i+4][j/2]=(Img[(4+i)*2][j]+Img[(4+i)*2+1][j]+Img[(4+i)*2][j+1]+Img[(4+i)*2+1][j+1])/4;
            IMG_zip[i+5][j/2]=(Img[(5+i)*2][j]+Img[(5+i)*2+1][j]+Img[(5+i)*2][j+1]+Img[(5+i)*2+1][j+1])/4;
            IMG_zip[i+6][j/2]=(Img[(6+i)*2][j]+Img[(6+i)*2+1][j]+Img[(6+i)*2][j+1]+Img[(6+i)*2+1][j+1])/4;
            IMG_zip[i+7][j/2]=(Img[(7+i)*2][j]+Img[(7+i)*2+1][j]+Img[(7+i)*2][j+1]+Img[(7+i)*2+1][j+1])/4;	  	  	  
        }
    }  
    for(j=0;j<188;j+=2)// 57-60行
    { 
        IMG_zip[56+0][j/2]=(Img[(0+56)*2][j]+Img[(0+56)*2+1][j]+Img[(0+56)*2][j+1]+Img[(0+56)*2+1][j+1])/4;
        IMG_zip[56+1][j/2]=(Img[(1+56)*2][j]+Img[(1+56)*2+1][j]+Img[(1+56)*2][j+1]+Img[(1+56)*2+1][j+1])/4;
        IMG_zip[56+2][j/2]=(Img[(2+56)*2][j]+Img[(2+56)*2+1][j]+Img[(2+56)*2][j+1]+Img[(2+56)*2+1][j+1])/4;
        IMG_zip[56+3][j/2]=(Img[(3+56)*2][j]+Img[(3+56)*2+1][j]+Img[(3+56)*2][j+1]+Img[(3+56)*2+1][j+1])/4;  	  	  
    } 
}

void ImRez_ori(void)
{
    uint8_t i,j;
    for(i=0;i<56;i+=8)// 56行 
    {
        for(j=0;j<188;j+=2) 
        { 
            IMG_zip[i+0][j/2]=(mt9v03x_image[(0+i)*2][j]+mt9v03x_image[(0+i)*2+1][j]+mt9v03x_image[(0+i)*2][j+1]+mt9v03x_image[(0+i)*2+1][j+1])/4;
            IMG_zip[i+1][j/2]=(mt9v03x_image[(1+i)*2][j]+mt9v03x_image[(1+i)*2+1][j]+mt9v03x_image[(1+i)*2][j+1]+mt9v03x_image[(1+i)*2+1][j+1])/4;
            IMG_zip[i+2][j/2]=(mt9v03x_image[(2+i)*2][j]+mt9v03x_image[(2+i)*2+1][j]+mt9v03x_image[(2+i)*2][j+1]+mt9v03x_image[(2+i)*2+1][j+1])/4;
            IMG_zip[i+3][j/2]=(mt9v03x_image[(3+i)*2][j]+mt9v03x_image[(3+i)*2+1][j]+mt9v03x_image[(3+i)*2][j+1]+mt9v03x_image[(3+i)*2+1][j+1])/4;
            IMG_zip[i+4][j/2]=(mt9v03x_image[(4+i)*2][j]+mt9v03x_image[(4+i)*2+1][j]+mt9v03x_image[(4+i)*2][j+1]+mt9v03x_image[(4+i)*2+1][j+1])/4;
            IMG_zip[i+5][j/2]=(mt9v03x_image[(5+i)*2][j]+mt9v03x_image[(5+i)*2+1][j]+mt9v03x_image[(5+i)*2][j+1]+mt9v03x_image[(5+i)*2+1][j+1])/4;
            IMG_zip[i+6][j/2]=(mt9v03x_image[(6+i)*2][j]+mt9v03x_image[(6+i)*2+1][j]+mt9v03x_image[(6+i)*2][j+1]+mt9v03x_image[(6+i)*2+1][j+1])/4;
            IMG_zip[i+7][j/2]=(mt9v03x_image[(7+i)*2][j]+mt9v03x_image[(7+i)*2+1][j]+mt9v03x_image[(7+i)*2][j+1]+mt9v03x_image[(7+i)*2+1][j+1])/4;	  	  	  
        }
    }
    for(j=0;j<188;j+=2)// 57-60行
    { 
        IMG_zip[56+0][j/2]=(mt9v03x_image[(0+56)*2][j]+mt9v03x_image[(0+56)*2+1][j]+mt9v03x_image[(0+56)*2][j+1]+mt9v03x_image[(0+56)*2+1][j+1])/4;
        IMG_zip[56+1][j/2]=(mt9v03x_image[(1+56)*2][j]+mt9v03x_image[(1+56)*2+1][j]+mt9v03x_image[(1+56)*2][j+1]+mt9v03x_image[(1+56)*2+1][j+1])/4;
        IMG_zip[56+2][j/2]=(mt9v03x_image[(2+56)*2][j]+mt9v03x_image[(2+56)*2+1][j]+mt9v03x_image[(2+56)*2][j+1]+mt9v03x_image[(2+56)*2+1][j+1])/4;
        IMG_zip[56+3][j/2]=(mt9v03x_image[(3+56)*2][j]+mt9v03x_image[(3+56)*2+1][j]+mt9v03x_image[(3+56)*2][j+1]+mt9v03x_image[(3+56)*2+1][j+1])/4;  	  	  
    }  
}
