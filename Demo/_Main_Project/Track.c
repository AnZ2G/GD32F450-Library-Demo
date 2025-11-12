/*********************************************************************************************************************
* @file				Track.c
* @author			胡籍
* @version			1.0
* @Target core		GD32F450ZIT6
* @board			Main_Board_V1.0
* @date				2021-9-9
********************************************************************************************************************/

#include "Track.h"
#include "Image.h"
#include "common.h"
#include "headfile.h"

uint8_t IMG_temp[60][94];

uint8_t error_buffer[60] = 
{1,1,1,1,1,1,1,1,1,1	//10
,2,2,2,2,2,2,2,2,2,2	//20
,4,4,4,4,4,4,4,4,4,4	//30
,4,4,4,4,4,4,4,4,4,4	//40
,2,2,2,2,2,2,2,2,2,2	//50
,1,1,1,1,1,1,1,1,1,1};  //60


/*********MiddleLineSearch**********/
int middle_line[RowMax+1];
int RightEdge[RowMax+1];
int LeftEdge[RowMax+1];
int RightEdgeC[RowMax+1];
int LeftEdgeC[RowMax+1];
int Width[RowMax+1]=
{
6,6,6,6,7,8,8,9,10,11,11,
12,13,14,14,15,16,16,17,18,19,
20,20,21,21,22,23,24,24,25,25,
26,27,28,28,29,29,30,31,32,33,
33,34,34,35,36,36,37,37,38,39,
40,41,41,41,42,43,43,43,43,
};
int MidPri;
char LastLine;

//-------------------------------------------------------------------------------------------------------------------
// @brief		赛道数组初始化
// @param		无
// @return		void
// Sample usage:			SetInitVal();//初始化赛道数组
//-------------------------------------------------------------------------------------------------------------------
void SetInitVal()
{
  int i;
  for(i=0;i<RowMax;i++)//赋初值             
  { 
      RightEdge[i]  = ColumnMax;
      LeftEdge[i]   = 0;
      RightEdgeC[i]  = ColumnMax;
      LeftEdgeC[i]   = 0;
      middle_line[i] = ColumnMax/2; 
  }
} 

//-------------------------------------------------------------------------------------------------------------------
// @brief		从两边向中间扫线
// @param		无
// @return		void
// Sample usage:			MiddleLineSearch1();//扫线
//-------------------------------------------------------------------------------------------------------------------
void MiddleLineSearch1()
{
    int8_t i,j,MoveValue=0;
    uint8_t FindedFlag;
//    uint8_t end = 0;


    for(i=0;i<RowMax;i++)//赋初值             
    { 
        RightEdge[i]  = ColumnMax;
        LeftEdge[i]   = 0;
        middle_line[i] = ColumnMax/2; 
        Width[RowMax] = 47;
    }

    for(j=0;j<=93;j++)//找首行左线
    {
        if(IMG_zip[59][j]) 
        {
            LeftEdge[59] = j;
            break;
        }
    }
    for(j=93;j>=0;j--)//找首行右线
    {
        if(IMG_zip[59][j]) 
        {
            RightEdge[59] = j;
            break;
        }
    }

    for(i=58;i>=0;i--)
    {
        FindedFlag = 0;

        if(LeftEdge[i]) MoveValue = -10;     
        else MoveValue = 10;

        for(j=MAX(LeftEdge[i+1]-MoveValue,0);j<LeftEdge[i+1]+10;j++)
        {
            if(IMG_zip[i][j+1]&&(!IMG_zip[i][j])&&j>=0) 
            {
                LeftEdge[i] = j;
                FindedFlag = 1;
                break;
            }
            if(IMG_zip[i][0]) 
            {
                LeftEdge[i] = 0;
                FindedFlag = 1;
                break;
            }
        }
        if(!FindedFlag)
        {
            for(j=LeftEdge[i+1];j>=0&&(j>LeftEdge[i+1]-10);j--)
            {
                if(IMG_zip[i][j]&&(!IMG_zip[i][j-1])) 
                {
                    LeftEdge[i] = j;
                    break;
                }
            }
        }

        for(j=RightEdge[i+1];j>RightEdge[i+1]-10;j--)
        {
            if(IMG_zip[i][j]&&IMG_zip[i][j-1]&&(!IMG_zip[i][j+1])) 
            {
                RightEdge[i] = j;
                FindedFlag = 1;
                break;
            }
            if(IMG_zip[i][93]) 
            {
                RightEdge[i] = 93;
                FindedFlag = 1;
                break;
            }
        }
        if(!FindedFlag)
        {
            for(j=RightEdge[i+1];j<RowMax&&(j<RightEdge[i+1]+10);j++)
            {
                if(IMG_zip[i][j]&&IMG_zip[i][j-1]&&(!IMG_zip[i][j+1])) 
                {
                    RightEdge[i] = j;
                    break;
                }
            }
        }

        if(LeftEdge[i]!=0 && RightEdge[i]!=ColumnMax-1)		//中线判断，没有丢线
        {				
            middle_line[i] = (LeftEdge[i] + RightEdge[i])/2;				
        }
        else if(LeftEdge[i]!=0 && RightEdge[i]==ColumnMax-1)//丢了右线
  	    {   					
            middle_line[i]=LeftEdge[i]+Width[i];
        }
        else if(LeftEdge[i]==0 && RightEdge[i]!=ColumnMax-1)//丢了左线
        {
            middle_line[i]=RightEdge[i]-Width[i];
        }
        else if(LeftEdge[i]==0 && RightEdge[i]==ColumnMax-1)//两边都丢了的话  
        {     
            middle_line[i] = 47; 
//            end = i;          
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		从中间往两边扫线
// @param		无
// @return		void
// Sample usage:			MiddleLineSearch2();//扫线
//-------------------------------------------------------------------------------------------------------------------
void MiddleLineSearch2(void)
{
   int16_t i         = 0;    //ColumnMax为最大列数  RowMax 为最大行数  60x80   
   int16_t j         = 0;
   middle_line[RowMax]  = ColumnMax/2;//第中间行 
   LeftEdge[RowMax]    = 0;
   RightEdge[RowMax]   = ColumnMax;
   Width[RowMax]       = ColumnMax;
   SetInitVal();          //数组复位 
   for(i=RowMax-1;i>4;i--)//全行扫描
   {
    if(i ==RowMax-1)//首行就以图像中心作为扫描起点
    {
       j = MidPri;
    } 
    else
    {
        j = middle_line[i+1];//否则就以上一行中点的位置作为本行扫描起点
    }   
    if(j <= 3)
    {
        j = 3;
    } 
    while(j >= 3)//j>=3有效范围内进行搜寻 
    {		
			if(IMG_zip[i][j-2]==Black_Point&&IMG_zip[i][j-1]==White_Point&&IMG_zip[i][j]==White_Point)//从右向左找到白黑跳变 
			{
				LeftEdge[i] =j;//找到则赋值 找不到保持原值0      
				break;//跳出本行寻线
			}
        j--;//列数往左移动
    }
    if(i==RowMax-1) //再找右边界
    {
    	j = MidPri;//如果首行，从图像中心开始搜寻
    }          
    else
    {
        j = middle_line[i+1];//否则从上一行中心位置开始搜寻
    }
     if(j >=ColumnMax-4)//j >=ColumnMax-2有效范围内搜寻右线
     {
        j = ColumnMax-4;
     }
     while(j <= ColumnMax-4)
     {    
				if(IMG_zip[i][j+2]==Black_Point&&IMG_zip[i][j+1]==White_Point&&IMG_zip[i][j]==White_Point)//从左向右找到白白黑跳变点
				{			
						RightEdge[i] = j;//找到则赋值   找不到保持原值
						break;//跳出本行寻线
				}
        j++;//列数往右移动
     }
     if(LeftEdge[i]!=0 && RightEdge[i]!=ColumnMax)//中线判断，没有丢线
     {				
				middle_line[i] = (LeftEdge[i] + RightEdge[i])/2;				
     }
     else if(LeftEdge[i]!=0 && RightEdge[i]==ColumnMax)//丢了右线
  	 {   					
				middle_line[i]=LeftEdge[i]+Width[i];
     }
     else if(LeftEdge[i]==0 && RightEdge[i]!=ColumnMax)//丢了左线
     {
				middle_line[i]=RightEdge[i]-Width[i];
     }
     else if(LeftEdge[i]==0 && RightEdge[i]==ColumnMax)//两边都丢了的话  
     {     
        if(i ==RowMax-1)//如果是首行就以图像中心作为中点
        {
            middle_line[i] = MidPri;
        }       
        else 
        {
			    middle_line[i]=middle_line[i+1];
        }             
      }
	   if((i<55&&((LeftEdge[i+1]>=75) || (
				 RightEdge[i+1]<10)||
			 (IMG_zip[i+1][middle_line[i]]==Black_Point&&IMG_zip[i+2][middle_line[i]]==Black_Point))))
		   {
			   LastLine = i+1;//最后一行，动态前瞻
            break;
			}    		
  }	
		if(middle_line[RowMax-1] >=75)   //赛道限幅
     {
         MidPri = 75;
     }          
     else if(middle_line[RowMax-1] <= 15)
     {
         MidPri = 15;
     }         
     else
     {
          MidPri = middle_line[RowMax-1];//记录本帧图像的最后一行的中线值，作为下一幅图像扫描起始点
     }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		通过中线数组计算中线值
// @param		
// @return		uint8_t		中线值
// Sample usage:			uint8_t BI_error = road_error();//计算偏差值
//-------------------------------------------------------------------------------------------------------------------
uint8_t road_error(void)
{
    uint8_t i;
    uint32_t sum=0;
    uint16_t count=0;
    for(i=59;middle_line[i]!=0;i--)
    {
        sum += middle_line[i] * error_buffer[i];
        count += error_buffer[i];
    }
    return (sum/count);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		通过中线值计算舵机打角
// @param		
// @return		uint8_t		舵机打角
// Sample usage:			uint8_t steer_value = find_road_control(BI_error,47);//计算偏差值
//-------------------------------------------------------------------------------------------------------------------
uint16_t find_road_control(uint8_t error, uint8_t middle)
{
    uint16_t steer_value;

    steer_value = (middle - error)*PD_P + 1400;
    
    if(steer_value>1570) steer_value = 1570;
    else if(steer_value<1230) steer_value = 1230;

    return steer_value;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		将中线写入压缩后的数组，用于显示
// @param		无
// @return		void
// Sample usage:			Middle_Print();//将中线写入IMG_Zip
//-------------------------------------------------------------------------------------------------------------------
void Middle_Print()
{
  uint16_t i;
  for(i=0;i<RowMax;i++)//赋初值             
  { 
      IMG_zip[i][middle_line[i]] = 0;
  }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		将线在TFT屏显示
// @param		无
// @return		void
// Sample usage:			Line_Print(middle_line);//将中线在TFT屏显示
//-------------------------------------------------------------------------------------------------------------------
void Line_Print(void)
{
  uint16_t i;
  for(i=0;i<RowMax;i++)//赋初值             
  { 
      TFTSPI_Draw_Dot(LeftEdge[i],i,0xfc08);
      TFTSPI_Draw_Dot(LeftEdgeC[i],i,0xf81f);
      TFTSPI_Draw_Dot(RightEdge[i],i,0x07e0);
      TFTSPI_Draw_Dot(middle_line[i],i,0);
  }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		发送寻线数组信息至上位机
// @param		
// @return		void
// Sample usage:			FireWater_Send(middle_line);//向上位机发送中线数组
//-------------------------------------------------------------------------------------------------------------------
void FireWater_Send(void)
{
  uint16_t i;
  printf("\r\n赛道宽度：\r\n");
  for(i=0;i<RowMax;i++)//赋初值             
  { 
      printf("%d,",(LeftEdge[i]));
      if(i%10==0) printf("\r\n");
  }
}

void Line_Repair(void)
{
    uint8_t i,i1,i2;
    i1 = 0;
    i2 = 0;
    for(i=50;i>10;i--)
    {
        if((LeftEdge[i]>=LeftEdge[i-1])&&(LeftEdge[i-1]>=LeftEdge[i-2])) i1 = i;
        break;
    }
    for(i=i1-2;i>10;i--)
    {
        if(LeftEdge[i]<LeftEdge[i-1]&&LeftEdge[i-1]<LeftEdge[i-2]) i2 = i-2;
        break;
    }
    if(i1!=0&&i2!=0)
    {
        for(i=59;i>1;i--) LeftEdgeC[i] = LeftEdge[i];
        for(i=i1;i>=i2;i--)
        {
            LeftEdgeC[i] = (uint8_t)((float)(LeftEdge[i2] - LeftEdge[i1]) / (i2-i1) * (i-i1));
        }
    }
    OLED_ShowNum(0,4,i1,2,16);
    OLED_ShowNum(16,4,i2,2,16);
}
