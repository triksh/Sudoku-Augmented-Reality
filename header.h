
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <math.h>
#include <bitset>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <ml.h>

using namespace cv;

#define FILE_NAME "F:\\sudoku1.jpg"
#define PATH "F:\\images\\"	
#define SX 20		// dimentions, you want your knn to be trained
#define SY 30

void createInputVec(Mat_<float>,Mat_<int>);		// convert training images to vectors
Mat trainPrePos(Mat);		// apply some preprocessing to img before feeding then to knn
int knnPrePos(Mat,Mat&);	// for testing data
bool SolveSudoku(int grid[9][9]);	// solve the sudoku puzzle
