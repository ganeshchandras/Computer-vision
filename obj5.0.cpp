#include <stdafx.h>
#include <stdio.h>
#include "core/core.hpp"
#include "highgui/highgui.hpp"
#include "features2d/features2d.hpp"
#include <features2d/features2d.hpp>
#include "contrib/contrib.hpp"
#include "nonfree/nonfree.hpp"
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;
vector<vector< DMatch >> good_matches2;
vector<KeyPoint> queryKeypoints;
vector<vector<KeyPoint> > trainKeypoints;
const string defaultDetectorType = "SURF";
const string defaultDescriptorType = "SURF";
//const string defaultMatcherType = "BruteForce-L1";
const string defaultMatcherType = "FlannBased";
const string defaultQueryImageName = "C:/OpenCv_2.4.8/opencv/samples/cpp/matching_to_many_images/query.png";
const string defaultFileWithTrainImages = "C:/OpenCv_2.4.8/opencv/samples/cpp/matching_to_many_images/train/trainImages.txt";
const string defaultDirToSaveResImages = "C:/OpenCv_2.4.8/opencv/sources/samples/cpp/matching_to_many_images/results";
//vector<char> mask;
static void printPrompt( const string& applName )
{
    cout << "/*\n"
         << " * This is a sample on matching descriptors detected on one image to descriptors detected in image set.\n"
         << " * So we have one query image and several train images. For each keypoint descriptor of query image\n"
         << " * the one nearest train descriptor is found the entire collection of train images. To visualize the result\n"
         << " * of matching we save images, each of which combines query and train image with matches between them (if they exist).\n"
         << " * Match is drawn as line between corresponding points. Count of all matches is equel to count of\n"
         << " * query keypoints, so we have the same count of lines in all set of result images (but not for each result\n"
         << " * (train) image).\n"
         << " */\n" << endl;

    cout << endl << "Format:\n" << endl;
    cout << "./" << applName << " [detectorType] [descriptorType] [matcherType] [queryImage] [fileWithTrainImages] [dirToSaveResImages]" << endl;
    cout << endl;

    cout << "\nExample:" << endl
         << "./" << applName << " " << defaultDetectorType << " " << defaultDescriptorType << " " << defaultMatcherType << " "
         << defaultQueryImageName << " " << defaultFileWithTrainImages << " " << defaultDirToSaveResImages << endl;
}

//static void maskMatchesByTrainImgIdx( const vector<vector<DMatch>>& matches, int trainImgIdx, vector<char>& mask )
//{
//    mask.resize( matches.size() );
//    fill( mask.begin(), mask.end(), 0 );
//	for(int j=0;j<matches.size();j++){
//    for( size_t i = 0; i < matches[j].size(); i++ )
//    {
//        if( matches[j][i].imgIdx == trainImgIdx )
//            mask[j] = 1;
//	}}
//}

static void readTrainFilenames( const string& filename, string& dirName, vector<string>& trainFilenames )
{
				char path2[1024];
				trainFilenames.clear();
    			for (int i=1; i<=5; i++)
				{
					// This is assuming your file names are in the structure image_0001.jpg, image_0002.jpg,...
					
					//string recogClass = "chair";
			//		sprintf(path2, "C:\\objDetection\\code\\101_ObjectCategories\\%s\\image_%04d.jpg", recogClass.c_str(),i);
					sprintf(path2,"image_%04d.jpg",i);
					cout << path2  << endl;
					trainFilenames.push_back(path2);
			//		cv::Mat imgTrain = cvLoadImage(path,CV_LOAD_IMAGE_GRAYSCALE);	
//						if (!imgTrain.data) {
	//					  cerr << "cannot find image file" <<path<< endl;
		//				  system("pause");
						  
						} 
	

    /*ifstream file( filename.c_str() );
    if ( !file.is_open() )
        return;

    size_t pos = filename.rfind('\\');
    char dlmtr = '\\';
    if (pos == String::npos)
    {
        pos = filename.rfind('/');
        dlmtr = '/';
    }
    dirName = pos == string::npos ? "" : filename.substr(0, pos) + dlmtr;

    while( !file.eof() )
    {
        string str; getline( file, str );
        if( str.empty() ) break;
        trainFilenames.push_back(str);
    }
    file.close();*/
}

static bool createDetectorDescriptorMatcher( const string& detectorType, const string& descriptorType, const string& matcherType,
                                      Ptr<FeatureDetector>& featureDetector,
                                      Ptr<DescriptorExtractor>& descriptorExtractor,
                                      Ptr<DescriptorMatcher>& descriptorMatcher )
{
    cout << "< Creating feature detector, descriptor extractor and descriptor matcher ..." << endl;
	//featureDetector = detector(new SiftFeatureDetector());
	featureDetector = new SiftFeatureDetector();

	//descriptorExtractor = DescriptorExtractor::create("BRISK");
	descriptorExtractor = new SiftDescriptorExtractor();
	descriptorMatcher = DescriptorMatcher::create( defaultMatcherType );
    cout << ">" << endl;

	bool isCreated = !( featureDetector.empty() || descriptorExtractor.empty() || descriptorMatcher.empty() );
    if( !isCreated )
		cout << "Can not create feature detector"<<featureDetector.empty()<<" or descriptor extractor"<<descriptorExtractor.empty()<<" or descriptor matcher "<<descriptorMatcher.empty()<<" of given types." << endl << ">" << endl;

    return isCreated;
}
//training readimages function....................................................................................
static bool readTrainImages( const string& trainFilename,vector <Mat>& trainImages, vector<string>& trainImageNames )
{
    cout << "< Reading the images for training..." << endl;
    string trainDirName;
    readTrainFilenames( trainFilename, trainDirName, trainImageNames );
    if( trainImageNames.empty() )
    {
        cout << "Train image filenames can not be read." << endl << ">" << endl;
        return false;
    }
    int readImageCount = 0;
    for( size_t i = 0; i < trainImageNames.size(); i++ )
    {
		
		//sprintf(path2, "C:\\objDetection\\code\\101_ObjectCategories\\%s\\image_%04d.jpg", recogClass.c_str(),i);
        trainDirName="C:\\objDetection\\code\\101_ObjectCategories\\chair\\";
		string filename = trainDirName + trainImageNames[i];
        Mat img = imread( filename, CV_LOAD_IMAGE_GRAYSCALE );
        if( img.empty() )
            cout << "Train image " << filename << " can not be read." << endl;
        else
            readImageCount++;
        trainImages.push_back( img );
    }
    if( !readImageCount )
    {
        cout << "All train images can not be read." << endl << ">" << endl;
        return false;
    }
    else
        cout << readImageCount << " train images were read." << endl;
    cout << ">" << endl;

    return true;
}
//end of the training readimages function

//static bool readImages( const string& queryImageName,Mat queryImage )
//{
//    cout << "< Reading the query image..." << endl;
//	char path2[1024];
//	string recogClass = "chair";
//	sprintf(path2, "C:\\objDetection\\code\\101_ObjectCategories\\%s\\image_0001.jpg", recogClass.c_str());
//    //queryImage = cvLoadImage(path2 ,CV_LOAD_IMAGE_GRAYSCALE);
//    if( queryImage.empty() )
//    {
//        cout << "Query image can not be read." << endl << ">" << endl;
//        return false;
//    }
//    string trainDirName;
//   // readTrainFilenames( trainFilename, trainDirName, trainImageNames );/////////////////////////////////////////////////
//    //if( trainImageNames.empty() )
//    //{
//      //  cout << "Train image filenames can not be read." << endl << ">" << endl;
//        //return false;
//    //}
//    int readImageCount = 0;
//  //  for( size_t i = 0; i < trainImageNames.size(); i++ )
//  //  {
//		//
//		////sprintf(path2, "C:\\objDetection\\code\\101_ObjectCategories\\%s\\image_%04d.jpg", recogClass.c_str(),i);
//  //      trainDirName="C:\\objDetection\\code\\101_ObjectCategories\\chair\\";
//		//string filename = trainDirName + trainImageNames[i];
//  //      Mat img = imread( filename, CV_LOAD_IMAGE_GRAYSCALE );
//  //      if( img.empty() )
//  //          cout << "Train image " << filename << " can not be read." << endl;
//  //      else
//  //          readImageCount++;
//  //      trainImages.push_back( img );
//  //  }
//    /*if( !readImageCount )
//    {
//        cout << "All train images can not be read." << endl << ">" << endl;
//        return false;
//    }*/
//        cout << readImageCount << " query images were read." << endl;
//    cout << ">" << endl;
//
//    return true;
//}
//detect train images keypoints..................................................................................
static void detectTrainKeypoints(
                      const vector<Mat>& trainImages, vector<vector<KeyPoint> >& trainKeypoints,
                      Ptr<FeatureDetector>& featureDetector )
{
    cout << endl << "< Extracting keypoints from training images..." << endl;
   // featureDetector->detect( queryImage, queryKeypoints );
    featureDetector->detect( trainImages, trainKeypoints );
    cout << ">" << endl;
}
//end of the detect train keypoints function

static void detectKeypoints( const Mat& queryImage, vector<KeyPoint>& queryKeypoints,
                      const vector<Mat>& trainImages, vector<vector<KeyPoint> >& trainKeypoints,
                      Ptr<FeatureDetector>& featureDetector )
{
    cout << endl << "< Extracting keypoints from query images..." << endl;
    featureDetector->detect( queryImage, queryKeypoints );
    //featureDetector->detect( trainImages, trainKeypoints );
    cout << ">" << endl;
}
//compute descriptors for training image............................................................................
static void computeTrainDescriptors( const Mat& queryImage, vector<KeyPoint>& queryKeypoints, Mat& queryDescriptors,
                         const vector<Mat>& trainImages, vector<vector<KeyPoint> >& trainKeypoints, vector<Mat>& trainDescriptors,
                         Ptr<DescriptorExtractor>& descriptorExtractor )
{
    cout << "< Computing descriptors for training keypoints..." << endl;
    //descriptorExtractor->compute( queryImage, queryKeypoints, queryDescriptors );
    descriptorExtractor->compute( trainImages, trainKeypoints, trainDescriptors );

    int totalTrainDesc = 0;
    for( vector<Mat>::const_iterator tdIter = trainDescriptors.begin(); tdIter != trainDescriptors.end(); tdIter++ )
        totalTrainDesc += tdIter->rows;

    cout << "Query descriptors count: " << queryDescriptors.rows << "; Total train descriptors count: " << totalTrainDesc << endl;
    cout << ">" << endl;


}
//end of the function...............................................................................................
static void computeDescriptors( const Mat& queryImage, vector<KeyPoint>& queryKeypoints, Mat& queryDescriptors,
                         const vector<Mat>& trainImages, vector<vector<KeyPoint> >& trainKeypoints, vector<Mat>& trainDescriptors,
                         Ptr<DescriptorExtractor>& descriptorExtractor )
{
    cout << "< Computing descriptors for query keypoints..." << endl;
    descriptorExtractor->compute( queryImage, queryKeypoints, queryDescriptors );
    /*descriptorExtractor->compute( trainImages, trainKeypoints, trainDescriptors );

    int totalTrainDesc = 0;
    for( vector<Mat>::const_iterator tdIter = trainDescriptors.begin(); tdIter != trainDescriptors.end(); tdIter++ )
        totalTrainDesc += tdIter->rows;*/

    cout << "Query descriptors count: " << queryDescriptors.rows<< endl;
    cout << ">" << endl;
}

static void matchDescriptors( const Mat& queryDescriptors, const vector<Mat>& trainDescriptors,
                       vector<vector<DMatch>>& matches, Ptr<DescriptorMatcher>& descriptorMatcher,Mat& queryImage )
{
    cout << "< Set train descriptors collection in the matcher and match query descriptors to them..." << endl;
    TickMeter tm;
	Mat img_matches;
    tm.start();
  descriptorMatcher->add( trainDescriptors );
   descriptorMatcher->train();
    tm.stop();
    double buildTime = tm.getTimeMilli();
	Mat mask = Mat::zeros(queryDescriptors.rows,trainDescriptors.size(), CV_8UC1);   
    tm.start();
	cout<<"size: "<<trainDescriptors.size();
	//vector<DMatch> m;
	//Mat td;
	//for (int i=0;i<trainDescriptors.size();i++){
	//	td = trainDescriptors.at(i); 
	//	Mat mask = Mat::zeros(queryDescriptors.rows,td.rows, CV_8UC1);    
	////	 descriptorMatcher->add( trainDescriptors[i] );
	descriptorMatcher->radiusMatch( queryDescriptors,matches,5.0f,mask,false );
	cout<<"done";
	// matches.push_back(m);
	//}


	/*Mat mask;
	  mask.create(queryDescriptors.rows, trainDescriptors.size(), CV_8UC1);
        mask.setTo(cv::Scalar::all(1));
std::vector<cv::gpu::GpuMat> masks(2);
    for (int mi = 0; mi < 2; mi++)
    {
        masks[mi] = cv::gpu::GpuMat(query.rows, train.rows / 2, CV_8UC1, cv::Scalar::all(1));
        for (int di = 0; di < queryDescCount / 2; di++)
            masks[mi].col(di * countFactor).setTo(cv::Scalar::all(0));
    }
descriptorMatcher->radiusMatch(queryDescriptors,matches,2.0f,mask,false);
    descriptorMatcher->match( queryDescriptors, matches );
	descriptorMatcher->match( queryDescriptors, matches );*/
	//vector<vector< DMatch >> good_matches2;
//good_matches2.reserve(matches.size());  
//double tresholdDist = 0.25 * sqrt(double(queryImage.size().height*queryImage.size().height + queryImage.size().width*queryImage.size().width));
////double tresholdDist =100000;
////cout<<"k: 5"<<"q: "<<queryKeypoints.at
//	double dist ;
//for(int k=0;k<5;k++)
//{
//	vector<DMatch> matt;
//	for (int i = 0; i <matches.size(); ++i)// matches.size()
//	{ 		
//		for (int j = 0; j <matches.at(i).size(); j++)//matches.at(i).size()
//		{
//			cout<<"k: "<<k<<"j: "<<j<<"i: "<<i<<"\n";
//			//Point2f from = queryKeypoints.at(matches.at(i).at(j).queryIdx).pt;
//			//Point2f to = trainKeypoints.at(k).at(matches.at(i).at(j).trainIdx).pt;
//	dist = sqrt((queryKeypoints.at(matches.at(i).at(j).queryIdx).pt.x - trainKeypoints.at(k).at(matches.at(i).at(j).trainIdx).pt.x) * (queryKeypoints.at(matches.at(i).at(j).queryIdx).pt.x - trainKeypoints.at(k).at(matches.at(i).at(j).trainIdx).pt.x) + (queryKeypoints.at(matches.at(i).at(j).queryIdx).pt.y - trainKeypoints.at(k).at(matches.at(i).at(j).trainIdx).pt.y) * (queryKeypoints.at(matches.at(i).at(j).queryIdx).pt.y - trainKeypoints.at(k).at(matches.at(i).at(j).trainIdx).pt.y));
//			//calculate local distance for each possible match
//		//	double dist = sqrt((from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y));
//
//			//save as best match if local distance is in specified area and on same height
//			//if (dist < 5.0f)
//			//{
//				//matt.push_back(matches.at(i).at(j));
//				//j = matches[i].size();
//			//}
//			
//		}
//		
//	}
//	if (matt.size() >0)
//	good_matches2.push_back(matt);
//	matt.clear();
//}
//cout<<"good matches is .........................................."<<good_matches2.size();
//for(int k=0;k<5;k++)
//{
//	for(int l=0;l<good_matches2[k].size();l++){
//		cout<<good_matches2[k][l]
//	}
//}
	//descriptorMatcher->radiusMatch(
	//  std::vector< DMatch > good_matches;

 // for( int i = 0; i < queryDescriptors.rows; i++ )
 // { if( matches[i].distance <= 200 )
 //   { good_matches.push_back( matches[i]); }
 // }
 //   drawMatches( img1, keypoints1, img2, keypoints2,
 //              good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
 //              vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

 // //-- Show detected matches
 // imshow( "Good Matches", img_matches );
    tm.stop();
    double matchTime = tm.getTimeMilli();

   // CV_Assert( queryDescriptors.rows == (int)matches.size() || matches.empty() );

    cout << "Number of matches: " << matches.size() << endl;
    cout << "Build time: " << buildTime << " ms; Match time: " << matchTime << " ms" << endl;
    cout << ">" << endl;
}

static void saveResultImages( const Mat& queryImage, const vector<KeyPoint>& queryKeypoints,
                       const vector<Mat>& trainImages, const vector<vector<KeyPoint> >& trainKeypoints,
                       const vector<vector<DMatch>>& matches, const vector<string>& trainImagesNames, const string& resultDir )
{
    cout << "< Save results..." << endl;
   // Mat drawImg;
	int point=0;
	cout<<"its here!!!!!!!!!!!!!!!!!!!";
	//vector< DMatch >big=good_matches2[0].size();
	
    //vector<char> mask;
	//for(int i=1;i<5;i++)
	//{
	//	if(good_matches2[i].size()>big)
	//	{
	//		point=i;
	//		//big=good_matches2[i].size();
	//	}
	//}
	cout<<"the point is"<<point;
	imshow("Good Matches",trainImages[point]);

	
   // for( size_t i = 0; i < trainImages.size(); i++ )
   // {
   //     if( !trainImages[i].empty() )
   //     {
   //         maskMatchesByTrainImgIdx( matches, (int)i, mask );
   //         drawMatches( queryImage, queryKeypoints, trainImages[i], trainKeypoints[i],
   //                      matches[1], drawImg, Scalar(255, 0, 0), Scalar(0, 255, 255), mask );
   //   /*      string filename ="C:\\objDetection\\code\\101_ObjectCategories\\result\\"+ trainImagesNames[i];
   //         if( !imwrite( filename, drawImg ) )
   //             cout << "Image " << filename << " can not be saved (may be because directory " << resultDir << " does not exist)." << endl;
			//else{
			//	cout<<"saved";
			//}*/
			//imshow("Good Matches",drawImg);
			//waitKey(0);
   //     }
   // }
    cout << ">" << endl;
}

int main(int argc, char** argv)
{
    string detectorType = defaultDetectorType;
    string descriptorType = defaultDescriptorType;
    string matcherType = defaultMatcherType;
    string queryImageName = defaultQueryImageName;
    string fileWithTrainImages = defaultFileWithTrainImages;
    string dirToSaveResImages = defaultDirToSaveResImages;
	VideoCapture cap(CV_CAP_ANY);//new line
		cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 640);
		 if(!cap.isOpened())  // check if we succeeded//new line
        return -1;//new line
   /* if( argc != 7 && argc != 1 )
    {
        printPrompt( argv[0] );
        return -1;
    }

    if( argc != 1 )
    {
        detectorType = argv[1]; descriptorType = argv[2]; matcherType = argv[3];
        queryImageName = argv[4]; fileWithTrainImages = argv[5];
        dirToSaveResImages = argv[6];
    }*/

    Ptr<FeatureDetector> featureDetector;
    Ptr<DescriptorExtractor> descriptorExtractor;
    Ptr<DescriptorMatcher> descriptorMatcher;
    if( !createDetectorDescriptorMatcher( detectorType, descriptorType, matcherType, featureDetector, descriptorExtractor, descriptorMatcher ) )
    {
        printPrompt( argv[0] );
        return -1;
    }

    Mat queryImage;
    vector<Mat> trainImages;
    vector<string> trainImagesNames;
	if( !readTrainImages( fileWithTrainImages, trainImages, trainImagesNames ) )//if for training images
    {
        printPrompt( argv[0] );
        return -1;
    }
  /*  if( !readImages( queryImageName, queryImage) )
    {
        printPrompt( argv[0] );
        return -1;
    }*/

    //vector<KeyPoint> queryKeypoints;
    //vector<vector<KeyPoint> > trainKeypoints;
	Mat queryDescriptors;
    vector<Mat> trainDescriptors;
	detectTrainKeypoints(trainImages, trainKeypoints, featureDetector );
	computeTrainDescriptors( queryImage, queryKeypoints, queryDescriptors,
                        trainImages, trainKeypoints, trainDescriptors,
                        descriptorExtractor );

	//start of the for loop for videocapture... all training must be comleated before this phase
	while(1){// start of the video capture loop
	string filename ="C:\\objDetection\\code\\101_ObjectCategories\\result1\\image_0001.jpg";
	cap >> queryImage;
	imwrite( filename, queryImage );
	queryImage = imread( filename, CV_LOAD_IMAGE_GRAYSCALE );
	/*if( !readImages( queryImageName, queryImage) )
    {
        printPrompt( argv[0] );
        return -1;
    }*/
    detectKeypoints( queryImage, queryKeypoints, trainImages, trainKeypoints, featureDetector );
    
    computeDescriptors( queryImage, queryKeypoints, queryDescriptors,
                        trainImages, trainKeypoints, trainDescriptors,
                        descriptorExtractor );
	trainImages.clear();
    vector<vector<DMatch>> matches;
    matchDescriptors( queryDescriptors, trainDescriptors, matches, descriptorMatcher,queryImage );
	char path3[1024];
	string recogClass = "chair";
	sprintf(path3, "C:\\OpenCv_2.4.8\\opencv\\sources\\samples\\cpp\\matching_to_many_images\\results");
	//dirToSaveResImages=path3;
	//C:\OpenCv_2.4.8\opencv\sources\samples\cpp\matching_to_many_images
	//C:/OpenCv_2.4.8/opencv/samples/cpp/matching_to_many_images/results
    saveResultImages( queryImage, queryKeypoints, trainImages, trainKeypoints,
                      matches, trainImagesNames, path3 );
	
	//waitKey(0);
	 bool compactResult=false;
	 
//	descriptorMatcher.radiusMatch(queryDescriptors,trainDescriptors,matches,2.0f);//.........................HERE..................................................
	//namedWindow("matches", 1);
 //   Mat img_matches;
	//drawMatches(queryImage, queryKeypoints, trainImages, trainKeypoints, matches, img_matches);
 //   imshow("matches", img_matches);
	//  std::vector< DMatch > good_matches;

 // for( int i = 0; i < queryDescriptors.rows; i++ )
 // { if( matches[i].distance <= 200 )
 //   { good_matches.push_back( matches[i]); }
 // }
 //   drawMatches( queryImage, queryKeypoints, trainImages, trainKeypoints,
 //              good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
 //              vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

 // //-- Show detected matches
 // imshow( "Good Matches", img_matches );
	//start..............................................................................................
//	std::vector< DMatch > good_matches;
	double min_dist = 100;
	double max_dist = 0;
	//for(int j1 = 0 ; j1 < 5; j1++){
	//for( int i = 0; i < trainImages[0].rows; i++ )
	//{ 
	//double dist = matches.size;
	for(int k12=0;k12<matches.size();k12++){
		for(int k13=0;k13<matches.size();k13++){
		cout<<"next is"<<matches[k12][k13].distance<<endl;
		}}
	cout<<matches.size();
   // if( dist < min_dist ) min_dist = dist;
    //if( dist > max_dist ) max_dist = dist;
  //}
	//}
	cout<<"max dist"<<max_dist;
	cout<<"min dist"<<min_dist;
	//printf("-- Max dist : %f \n", max_dist );
  //printf("-- Min dist : %f \n", min_dist );
  //end....................................................................................................
	matches.clear();
		int k;
	cout << "Do you want to continue 1/0"<< endl;
	cin>>k;
	if(k==1)
	{
		break;
	}
	}//end of the video capture loop
    return 0;


}
