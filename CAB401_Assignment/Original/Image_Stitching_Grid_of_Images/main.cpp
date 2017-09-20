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
Mat gray_image_1;
Mat gray_image_2;
Mat first_image;
Mat second_image;
Mat result;
Mat H;
Mat ROI;
Mat cropped;
Mat crop_image;
Mat crop_image_gray;
Mat img_otsu;
Mat half;

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

// Global int, string and bool
int i = 0;
int j = 0;
int k = 0;
int image_count = 0;
double angle;
string file_name;
string file_name_cropped;
string part_one;
string part_two;
int numRow;
int numCol;
bool useToOriginImages = true;
int currentHeight;
int currentWidth;
// User input global variables
int columns;
int rows;
bool userInputError = false;
string rowsEqualColumnsInput;
bool sameColAndRow;
int imageAmountPerRow[] = {};

/*
* Requests how many images the user wants stitched together
* Pre: Waits for user input
* Post: Uses user's input for the number of images, columns and rows that are required to be stitched together
*/
void userInput() {
    // How many columns
    std::cout << "What is the maximum number of columns?:" << std::endl;
    std::cin >> columns;
    if(std::cin.fail()) {
        userInputError = true;
    }
    // How many rows
    std::cout << "What is the maximum number of rows?: " << std::endl;
    std::cin >> rows;
    if(std::cin.fail()) {
        userInputError = true;
    }
    // Are there the same amount of images in each column and row
    std::cout << "For each column and row, are the number of images the same?: Y/N" << std::endl;
    std::cin >> rowsEqualColumnsInput;
    if(rowsEqualColumnsInput == "Y" || rowsEqualColumnsInput == "y") {
        // How many images in a row
        std::cout << "How many images are in a row?:" << std::endl;
        std::cin >> imageAmountPerRow[1];
    }
    else if(rowsEqualColumnsInput == "N" || rowsEqualColumnsInput == "n") {
        sameColAndRow = false;
        // How many images in each row
        for(numRow = 1; numRow <= rows; numRow++) {
            std::cout << "How many images are in row " << numRow << "?:" << std::endl;
            std::cin >> imageAmountPerRow[numRow];
            if(std::cin.fail()) {
                userInputError = true;
            }
        }
    }
    else {
        userInputError = true;
    }
}

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
void loadImages() {
    // Set which image to load and from where
    // If the first image of the set needs to be used
    if(useToOriginImages == true) {
        // Find image name based on column and row number
        std::stringstream s1;
        s1 << numRow;
        part_one = s1.str();
        std::stringstream s2;
        s2 << numCol;
        part_two = s2.str();
        file_name = "Images/Satelite Images/" + part_one + part_two + ".jpg";
        first_image = imread(file_name);
        useToOriginImages = false;
    }
    // If the first image of the set has all ready been used
    else {
        // Find image name based on column and row number
        std::stringstream s1;
        s1 << numRow;
        part_one = s1.str();
        int nextColumn = numCol - 1;
        std::stringstream s2;
        s2 << nextColumn;
        part_two = s2.str();
        file_name = part_one + part_two + "_cropped.jpg";
        first_image = imread(file_name);
    }
    currentHeight = first_image.rows;
    currentWidth = first_image.cols;
    // Find image name based on column and row number
    std::stringstream s1;
    s1 << numRow;
    part_one = s1.str();
    int nextColumn = numCol + 1;
    std::stringstream s2;
    s2 << nextColumn;
    part_two = s2.str();
    file_name = "Images/Satelite Images/" + part_one + part_two + ".jpg";
    second_image = imread(file_name);
    currentWidth = currentWidth + second_image.cols;
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
    if(!gray_image_1.data) {
        std::cout << "Error Reading Image " << numRow << numCol << std::endl;
        return 0;
    }
    if(!gray_image_2.data) {
        std::cout << "Error Reading Image " << numRow << numCol + 1 << std::endl;
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
    cv::Mat half(result, cv::Rect(0, 0, first_image.cols, first_image.rows));
    first_image.copyTo(half);
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
    std::stringstream s1;
    s1 << numRow;
    part_one = s1.str();
    std::stringstream s2;
    s2 << numCol;
    part_two = s2.str();
    file_name = part_one + part_two + "_worked.jpg";
    imwrite(file_name, result, compression_params);
}

/*
* Takes the image produce from the image stitching, identifies black addition to image and removes it
* Pre: Image with black pixels area is read
* Post: Black pixels area cropped out and written
*/
void cropBlack() {
    // Read images
    crop_image = imread(file_name);
    cvtColor(crop_image, crop_image_gray, CV_RGB2GRAY);
    cv::threshold(crop_image_gray, img_otsu, 0.0, 255.0, cv::THRESH_BINARY + cv::THRESH_OTSU);
    // Crop dimensions
    int startX = 0;
    int startY = 0;
    int width = currentWidth;
    // Find how many black pixels in a row
    for (int pixel = ((3 * currentWidth) / 4); pixel < img_otsu.cols; pixel++) {
        // not finding black pixels
        int identifyPixel = img_otsu.at<uchar>(1, pixel);
        if (identifyPixel == 0) {
            width = width - 1;
        }
    }
    int height = currentHeight;
    // Rectangle Region of Interest (roi)
    Mat ROI(crop_image, Rect(startX, startY, width, height));
    // Copy data to matrix
    ROI.copyTo(cropped);
    setImageQuality();
    file_name_cropped = part_one + part_two + "_cropped.jpg";
    imwrite(file_name_cropped, cropped, compression_params);
}

/*
* Rotates two images
* Pre: Called from main()
* Post: Rotates images -90 degrees to allow for image stitching to occur
*/
//int rotateImage() {
//    if(k == 2) {
//        // Loads the 1st Input Image and Prepares a Grayscale of the Image
//        image_1_input = complete_image;
//        k++;
//        file_name = "Completed.jpg";
//    }
//    if(k == 1) {
//        // Loads the 1st Input Image and Prepares a Grayscale of the Image
//        image_1_input = second_image;
//        k++;
//        file_name = "1112_rotate.jpg";
//    }
//    if(k == 0) {
//        // Loads the 1st Input Image and Prepares a Grayscale of the Image
//        image_1_input = first_image;
//        k++;
//        file_name = "910_rotate.jpg";
//    }
//    if(k > 0 && k <= 2) {
//        // Rotate
//        double angle = -90;
//    }
//    if(k == 3) {
//        // Rotate
//        double angle = 90;
//        k++;
//    }
//    if(k < 0 && k > 4) {
//       std::cout << "int k error: stopping program" << std::endl;
//    return 0;
//    }
//    // get rotation matrix for rotating the image around its center
//    Point2f centerOne(image_1_input.cols / 2.0, image_1_input.rows / 2.0);
//    Mat rotOne = getRotationMatrix2D(centerOne, angle, 1.0);
//    // determine bounding rectangle
//    Rect boxOne = RotatedRect(centerOne, image_1_input.size(), angle).boundingRect();
//    // adjust transformation matrix
//    rotOne.at<double>(0, 2) += boxOne.width / 2.0 - centerOne.x;
//    rotOne.at<double>(1, 2) += boxOne.height / 2.0 - centerOne.y;
//    warpAffine(image_1_input, image_1, rotOne, boxOne.size());
//    // Write image
//    setImageQuality();
//    imwrite(file_name, image_1, compression_params);
//}

/*
* Clears data in Mats used for input images, grayscale of input images, image to crop and result image
* Pre: Two images were stitched together
* Post: continues to main()
*/
void clearAll() {
    gray_image_1.release();
    gray_image_2.release();
    first_image.release();
    second_image.release();
    result.release();
    H.release();
    ROI.release();
    cropped.release();
    crop_image_gray.release();
    img_otsu.release();
    crop_image.release();
    half.release();
    keypoints_object, keypoints_scene;
    descriptors_object.release(), descriptors_scene.release();
}

/*
* Image stitching program using OpenCV. Takes multiple images and stitches them together until grid of images form one image
* Pre: Program executed
* Post: Final image produced with he time taken to produce the result
*/
int main() {
    // Takes user input
    userInput();
    // Check for user input errors
    if(userInputError == true) {
        std::cout << "User failed to input an integer or appropriate character" << std::endl;
        return 0;
    }
    // Stitch a set of images together in each row
    for(numRow = 1; numRow <= rows; numRow++) {
        useToOriginImages = true;
        std::cout << "Stitching images for row " << numRow << std::endl;
        for(numCol = 1; numCol <= imageAmountPerRow[numRow]; numCol++) {
            loadImages();
            convertGrayscale();
            imageCheck();
            imageStitch();
            cropBlack();
            clearAll();
        }
    }
    waitKey(0);
    return 0;
 }
