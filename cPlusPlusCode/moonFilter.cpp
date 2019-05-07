#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<iostream>

using namespace std;
using namespace cv;

//interpolation
void interpolation(uchar* lut, float* curve, float* originalValue){
    for(int i=0; i<256; i++){
        int j=0;
        float a = i;
        while (a>originalValue[j]){
            j++;
        }
        if(a == originalValue[j]){
            lut[i] = curve[j];
            continue;
        }
        float slope = ((float)(curve[j] - curve[j-1]))/((float)(originalValue[j] - originalValue[j-1]));
        float constant = curve[j] - slope * originalValue[j];
        lut[i] = slope * a + constant;
    }
}

int main(){

    //Read input image
    Mat image = imread("../assets/anish.jpg");

    //check if image exists
    if(image.empty()){
        cout<<"can not find image"<<endl;
        return 0;
    }

    //declare variable to store the output
    Mat output;

    //create a copy of the input image to work on
    output = image.clone();

    //covert to Lab color space
    cvtColor(image, output, COLOR_BGR2Lab);

    //split the channels
    vector<Mat> channels;
    split(output, channels);

    //interpolation values
    float originValues[]={0, 15, 30, 50, 70, 90, 120, 160, 180, 210, 255 };
    float values[]=      {0, 0, 5, 15, 60, 110, 150, 190, 210, 230, 255 };

    //create lookup table
    Mat lookuptable(1, 256, CV_8U);
    uchar* lut = lookuptable.ptr();

    //Apply interpolation and create lookup table
    interpolation(lut, values, originValues);

    //Apply mapping for L Channel
    LUT(channels[0], lookuptable, channels[0]);

    //merge back the channels
    merge(channels, output);

    //convert back to BGR color space
    cvtColor(output, output, COLOR_Lab2BGR);

    //Desaturate the image
    //convert to HSV color space
    cvtColor(output, output, COLOR_BGR2HSV);

    //split into channels
    split(output, channels);

    //Multiply S channel by saturation scale value
    channels[1] = channels[1] * 0.01;

    //Limit the pixel values between 0 and 255
    min(channels[1], 255, channels[1]);
    max(channels[1], 0, channels[1]);

    //merge back the channels
    merge(channels, output);

    //convert back to BGR color space
    cvtColor(output, output, COLOR_HSV2BGR);

    //create windows to display images
    namedWindow("image", WINDOW_AUTOSIZE);
    namedWindow("moon", WINDOW_AUTOSIZE);

    //display images
    imshow("image", image);
    imshow("moon", output);

    //press esc to exit the program
    waitKey(0);

    //destroy all the opened windows
    destroyAllWindows();

    return 0;
}