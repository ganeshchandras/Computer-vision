#include "stdafx.h"
#include "opencv2/core/core_c.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "windows.h"
#include "ml.h"
#include "cv.h"
#include "nonfree/features2d.hpp"
#include "features2d/features2d.hpp"
#include "calib3d/calib3d.hpp"
#include "legacy/legacy.hpp"
#include "string.h"
#include "highgui.h"
using namespace std;
using namespace cv;
#define DICTIONARY_BUILD 1
int main()
{
#if DICTIONARY_BUILD == 1
	//Step 1 - Obtain the set of bags of features.

	//to store the input file names
	char * filename = new char[1024];		
	//to store the current input image
	Mat input;	

	//To store the keypoints that will be extracted by SIFT
	vector<KeyPoint> keypoints12;
	//To store the SIFT descriptor of current image
	Mat descriptor;
	//To store all the descriptors that are extracted from all the images.
	Mat featuresUnclustered;
	//The SIFT feature extractor and descriptor
	SiftDescriptorExtractor detector12;	
	string fileFolder = "C:\\objDetection\\code\\101_ObjectCategories";
ifstream Labels("C:\\objDetection\\code\\labels_small.txt"); // put the 
string line;
int j=0;
char path[1024];
getline(Labels,line);
cout<<"training vocabulary";
	//I select 20 (1000/50) images from 1000 images to extract feature descriptors and build the vocabulary
	for(int f=1;f<=150;f+=10){		
		//create the file name of an image
		sprintf(filename, "%s\\%s\\image_%04d.jpg", fileFolder.c_str(),line.c_str(), f );
		//sprintf(filename,"G:\\testimages\\image\\%i.jpg",f);--------------------------------line
		//open the file
		input = imread(filename, CV_LOAD_IMAGE_GRAYSCALE); //Load as grayscale				
		//detect feature points
		detector12.detect(input, keypoints12);
		//compute the descriptors for each keypoint
		detector12.compute(input, keypoints12,descriptor);		
		//put the all feature descriptors in a single Mat object 
		featuresUnclustered.push_back(descriptor);		
		//print the percentage
		printf("%d image done\n",f);//------------------------line
	}	


	//Construct BOWKMeansTrainer
	//the number of bags
	cout<<"\n Train desc"<<featuresUnclustered.rows;
	//int dictionarySize=200;
	//define Term Criteria
	//TermCriteria tc(CV_TERMCRIT_ITER,100,0.001);
	//retries number
	//int retries=1;
	//necessary flags
	//int flags=KMEANS_PP_CENTERS;
	//Create the BoW (or BoF) trainer
	BOWKMeansTrainer bowTrainer(500);
	cout<<"\n1";
	bowTrainer.add(featuresUnclustered);
	cout<<"\n2";
	//cluster the feature vectors
	Mat vocabulary = bowTrainer.cluster();
	cout<<"\n3";
	cout<<"clustering done";
	cout<<"\nsize of the vocabulary:"<<vocabulary.size();
	//Mat dictionary=bowTrainer.cluster(featuresUnclustered);	
	//store the vocabulary
	//FileStorage fs("vocabulary.xml", FileStorage::WRITE);
	//fs << "vocabulary" << vocabulary;
	//fs.release();
	cout<<"\ndone\n";
//----------------------------------------dic build 2---------------------------------------------------------------
	//Mat vocabulary;
	//FileStorage fs1("vocabulary.xml", FileStorage::READ);
	//cout<<"Reading Vocabulary from file";
	//fs1["vocabulary"]>>vocabulary;
	//fs1.release();
	//---------------------------------------------------------------------------------------------
	Mat img;
	Ptr<FeatureDetector> detector(new SiftFeatureDetector);
	Ptr<DescriptorExtractor> extractor(new SiftDescriptorExtractor);
	Ptr<DescriptorMatcher> matcher(new BruteForceMatcher<L2 <float> > );
	BOWImgDescriptorExtractor bowide(extractor,matcher);
	bowide.setVocabulary(vocabulary);
	vector<KeyPoint> keypoints;
	Mat desc,desc1,desc2,desc3;
	//---------------------------------------------------------------------------------------------
	int fileno=1;
	char  response[100];
	char compName[1024];
	string fileFolder1 = "C:\\objDetection\\code\\101_ObjectCategories";
	ifstream Labels1("C:\\objDetection\\code\\labels_small.txt"); // put the 
	string line1;
	getline(Labels1,line1);
	int imageno;
	int k12;
	int limittill=1;
	while(fileno<=3)
	{
		//cout<<"\n Enter Component for training";
		//cin>>compName;
		if(fileno==1)sprintf(response,"L1Response.xml");
		else if(fileno==2)sprintf(response,"L2Response.xml");
		else sprintf(response,"L3Response.xml");
		k12=limittill+fileno-1;
		limittill=fileno*50;
		//response='L'+fileno;
		//strcpy(response,compName);
		
		//strcat(response,"Response.xml");
		cout<<"\n Adding to "<<response;
		//chdir(compName);
		
		string imgName;
		//ifstream f1;
		//f1.open(compName);
		
		FileStorage fs2(response, FileStorage::WRITE);
		//int flag =0;
		//Mat desc,desc2,desc3;
		//Mat responseCopy;
		for(imageno=k12;imageno<=limittill;imageno+=10)
		{
			sprintf(compName, "%s\\%s\\image_%04d.jpg", fileFolder1.c_str(),line1.c_str(), imageno );
			Mat responseHist(1,30,CV_32FC1);
		//	f1>>imgName;
			//cout<<"\n"<<imgName;
			img=imread(compName);
			if(!img.data)
				continue;
			cvtColor(img,img,CV_BGR2GRAY);
			SIFT sift;
			vector<Point2f> points;
			sift(img,img,keypoints,desc1,false);
			
			bowide.compute(img,keypoints,responseHist);
			
			int i;
			cout<<"\n ResponseHist:"<<responseHist.rows<<","<<responseHist.cols<<","<<responseHist.type()<<","<<responseHist.channels();
			//cout<<"\n"<<desc.cols;
			
			if(responseHist.rows!=0&&fileno==1)
			{ 
				
				desc.push_back(responseHist);
				
			}
			else if(responseHist.rows!=0&&fileno==2)
			{ 
				
				desc2.push_back(responseHist);
				
			}
			if(responseHist.rows!=0&&fileno==3)
			{ 
				
				desc3.push_back(responseHist);
				
			}
			//cout<<"\n "<<desc.rows<<" "<<desc.cols;
			
		}
		
		fs2<<"responseHist"<<desc;
		
		fs2.release();
		fileno++;
		//f1.close();
		
	}
//--------------------------------------------dic 3--------------------------------------------------
Mat trainData,labels;
	
	int count = 0;
	//chdir("Leaf1");
	FileStorage fs1("L1Response.xml",FileStorage::READ);
	Mat resResponse;
	fs1["responseHist"]>>resResponse;
	count=desc.rows;
	
	trainData.push_back(desc);
	Mat reslabels=Mat::zeros(count,1,CV_32SC1);
	
	labels.push_back(reslabels);
	
	cout<<"trainData:"<<trainData.rows<<","<<trainData.cols<<"\n Label:"<<labels.rows;
	fs1.release();
	
	//chdir("../Leaf2");
	FileStorage fs2("L2Response.xml",FileStorage::READ);
	Mat capResponse;
	fs2["responseHist"]>>capResponse;
	Mat caplabels=Mat::ones(desc2.rows, 1, CV_32SC1);
	trainData.push_back(desc2);
	labels.push_back(caplabels);
	cout<<"l2 done";
	
	//chdir("../Leaf3");
	FileStorage fs3("L3Response.xml",FileStorage::READ);
	Mat indResponse;
	fs3["responseHist"]>>indResponse;
	Mat indlabels=Mat::ones(desc3.rows, 1, CV_32SC1);
	indlabels.setTo((float)2);
	trainData.push_back(desc3);
	labels.push_back(indlabels);
//	chdir("..");
	cout<<"l3 done";
	cout<<"\ntrain:\n"<<trainData;
	cout<<"\n LAbels:\n"<<labels;
	CvSVMParams params;
	params.svm_type = CvSVM::C_SVC;
	//params.kernel_type = LINEAR;
	params.term_crit =  cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
	
	//Train array:DD array with responsehistograms values
	
	Mat svmData;
	trainData.convertTo(svmData,CV_32F);
	Mat labelData;
	labelData=labels.t(); //labels: Array of labels
	
	cout<<"\n\n\nconversion done";
	cout<<"\n\n\nSize of svm data"<<svmData.size();
	//SVM training:
	
	CvSVM svm;
	params=svm.get_params();
	
	svm.train_auto(svmData, labelData, Mat(), Mat(), params, 10);
	cout<<"training done";
	svm.save("classifier.xml","circuit");
	
	//End of training
	//---------------------testing-------------------------------------------------------------------
	char path19[1024];
	sprintf(path19, "C:\\objDetection\\code\\101_ObjectCategories\\result1\\image_0001.jpg");
	Mat img23=imread(path19);
		if(!img23.data){ cout<<"image not found"; exit(0);}
//			continue;
		Ptr<FeatureDetector> detector23(new SiftFeatureDetector());
		Ptr<DescriptorExtractor> extractor23(new SiftDescriptorExtractor());
		Ptr<DescriptorMatcher> matcher23(new BruteForceMatcher<L2 <float> >() );
		BOWImgDescriptorExtractor bowide23(extractor,matcher);
		bowide23.setVocabulary(vocabulary);
		vector<KeyPoint> keypoints23;
		Mat desc23(0,30,CV_64FC1);
		Mat responseHist23;
		cvtColor(img23,img23,CV_BGR2GRAY);
		SIFT sift23;
		vector<Point2f> points23;
		int i;
		sift23(img23,img23,keypoints23,desc23,false);
		bowide23.compute(img23,keypoints23,responseHist23);
		float predLabel=5;
		if(responseHist23.rows!=0)
		{
			predLabel = svm.predict(responseHist23, false);
		}
		cout<<"\n value:"<<predLabel;
	return 0;
#else
//Test
	CvSVM svm;
	svm.load("classifier.xml","circuit");
	//Mat testImg = imread("Test");
	
	Mat vocabulary;
	
	FileStorage fs3("vocabulary.xml", FileStorage::READ);
	cout<<"Reading Vocabulary from file";
	
	fs3["vocabulary"]>>vocabulary;
	fs3.release();
	//chdir("Test");
	//ifstream f1;
	//f1.open("test");
	char path1[1024];
	sprintf(path1, "C:\\objDetection\\code\\101_ObjectCategories\\result1\\image_0001.jpg");
//	while(!f1.eof())
	//{
		//string imgName;
		//f1>>imgName;
		//cout<<"\n"<<imgName;
		Mat img=imread(path1);
		if(!img.data){ cout<<"image not found"; exit(0);}
//			continue;
		Ptr<FeatureDetector> detector(new SiftFeatureDetector());
		Ptr<DescriptorExtractor> extractor(new SiftDescriptorExtractor());
		Ptr<DescriptorMatcher> matcher(new BruteForceMatcher<L2 <float> >() );
		BOWImgDescriptorExtractor bowide(extractor,matcher);
		bowide.setVocabulary(vocabulary);
		vector<KeyPoint> keypoints;
		Mat desc(0,30,CV_64FC1);
		Mat responseHist;
		cvtColor(img,img,CV_BGR2GRAY);
		SIFT sift;
		vector<Point2f> points;
		int i;
		sift(img,img,keypoints,desc,false);
		bowide.compute(img,keypoints,responseHist);
		float predLabel=5;
		if(responseHist.rows!=0)
		{
			predLabel = svm.predict(responseHist, false);
		}
		cout<<"\n value:"<<predLabel;
	//}	
	return 0;
#endif

}