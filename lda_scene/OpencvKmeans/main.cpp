#include <cv.h>
#include <highgui.h>
#include <cxcore.h>

#include <iostream>
#include<fstream>
using namespace std;
#include "variable.h"
char file_feature_in[281]=".\\testf\\";
//char file_feature_in1[281]=".\\testf1\\";
char file_feature_in4[281]=".\\testf4\\";
char file_feature_in16[281]=".\\testf16\\";
//char feature_num[281]=".\\num\\";
char file_in[255];
char file_out[255];
/*#define CV_IMAGE_ELEM(image,elemtype,row,col)\
	(((elemtype*)((image)->imageData+(image)->widthStep*(row)))[(col)])*/
float Distance(float img_1[9], float img_2[9])
{
	float dis=0;
	for(int i=0;i<9;i++)
	{
		if(img_1[i]>img_2[i])
		{
			dis+=(img_1[i]-img_2[i])*(img_1[i]-img_2[i]);
		}
		else
		{
			dis+=(img_2[i]-img_1[i])*(img_2[i]-img_1[i]);
		}
	}
	return dis;
}
/***************************************************************
 *
 *	函数名称：main
 *	函数功能：主函数
 ***************************************************************/
int main()
{
	ifstream inf("col.txt");
	/////////////////////////////////////////////////////
	ofstream onf("sampleword.txt");
	ofstream onf2("testword.txt");
	ofstream onf_feature1("samplefeature.txt");
	ofstream onf_feature2("testfeature.txt");
	////////////////////////////////////////////////////
	ofstream onf_4("sampleword4.txt");
	ofstream onf2_4("testword4.txt");
	ofstream onf_feature1_4("samplefeature4.txt");
	ofstream onf_feature2_4("testfeature4.txt");
	/////////////////////////////////////////////////////
	ofstream onf_16("sampleword16.txt");
	ofstream onf2_16("testword16.txt");
	ofstream onf_feature1_16("samplefeature16.txt");
	ofstream onf_feature2_16("testfeature16.txt");
	//////////////////////////////////////////////////////
	int count;
	int a,b;
	double x,y,orl,src;
	inf>>count;
	int count_i=0;
	//vector<vector<float> > feature;//(a, vector<float>(b));
	float** feature = new float* [count];
	for (int i=0; i<count; i++)
		feature[i] = new float[9];
    for(int Class=1;Class<10;Class++)
	{
		for(int Number=1;Number<51;Number++ )
		{
			int tem=0;
			while(1)
			{
				sprintf(file_in, "%s%d\\%d\\%d.txt", file_feature_in, Class, Number,tem);
				ifstream infile(file_in);
				if(infile.fail())
				{
					cout<<"a image feature is over!"<<endl;
					break;
				}
				else
				{
					infile>>a>>b;
					for(int i=0;i<a;i++)
					{
						infile>>x>>y>>orl>>src;
						for(int j=0;j<9;j++)
						{
							//infile>>sample;
							infile>>feature[count_i][j];//=sample;
							//cout<<feature[i][j];
						}
						count_i++;
					}
					tem++;
				}
			}
			cout<<"读完一张图片!"<<endl;
		}
		cout<<"读完一类图像！"<<endl;
	}
	cout<<count_i<<endl;
	//利用Kmeans来完成特征的聚类，形成单词表，单词表为每一聚类的中心（一个9维的向量）
  	CvMat  *samples=cvCreateMat(count_i, 9, CV_32FC1);
	//samples=&cvMat(col, 9, CV_32FC1,feature);
	CvMat *centers=cvCreateMat(80, 9, CV_32FC1);
	CvMat *label=cvCreateMat(count_i, 1, CV_32SC1);
	#define ImMat(ROW,COL) ((float *)(samples->data.fl+samples->step/sizeof(float)*(ROW)))[(COL)]
//	#define ImMat_id(ROW,COL) ((int *)(label->data.fl+label->step/sizeof(int)*(ROW)))[(COL)]
	#define ImMat_center(ROW,COL) ((float *)(centers->data.fl+centers->step/sizeof(float)*(ROW)))[(COL)]
	int temp=0;
	for(int rows=0;rows<count_i;rows++)
	{
		for(int cols=0;cols<9;cols++)
		{
			//ImMat(rows,cols)=feature[rows][cols];
			samples->data.fl[temp++]=feature[rows][cols];
		}
	}
	//cvKMeans2(samples,25,label,cvTermCriteria( CV_TERMCRIT_EPS, 10, 1.0 ),3, (CvRNG *)0,1,centers);
	//KMEANS_RANDOM_CENTERS
	cvKMeans2(samples,80,label,cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0 ),3, (CvRNG *)0,2,centers);
	ofstream fcenter("center.txt");
		
	float Center_sample[80][9];
	//int tem=0;
	for(int i=0;i<80;i++)
	{
		//fcenter<<"Number"<<i+1<<":"<<endl;
		for(int j=0;j<9;j++)
		{
			Center_sample[i][j]=ImMat_center(i,j);//centers->data.fl[tem++];//ImMat_center(i,j);
			fcenter<<ImMat_center(i,j)<<"  ";
		}
		fcenter<<endl;
	}
	cout<<"center Over"<<endl;
	/////////////////////////////////////////////////
	//SPM方法来完成词典特征:
	//80个中心，则将三种不同层次的图片合起来得到的特征向量维数是80*（1+4+16）
	//依然分为训练样本和测试样本分别统计
	/////////////////////////////////////////////////
	//统计训练样本词频
	for(int Class=1;Class<10;Class++)
	{
		for(int Number=1;Number<101;Number++ )
		{
			int c[80]={0};//记录每个数字有多少个
			int tem=0;
			//sprintf(file_out, "%s%d\\%d.txt", feature_num, Class, Number);
			
			while(1)
			{
				sprintf(file_in, "%s%d\\%d\\%d.txt", file_feature_in, Class, Number,tem);
				ifstream infile(file_in);
				if(infile.fail())
				{
					cout<<"read feature is over";
					break;
				}
				else
				{
					
					infile>>a>>b;
					float** feature_every = new float* [a];
					for (int i=0; i<a; i++)
						feature_every[i] = new float[9];
					for(int i=0;i<a;i++)
					{
						infile>>x>>y>>orl>>src;
						for(int j=0;j<9;j++)
						{
							infile>>feature_every[i][j];
						}
						float dismin=Distance(Center_sample[0], feature_every[i]);
						long feature_N=0;
						float distance=0;
						for(int words_num=0;words_num<80;words_num++)//计算每一个feature与每一个单词的距离，选择距离最短的
						{
							distance=Distance(Center_sample[words_num], feature_every[i]);
							if(distance<dismin)
							{
								dismin=distance;
								feature_N=words_num;
							}
						}
						c[feature_N]++;
						//featurenum<<feature_N<<endl;
					//cout<<feature_N<<"	";
					}
					tem++;
				}

			}
			
			int L=0;
			for(int k=0;k<80;k++)
			{
				if(c[k]!=0)
				{
					L++;
				}
			}
			onf<<L<<' ';
			for(int k=0;k<80;k++)
			{
				onf_feature1<<c[k]<<' ';
				//ofstream onf_feature2("testfeature.txt");
				if(c[k]!=0)
				{
					onf<<k<<":"<<c[k]<<' ';
				}
			}
			onf<<endl;
			onf_feature1<<endl;
		}
	}
	
/////////////////////////////////////////////////////////////////////////////
	for(int Class=1;Class<10;Class++)
	{
		for(int Number=1;Number<101;Number++ )
		{
			
			//sprintf(file_out, "%s%d\\%d.txt", feature_num, Class, Number);
			for(int ii=0;ii<4;ii++)
			{
				int c[80]={0};//记录每个数字有多少个
				int tem=0;
				while(1)
				{
					sprintf(file_in, "%s%d\\%d\\%d\\%d.txt", file_feature_in4, Class, Number,ii,tem);
					ifstream infile(file_in);
					if(infile.fail())
					{
						cout<<"read feature is over";
						break;
					}
					else
					{
					
						infile>>a>>b;
						float** feature_every = new float* [a];
						for (int i=0; i<a; i++)
							feature_every[i] = new float[9];
						for(int i=0;i<a;i++)
						{
							infile>>x>>y>>orl>>src;
							for(int j=0;j<9;j++)
							{
								infile>>feature_every[i][j];
							}
							float dismin=Distance(Center_sample[0], feature_every[i]);
							long feature_N=0;
							float distance=0;
							for(int words_num=0;words_num<80;words_num++)//计算每一个feature与每一个单词的距离，选择距离最短的
							{
								distance=Distance(Center_sample[words_num], feature_every[i]);
								if(distance<dismin)
								{
									dismin=distance;
									feature_N=words_num;
								}
							}
							c[feature_N]++;
							//featurenum<<feature_N<<endl;
							//cout<<feature_N<<"	";
						}
						tem++;
					}
				}

			
			
				int L=0;
				for(int k=0;k<80;k++)
				{
					if(c[k]!=0)
					{
						L++;
					}
				}
				onf_4<<L<<' ';
				for(int k=0;k<80;k++)
				{
					onf_feature1_4<<c[k]<<' ';
					//ofstream onf_feature2("testfeature.txt");
					if(c[k]!=0)
					{
						onf_4<<k<<":"<<c[k]<<' ';
					}
				}
				onf_4<<endl;
				onf_feature1_4<<endl;
			}
		}
	}
///////////////////////////////////////////////////////////////////////////////
	for(int Class=1;Class<10;Class++)
	{
		for(int Number=1;Number<101;Number++ )
		{
			
			//sprintf(file_out, "%s%d\\%d.txt", feature_num, Class, Number);
			for(int ii=0;ii<16;ii++)
			{
				int c[80]={0};//记录每个数字有多少个
				int tem=0;
				while(1)
				{
					sprintf(file_in, "%s%d\\%d\\%d\\%d.txt", file_feature_in16, Class, Number,ii,tem);
					ifstream infile(file_in);
					if(infile.fail())
					{
						cout<<"read feature is over";
						break;
					}
					else
					{
					
						infile>>a>>b;
						float** feature_every = new float* [a];
						for (int i=0; i<a; i++)
							feature_every[i] = new float[9];
						for(int i=0;i<a;i++)
						{
							infile>>x>>y>>orl>>src;
							for(int j=0;j<9;j++)
							{
								infile>>feature_every[i][j];
							}
							float dismin=Distance(Center_sample[0], feature_every[i]);
							long feature_N=0;
							float distance=0;
							for(int words_num=0;words_num<80;words_num++)//计算每一个feature与每一个单词的距离，选择距离最短的
							{
								distance=Distance(Center_sample[words_num], feature_every[i]);
								if(distance<dismin)
								{
									dismin=distance;
									feature_N=words_num;
								}
							}
							c[feature_N]++;
							//featurenum<<feature_N<<endl;
							//cout<<feature_N<<"	";
						}
						tem++;
					}
				}

			
			
				int L=0;
				for(int k=0;k<80;k++)
				{
					if(c[k]!=0)
					{
						L++;
					}
				}
				onf_16<<L<<' ';
				for(int k=0;k<80;k++)
				{
					onf_feature1_16<<c[k]<<' ';
					//ofstream onf_feature2("testfeature.txt");
					if(c[k]!=0)
					{
						onf_16<<k<<":"<<c[k]<<' ';
					}
				}
				onf_16<<endl;
				onf_feature1_16<<endl;
			}
		}
	}
////////////////////////////////////////////////////////////////
	cout<<"sample words is over"<<endl;

///////////////////////////////////////////////////////////////////
	/*
	//统计测试样本词频
	*/
////////////////////////////////////////////////////////////////////
	for(int Class=1;Class<10;Class++)
	{
		for(int Number=101;Number<201;Number++ )
		{
			int c[80]={0};//记录每个数字有多少个
			int tem=0;
			//sprintf(file_out, "%s%d\\%d.txt", feature_num, Class, Number);
				while(1)
				{
					sprintf(file_in, "%s%d\\%d\\%d.txt", file_feature_in, Class, Number,tem);
					ifstream infile(file_in);
					if(infile.fail())
					{
						cout<<"read feature is over";
						break;
					}
					else
					{
						infile>>a>>b;
						float** feature_every = new float* [a];
						for (int i=0; i<a; i++)
							feature_every[i] = new float[9];
						for(int i=0;i<a;i++)
						{
							infile>>x>>y>>orl>>src;
							for(int j=0;j<9;j++)
							{
								infile>>feature_every[i][j];
							}
							float dismin=Distance(Center_sample[0], feature_every[i]);
							int feature_N=0;
							float distance=0;
							for(int words_num=0;words_num<80;words_num++)//计算每一个feature与每一个单词的距离，选择距离最短的
							{
								distance=Distance(Center_sample[words_num], feature_every[i]);
								if(distance<dismin)
								{
									dismin=distance;
									feature_N=words_num;
								}
							}
							c[feature_N]++;
							//featurenum<<feature_N<<endl;
							//cout<<feature_N<<"	";
						}
						tem++;
					}

				}
			
				int L=0;
				for(int k=0;k<80;k++)
				{
					if(c[k]!=0)
					{
						L++;
					}
				}
				onf2<<L<<' ';
				for(int k=0;k<80;k++)
				{
					onf_feature2<<c[k]<<' ';
					if(c[k]!=0)
					{
						onf2<<k<<":"<<c[k]<<' ';
					}
				}
				onf2<<endl;
				onf_feature2<<endl;
			
		}
	}
/////////////////////////////////////////////////////////////////////	
	for(int Class=1;Class<10;Class++)
	{
		for(int Number=101;Number<201;Number++ )
		{
			
			//sprintf(file_out, "%s%d\\%d.txt", feature_num, Class, Number);
			for(int ii=0;ii<16;ii++)
			{
				int c[80]={0};//记录每个数字有多少个
				int tem=0;
				while(1)
				{
					sprintf(file_in, "%s%d\\%d\\%d\\%d.txt", file_feature_in16, Class, Number,ii,tem);
					ifstream infile(file_in);
					if(infile.fail())
					{
						cout<<"read feature is over";
						break;
					}
					else
					{
						infile>>a>>b;
						float** feature_every = new float* [a];
						for (int i=0; i<a; i++)
							feature_every[i] = new float[9];
						for(int i=0;i<a;i++)
						{
							infile>>x>>y>>orl>>src;
							for(int j=0;j<9;j++)
							{
								infile>>feature_every[i][j];
							}
							float dismin=Distance(Center_sample[0], feature_every[i]);
							int feature_N=0;
							float distance=0;
							for(int words_num=0;words_num<80;words_num++)//计算每一个feature与每一个单词的距离，选择距离最短的
							{
								distance=Distance(Center_sample[words_num], feature_every[i]);
								if(distance<dismin)
								{
									dismin=distance;
									feature_N=words_num;
								}
							}
							c[feature_N]++;
							//featurenum<<feature_N<<endl;
							//cout<<feature_N<<"	";
						}
						tem++;
					}

				}
			
				int L=0;
				for(int k=0;k<80;k++)
				{
					if(c[k]!=0)
					{
						L++;
					}
				}
				onf2_16<<L<<' ';
				for(int k=0;k<80;k++)
				{
					onf_feature2_16<<c[k]<<' ';
					if(c[k]!=0)
					{
						onf2_16<<k<<":"<<c[k]<<' ';
					}
				}
				onf2_16<<endl;
				onf_feature2_16<<endl;
			}
		}
	}
///////////////////////////////////////////////////////////////////////////////
	for(int Class=1;Class<10;Class++)
	{
		for(int Number=101;Number<201;Number++ )
		{
			
			//sprintf(file_out, "%s%d\\%d.txt", feature_num, Class, Number);
			for(int ii=0;ii<4;ii++)
			{
				int c[80]={0};//记录每个数字有多少个
				int tem=0;
				while(1)
				{
					sprintf(file_in, "%s%d\\%d\\%d\\%d.txt", file_feature_in4, Class, Number,ii,tem);
					ifstream infile(file_in);
					if(infile.fail())
					{
						cout<<"read feature is over";
						break;
					}
					else
					{
						infile>>a>>b;
						float** feature_every = new float* [a];
						for (int i=0; i<a; i++)
							feature_every[i] = new float[9];
						for(int i=0;i<a;i++)
						{
							infile>>x>>y>>orl>>src;
							for(int j=0;j<9;j++)
							{
								infile>>feature_every[i][j];
							}
							float dismin=Distance(Center_sample[0], feature_every[i]);
							int feature_N=0;
							float distance=0;
							for(int words_num=0;words_num<80;words_num++)//计算每一个feature与每一个单词的距离，选择距离最短的
							{
								distance=Distance(Center_sample[words_num], feature_every[i]);
								if(distance<dismin)
								{
									dismin=distance;
									feature_N=words_num;
								}
							}
							c[feature_N]++;
							//featurenum<<feature_N<<endl;
							//cout<<feature_N<<"	";
						}
						tem++;
					}

				}
			
				int L=0;
				for(int k=0;k<80;k++)
				{
					if(c[k]!=0)
					{
						L++;
					}
				}
				onf2_4<<L<<' ';
				for(int k=0;k<80;k++)
				{
					onf_feature2_4<<c[k]<<' ';
					if(c[k]!=0)
					{
						onf2_4<<k<<":"<<c[k]<<' ';
					}
				}
				onf2_4<<endl;
				onf_feature2_4<<endl;
			}
		}
	}
///////////////////////////////////////////////////////////////////////////////////
	ifstream Inff("sampleword.txt");
	ifstream Inf2("testword.txt");
	ifstream Inf_feature1("samplefeature.txt");
	ifstream Inf_feature2("testfeature.txt");
	////////////////////////////////////////////////////
	ifstream Inf_4("sampleword4.txt");
	ifstream Inf2_4("testword4.txt");
	ifstream Inf_feature1_4("samplefeature4.txt");
	ifstream Inf_feature2_4("testfeature4.txt");
	/////////////////////////////////////////////////////
	ifstream Inf_16("sampleword16.txt");
	ifstream Inf2_16("testword16.txt");
	ifstream Inf_feature1_16("samplefeature16.txt");
	ifstream Inf_feature2_16("testfeature16.txt");
	///////////////////////////////////////////////////
	ofstream sampleWorlsAll("sampleWorlsAll.txt");
	ofstream sampleFeatureAll("sampleFeatureAll.txt");
	ofstream testWorlsAll("testWorlsAll.txt");
	ofstream testFeatureAll("testFeatureAll.txt");
	short featureAll[900][1680];
	short testALL[900][1680];
	int zero_sample[900]={0};
	int zero_test[900]={0};
	for(int Sample=0;Sample<900;Sample++)
	{
		int T=0;
		for(int num=0;num<80;num++)
		{
			Inf_feature1>>featureAll[Sample][T]; 
			Inf_feature2>>testALL[Sample][T];
			if(featureAll[Sample][T]!=0)
			{
				zero_sample[Sample]++;
			}
			if(testALL[Sample][T]!=0)
			{
				zero_test[Sample]++;
			}
			T++;
		}
		for(int i=0;i<4;i++)
		{
			for(int num=0;num<80;num++)
			{
				Inf_feature1_4>>featureAll[Sample][T];
				Inf_feature2_4>>testALL[Sample][T];
				if(featureAll[Sample][T]!=0)
				{
					zero_sample[Sample]++;
				}
				if(testALL[Sample][T]!=0)
				{
					zero_test[Sample]++;
				}
				T++;
			}
		}
		for(int i=0;i<16;i++)
		{
			for(int num=0;num<80;num++)
			{
				Inf_feature1_16>>featureAll[Sample][T];
				Inf_feature2_16>>testALL[Sample][T];
				if(featureAll[Sample][T]!=0)
				{
					zero_sample[Sample]++;
				}
				if(testALL[Sample][T]!=0)
				{
					zero_test[Sample]++;
				}
				T++;
			}
		}
	}
	for(int i=0;i<900;i++)
	{
		sampleWorlsAll<<zero_sample[i]<<' ';
		testWorlsAll<<zero_test[i]<<' ';
		for(int j=0;j<1680;j++)
		{
			sampleFeatureAll<<featureAll[i][j]<<' ';
			testFeatureAll<<testALL[i][j]<<' ';
			if(featureAll[i][j]!=0)
			{
				sampleWorlsAll<<j<<':'<<featureAll[i][j]<<' ';
			}
			if(testALL[i][j]!=0)
			{
				testWorlsAll<<j<<':'<<testALL[i][j]<<' ';
			}
		}
		sampleWorlsAll<<endl;
		testWorlsAll<<endl;
	}
/////////////////////////////////////////////////////////////////////
	system("pause");
	cvWaitKey(0);
	return 0;
}