#include "header.h"


int knnPrePos(Mat img,Mat tempI) {
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

	int idx=0; double area=0,tempA;
	for(int i=0; i<contours.size(); ++i)
	{
		tempA=contourArea(contours[i]);
		if(tempA>area) {
			area=tempA;
			idx=i;
		}
	}
	rect = boundingRect(contours[idx]);

		// centering the image
	int colLeft=rect.x,colRight=rect.x+rect.width;
	int rowBottom=rect.y+rect.height,rowTop=rect.y;
    int startAtX = (tempI.cols/2)-(colRight-colLeft)/2;
    int startAtY = (tempI.rows/2)-(rowBottom-rowTop)/2;
    for(int y=startAtY;y<(tempI.rows/2)+(rowBottom-rowTop)/2;y++)
    {
        uchar *ptr = tempI.ptr<uchar>(y);
        for(int x=startAtX;x<(tempI.cols/2)+(colRight-colLeft)/2;x++)
        {
            ptr[x] = img.at<uchar>(rowTop+(y-startAtY),colLeft+(x-startAtX));
        }
    }

	return 1;
}
  
