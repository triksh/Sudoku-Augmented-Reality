#include "header.h"

#define FILE_NAME "F:\\sudoku1.jpg"

int main() {
	Mat imgIn,imgG,imgP,imgB,temp;
	Mat element = getStructuringElement( MORPH_RECT, Size(3,3), Point(-1,-1) );

	namedWindow("Input",CV_WINDOW_AUTOSIZE);
	namedWindow("temp",CV_WINDOW_AUTOSIZE);
	namedWindow("output",CV_WINDOW_AUTOSIZE);

		//load the image
	imgIn = imread(FILE_NAME);
	cvtColor(imgIn,imgG,CV_BGR2GRAY);

	// /* Image pre-processing */ 		
		// remove the noise
	GaussianBlur(imgG, imgP, Size(11,11), 0);
	//medianBlur(imgP,imgP,3);

		// Binarization of image
	adaptiveThreshold(imgP,imgB,255,CV_ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY_INV,5,2);
	dilate( imgB, imgB, element);


	// /*  Grid Detection */
	
		//contour detection
	std::vector<Point> rectP; 
	{
		std::vector<std::vector<Point> > contours;
		findContours( imgB, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

		int idx; double area=0,tempA;

		for(int i=0; i<contours.size(); ++i)
		{
			tempA=contourArea(contours[i]);
			if(tempA>area) {
				area=tempA;
				idx=i;
			}
		}

		approxPolyDP(contours[idx],rectP,arcLength(contours[idx],true)*0.05,true);

	}

		// points detection and sorting
	int len;
	{
		if(rectP.size() != 4) {
			std::cout<<"error in detection.\n";
			return 0;
		}
		
		std::pair<int,int> pr[4];
		for(int i=0;i<4;++i) {
			pr[i].first=rectP[i].x;
			pr[i].second=rectP[i].y;
		}
		sort(pr,pr+4);
		if(pr[0].second < pr[1].second) {
			rectP[0].x = pr[0].first;
			rectP[0].y = pr[0].second;
			rectP[3].x = pr[1].first;
			rectP[3].y = pr[1].second; 
		} else {
			rectP[0].x = pr[1].first;
			rectP[0].y = pr[1].second;
			rectP[3].x = pr[0].first;
			rectP[3].y = pr[0].second; 
		}
		if(pr[2].second < pr[3].second) {
			rectP[1].x = pr[2].first;
			rectP[1].y = pr[2].second;
			rectP[2].x = pr[3].first;
			rectP[2].y = pr[3].second; 
		} else {
			rectP[2].x = pr[2].first;
			rectP[2].y = pr[2].second;
			rectP[1].x = pr[3].first;
			rectP[1].y = pr[3].second; 
		}

		len = (int)pow(rectP[0].x - rectP[4].x,2)+(int)pow(rectP[0].y - rectP[4].y,2);
		int tempL;
		for(int i=0;i<3;++i) {
			tempL = (int)pow(rectP[i].x - rectP[i+1].x,2)+(int)pow(rectP[i].y - rectP[i+1].y,2);
			if(tempL>len) len = tempL;
		}
		len =(int)std::sqrt(len);	
	}
	
		// perspective transformation	
	Point2f rectPF[4],rectPI[4];
	rectPF[0].x=0; rectPF[0].y=0;
	rectPF[1].x=float(len-1); rectPF[1].y=0;
	rectPF[2].x=float(len-1); rectPF[2].y=float(len-1);
	rectPF[3].x=0; rectPF[3].y=float(len-1);

	for(int i=0;i<4;++i){
		rectPI[i].x = (float)rectP[i].x;
		rectPI[i].y = (float)rectP[i].y;
	}

	Mat imgU = Mat(Size(len, len), CV_8UC1);
	warpPerspective(imgG, imgU, getPerspectiveTransform(rectPI, rectPF), Size(len, len));


	// /* Extracting digits from grid */
	Mat gridIn;
	adaptiveThreshold(imgU,gridIn,255,CV_ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY_INV,101,1);

	imshow("output",imgIn);
	imshow("temp",imgU);
	
	int lenC = len/9,idc,idr=0;
	for(int i=0;i<9;++i) {
		idc = 0;
		for(int j=0;j<9;++j) {
			Mat gridC = gridIn(Range(idr,idr+lenC),Range(idc,idc+lenC)),gridO;
			gridO = gridO.zeros(gridC.rows,gridC.cols,CV_8UC1);	
			knnPrePos(gridC,gridO);
			gridO.reshape(1,1);

				imshow("Input",gridO);
				waitKey(0);
			idc += lenC;
		}
		idr += lenC;
	}

	waitKey(0);
	return 0;
}
