#include "terrain_recognition.h"

#include "opencv2/opencv.hpp"
#include "image_recognition.h"


TerrainRecognition::TerrainRecognition(std::string json_config_file_name)
{
  JsonConfig json(json_config_file_name);
  this->json_config = json.result;
}

TerrainRecognition::TerrainRecognition(Json::Value json_config)
{
  this->json_config = json_config;
}

TerrainRecognition::~TerrainRecognition()
{

}

int TerrainRecognition::run()
{
    unsigned int width  = json_config["input_width"].asInt();
    unsigned int height = json_config["input_height"].asInt();
    unsigned int camera_id = json_config["camera_id"].asInt();


    //cv::VideoCapture camera(camera_id); // open the default camera

    cv::VideoCapture camera("http://80.242.36.63/mjpg/video.mjpg?resolution=1024x768&fps=1");

    //cv::VideoCapture camera("http://80.242.36.69/mjpg/video.mjpg?resolution=640x480&fps=1");

    if (!camera.isOpened())  // check if we succeeded
      return -1;

    camera.set(CV_CAP_PROP_FRAME_WIDTH,width);
    camera.set(CV_CAP_PROP_FRAME_HEIGHT,height);

    width = camera.get(CV_CAP_PROP_FRAME_WIDTH);
    height = camera.get(CV_CAP_PROP_FRAME_HEIGHT);

    ImageRecognition image_recognition(json_config["image_recognition"], width, height);

    while (1)
    {
      char c=(char)cv::waitKey(25);
      if(c==27)
        break;


      cv::Mat input;
      camera >> input;

      if (input.empty())
        break;

      auto vect = mat_to_vect(input, width, height);

      image_recognition.process(vect);

      auto output = vect_to_mat(vect, width, height);

      if (json_config["visualisation"].asBool())
      {
        for (unsigned int y = 0; y < image_recognition.result.size(); y++)
        for (unsigned int x = 0; x < image_recognition.result[y].size(); x++)
        {
          auto item = image_recognition.result[y][x];
          std::string label = std::to_string(item.class_id());
          cv::putText(output, label, cvPoint(item.x(), item.y()),
          cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(100,100,100), 1, CV_AA);
        }

        cv::imshow("Frame", output);

      }
    }


    return 0;
}


std::vector<float> TerrainRecognition::mat_to_vect(cv::Mat &mat, unsigned int width, unsigned int height)
{
  std::vector<float> result(3*height*width);

  for (unsigned int y = 0; y < height; y++)
  for (unsigned int x = 0; x < width; x++)
  {
    cv::Vec3b intensity = mat.at<cv::Vec3b>(y, x);
    unsigned char blue  = intensity.val[0];
    unsigned char green = intensity.val[1];
    unsigned char red   = intensity.val[2];

    result[(0*height + y)*width + x] = red/256.0;
    result[(1*height + y)*width + x] = green/256.0;
    result[(2*height + y)*width + x] = blue/256.0;
  }

  return result;
}

cv::Mat TerrainRecognition::vect_to_mat(std::vector<float> &v, unsigned int width, unsigned int height)
{
  cv::Mat result(height, width, CV_8UC3);


  for (unsigned int y = 0; y < height; y++)
  for (unsigned int x = 0; x < width; x++)
  {
    result.at<cv::Vec3b>(y, x)[2] = v[(0*height + y)*width + x]*256;
    result.at<cv::Vec3b>(y, x)[1] = v[(1*height + y)*width + x]*256;
    result.at<cv::Vec3b>(y, x)[0] = v[(2*height + y)*width + x]*256;
  }


  return result;
}
