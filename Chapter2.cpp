#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat img;


vector<vector<int>>lastp;


vector<vector<int>>myColor{ {124,48,117,143,170,255} ,
					{68,72,156,102,126,255} };

vector<Scalar>myColorvl{ { 255,0,255} ,{0,255,0} };
Mat imGray, imBlur, imgCanny, imgDil, imgErode;


Point getCountor(Mat imgDil)
{
	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;
	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	vector<vector<Point>>conPoly(contours.size());
	vector<Rect>boundRect(contours.size());
	
	Point myp(0, 0);

	for (int i = 0;i< contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		cout << area << endl;
		
		string objectType;
		if (area > 1000)
		{
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			cout << conPoly[i].size();
			boundRect[i] = boundingRect(conPoly[i]);
			myp.x = boundRect[i].x + boundRect[i].width / 2;
			myp.y = boundRect[i].y;

			drawContours(img, conPoly, i, Scalar(0, 0, 0), 2);
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 255, 255), 5);

		}

		

	}
	return myp;



}













vector<vector<int>>findcolor(Mat img)
{
	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	for (int i = 0; i < myColor.size(); i++)
	{

		Scalar lower(myColor[i][0] , myColor[i][1], myColor[i][2]);
		Scalar upper(myColor[i][3], myColor[i][4], myColor[i][5]);
		Mat mask;
		inRange(imgHSV, lower, upper, mask);
		imshow(to_string(i), mask);
		getCountor(mask);

		Point mypoint = getCountor(mask);
		circle(img,mypoint,10,Scalar(255,0,255), FILLED);

		if (mypoint.x != 0) {
			lastp.push_back({ mypoint.x,mypoint.y,i });
		}
	}
	return lastp;
}


void draww(vector<vector<int>>lastp, vector<Scalar>myColorvl) {

	for (int i = 0; i < lastp.size(); i++)
	{
		circle(img, Point(lastp[i][0], lastp[i][1]), 10, myColorvl[lastp[i][2]], FILLED);

	}

}

	

void main() {
	
	VideoCapture pat(0);
	


	while (true)
	{

		
		pat.read(img);
		lastp=findcolor(img);
		draww(lastp,myColorvl);
		
		imshow("n",img);

		waitKey(1);

	}


}