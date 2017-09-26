#include <iostream>
#include <fstream>
#include <string>
#include<stdio.h>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <time.h>
#include <dirent.h>
#include <sstream>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;
using namespace cv;
int countOfHumans = 0;
int olgImages = 1;
int cntForThreshold = 0;
int saveName = 1;
inline void directoryExistsOrCreate(const char* pzPath)
{
    DIR *pDir;
    if ( pzPath == NULL || (pDir = opendir (pzPath)) == NULL)
        mkdir(pzPath, 0777);
    else if(pDir != NULL)
        (void) closedir (pDir);
}
int incr = 0;
inline bool saveImg(Mat image, const string DIRECTORY, const string EXTENSION, const char * DIR_FORMAT, const char * FILE_FORMAT)
{
    stringstream ss;
    time_t seconds;
    struct tm * timeinfo;
    char TIME[80];
    time (&seconds);

    timeinfo = localtime (&seconds);
    

    strftime (TIME,80,DIR_FORMAT,timeinfo);
    ss.str("");
    ss << DIRECTORY << TIME;
    directoryExistsOrCreate(ss.str().c_str());
    ss << "/cropped";
    directoryExistsOrCreate(ss.str().c_str());


    strftime (TIME,80,FILE_FORMAT,timeinfo);
    ss.str("");
    if(incr < 100) incr++; 
    else incr = 0;
    ss << DIRECTORY << TIME << static_cast<int>(incr) << EXTENSION;
    return imwrite(ss.str().c_str(), image);
}
//------------------------------start of face detection function--------------------------------------------------------------------------
static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

int searchForFace(int c, int fin)
{
    //string fn_haar = string(argv[1]);
    //string fn_csv = string(argv[2]);
    //int deviceId = atoi(argv[3]);
    string fn_haar = "/home/dell/opencv/opencv-2.4.9/data/haarcascades/haarcascade_frontalface_alt.xml";
	string fn_csv = "imgData1.csv";
	int deviceId = 0;
    vector<Mat> images;
    vector<int> labels;
    try {
        read_csv(fn_csv, images, labels);
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
        exit(1);
    }
    int im_width = images[0].cols;
    int im_height = images[0].rows;

    //Ptr<FaceRecognizer> model = createFisherFaceRecognizer();
    Ptr<FaceRecognizer> model = createLBPHFaceRecognizer();
    model->train(images, labels);
    CascadeClassifier haar_cascade;
    haar_cascade.load(fn_haar);
    //VideoCapture cap(deviceId);
    /*if(!cap.isOpened()) {
        cerr << "Capture Device ID " << deviceId << "cannot be opened." << endl;
        return -1;
    }*/
    Mat frame;
  //  for(;;) {
//for(int l=1;l<=fin;l++){
for(int l=olgImages;l<=countOfHumans;l++){
	char buffer [50];
	//sprintf(buffer,"detectedHumans/%d%dcropHuman.jpg",c,l);
	sprintf(buffer,"detectedHumans/%dcropHuman.jpg",l);
    cout<<c<<l<<"cropHuman";
       // cap >> frame;
	frame = imread(buffer,CV_LOAD_IMAGE_COLOR);
      
        Mat original = frame.clone();
       
        Mat gray;
if(original.empty()){continue;}
	else if(original.channels()>1){
	//countofimgsc++;
        cvtColor(original, gray, CV_BGR2GRAY);
       }
	else gray = original;
       // cvtColor(original, gray, CV_BGR2GRAY);
       
        vector< Rect_<int> > faces;
        haar_cascade.detectMultiScale(gray, faces);
        for(int i = 0; i < faces.size(); i++) {
            
            Rect face_i = faces[i];
           
            Mat face = gray(face_i);
         
            Mat face_resized;
            cv::resize(face, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
            
            int prediction = model->predict(face_resized);
            rectangle(original, face_i, CV_RGB(0, 255,0), 1);
			//string box_text = format("%d", prediction);
			string box_text;
            if(prediction != 26){
            	box_text = format("INTRUDER");
			}
			else box_text = format("%d", prediction);
            if(prediction !=26 && cntForThreshold<=5)
	    	{
				cntForThreshold++;
	   	    }
			else cntForThreshold = 0;
            int pos_x = std::max(face_i.tl().x - 10, 0);
            int pos_y = std::max(face_i.tl().y - 10, 0);
           
            putText(original, box_text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
        }
        
        //imshow("face_recognizer", original);
//int k1=l+1001;
	if(cntForThreshold == 5){
	char buffer1 [50];
	sprintf(buffer1,"face_cropped/%dface.jpg",saveName);
	saveName++;
	imwrite(buffer1, original );
      }  
        char key = (char) waitKey(20);
       
        if(key == 27)
            break;
    }
olgImages = countOfHumans+1;
    return 0;
}
//------------------------------end of face detection function----------------------------------------------------------------------------
//------------------------------start of human function-----------------------------------------------------------------------------------
int searchForHuman(Mat hum,int c)
{
    /*VideoCapture cap(CV_CAP_ANY);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);    
    if (!cap.isOpened())
        return -1;
 */
    Mat img;
    int flag = 0;
    HOGDescriptor hog;
   // hog.winSize = Size(48, 96);
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

 //hog.setSVMDetector(HOGDescriptor::getDaimlerPeopleDetector());
    //namedWindow("video capture", CV_WINDOW_AUTOSIZE);
//int i=0;
       //cap >> img;
	//char buffer [50];
	//sprintf(buffer,"../../pi/motion_src/pics/05Mar2015/%d.jpg",l);
	//img = imread(buffer,CV_LOAD_IMAGE_COLOR);
	img = hum;
        if (!img.data){cout<<"nope";}
 
        vector<Rect> found, found_filtered;
        hog.detectMultiScale(img, found, 0, Size(8,8), Size(32,32), 1.05, 2);
        size_t i, j;
        for (i=0; i<found.size(); i++)
        {
            Rect r = found[i];
            for (j=0; j<found.size(); j++)
                if (j!=i && (r & found[j])==r)
                    break;
            if (j==found.size())
                found_filtered.push_back(r);
        }
	Rect crop;
       /* for (i=0; i<found_filtered.size(); i++)
	{
	    Rect r = found_filtered[i];
        r.x += cvRound(r.width*0.1);
	    r.width = cvRound(r.width*0.8);
	    r.y += cvRound(r.height*0.06);
	    r.height = cvRound(r.height*0.9);
	    rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 2);
	    cout<<"rx is"+r.x;
	    cout<<"rwidth is "+r.width;
	    cout<<"ry is"+r.y;
	    cout<<"rheight is"+r.height;
	    flag = i;
	    Mat cropped = img(r);
	    char buffer2 [50];
		countOfHumans++;
	    //sprintf(buffer2,"detectedHumans/%d%dcropHuman.jpg",c,i);
		sprintf(buffer2,"detectedHumans/%dcropHuman.jpg",countOfHumans);
	    imwrite(buffer2, cropped );
	}*/
        for (i=0; i<found_filtered.size(); i++)
	{ 
	   try{
	    Rect r = found_filtered[i];
            r.x += cvRound(r.width*0.1);
	    r.width = cvRound(r.width*0.8);
	    r.y += cvRound(r.height*0.06);
	    r.height = cvRound(r.height*0.9);
	   
	    rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 2);
            
            
	    cout<<"rx is"+r.x;
	    cout<<"rwidth is "+r.width;
	    cout<<"ry is"+r.y;
	    cout<<"rheight is"+r.height;
	    Mat cropped = img(r);
	    char buffer2 [50];
	   // sprintf(buffer2,"coped/%d%dcrop.jpg",i,l);
	   countOfHumans++;
	    sprintf(buffer2,"detectedHumans/%dcropHuman.jpg",countOfHumans);
	    imwrite(buffer2, cropped );
            }
	     catch (...)
	    {
		break;
	    }
	}
	//i++;
	//namedWindow("video capture1", CV_WINDOW_AUTOSIZE);
        //imshow("video capture1", img);
	//int k1=l+20;
	//char buffer1 [50];
	//sprintf(buffer1,"%d.jpg",k1);
	//imwrite(buffer1, img );
	//sprintf(buffer1,"%dcrop.jpg",k1);
	//imwrite(buffer1, cropped );
        //if (waitKey(20) >= 0)
            //break;
    
    return flag;
}
//-------------------------------end of human function------------------------------------------------------------------------------------
inline int detectMotion(const Mat & motion, Mat & result, Mat & result_cropped,
                 int x_start, int x_stop, int y_start, int y_stop,
                 int max_deviation,
                 Scalar & color)
{
    Scalar mean, stddev;
    meanStdDev(motion, mean, stddev);

    if(stddev[0] < max_deviation)
    {
        int number_of_changes = 0;
        int min_x = motion.cols, max_x = 0;
        int min_y = motion.rows, max_y = 0;

        for(int j = y_start; j < y_stop; j+=2){ 
            for(int i = x_start; i < x_stop; i+=2){ 

                if(static_cast<int>(motion.at<uchar>(j,i)) == 255)
                {
                    number_of_changes++;
                    if(min_x>i) min_x = i;
                    if(max_x<i) max_x = i;
                    if(min_y>j) min_y = j;
                    if(max_y<j) max_y = j;
                }
            }
        }
        if(number_of_changes){

            if(min_x-10 > 0) min_x -= 10;
            if(min_y-10 > 0) min_y -= 10;
            if(max_x+10 < result.cols-1) max_x += 10;
            if(max_y+10 < result.rows-1) max_y += 10;

            Point x(min_x,min_y);
            Point y(max_x,max_y);
            Rect rect(x,y);
            Mat cropped = result(rect);
            cropped.copyTo(result_cropped);
            rectangle(result,rect,color,1);
        }
        return number_of_changes;
    }
    return 0;
}

int main (int argc, char * const argv[])
{
    const string DIR = "/home/dell/pi/motion_src/pics/";
    const string EXT = ".jpg"; 
    const int DELAY = 10; 
    const string LOGFILE = "/home/dell/pi/motion_src/log";

   
    string DIR_FORMAT = "%d%h%Y";
    string FILE_FORMAT = DIR_FORMAT + "/" + "%d%h%Y_%H%M%S"; 
    string CROPPED_FILE_FORMAT = DIR_FORMAT + "/cropped/" + "%d%h%Y_%H%M%S"; 

   
    //CvCapture * camera = cvCaptureFromCAM(CV_CAP_ANY);
    CvCapture * camera = cvCaptureFromFile("ganesh1.MOV");
    cvSetCaptureProperty(camera, CV_CAP_PROP_FRAME_WIDTH, 1280); 
    cvSetCaptureProperty(camera, CV_CAP_PROP_FRAME_HEIGHT, 720); 
    
   
    Mat result, result_cropped;
    int humanDetected;
    int faceDection;
    Mat prev_frame = result = cvQueryFrame(camera);
    Mat current_frame = cvQueryFrame(camera);
    Mat next_frame = cvQueryFrame(camera);
    cvtColor(current_frame, current_frame, CV_RGB2GRAY);
    cvtColor(prev_frame, prev_frame, CV_RGB2GRAY);
    cvtColor(next_frame, next_frame, CV_RGB2GRAY);
    Mat d1, d2, motion;
    int number_of_changes, number_of_sequence = 0;
    Scalar mean_, color(0,255,255); 
    

    int x_start = 10, x_stop = current_frame.cols-11;
    int y_start = 350, y_stop = 530;

    int there_is_motion = 5;
    

    int max_deviation = 20;
    

    Mat kernel_ero = getStructuringElement(MORPH_RECT, Size(2,2));
    
    int countForNotOverlap = 0;
    while (true){

        prev_frame = current_frame;
        current_frame = next_frame;
        next_frame = cvQueryFrame(camera);
        result = next_frame;
		if(next_frame.empty())
    		break;
		else if(next_frame.channels()>1)
    		cvtColor(next_frame, next_frame, CV_BGR2GRAY);
		else next_frame = next_frame;
        //cvtColor(next_frame, next_frame, CV_RGB2GRAY);
	countForNotOverlap++;

        absdiff(prev_frame, next_frame, d1);
        absdiff(next_frame, current_frame, d2);
        bitwise_and(d1, d2, motion);
        threshold(motion, motion, 35, 255, CV_THRESH_BINARY);
        erode(motion, motion, kernel_ero);
        
        number_of_changes = detectMotion(motion, result, result_cropped,  x_start, x_stop, y_start, y_stop, max_deviation, color);
        

        if(number_of_changes>=there_is_motion)
        {
            if(number_of_sequence>0){ 
		humanDetected = searchForHuman(result,countForNotOverlap);
		if(humanDetected>=0)
		{
			faceDection = searchForFace(countForNotOverlap,humanDetected);
		}
                saveImg(result,DIR,EXT,DIR_FORMAT.c_str(),FILE_FORMAT.c_str());
		saveImg(result_cropped,DIR,EXT,DIR_FORMAT.c_str(),CROPPED_FILE_FORMAT.c_str());
                //saveImg(humanDetected,DIR,EXT,DIR_FORMAT.c_str(),CROPPED_FILE_FORMAT.c_str());
            }
            number_of_sequence++;
        }
        else
        {
            number_of_sequence = 0;
            cvWaitKey (DELAY);
        }
    }
    return 0;    
}
