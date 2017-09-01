#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>

// From OpenCV-2.4.9
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

// Images
Mat image_1;
Mat image_2;
Mat image_3;
Mat image_4;
Mat image_5;
Mat image_6;
Mat image_7;
Mat image_8;
Mat gray_image_1;
Mat gray_image_2;
Mat first_image;
Mat second_image;
Mat image_set_1;
Mat image_set_2;
Mat image_set_3;
Mat image_set_4;
Mat image_1_input;
Mat result;
Mat complete_image;
Mat H;

// Global variables for descriptor and good matches
Mat descriptors_object, descriptors_scene;
std::vector <KeyPoint> keypoints_object, keypoints_scene;
std::vector <DMatch> matches;
SurfDescriptorExtractor extractor;
FlannBasedMatcher matcher;

// Global variables for good matches and homography
std::vector <Point2f> obj;
std::vector <Point2f> scene;

// Distance
double max_dist = 0;
double min_dist = 100;

// Global image quality and effects
vector <int> compression_params;

// Global int and string
int i = 0;
int j = 0;
int k = 0;
int image_count = 0;
double angle;
string file_name;
string file_name_cropped;
string part_one;
string part_two;

/*
* Sets JPEG image quality to highest possible quality
* Pre: Called before image is written
* Post: imwrite is then performed
*/
void setImageQuality() {
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(100);
}

/*
* Loads each image from directory to respective Mat
* Pre: Called from main()
* Post: Used for final grid image and for convertGrayscale()
*/
int loadImages() {
    if(i == 0) {
        first_image = imread("Images/1.jpg");
        second_image = imread("Images/2.jpg");
    }
    if(i == 1) {
        first_image = imread("Images/3.jpg");
        second_image = imread("Images/4.jpg");
    }
    if(i == 2) {
        first_image = imread("Images/5.jpg");
        second_image = imread("Images/6.jpg");
    }
    if(i == 4) {
        first_image = imread("Images/7.jpg");
        second_image = imread("Images/8.jpg");
    }
    if (i < 0 && i > 4) {
        std::cout << "int i error: stopping program" << std::endl;
        return 0;
    }
}

/*
* Convert to loaded images to grayscale
* Pre: Takes all Mat image_1, image_nth loaded from loadImages()
* Post: Will be used to check image errors in imageCheck()
*/
void convertGrayscale() {
    cvtColor(first_image, gray_image_1, CV_RGB2GRAY);
    cvtColor(second_image, gray_image_2, CV_RGB2GRAY);
}

/*
* Checks if image files can be read
* Pre: Takes all Mat grayscale images and checks if images cannot be read
* Post: If True: an error is shown and program stops
* Post: If False: imageStitch() is called from main()
*/
int imageCheck() {
    image_count++;
    if(!gray_image_1.data) {
        std::cout << "Error Reading Image " << image_count << std::endl;
        return 0;
    }
    image_count++;
    if(!gray_image_2.data) {
        std::cout << "Error Reading Image " << image_count << std::endl;
        return 0;
    }
}

/*
* Create image descriptors based on object and scenes of input images
* Pre: Detects keypoints in grascale images
* Post: Finds matches and proceeds to goodMatches()
*/
void imageDescriptors() {
    // Detect the keypoints using SURF Detector
    // Based from Anna Huaman's 'Features2D + Homography to find a known object' Tutorial
    int minHessian = 50;

    SurfFeatureDetector detector (minHessian);
    detector.detect(gray_image_2, keypoints_object);
    detector.detect(gray_image_1, keypoints_scene);

    // Calculate Feature Vectors (descriptors)
    // Based from  Anna Huaman's 'Features2D + Homography to find a known object' Tutorial
    extractor.compute(gray_image_2, keypoints_object, descriptors_object);
    extractor.compute(gray_image_1, keypoints_scene, descriptors_scene);

    // Matching descriptor vectors using FLANN matcher
    // Based from  Anna Huaman's 'Features2D + Homography to find a known object' Tutorial
    matcher.match(descriptors_object, descriptors_scene, matches);

    // Quick calculation of max and min distances between keypoints
    // Based from  Anna Huaman's 'Features2D + Homography to find a known object' Tutorial
    for (int i = 0; i < descriptors_object.rows; i++) {
        double dist = matches[i].distance;
        if (dist < min_dist) {
            min_dist = dist;
        }
    }
}

/*
* Takes matches found from grayscale images
* Pre: Determines which matches are the best to be used for each image
* Post: homographyImageResult() is called
*/
void goodMatches() {
    // Use matches tha have a distance that is less than 3 * min_dist
    std::vector <DMatch> good_matches;

    for (int i = 0; i < descriptors_object.rows; i++){
        if (matches[i].distance < 3 * min_dist) {
            good_matches.push_back (matches[i]);
        }
    }

    for (int i = 0; i < good_matches.size(); i++) {
        // Get the keypoints from the good matches
        obj.push_back (keypoints_object[good_matches[i].queryIdx].pt);
        scene.push_back (keypoints_scene[good_matches[i].trainIdx].pt);
    }
}

/*
* Stitches two images together
* Pre: Finds the homography from the grayscaled images based on the best matches
* Post: Combines images togetger with combined size dimensions and cropBlack() is called
*/
void homographyImageResult() {
    // Find the Homography Matrix
    H = findHomography(obj, scene, CV_RANSAC);
    // Use the Homography Matrix to warp the images
    warpPerspective(second_image, result, H, cv::Size (second_image.cols + first_image.cols, second_image.rows));
    cv::Mat half(result, cv::Rect (0, 0, first_image.cols, first_image.rows));
    first_image.copyTo(half);
}

/*
* Clears data in Mats used for input images, grayscale of input images, image to crop and result image
* Pre: Two images were stitched together
* Post: continues to main()
*/
void clearMats() {
    gray_image_1.release();
    gray_image_2.release();
    first_image.release();
    second_image.release();
    image_1_input.release();
    result.release();
    H.release();
}

/*
* Calls imageDescriptors(), goodMatches() and homographyImageResult()
* Pre: Two images get stitched through process
* Post: Image written to be used for cropBlack()
*/
void imageStitch() {
    imageDescriptors();
    goodMatches();
    homographyImageResult();
    setImageQuality();
    // Write image
    image_count--;
    std::stringstream s1;
    s1 << image_count;
    part_one = s1.str();
    image_count++;
    std::stringstream s2;
    s2 << image_count;
    part_two = s2.str();
    file_name = part_one + part_two + ".jpg";
    imwrite(file_name, result, compression_params);
}

/*
* Takes the image produce from the image stitching, identifies black addition to image and removes it
* Pre: Image with black pixels area is read
* Post: Black pixels area cropped out and written
*/
void cropBlack() {
    const int threshValue = 20;
     // Set to 5%
    const float borderThresh = 0.05f;
    // Read images
    Mat crop_image = imread(file_name);
    Mat crop_image_gray;
    // Convert to Images to Grayscale
    cvtColor(crop_image, crop_image_gray, CV_RGB2GRAY);
    // Threshold
    Mat thresholded;
    threshold(crop_image_gray, thresholded, threshValue, 255, THRESH_BINARY);
    morphologyEx(thresholded, thresholded, MORPH_CLOSE,
    // Rectangle Structure and Borders
    getStructuringElement(MORPH_RECT, Size(3, 3)),
    Point(-1, -1), 2, BORDER_CONSTANT, Scalar(0));
    Point thresholdLength;
    Point borderRectangle;
    // Search for threshold length rows
    for (int row = 0; row < thresholded.rows; row++) {
        if (countNonZero(thresholded.row(row)) > borderThresh * thresholded.cols) {
            thresholdLength.y = row;
            break;
        }
    }
    // Search for threshold length columns
    for (int col = 0; col < thresholded.cols; col++) {
        if (countNonZero(thresholded.col(col)) > borderThresh * thresholded.rows) {
            thresholdLength.x = col;
            break;
        }
    }
    // Search for border of rectangle rows
    for (int row = thresholded.rows - 1; row >= 0; row--) {
        if (countNonZero(thresholded.row(row)) > borderThresh * thresholded.cols) {
            borderRectangle.y = row;
            break;
        }
    }
    // Search for border of rectangle columns
    for (int col = thresholded.cols - 1; col >= 0; col--) {
        if (countNonZero(thresholded.col(col)) > borderThresh * thresholded.rows) {
            borderRectangle.x = col;
            break;
        }
    }
    // Rectangle Region of Interest (roi)
    Rect roi(thresholdLength, borderRectangle);
    Mat cropped = crop_image(roi);
    setImageQuality();
    file_name_cropped = part_one + part_two + "_cropped.jpg";
    imwrite(file_name_cropped, cropped, compression_params);
    clearMats();
}

/*
* Loads the four stitching cropped images from directory to respective Mat
* Pre: Called from main()
* Post: Used for final grid image and for convertGrayscale()
*/
void loadCombinedImages() {
    image_set_1 = imread("12_cropped.jpg");
    image_set_2 = imread("34_cropped.jpg");
    image_set_3 = imread("56_cropped.jpg");
    image_set_4 = imread("78_cropped.jpg");
}

int prepStitchedImages() {
    if(j == 0) {
        first_image = image_set_1;
        second_image = image_set_2;
    }
    if(j == 1) {
        first_image = image_set_3;
        second_image = image_set_4;
    }
    if (j < 0 && j > 1) {
        std::cout << "int j error: stopping program" << std::endl;
        return 0;
    }
}

/*
* Loads the two halves images
* Pre: Called from main()
* Post: Used for final grid image and for convertGrayscale()
*/
void loadHalfImages() {
    image_set_1 = imread("910_cropped.jpg");
    image_set_2 = imread("1112_cropped.jpg");
    first_image = image_set_1;
    second_image = image_set_2;
}

/*
* Rotates two images
* Pre: Called from main()
* Post: Rotates images -90 degrees to allow for image stitching to occur
*/
int rotateImage() {
    if(k == 2) {
        // Loads the 1st Input Image and Prepares a Grayscale of the Image
        image_1_input = complete_image;
        k++;
        file_name = "Completed.jpg";
    }
    if(k == 1) {
        // Loads the 1st Input Image and Prepares a Grayscale of the Image
        image_1_input = second_image;
        k++;
        file_name = "1112_rotate.jpg";
    }
    if(k == 0) {
        // Loads the 1st Input Image and Prepares a Grayscale of the Image
        image_1_input = first_image;
        k++;
        file_name = "910_rotate.jpg";
    }
    if(k > 0 && k <= 2) {
        // Rotate
        double angle = -90;
    }
    if(k == 3) {
        // Rotate
        double angle = 90;
        k++;
    }
    if(k < 0 && k > 4) {
       std::cout << "int k error: stopping program" << std::endl;
    return 0;
    }
    // get rotation matrix for rotating the image around its center
    Point2f centerOne(image_1_input.cols / 2.0, image_1_input.rows / 2.0);
    Mat rotOne = getRotationMatrix2D(centerOne, angle, 1.0);
    // determine bounding rectangle
    Rect boxOne = RotatedRect(centerOne, image_1_input.size(), angle).boundingRect();
    // adjust transformation matrix
    rotOne.at<double>(0, 2) += boxOne.width / 2.0 - centerOne.x;
    rotOne.at<double>(1, 2) += boxOne.height / 2.0 - centerOne.y;
    warpAffine(image_1_input, image_1, rotOne, boxOne.size());
    // Write image
    setImageQuality();
    imwrite(file_name, image_1, compression_params);
}

/*
* Loads the rotated two images
* Pre: Called from main()
* Post: Used for final grid image and for convertGrayscale()
*/
void loadRotatedImages() {
    image_set_1 = imread("910_rotate.jpg");
    image_set_2 = imread("1112_rotate.jpg");
    first_image = image_set_1;
    second_image = image_set_2;
}

/*
* Loads the rotated two images
* Pre: Called from main()
* Post: Used for final grid image and for convertGrayscale()
*/
void almostCompletedImage() {
    image_set_1 = imread("1314_rotate.jpg");
    complete_image = image_set_1;
}

/*
* Image stitching program using OpenCV. Takes multiple images and stitches them together until grid of images form one image
* Pre: Program executed
* Post: Final image produced with he time taken to produce the result
*/
int main() {
    while(i != 4) {
        loadImages();
        convertGrayscale();
        imageCheck();
        imageStitch();
        cropBlack();
        i++;
    }
    loadCombinedImages();
    while(j != 2) {
        prepStitchedImages();
        convertGrayscale();
        imageCheck();
        imageStitch();
        cropBlack();
        j++;
    }
    loadHalfImages();
    convertGrayscale();
    imageCheck();
    while(k != 2) {
        rotateImage();
    }
    loadRotatedImages();
    convertGrayscale();
    imageStitch();
    almostCompletedImage();
    imageCheck();
    rotateImage();
    cropBlack();
    waitKey(0);
    return 0;
 }
