#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat img_src;
string szWinName1 = "Main";
string szTrackbarName1 = "brightness";
string szTrackbarName2 = "contrast";

int calcBrightness(Mat m);
void onChange_Brightness(int value, void*);
void onChange_Contrast(int value, void*);
void addBrightness(Mat m, int v);
void multiplyContrast(Mat m, double v);

int main() {
	namedWindow(szWinName1);//윈도우 생성
	createTrackbar(szTrackbarName1, szWinName1, 0, 255, onChange_Brightness);
	createTrackbar(szTrackbarName2, szWinName1, 0, 4, onChange_Contrast);
	img_src = imread("anduin.jpg");
	int brightness = calcBrightness(img_src);
	imshow(szWinName1, img_src);

	setTrackbarPos(szTrackbarName1, szWinName1, brightness);

	waitKey();
	return 0;
}

void onChange_Brightness(int value, void*) {
	int b_before = calcBrightness(img_src);
	addBrightness(img_src, value - b_before);
	imshow(szWinName1, img_src);
}

//두 개를 같이 적용 시키려면 콘트라스트도 원본이미지를 적용하거나
//아니면 밝기도 tmp 이미지를 만들어서 변경시키면 가능 
void onChange_Contrast(int value, void*) {
	double v = 1 + value*0.5 ;
	Mat img_tmp = img_src.clone();
	multiplyContrast(img_tmp, v); //보여주는 건 원 이미지 ! 
	imshow(szWinName1, img_tmp);
}

double mymin(double a, double b) {
	return (a < b) ? a : b;
}

void multiplyContrast(Mat m, double v) {
	m = m * v;
}

int calcBrightness(Mat m) {
	int sum = 0;
	uchar* p;
	for (int i = 0; i < m.rows; i++) {
		p = m.ptr<uchar>(i);
		for (int j = 0; j < m.cols; j++) {
			sum = sum + ((p + j * 3)[0] + (p + j * 3)[1] + (p + j * 3)[2]) / 3;
		}
	}
	/*
	 for(int i = 0; i<m.rows; i++){
	for(int j=0; j<m.cols;j++){
		sum = sum+(m.at<Vec3b>(i,j)[0] + m.at<Vec3b>(i,j)[1] +m.at<Vec3b>(i,j)[2])/3;
	}
}
	*/
	return sum / (m.rows * m.cols);
}

void addBrightness(Mat m, int v) {
	int sum = 0;
	uchar* p;
	for (int i = 0; i < m.rows; i++) {
		p = m.ptr<uchar>(i);
		for (int j = 0; j < m.cols; j++) {
			for (int k = 0; k < 3; k++) {
				if (v + (p)[j * 3 + k] > 255)
					(p)[j * 3 + k] = 255;
				else if (v + (p)[j * 3 + k] < 0) //예외처리
					(p)[j * 3 + k] = 0;
				else
					(p)[j * 3 + k] += v;
			}
		}
	}

}
