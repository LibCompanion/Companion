#include <opencv2/opencv.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <time.h>

using namespace cv;
using namespace cv::ml;
using namespace std;

void sample_neg( const vector< Mat > & full_neg_lst, vector< Mat > & neg_lst, const Size & size ) {
    Rect box;
    box.width = size.width;
    box.height = size.height;

    const int size_x = box.width;
    const int size_y = box.height;

    srand((unsigned int) time(NULL));

    vector<Mat>::const_iterator img = full_neg_lst.begin();
    vector<Mat>::const_iterator end = full_neg_lst.end();
    for (; img != end; ++img) {
        box.x = rand() % (img->cols - size_x);
        box.y = rand() % (img->rows - size_y);
        Mat roi = (*img)(box);
        neg_lst.push_back(roi.clone());
    }
}


void load_images( const string & prefix, const string & filename, vector< Mat > & img_lst ) {
    string line;
    ifstream file;
    bool end_of_parsing = false;

    file.open( (prefix+filename).c_str() );
    if(!file.is_open()) {
        cerr << "Unable to open the list of images from " << filename << " filename." << endl;
        exit( -1 );
    }

    while(!end_of_parsing) {
        getline( file, line );
        if(line.empty()) {
            end_of_parsing = true;
        } else {
            Mat img = imread( (prefix+line).c_str()); // load the image
            if(!img.empty()) {
                img_lst.push_back( img.clone() );
            }
        }
    }
}

void compute_hog(const vector<Mat> &img_lst, vector<Mat> &gradient_lst, const Size &size) {

    HOGDescriptor hog;
    hog.winSize = size;
    Mat gray;
    vector< Point > location;
    vector< float > descriptors;

    vector< Mat >::const_iterator img = img_lst.begin();
    vector< Mat >::const_iterator end = img_lst.end();
    for( ; img != end ; ++img )
    {
        cvtColor( *img, gray, COLOR_BGR2GRAY );
        hog.compute( gray, descriptors, Size( 8, 8 ), Size( 0, 0 ), location );
        gradient_lst.push_back( Mat( descriptors ).clone() );
    }
}

void get_svm_detector(const Ptr<SVM>& svm, vector< float > & hog_detector )
{
    // get the support vectors
    Mat sv = svm->getSupportVectors();
    const int sv_total = sv.rows;
    // get the decision function
    Mat alpha, svidx;
    double rho = svm->getDecisionFunction(0, alpha, svidx);

    CV_Assert( alpha.total() == 1 && svidx.total() == 1 && sv_total == 1 );
    CV_Assert( (alpha.type() == CV_64F && alpha.at<double>(0) == 1.) ||
               (alpha.type() == CV_32F && alpha.at<float>(0) == 1.f) );
    CV_Assert( sv.type() == CV_32F );
    hog_detector.clear();

    hog_detector.resize(sv.cols + 1);
    memcpy(&hog_detector[0], sv.ptr(), sv.cols*sizeof(hog_detector[0]));
    hog_detector[sv.cols] = (float)-rho;
}


/*
* Convert training/testing set to be used by OpenCV Machine Learning algorithms.
* TrainData is a matrix of size (#samples x max(#cols,#rows) per samples), in 32FC1.
* Transposition of samples are made if needed.
*/
void convert_to_ml(const std::vector< cv::Mat > & train_samples, cv::Mat& trainData )
{
    //--Convert data
    const int rows = (int)train_samples.size();
    const int cols = (int)std::max( train_samples[0].cols, train_samples[0].rows );
    cv::Mat tmp(1, cols, CV_32FC1); //< used for transposition if needed
    trainData = cv::Mat(rows, cols, CV_32FC1 );
    vector< Mat >::const_iterator itr = train_samples.begin();
    vector< Mat >::const_iterator end = train_samples.end();
    for( int i = 0 ; itr != end ; ++itr, ++i )
    {
        CV_Assert( itr->cols == 1 ||
                   itr->rows == 1 );
        if( itr->cols == 1 )
        {
            transpose( *(itr), tmp );
            tmp.copyTo( trainData.row( i ) );
        }
        else if( itr->rows == 1 )
        {
            itr->copyTo( trainData.row( i ) );
        }
    }
}



void train_svm( const vector< Mat > & gradient_lst, const vector< int > & labels )
{

    Mat train_data;
    convert_to_ml( gradient_lst, train_data );

    clog << "Start training...";
    Ptr<SVM> svm = SVM::create();
    /* Default values to train SVM */
    svm->setCoef0(0.0);
    svm->setDegree(3);
    svm->setTermCriteria(TermCriteria( CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, 1e-3 ));
    svm->setGamma(0);
    svm->setKernel(SVM::LINEAR);
    svm->setNu(0.5);
    svm->setP(0.1); // for EPSILON_SVR, epsilon in loss function?
    svm->setC(0.01); // From paper, soft classifier
    svm->setType(SVM::EPS_SVR); // C_SVC; // EPSILON_SVR; // may be also NU_SVR; // do regression task
    svm->train(train_data, ROW_SAMPLE, Mat(labels));
    clog << "...[done]" << endl;

    svm->save( "my_people_detector.yml" );
}

void draw_locations( Mat & img, const vector< Rect > & locations, const Scalar & color )
{
    if( !locations.empty() )
    {
        vector< Rect >::const_iterator loc = locations.begin();
        vector< Rect >::const_iterator end = locations.end();
        for( ; loc != end ; ++loc )
        {
            rectangle( img, *loc, color, 2 );
        }
    }
}

void test_it( const Size & size )
{
    char key = 27;
    Scalar reference( 0, 255, 0 );
    Scalar trained( 0, 0, 255 );
    Mat img, draw;
    Ptr<SVM> svm;
    HOGDescriptor hog;
    HOGDescriptor my_hog;
    my_hog.winSize = size;
    VideoCapture video;
    vector< Rect > locations;

    // Load the trained SVM.
    svm = StatModel::load<SVM>("my_people_detector.yml");
    // Set the trained svm to my_hog
    vector< float > hog_detector;
    get_svm_detector( svm, hog_detector );
    my_hog.setSVMDetector( hog_detector );
    // Set the people detector.
    hog.setSVMDetector(hog.getDefaultPeopleDetector());
    // Open the camera.
    video.open("D:/Data/Master/Testcase/INRIA/people.mp4");
    //video.open(0);
    if( !video.isOpened() )
    {
        cerr << "Unable to open the device 0" << endl;
        exit( -1 );
    }

    bool end_of_process = false;
    while( !end_of_process ) {
        video >> img;

        if (!img.empty()) {

            // All black?
            locations.clear();
            my_hog.detectMultiScale(img, locations);
            draw_locations(img, locations, trained);

            imshow("Video", img);
            waitKey(100);
        }



        /*
               draw = img.clone();

               locations.clear();
               hog.detectMultiScale( img, locations );
               draw_locations( draw, locations, reference );

               locations.clear();
               my_hog.detectMultiScale( img, locations );
               draw_locations( draw, locations, trained );

               imshow( "Video", draw );
               waitKey(0);

               key = (char)waitKey( 10 );
               if( 27 == key )
                   end_of_process = true;
                   */
    }
}

int main( int argc, char** argv )
{

    vector< Mat > pos_lst;
    vector< Mat > neg_lst;
    vector< Mat > full_neg_lst;
    vector< Mat > gradient_lst;
    vector< int > labels;
/*
    // Step 1 load positive and negative samples which are same size!!!
    load_images("D:/Data/Master/Testcase/INRIA/96x160/", "pos.lst", pos_lst );
    labels.assign( pos_lst.size(), +1 );
    const unsigned int old = (unsigned int)labels.size();

    load_images("D:/Data/Master/Testcase/INRIA/neg/", "neg.lst", full_neg_lst );
    // If samples are not same size
    sample_neg(full_neg_lst, neg_lst, Size(96,160)); // Only needs if samples are not equal take sub image from it !
    labels.insert(labels.end(), neg_lst.size(), -1 );

    CV_Assert( old < labels.size() );

    // Extract data from samples
    compute_hog(pos_lst, gradient_lst, Size(96, 160));
    compute_hog(neg_lst, gradient_lst, Size(96, 160));

    // If samples are not same size an error will be occurred by converting svm training data
    train_svm( gradient_lst, labels );
*/
    test_it(Size(96, 160)); // change with your parameters

    return 0;
}