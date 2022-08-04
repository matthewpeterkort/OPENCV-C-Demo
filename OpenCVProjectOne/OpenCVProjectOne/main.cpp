#include <iostream>
#include<opencv2/opencv.hpp>
using namespace cv;

#define RESIZE_X  600
#define RESIZE_Y  600
// this program is basically used for trying to  superimpose 2 images without using the addWeighted function. 
// Also, I hadn't used openCV in C++ so I was mostly playing around with it a bit.
// AddWeighted is used once to show what it would look like forcomparison.
int main()
{
   // any two photos can be used but photos that are close to or over 600x600 work best 
    Mat img_two = cv::imread("C:/Users/matth/Downloads/nature_3.jpg");
    Mat img_three = cv::imread("C:/Users/matth/Downloads/nature_4.jpg");

   
    Mat img_two_resized;
    Mat img_three_resized;
    Mat RESULT;
    resize(img_two, img_two_resized, Size(RESIZE_X, RESIZE_Y), INTER_LINEAR);
    resize(img_three, img_three_resized, Size(RESIZE_X, RESIZE_Y), INTER_LINEAR);
    double beta = (1.0 - 0.5);
    addWeighted(img_two_resized, 0.5, img_three_resized, beta, 0.0, RESULT);

    imshow("IMGONE", RESULT);
    imshow("Base_img_one", img_three_resized);
    imshow("Base_img_two", img_two_resized);


    //imshow("IMGTWO", img_three_resized);
    bool this_value = true;
    int I_VALUE = 0;
    int J_VALUE = 0;
    // checkerboarad the two images together in 3x3 increments
    while (I_VALUE < RESIZE_X) {
        while (J_VALUE < RESIZE_X) {
            for (int i = I_VALUE; i < I_VALUE + (RESIZE_Y / 200); i += 1) {
                for (int j = J_VALUE; j < J_VALUE + (RESIZE_Y / 200); j += 1) {

                    img_three_resized.at<Vec3b>(i, j)[0] = img_two_resized.at<Vec3b>(i, j)[0];
                    img_three_resized.at<Vec3b>(i, j)[1] = img_two_resized.at<Vec3b>(i, j)[1];
                    img_three_resized.at<Vec3b>(i, j)[2] = img_two_resized.at<Vec3b>(i, j)[2];
                    
                }
            }

            J_VALUE += 6;
        }
        if (this_value == true) {
            J_VALUE = 3;
            this_value = false;
        }
        else {
            J_VALUE = 0;
            this_value = true;
        }
        
        I_VALUE += 3;
    }

    // turn up the brightness and the contrast a bit
    double alpha = 1.15;
    int beta_two = 25;
    for (int y = 0; y < img_three_resized.rows; y++) {
        for (int x = 0; x < img_three_resized.cols; x++) {
            for (int c = 0; c < img_three_resized.channels(); c++) {
                img_three_resized.at<Vec3b>(y, x)[c] =
                    saturate_cast<uchar>(alpha * img_three_resized.at<Vec3b>(y, x)[c] + beta_two);
            }
        }
    }

    imshow("First OpenCV Application_FOUR", img_three_resized);
    
    // apply some binary erosion and blur/weighting in an attempt to blend the two images together
    int morph_size = 2;
    Mat element = getStructuringElement(MORPH_RECT, Size(1 * morph_size + 1,1 * morph_size + 1),Point(1, 1));
    Mat erod, dill;
    erode(img_three_resized, erod, element,Point(-1, -1), 1);
    
    GaussianBlur(erod, dill, cv::Size(0, 0), 5);
    addWeighted(erod, 1.5, dill, -0.5, 0, dill);

    // apply one last round of blurring to blend the boxes from teh erosion together a bit better
    GaussianBlur(erod, dill, cv::Size(7,7 ), 0);
    imshow("First OpenCV Application_FOUR_BLURRED_ERODED_SHARPENED", dill);

    cv::waitKey(0);
    destroyAllWindows();
    return 0;
}