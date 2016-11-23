// HOG.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <fstream>
using namespace std;
using namespace cv;
#define PI 3.14
char file[255];
char file_out[255];
char file_out2[255];
char file_path_e[400] = ".\\patch\\";
char file_path_e4[400] = ".\\patch4\\";
char file_path_e16[400] = ".\\patch16\\";
char file_feature_out[255]=".\\testf\\";
char file_feature_out4[255]=".\\testf4\\";
char file_feature_out16[255]=".\\testf16\\";
float *feature_data;
int a=0,b=0,c=0,d=0;	
int m=1,n=9;
int _tmain(int argc, _TCHAR* argv[])
{

	IplImage *src = NULL;  // source imagre
   // IplImage *histimg = 0; // histogram image 
   // CvHistogram *hist = 0; // define multi_demention histogram
   // IplImage* canny;
    CvMat* canny_m;
   // IplImage* dx=NULL; // the sobel x difference 
   // IplImage* dy=NULL; // the sobel y difference 
	
    CvMat* gradient; // value of gradient
    CvMat* gradient_dir; // direction of gradient
    CvMat* dx_m; // format transform to matrix
    CvMat* dy_m;
    CvMat* mask;
    CvSize  size;
   // IplImage* gradient_im;
    int i,j;
    float theta;       //方向
    int hdims = 9;     // 划分HIST的个数，越高越精确
    float hranges_arr[] = {-PI/2,PI/2}; // 直方图的上界和下界
    float* hranges = hranges_arr;
                                                                                                                                                                                                                                                              
    float max_val;  // 
    int bin_w;
    for(int Class=1;Class<14;Class++)
	{
		for(int Number=1;Number<201;Number++)
		{
			int num=0;
			while(1)
			{
				IplImage* dx=NULL; // the sobel x difference 
				IplImage* dy=NULL; // the sobel y difference 
				IplImage *histimg = 0; // histogram image 
				IplImage* gradient_im;
				CvHistogram *hist = 0; // define multi_demention histogram
				IplImage* canny;
				float feature_patch[9]={0};
				sprintf(file, "%s%d\\%d\\%d.jpg", file_path_e, Class,Number, num);
				sprintf(file_out, "%s%d\\%d\\%d.txt", file_feature_out, Class, Number,num);
				sprintf(file_out2, "md %s%d\\%d", file_feature_out, Class,Number);
				system(file_out2);
				ofstream fout(file_out);
				if((src = cvLoadImage(file,CV_LOAD_IMAGE_GRAYSCALE))!=0)
				{
					size=cvGetSize(src);
					canny=cvCreateImage(cvGetSize(src),8,1);//边缘图像
					dx=cvCreateImage(cvGetSize(src),32,1);//x方向上的差分//此处的数据类型为U 不怕溢出吗？
					dy=cvCreateImage(cvGetSize(src),32,1);
					gradient_im=cvCreateImage(cvGetSize(src),32,1);//梯度图像
					canny_m=cvCreateMat(size.height,size.width,CV_32FC1);//边缘矩阵
					dx_m=cvCreateMat(size.height,size.width,CV_32FC1);
					dy_m=cvCreateMat(size.height,size.width,CV_32FC1);
					gradient=cvCreateMat(size.height,size.width,CV_32FC1);//梯度矩阵
					gradient_dir=cvCreateMat(size.height,size.width,CV_32FC1);//梯度方向矩阵
					mask=cvCreateMat(size.height,size.width,CV_32FC1);//掩码
					cvConvert(canny,canny_m);//把图像转换为矩阵
					cvSobel(src,dx,1,0,3);// 一阶X方向的图像差分:dx
					cvSobel(src,dy,0,1,3);// 一阶Y方向的图像差分:dy
					cvConvert(dx,dx_m);
					cvConvert(dy,dy_m);
					//cvAdd(dx_m, dy_m,gradient); // value of gradient//梯度不是等于根号下x的导数的平方加上y导数的平方吗
					cvDiv(dx_m,dy_m,gradient_dir); // direction
					for(i=0;i<size.height;i++)
					for(j=0;j<size.width;j++)
					{
						if(cvmGet(canny_m,i,j)!=0 && cvmGet(gradient,i,j)!=0)//此行是什么意思？只看边缘上的方向？
						{
							theta=cvmGet(gradient_dir,i,j);
							theta=atan(theta);
							cvmSet(gradient_dir,i,j,theta);//给梯度方向矩阵逐点赋值  
						}
						else
						{
							cvmSet(gradient_dir,i,j,0);
						}    
					}
					hist = cvCreateHist( 1, &hdims, CV_HIST_ARRAY, &hranges, 1 );  
					// 创建一个指定尺寸的直方图，并返回创建的直方图指针
					histimg = cvCreateImage( cvSize(320,200), 8, 3 ); // 创建一个图像，通道
					cvZero( histimg ); // 清；
					cvConvert(gradient_dir,gradient_im);//把梯度方向矩阵转化为图像
   
					cvCalcHist( &gradient_im, hist, 0,canny); // 计算直方图
					cvGetMinMaxHistValue( hist, 0, &max_val, 0, 0 );  // 只找最大值
					cvConvertScale( hist->bins, hist->bins, max_val ? 255. / max_val : 0., 0 ); 
					// 缩放bin 到区间[0,255] ，比例系数
					cvZero( histimg );
					bin_w = histimg->width /16;  // hdims: 条的个数，则bin_w 为条的宽度
					// 画直方图
					fout<<m<<' '<<n<<endl;
					fout<<a<<' '<<b<<' '<<c<<' '<<d<<endl;
					double sum=0;
					for( i = 0; i < hdims; i++ )
					{
						double val = ( cvGetReal1D(hist->bins,i)*histimg->height/255 );
						sum+=val;
					}
					for( i = 0; i < hdims; i++ )
					{
						double val = ( cvGetReal1D(hist->bins,i)*histimg->height/255 )/sum;
						fout<<val<<' ';
					}
					fout<<endl;
					num++;
					cvReleaseHist ( &hist );
					cvReleaseImage( &dx );
					cvReleaseImage( &dy );
					cvReleaseImage( &histimg );
					cvReleaseImage( &gradient_im );
					cvReleaseImage( &canny );	
				}
				else
				{
					break;
				}
			}
		}
	}
	///////////////////////////////////////////////////////////////////////
	for(int Class=1;Class<14;Class++)
	{
		for(int Number=1;Number<201;Number++)
		{
			for(int ii=0;ii<16;ii++)
			{
				int num=0;
				while(1)
				{
					IplImage* dx=NULL; // the sobel x difference 
					IplImage* dy=NULL; // the sobel y difference 
					IplImage *histimg = 0; // histogram image 
					IplImage* gradient_im;
					CvHistogram *hist = 0; // define multi_demention histogram
					IplImage* canny;
					float feature_patch[9]={0};
					sprintf(file, "%s%d\\%d\\%d\\%d.jpg", file_path_e16, Class,Number,ii, num);
					sprintf(file_out, "%s%d\\%d\\%d\\%d.txt", file_feature_out16, Class, Number,ii,num);
					sprintf(file_out2, "md %s%d\\%d\\%d", file_feature_out16, Class,Number,ii);
					system(file_out2);
					ofstream fout(file_out);
					//ofstream fout(file_out);
					if((src = cvLoadImage(file,CV_LOAD_IMAGE_GRAYSCALE))!=0)
					{
						size=cvGetSize(src);
						canny=cvCreateImage(cvGetSize(src),8,1);//边缘图像
						dx=cvCreateImage(cvGetSize(src),32,1);//x方向上的差分//此处的数据类型为U 不怕溢出吗？
						dy=cvCreateImage(cvGetSize(src),32,1);
						gradient_im=cvCreateImage(cvGetSize(src),32,1);//梯度图像
						canny_m=cvCreateMat(size.height,size.width,CV_32FC1);//边缘矩阵
						dx_m=cvCreateMat(size.height,size.width,CV_32FC1);
						dy_m=cvCreateMat(size.height,size.width,CV_32FC1);
						gradient=cvCreateMat(size.height,size.width,CV_32FC1);//梯度矩阵
						gradient_dir=cvCreateMat(size.height,size.width,CV_32FC1);//梯度方向矩阵
						mask=cvCreateMat(size.height,size.width,CV_32FC1);//掩码
						cvConvert(canny,canny_m);//把图像转换为矩阵
						cvSobel(src,dx,1,0,3);// 一阶X方向的图像差分:dx
						cvSobel(src,dy,0,1,3);// 一阶Y方向的图像差分:dy
						cvConvert(dx,dx_m);
						cvConvert(dy,dy_m);
						//cvAdd(dx_m, dy_m,gradient); // value of gradient//梯度不是等于根号下x的导数的平方加上y导数的平方吗
						cvDiv(dx_m,dy_m,gradient_dir); // direction
						for(i=0;i<size.height;i++)
						for(j=0;j<size.width;j++)
						{
							if(cvmGet(canny_m,i,j)!=0 && cvmGet(gradient,i,j)!=0)//此行是什么意思？只看边缘上的方向？
							{
								theta=cvmGet(gradient_dir,i,j);
								theta=atan(theta);
								cvmSet(gradient_dir,i,j,theta);//给梯度方向矩阵逐点赋值  
							}
							else
							{
								cvmSet(gradient_dir,i,j,0);
							}    
						}
						hist = cvCreateHist( 1, &hdims, CV_HIST_ARRAY, &hranges, 1 );  
						// 创建一个指定尺寸的直方图，并返回创建的直方图指针
						histimg = cvCreateImage( cvSize(320,200), 8, 3 ); // 创建一个图像，通道
						cvZero( histimg ); // 清；
						cvConvert(gradient_dir,gradient_im);//把梯度方向矩阵转化为图像
   
						cvCalcHist( &gradient_im, hist, 0,canny); // 计算直方图
						cvGetMinMaxHistValue( hist, 0, &max_val, 0, 0 );  // 只找最大值
						cvConvertScale( hist->bins, hist->bins, max_val ? 255. / max_val : 0., 0 ); 
						// 缩放bin 到区间[0,255] ，比例系数
						cvZero( histimg );
						bin_w = histimg->width /16;  // hdims: 条的个数，则bin_w 为条的宽度
						// 画直方图
						fout<<m<<' '<<n<<endl;
						fout<<a<<' '<<b<<' '<<c<<' '<<d<<endl;
						double sum=0;
						for( i = 0; i < hdims; i++ )
						{
							double val = ( cvGetReal1D(hist->bins,i)*histimg->height/255 );
							sum+=val;
						}
						for( i = 0; i < hdims; i++ )
						{
							double val = ( cvGetReal1D(hist->bins,i)*histimg->height/255 )/sum;
							fout<<val<<' ';
						}
						fout<<endl;
						num++;
						cvReleaseHist ( &hist );
						cvReleaseImage( &dx );
						cvReleaseImage( &dy );
						cvReleaseImage( &histimg );
						cvReleaseImage( &gradient_im );
						cvReleaseImage( &canny );	
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	///////////////////////////////////////////////////////////////////////
	for(int Class=1;Class<14;Class++)
	{
		for(int Number=1;Number<201;Number++)
		{
			for(int ii=0;ii<4;ii++)
			{
				int num=0;
				while(1)
				{
					IplImage* dx=NULL; // the sobel x difference 
					IplImage* dy=NULL; // the sobel y difference 
					IplImage *histimg = 0; // histogram image 
					IplImage* gradient_im;
					CvHistogram *hist = 0; // define multi_demention histogram
					IplImage* canny;
					float feature_patch[9]={0};
					sprintf(file, "%s%d\\%d\\%d\\%d.jpg", file_path_e4, Class,Number,ii,num);
					sprintf(file_out, "%s%d\\%d\\%d\\%d.txt", file_feature_out4, Class, Number,ii,num);
					sprintf(file_out2, "md %s%d\\%d\\%d", file_feature_out4, Class,Number,ii);
					system(file_out2);
					ofstream fout(file_out);
					//ofstream fout(file_out);
					if((src = cvLoadImage(file,CV_LOAD_IMAGE_GRAYSCALE))!=0)
					{
						size=cvGetSize(src);
						canny=cvCreateImage(cvGetSize(src),8,1);//边缘图像
						dx=cvCreateImage(cvGetSize(src),32,1);//x方向上的差分//此处的数据类型为U 不怕溢出吗？
						dy=cvCreateImage(cvGetSize(src),32,1);
						gradient_im=cvCreateImage(cvGetSize(src),32,1);//梯度图像
						canny_m=cvCreateMat(size.height,size.width,CV_32FC1);//边缘矩阵
						dx_m=cvCreateMat(size.height,size.width,CV_32FC1);
						dy_m=cvCreateMat(size.height,size.width,CV_32FC1);
						gradient=cvCreateMat(size.height,size.width,CV_32FC1);//梯度矩阵
						gradient_dir=cvCreateMat(size.height,size.width,CV_32FC1);//梯度方向矩阵
						mask=cvCreateMat(size.height,size.width,CV_32FC1);//掩码
						cvConvert(canny,canny_m);//把图像转换为矩阵
						cvSobel(src,dx,1,0,3);// 一阶X方向的图像差分:dx
						cvSobel(src,dy,0,1,3);// 一阶Y方向的图像差分:dy
						cvConvert(dx,dx_m);
						cvConvert(dy,dy_m);
						//cvAdd(dx_m, dy_m,gradient); // value of gradient//梯度不是等于根号下x的导数的平方加上y导数的平方吗
						cvDiv(dx_m,dy_m,gradient_dir); // direction
						for(i=0;i<size.height;i++)
						for(j=0;j<size.width;j++)
						{
							if(cvmGet(canny_m,i,j)!=0 && cvmGet(gradient,i,j)!=0)//此行是什么意思？只看边缘上的方向？
							{
								theta=cvmGet(gradient_dir,i,j);
								theta=atan(theta);
								cvmSet(gradient_dir,i,j,theta);//给梯度方向矩阵逐点赋值  
							}
							else
							{
								cvmSet(gradient_dir,i,j,0);
							}    
						}
						hist = cvCreateHist( 1, &hdims, CV_HIST_ARRAY, &hranges, 1 );  
						// 创建一个指定尺寸的直方图，并返回创建的直方图指针
						histimg = cvCreateImage( cvSize(320,200), 8, 3 ); // 创建一个图像，通道
						cvZero( histimg ); // 清；
						cvConvert(gradient_dir,gradient_im);//把梯度方向矩阵转化为图像
   
						cvCalcHist( &gradient_im, hist, 0,canny); // 计算直方图
						cvGetMinMaxHistValue( hist, 0, &max_val, 0, 0 );  // 只找最大值
						cvConvertScale( hist->bins, hist->bins, max_val ? 255. / max_val : 0., 0 ); 
						// 缩放bin 到区间[0,255] ，比例系数
						cvZero( histimg );
						bin_w = histimg->width /16;  // hdims: 条的个数，则bin_w 为条的宽度
						// 画直方图
						fout<<m<<' '<<n<<endl;
						fout<<a<<' '<<b<<' '<<c<<' '<<d<<endl;
						double sum=0;
						for( i = 0; i < hdims; i++ )
						{
							double val = ( cvGetReal1D(hist->bins,i)*histimg->height/255 );
							sum+=val;
						}
						for( i = 0; i < hdims; i++ )
						{
							double val = ( cvGetReal1D(hist->bins,i)*histimg->height/255 )/sum;
							fout<<val<<' ';
						}
						fout<<endl;
						num++;
						cvReleaseHist ( &hist );
						cvReleaseImage( &dx );
						cvReleaseImage( &dy );
						cvReleaseImage( &histimg );
						cvReleaseImage( &gradient_im );
						cvReleaseImage( &canny );	
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	////////////////////////////////////////////////////////////////////////
    cvDestroyWindow("src");
    cvDestroyWindow("Histogram");
    cvReleaseImage( &src );
   // cvReleaseImage( &histimg );
   // cvReleaseHist ( &hist );
	system("pause");
    return 0;
}

