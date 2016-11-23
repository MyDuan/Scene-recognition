// LBP.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <fstream>
using namespace std;
using namespace cv;
char file[255];
char file_out[255];
char file_out2[255];
char file_path_e[400] = ".\\patch\\";
char file_path_e4[400] = ".\\patch_4\\";
char file_path_e16[400] = ".\\patch_16\\";
//char file_path_e1[400] = ".\\testdata\\";
char file_feature_out[255]=".\\testf\\";
//char file_feature_out1[255]=".\\testf1\\";
char file_feature_out4[255]=".\\testf4\\";
char file_feature_out16[255]=".\\testf16\\";
float *feature_data;
int a=0,b=0,c=0,d=0;	
int m=1,n=256;
int _tmain(int argc, _TCHAR* argv[])
{
int tmp[8]={0};
int sum=0;
int k=0;
IplImage *src=NULL;

for(int Class=1;Class<7;Class++)
{
	for(int Number=1;Number<201;Number++)
	{
		int num=0;
		
		while(1)
		{
			float feature_patch[256]={0};
			sprintf(file, "%s%d\\%d\\%d.jpg", file_path_e, Class,Number, num);
			sprintf(file_out, "%s%d\\%d\\%d.txt", file_feature_out, Class, Number,num);
			sprintf(file_out2, "md %s%d\\%d", file_feature_out, Class,Number);
			system(file_out2);
			ofstream fout(file_out);
			if((src = cvLoadImage(file,1))!=0)
			{

				int lbp;
				
				//cvShowImage("img",src);
				//cvWaitKey(0);
				uchar* data=(uchar*)src->imageData;
				int step=src->widthStep;
				for(int i=1;i<src->height-1;i++)
				for(int j=1;j<src->width-1;j++)
				{
					if(data[(i-1)*step+j-1]>data[i*step+j])
						 tmp[0]=1;
					else
						 tmp[0]=0;
					if(data[i*step+(j-1)]>data[i*step+j])
						tmp[1]=1;
					else
						 tmp[1]=0;
					if(data[(i+1)*step+(j-1)]>data[i*step+j])
						 tmp[2]=1;
					else
						 tmp[2]=0;
					if (data[(i+1)*step+j]>data[i*step+j])
						 tmp[3]=1;
					else
						tmp[3]=0;
					if (data[(i+1)*step+(j+1)]>data[i*step+j])
						tmp[4]=1;
					else
						tmp[4]=0;
					if(data[i*step+(j+1)]>data[i*step+j])
						tmp[5]=1;
					else
						tmp[5]=0;
					if(data[(i-1)*step+(j+1)]>data[i*step+j])
						tmp[6]=1;
					else
						tmp[6]=0;
					if(data[(i-1)*step+j]>data[i*step+j])
						tmp[7]=1;
					else
						tmp[7]=0;
					lbp=(tmp[0]+tmp[1]*2+tmp[2]*4+tmp[3]*8+tmp[4]*16+tmp[5]*32+tmp[6]*64+tmp[7]*128);
					feature_patch[lbp]++;
				}
				int sum=0;
				for(int i=0;i<256;i++)
				{
					sum+=feature_patch[i];
				}
				fout<<m<<' '<<n<<endl;
				fout<<a<<' '<<b<<' '<<c<<' '<<d<<endl;
				for(int i=0;i<256;i++)
				{
					feature_patch[i]=(float)feature_patch[i]/sum;
					fout<<feature_patch[i]<<' ';
				}
				fout<<endl;
				//cout<<endl<<tem;
				num++;
			}
			else
			{
				break;
			}
		}
		
	}
}
///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
sum=0;
k=0;
for(int Class=1;Class<7;Class++)
{
	for(int Number=1;Number<201;Number++)
	{
		for(int i=0;i<16;i++)
		{
			int num=0;
		
			while(1)
			{
				float feature_patch[256]={0};
				sprintf(file, "%s%d\\%d\\%d\\%d.jpg", file_path_e16, Class,Number,i, num);
				sprintf(file_out, "%s%d\\%d\\%d\\%d.txt", file_feature_out16, Class, Number,i,num);
				sprintf(file_out2, "md %s%d\\%d\\%d", file_feature_out16, Class,Number,i);
				system(file_out2);
				ofstream fout(file_out);
				if((src = cvLoadImage(file,1))!=0)
				{

					int lbp;
				
					//cvShowImage("img",src);
					//cvWaitKey(0);
					uchar* data=(uchar*)src->imageData;
					int step=src->widthStep;
					for(int i=1;i<src->height-1;i++)
					for(int j=1;j<src->width-1;j++)
					{
						if(data[(i-1)*step+j-1]>data[i*step+j])
							tmp[0]=1;
						else
							tmp[0]=0;
						if(data[i*step+(j-1)]>data[i*step+j])
							tmp[1]=1;
						else
							tmp[1]=0;
						if(data[(i+1)*step+(j-1)]>data[i*step+j])
							tmp[2]=1;
						else
							tmp[2]=0;
						if (data[(i+1)*step+j]>data[i*step+j])
							tmp[3]=1;
						else
							tmp[3]=0;
						if (data[(i+1)*step+(j+1)]>data[i*step+j])
							tmp[4]=1;
						else
							tmp[4]=0;
						if(data[i*step+(j+1)]>data[i*step+j])
							tmp[5]=1;
						else
							tmp[5]=0;
						if(data[(i-1)*step+(j+1)]>data[i*step+j])
							tmp[6]=1;
						else
							tmp[6]=0;
						if(data[(i-1)*step+j]>data[i*step+j])
							tmp[7]=1;
						else
							tmp[7]=0;
						lbp=(tmp[0]+tmp[1]*2+tmp[2]*4+tmp[3]*8+tmp[4]*16+tmp[5]*32+tmp[6]*64+tmp[7]*128);
						feature_patch[lbp]++;
					}
					int sum=0;
					for(int i=0;i<256;i++)
					{
						sum+=feature_patch[i];
					}
					fout<<m<<' '<<n<<endl;
					fout<<a<<' '<<b<<' '<<c<<' '<<d<<endl;
					for(int i=0;i<256;i++)
					{
						feature_patch[i]=(float)feature_patch[i]/sum;
						fout<<feature_patch[i]<<' ';
					}
					fout<<endl;
					//cout<<endl<<tem;
					num++;
				}
				else
				{
					break;
				}
			}
		}
		
	}
}
///////////////////////////////////////////////////////////////////////////////////////////
sum=0;
k=0;
for(int Class=1;Class<7;Class++)
{
	for(int Number=1;Number<201;Number++)
	{
		for(int i=0;i<4;i++)
		{
			int num=0;
		
			while(1)
			{
				float feature_patch[256]={0};
				sprintf(file, "%s%d\\%d\\%d\\%d.jpg", file_path_e4, Class,Number,i, num);
				sprintf(file_out, "%s%d\\%d\\%d\\%d.txt", file_feature_out4, Class, Number,i,num);
				sprintf(file_out2, "md %s%d\\%d\\%d", file_feature_out4, Class,Number,i);
				system(file_out2);
				ofstream fout(file_out);
				if((src = cvLoadImage(file,1))!=0)
				{

					int lbp;
				
					//cvShowImage("img",src);
					//cvWaitKey(0);
					uchar* data=(uchar*)src->imageData;
					int step=src->widthStep;
					for(int i=1;i<src->height-1;i++)
					for(int j=1;j<src->width-1;j++)
					{
						if(data[(i-1)*step+j-1]>data[i*step+j])
							tmp[0]=1;
						else
							tmp[0]=0;
						if(data[i*step+(j-1)]>data[i*step+j])
							tmp[1]=1;
						else
							tmp[1]=0;
						if(data[(i+1)*step+(j-1)]>data[i*step+j])
							tmp[2]=1;
						else
							tmp[2]=0;
						if (data[(i+1)*step+j]>data[i*step+j])
							tmp[3]=1;
						else
							tmp[3]=0;
						if (data[(i+1)*step+(j+1)]>data[i*step+j])
							tmp[4]=1;
						else
							tmp[4]=0;
						if(data[i*step+(j+1)]>data[i*step+j])
							tmp[5]=1;
						else
							tmp[5]=0;
						if(data[(i-1)*step+(j+1)]>data[i*step+j])
							tmp[6]=1;
						else
							tmp[6]=0;
						if(data[(i-1)*step+j]>data[i*step+j])
							tmp[7]=1;
						else
							tmp[7]=0;
						lbp=(tmp[0]+tmp[1]*2+tmp[2]*4+tmp[3]*8+tmp[4]*16+tmp[5]*32+tmp[6]*64+tmp[7]*128);
						feature_patch[lbp]++;
					}
					int sum=0;
					for(int i=0;i<256;i++)
					{
						sum+=feature_patch[i];
					}
					fout<<m<<' '<<n<<endl;
					fout<<a<<' '<<b<<' '<<c<<' '<<d<<endl;
					for(int i=0;i<256;i++)
					{
						feature_patch[i]=(float)feature_patch[i]/sum;
						fout<<feature_patch[i]<<' ';
					}
					fout<<endl;
					//cout<<endl<<tem;
					num++;
				}
				else
				{
					break;
				}
			}
		}
		
	}
}
//cvShowImage("dst",dst);
//cvWaitKey(-1);
cvReleaseImage(&src);
//cvReleaseImage(&dst);
cvDestroyWindow("img");
//cvDestroyWindow("dst");

	return 0;
}

