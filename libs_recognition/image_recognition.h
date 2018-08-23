#ifndef _IMAGE_RECOGNITION_H_
#define _IMAGE_RECOGNITION_H_


#include <json_config.h>
#include <cnn.h>

class ImageRecognition
{
  protected:
    Json::Value json_config;

    unsigned int min_x, max_x;
    unsigned int min_y, max_y;
    unsigned int stride_x, stride_y;
    unsigned int width, height;
    unsigned int width_crop, height_crop;

    unsigned int network_input_width, network_input_height;


    unsigned int classes_count;

    std::vector<std::vector<float>> palette;

  public:
    std::vector<std::vector<int>> result;

  public:
    ImageRecognition(Json::Value json_config, unsigned int width, unsigned int height);
    virtual ~ImageRecognition();

    void process(std::vector<float> &image);

    unsigned int get_classes_count()
    {
      return classes_count;
    }


  protected:
    void put_pixel(std::vector<float> &image, unsigned int y, unsigned int x, float r, float g, float b);
    std::vector<float> get_pixel(std::vector<float> &image, unsigned int y, unsigned int x);

    void init_palette();


  protected:
    CNN *nn;

    int nn_process(std::vector<float> &image, unsigned int y, unsigned int x);
    unsigned int argmax(std::vector<float> &v);

};


#endif
