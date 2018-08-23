#ifndef _TERRAIN_RECOGNITION_H_
#define _TERRAIN_RECOGNITION_H_

#include <json_config.h>
#include "opencv2/opencv.hpp"

class TerrainRecognition
{
  protected:
    Json::Value json_config;

  public:
    TerrainRecognition(std::string json_config_file_name);
    TerrainRecognition(Json::Value json_config);
    virtual ~TerrainRecognition();

  public:
    int run();

  protected:
    std::vector<float> mat_to_vect(cv::Mat &mat, unsigned int width, unsigned int height);
    cv::Mat vect_to_mat(std::vector<float> &v, unsigned int width, unsigned int height);


};

#endif
