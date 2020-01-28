#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <stdio.h>
#include <iostream>
#include <ctype.h>

using namespace std;
using namespace cv;
Point2f mousse_point;
int nb_points = 0;
bool addRemovePt =false;
vector<Point2f> pts_list;
int thr =230; // ajust threshold to detect white here

static void help()
{
    // print a welcome message, and the OpenCV version
    cout <<  "This is line threshold tracker - Antoine Barbot a.barbot@imperial.ac.uk 07/2019"<< endl;
    cout <<  "You can click on the video to add tracking lines"<< endl;
    cout << " The progmamme will return the location of the last point cutting the threshold"<< endl;
    cout << " on theses lines in log.csv. Thresold is define by global variable thr"<< endl;
    cout << " it was developed using  OpenCV version 3.4.3 and is currently running with" << CV_VERSION << endl;
    cout << "\n you need provide a path to a video as an argument.";
    cout << "\nHot keys: \n"
    "\tESC - quit the program\n"
    "\ts - strart tracking\n"
    "\t click to add point \n"
    << endl;
}

static void onMouse( int event, int x, int y, int /*flags*/, void* /*param*/ )
{
    if( event == EVENT_LBUTTONDOWN )
    {
        //nb_points += 1;
        mousse_point = Point2f((float)x, (float)y);
        //       vector<Point2f> tmp;
        //       tmp.push_back(point);
        //       cornerSubPix( gray, tmp, winSize, Size(-1,-1), termcrit);
        //       points[1].push_back(tmp[0]);
        //       addRemovePt = false;
        addRemovePt = true;
        cout << "clik" << endl;
    }
}

// detect the point to cut threshold the most to the right
Point2f max_line( int y_position, int threshold, Mat image)
{
    //cout<<"col "<<image.cols<<endl;
    //cout<<"row "<<image.rows<<endl;
    int x_size = image.cols;
    int y_size = image.rows;
    int width = image.cols; // columns are Y axis
    int i=0;
    int k=0;
    int pixel, prev_pixel;
    int min = 255;
    int max =0;
    pixel=int(image.at<unsigned char>(y_position,0));
    for (i=1; i<width; i++) { // for every ellement of the row det
        //cout <<"y_position"<<y_position <<endl;
        prev_pixel = pixel;
        //pixel=int(image.at<Vec3b>(y_position-int(y_size/2),int(i-x_size/2))[1]);
        pixel=int(image.at<unsigned char>(y_position,i));
        if (pixel < min) {min=pixel;}
        if (pixel > max) {max=pixel;}
        //cout << pixel <<endl;
        if ( (prev_pixel>threshold and pixel<=threshold) or (prev_pixel<threshold and pixel>=threshold))
            {
                k=i; // store the position at wich the threshold is crossed
            }
        
    }
    cout << "min:" << min << "  max:"<<max<<endl;
    //getchar();
    return Point2f(int(k), (int)y_position);
}
int main( int argc, char** argv ){
    
  // declares all required variables
  Mat frame, grey,image;
  // set input video
  std::string video = argv[1];
  VideoCapture cap(video);
  bool tracking = true;
  Point2f tr_pt;
//log file
 FILE * pFile;
 pFile = fopen ("log.csv","w");
    
help();
    
    namedWindow( "lines track", 1 );
    setMouseCallback( "lines track", onMouse, 0 );

    for ( ;; ){
    if (tracking) // s putton can be pressed to pause
    {
    // get frame from the video
    cap >> frame;
    if(frame.rows==0 || frame.cols==0)
    {
        fclose (pFile);
        break;
    }
    frame.copyTo(image);
    cvtColor(image, grey, COLOR_BGR2GRAY);
    if (pts_list.size()>0)
    {
        // stop the program if no more images
        for (int i =0; i<pts_list.size();i++)
            {
                tr_pt = max_line(pts_list[i].y,thr,grey);
                circle( grey, tr_pt, 5, Scalar(0,0,255), 1, 8);
                fprintf (pFile, "point %d,%f,%f",i,tr_pt.x,tr_pt.y);
            }
        fprintf(pFile,"\n")
        ;
        
    }
    //cout << addRemovePt<< endl;
    if (addRemovePt)
      {
          pts_list.push_back(mousse_point);
          addRemovePt =false;
          cout << "add point"<<pts_list<< endl;
          
      }

    // show image with the tracked object
    imshow("lines track",grey);

    } // end if tracking true
    //quit on ESC button
      char c = (char)waitKey(50);
      if( c == 27 )
          break;
      switch( c )
      {
          case 's': // pause if s pressed
              tracking = !tracking;
	      if (tracking) { cout << "tracking running ... \n"<< endl;}
              else { cout << "video is paused \n "<< endl;}
              break;
      }
  }//for loop
  return 0;
}
