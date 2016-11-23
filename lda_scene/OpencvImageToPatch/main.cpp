#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include <highgui.h>
#include <cvaux.h>
#include <string>
#include <iostream>
#include <fstream>
#include<malloc.h>
#include<math.h>
#include<assert.h>
#include<ctype.h>
#include<time.h>
#include<vector>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
using namespace cv;
using namespace std;
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/core/core.hpp"
char file[255];
char file_out[255];
char file_out2[255];
char file_path_e[400] = ".\\testdata\\";
char file_feature_out[255]=".\\patch_4\\";
/***************************************************************
 *
 *	函数名称：main
 *	函数功能：主函数
 ***************************************************************/

void cutimag(Mat src_img,int m,int n,Vector<Mat> ceil_img,int Class,int Num);
int main()
{
	int featureSum = 0;
	//sprintf(file, "%s%d\\%d.jpg", file_path_e, 1, 1);  
	//int a=267/(16+10);
	//int b=200/(16+10);

	//读取图像
	for(int Class=1;Class<14;Class++)
	{
		for(int Number=1;Number<201;Number++)
		{
				sprintf(file, "%s%d\\%d.jpg", file_path_e, Class, Number);
				Mat img = imread(file,1);  
				int height = img.rows;  
				int width  = img.cols;
				//int a1=height/(16+5);
				//int b1=width/(16+5);
				int a1=2;
				int b1=2;
				//imshow("src img",img);
				Vector<Mat> ceil_img = a1*b1;  
				cutimag(img,height/a1,width/b1,ceil_img,Class,Number);
		}
	}
	cvWaitKey(0);
	system("pause");
	return 0;
}
void cutimag(Mat src_img,int m,int n,Vector<Mat> ceil_img,int Class,int Num)
{  
  
   // int t = m * n; 
	int tem=0;
    int height = src_img.rows;  
    int width  = src_img.cols;  
	cout<<height<<' '<<width<<endl;
    int ceil_height=m;// = height/m;  
    int ceil_width=n;//  = width/n;   
    int ceil_num1= height/(m);
	int ceil_num2= width/(n); 
    Mat roi_img,tmp_img;  
  
    Point p1,p2;  

    for(int i = 0;i<ceil_num1;i++) 
	{
        for(int j = 0;j<ceil_num2;j++)
		{  
            //p1 =  
			
			if(i==0&&j==0)
			{
				Rect rect(j*ceil_width,i*ceil_height,ceil_width,ceil_height);
				 src_img(rect).copyTo(roi_img);
			}
			if(i==0&&j!=0)
			{
				Rect rect(j*ceil_width,i*ceil_height,ceil_width,ceil_height);
				 src_img(rect).copyTo(roi_img); 
			}
			if(j==0&&i!=0)
			{
				Rect rect(j*ceil_width,i*ceil_height,ceil_width,ceil_height);
				 src_img(rect).copyTo(roi_img); 
			}
            if(j!=0&&i!=0)
			{
				Rect rect(j*ceil_width,i*ceil_height,ceil_width,ceil_height);
				 src_img(rect).copyTo(roi_img);
			}
            ceil_img.push_back(roi_img);
			sprintf(file_out, "%s%d\\%d\\%d.jpg", file_feature_out,Class,Num,tem);
			sprintf(file_out2, "md %s%d\\%d", file_feature_out,Class,Num);
			system(file_out2);
			IplImage *src=NULL;
            src=&IplImage(roi_img);
			cvSaveImage(file_out, src); 
			tem++;
            waitKey(0);
        } 
	}
}