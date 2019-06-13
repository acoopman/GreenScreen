#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
  if(argc < 3)
    {
      printf("Usage: >> ./green green_screen.mp4 background.mp4\n");
      return 1;
    }
  
  //make the videocapture classes
  VideoCapture green_cap;
  VideoCapture back_cap;

  //read in a file
  green_cap.open(argv[1]);
  
  if(!green_cap.isOpened())  // check if we succeeded
    return -1;
  
  back_cap.open(argv[2]); 
  
  if(!back_cap.isOpened())  // check if we succeeded
    return -1;
    
  Mat green_frame;
  Mat back_frame;
  int pixel;
  int green_thresh = 160;
  int blue_thresh = 175;
  int red_thresh = 175;
  int height = 0;
  int width = 0;

  //capture 1 frame to get input size
  green_cap >> green_frame;
  height  = green_frame.rows; // 720p x 1280
  width = green_frame.cols;

  //open a video_out so we can write the output to file
  VideoWriter video_out("output.avi",CV_FOURCC('M','J','P','G'),25, Size(width,height*1.5));
  
  while(1)
    {
      //get the frames in
      green_cap >> green_frame;
      back_cap >> back_frame;

      //make sure the frames actually are read
      if( (green_frame.data == NULL) || (back_frame.data == NULL))
	{
	  break;
	}

      //create resized frames
      Mat small_green(height/2, width/2, 3);
      Mat small_back(height/2, width/2, 3);
      cv::resize(green_frame, small_green, cv::Size(), 0.5, 0.5);
      cv::resize(back_frame, small_back, cv::Size(), 0.5, 0.5);

      //set pointers to find the BGR of the pixel
      uint8_t* green_ptr = green_frame.data;
      uint8_t* back_ptr = back_frame.data;
    
      //printf("image height x width = %i, %i\n", height, width); 
      for(int i = 0; i< height; i++)
	{
	  for(int j = 0; j< width; j++)
	    {
	      //input is BGR
	      pixel = j+i*width;
	      uint8_t blue_value  = green_ptr[3 * pixel + 0];
	      uint8_t green_value = green_ptr[3 * pixel + 1];
	      uint8_t red_value   = green_ptr[3 * pixel + 2];

	      //make a threshold for green, and a maximum threshold for red and blue
	      //threshold for red and blue needed for very light colors such as white where every color is at a high value
	      if(green_value > green_thresh && red_value < red_thresh && blue_value < blue_thresh)
		{
		  green_ptr[3 * pixel + 0] = back_ptr[3 * pixel + 0];
		  green_ptr[3 * pixel + 1] = back_ptr[3 * pixel + 1];
		  green_ptr[3 * pixel + 2] = back_ptr[3 * pixel + 2];
		}
	    }
	}

      //Making a final output.
      Mat final_frame(height * 1.5, width, 3);
      Mat bottom(height/2, width, 3);
      cv::hconcat(small_green, small_back, bottom);
      cv::vconcat(green_frame, bottom, final_frame);
      imshow("final output",final_frame);

      //write final output to file for this frame
      video_out.write(final_frame);

      //press q to quit -------------------------------------------
      int key = waitKey(30);
      if(key == 'q') //quit	  
	break;
     
    }//while end

  
  // When everything done, release the video capture and write object
  green_cap.release();
  back_cap.release();
  video_out.release();
  
  // Closes all the windows
  destroyAllWindows();
  
  
  return 0;
}


