#include <cv.h>
#include <highgui.h>
#include <ml.h>
#include <time.h>
#include <opencv2/ml/ml.hpp>
#include <fstream>
using namespace cv;
using namespace std;
int main()
{
 // step 1:  
//训练数据的分类标记，即6类,每类100个训练样本
 float labels[600];  
 for(int i=0;i<600;i++)
 {
	if(i/100==0)
		labels[i]=1;
	else if(i/100==1)
		labels[i]=2;
	else if(i/100==2)
		labels[i]=3;
	else if(i/100==3)
		labels[i]=4;
	else if(i/100==4)
		labels[i]=5;
	else if(i/100==5)
		labels[i]=6;
	/*else if(i/100==6)
		labels[i]=7;
	else if(i/100==7)
		labels[i]=8;
	else if(i/100==8)
		labels[i]=9;*/
	/*else if(i/100==9)
		labels[i]=10;
	else if(i/100==10)
		labels[i]=9;
	else if(i/100==11)
		labels[i]=12;
	else if(i/100==12)
		labels[i]=13;*/
 }
 CvMat labelsMat = cvMat(600, 1, CV_32FC1, labels);  
 //训练数据矩阵

 float trainingData[600][40];
// ifstream f1("sampleFeatureAll.txt");
 ifstream f1("1.theta");
 //int sum[600]={0};
 for(int i=0;i<600;i++)
 {
	 for(int j=0;j<40;j++)
	 {
		 f1>>trainingData[i][j];
	//	 sum[i]+=trainingData[i][j];
	 }
	 /*for(int j=0;j<40;j++)
	 {
		 trainingData[i][j]=(float)trainingData[i][j]/sum[i];
	 }*/
 }
 cout<<"theta is ok"<<endl;  
 CvMat trainingDataMat = cvMat(600, 40, CV_32FC1, trainingData);  
 
 // step 2:  
 //训练参数设定
 CvSVMParams params;  
 params.svm_type = CvSVM::C_SVC;				 //SVM类型
 params.kernel_type = CvSVM::LINEAR;			 //核函数的类型
 
 //SVM训练过程的终止条件, max_iter:最大迭代次数  epsilon:结果的精确性
 params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 1000, FLT_EPSILON ); 
 
 // step 3:  
 //启动训练过程
 CvSVM SVM;  
 SVM.train( &trainingDataMat, &labelsMat, NULL,NULL, params); 

 // step 4:  
 //使用训练所得模型对新样本进行分类测试
  //训练数据矩阵
 //float testData[400][40];
 //ifstream f2("testFeatureAll.txt");
 ifstream f2("2.theta");
 //int sum2[600]={0};
 for(int i=0;i<600;i++)
 {
	 for(int j=0;j<40;j++)
	 {
		 f2>>trainingData[i][j];
	//	 sum2[i]+=trainingData[i][j];
	 }
	 /*for(int j=0;j<40;j++)
	 {
		 trainingData[i][j]=(float)trainingData[i][j]/sum2[i];
	 }*/
 }
 cout<<"theta is ok"<<endl;  
 CvMat testDataMat = cvMat(600, 40, CV_32FC1, trainingData);
// float sample[400][40];
 int tem=0;
 for(int i=0;i<600;i++)
 {
	 for(int j=0;j<40;j++)
	 {
		trainingData[i][j]=testDataMat.data.fl[tem++];
	 }
 }
 int sun=0;
 for(int i=0;i<600;i++)
 {
	CvMat sampleDataMat = cvMat(1, 40, CV_32FC1, trainingData[i]);
	float response = SVM.predict(&sampleDataMat); 
	if((int)response==((i/100)+1))
		sun++;
	if(((i/100)+1)==11&&(int)response==9)
		sun++;
	if(i%100==0)
	{
		cout<<endl;
	}
	cout<<response<<' ';
 }
 cout<<endl;
 cout<<"准确率="<<(float)sun/600<<endl;
 // step 5:  
 //获取支持向量
 int c = SVM.get_support_vector_count();  
 cout<<endl;
 for (int i=0; i<c; i++)  
 {  
    const float* v = SVM.get_support_vector(i);  
    cout<<*v<<" ";
 }  
 cout<<endl;
 
 system("pause");
 return 0;
}
