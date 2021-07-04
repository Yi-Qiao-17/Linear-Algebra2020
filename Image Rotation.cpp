#include<iostream>
#include<algorithm>
#include<fstream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void rotateImage(Mat src, Mat& img_rotate, double degree)
{
    //以圖片中心旋轉
    Point2f center(src.cols / 2, src.rows / 2);
    
    ////<法1>
    //int length = 0;
    //length = sqrt(src.cols * src.cols + src.rows * src.rows);
    ////计算二维旋转的仿射变换矩阵  
    //Mat M = getRotationMatrix2D(center, degree, 1);
    //warpAffine(src, img_rotate, M, Size(length, length), 1, 0, Scalar(0, 0, 0));//仿射变换，背景色填充为黑色  
    
    //<法2>
    Mat rot = getRotationMatrix2D(center, degree, 1.0);
    //Rect box = RotatedRect(center, src.size(), degree).boundingRect(); // get circumscribed rectangle
    warpAffine(src, img_rotate, rot, src.size());
}



double CalcDegree(const Mat& srcImage, Mat& dst)
{
    Mat midImage, dstImage;

    Canny(srcImage, midImage, 500, 100);
    dstImage = midImage;
    //cvtColor(midImage, dstImage, 500);

    
    //執行霍夫線變換，檢測直線
    vector<Vec4i> lines; //存放檢測結果的vector
    
    HoughLinesP(dst, lines, 1, CV_PI / 180, 250, 200, 10);
    

    
    int x1, y1, x2, y2; //直線的兩個端點
    int x, y;  //直線的中點
    double angle, rotate_angle=0; //直線的角度，擺正直線需要旋轉的角度
    double line_length=0; //直線長度
  
    int Mx1=0, Mx2=0, My1=0, My2=0;
   
    //逐個分析各條直線，判斷哪個是主線條
    for (size_t i = 0; i < lines.size(); i++) {
        //取得直線的兩個端點座標
        x1 = lines[i][0]; y1 = lines[i][1]; x2 = lines[i][2]; y2 = lines[i][3];
        double temp = 0.0;
        temp= sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
        if (temp > line_length) {
            line_length = temp;
            Mx1 = x1, Mx2 = x2, My1 = y1, My2 = y2;
        }                
               
    }
    double max_angle = 6; //可旋轉的最大角度
    rotate_angle = (Mx1 == Mx2) ? 90 : (atan((My1 - My2) * 1.0 / (Mx2 - Mx1))) / CV_PI * 180;
    //rotate_angle = atan(fabs(My1 - My2) / fabs(Mx1 - Mx2));
    /*double res = rotate_angle / CV_PI * 180;

    while (res > 360)res -= 360;
    */
    if (rotate_angle < 45)
    {
        // consider in the horizontal direction
        if (My1 > My2)
        {
            rotate_angle = -rotate_angle;
        }
    }
    else
    {
        // consider in the vertical direction
        if (My1 > My2)
        {
            rotate_angle = 90 - rotate_angle;
        }
        else
        {
            rotate_angle = rotate_angle - 90;
        }
    }

   

    
    return rotate_angle;
}

//void ImageRecify(const char* pInFileName, const char* pOutFileName)
//{
//    double degree;
//    Mat src = imread(pInFileName);
//    //namedWindow("mywindow", WINDOW_NORMAL);
//    //imshow("原始图", src);
//    Mat dst;
//    //倾斜角度矫正
//    Canny(src, dst, 500, 100);
//    degree = CalcDegree(src, dst);
//    rotateImage(src, dst, degree);
//    //cout << "angle:" << degree << endl;
//    //namedWindow("output", 0);
//    imshow(argv[2], dst);
//   // imwrite("output.jpg", dst);
//
//    
//}

int main(int argc, char* argv[]) {
   /* ifstream f;
    f.open(argv[1]);
    if (!f) {
        cout << "Jpg File could not be opened" << endl;
        exit(1);
    }*/
	

    //ImageRecify(argv[1]/*"exp2.jpg"*/, "FinalImage.jpg");
    double degree;
    Mat src = imread(argv[1]/*"exp2.jpg"*/);
    //namedWindow("mywindow", WINDOW_NORMAL);
    //imshow("原始图", src);
    Mat dst;
    //倾斜角度矫正
    Canny(src, dst, 500, 100);
    degree = CalcDegree(src, dst);
    rotateImage(src, dst, degree);
    //cout << "angle:" << degree << endl;
    //namedWindow("output", 0);
    //imshow(argv[2]/*"output.jpg"*/, dst);
    imwrite(argv[2]/*"output.jpg"*/, dst);

   
    return 0;


    
	/*namedWindow("mywindow", WINDOW_NORMAL);
    imshow("mywindow", gray);*/
	

}