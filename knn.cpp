
#include "header.h"
#include<conio.h>

void createInputVec(Mat_<float> features,Mat_<int> labels) {  
	 Mat img;  
	 char file[255];  
	 int l=0;
	 for (int j = 0; j < 9; j++)  {  
		 for(int k=0;k<2;++k) {
			  sprintf(file, "%s%d_%d.jpg", PATH, j+1,k);  
			  img = imread(file, 1);  
			  if (!img.data)  {  
				std::cout << "File " << file << " not found\n";
				continue;  
			  }  
			  cvtColor(img,img,CV_BGR2GRAY);
	//		  img = trainPrePos(img); 
			  img = img.reshape(1,1);
			  for(int i=0;i<SX*SY;++i) {
					features.at<float>(l,i)=float(img.at<uchar>(0,i));
				}
				labels.at<int>(0,l++)=j+1;	
		 }
	 }  
} 

Mat trainPrePos(Mat img)  
{   
//	GaussianBlur(img, img, Size(5, 5), 2, 2);  
	adaptiveThreshold(img, img, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 11, 2);  

	 std::vector<std::vector<Point> >contours;  
	 Mat contourImage,out;  
	 img.copyTo(contourImage);  
	 findContours(contourImage, contours, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);  
  
	 int idx = 0;  
	 int area = 0;  
	 for (int i = 0; i < contours.size(); i++)  {  
		  if (area < contours[i].size() )  {  
			   idx = i;  
			   area = contours[i].size();  
		  }  
	 }  
  
	 Rect rec = boundingRect(contours[idx]);  
  
	 resize(img(rec),out, Size(SX, SY));  
	 return out;  
}

int knnPrePos(Mat img,Mat &tempI) {
		// removing the edges
	for(int i=0;i<img.size().height;++i) {
		floodFill(img,Point(0,i),Scalar(0,0,0));
		floodFill(img,Point(img.size().height-1,i),Scalar(0,0,0));
		floodFill(img,Point(i,0),Scalar(0,0,0));
		floodFill(img,Point(i,img.size().height-1),Scalar(0,0,0));
	}
		// determining the bounding rect
	std::vector<std::vector<Point> > contours;
	Rect rect;

	findContours( img.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );	
	
	if(contours.size() == 0)		// blank cell 
		return 0;

	 int idx = 0;  
	 int area = 0;  
	 for (int i = 0; i < contours.size(); i++)  {  
		  if (area < contours[i].size() )  {  
			   idx = i;  
			   area = contours[i].size();  
		  }  
	 }  

	rect = boundingRect(contours[idx]);
	img(rect).copyTo(tempI);
	resize(tempI,tempI, Size(SX, SY));  
	bitwise_not(tempI,tempI);

	return 1;
}
