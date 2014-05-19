#include <cv.h>
#include <highgui.h>

using namespace cv;

int main( int argc, char** argv )
{
  // Original Image
  Mat image_bgr;
  image_bgr = imread( argv[1], 1 );

  if( argc != 2 || !image_bgr.data )
    {
      printf( "No image data \n" );
      return -1;
    }

  namedWindow( "Original Image", CV_WINDOW_AUTOSIZE );
  imshow( "Original Image", image_bgr );

  // Shadow Removal Algorithm
  Mat image_hsv;
  cvtColor(image_bgr, image_hsv, CV_BGR2HSV);


  waitKey(0);

  return 0;
}
