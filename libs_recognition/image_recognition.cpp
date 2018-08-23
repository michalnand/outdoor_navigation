#include "image_recognition.h"

#include <math.h>


ImageRecognition::ImageRecognition(Json::Value json_config, unsigned int width, unsigned int height)
{
  this->json_config = json_config;

  this->width       = width;
  this->height      = height;

  this->stride_x  = this->json_config["stride_x"].asInt();
  this->stride_y  = this->json_config["stride_y"].asInt();

  this->min_x     = this->json_config["min_x"].asInt();
  this->min_y     = this->json_config["min_y"].asInt();
  this->max_x     = this->json_config["max_x"].asInt();
  this->max_y     = this->json_config["max_y"].asInt();


  this->network_input_height = this->json_config["network_input_height"].asInt();
  this->network_input_width = this->json_config["network_input_width"].asInt();

  if (min_x < network_input_width/2)
    min_x = network_input_width/2;
  if (min_y < network_input_height/2)
    min_y = network_input_height/2;

  if (max_x > width - network_input_width/2)
    max_x = width - network_input_width/2;
  if (max_y > height - network_input_height/2)
    max_y = height - network_input_height/2;

  this->classes_count = 4;

  width_crop  = this->max_x - this->min_x;
  height_crop = this->max_y - this->min_y;

  result.resize(height_crop/stride_y);


  for (unsigned int j = 0; j < result.size(); j++)
  {
    result[j].resize(width_crop/stride_x);

    for (unsigned int i = 0; i < result[j].size(); i++)
      result[j][i] = -1;
  }

  init_palette();

  nn = nullptr;

  nn = new CNN(this->json_config["cnn_config"].asString());
}

ImageRecognition::~ImageRecognition()
{
  if (nn != nullptr)
    delete nn;
}

void ImageRecognition::process(std::vector<float> &image)
{
  for (unsigned int y = min_y; y < max_y; y+= stride_y)
  {
    for (unsigned int x = min_x; x < max_x; x+= stride_x)
    {
      int result = nn_process(image, y, x);

      auto color = palette[result];

      for (unsigned int ky = 0; ky < network_input_height; ky++)
      for (unsigned int kx = 0; kx < network_input_width; kx++)
      {
        auto original = get_pixel(image, y + ky, x + kx);

        float k = 0.5;

        put_pixel(image, y + ky, x + kx,  ((1.0 - k)*color[0] + k*original[0]),
                                          ((1.0 - k)*color[1] + k*original[1]),
                                          ((1.0 - k)*color[2] + k*original[2]));
      }
    }
  }
}


int ImageRecognition::nn_process(std::vector<float> &image, unsigned int y, unsigned int x)
{
  std::vector<float> input(3*network_input_height*network_input_width);

  for (unsigned int ky = 0; ky < network_input_height; ky++)
  for (unsigned int kx = 0; kx < network_input_width; kx++)
  {
    auto original = get_pixel(image, y + ky, x + kx);
    input[(0*network_input_height + ky)*network_input_width + kx] = original[0];
    input[(1*network_input_height + ky)*network_input_width + kx] = original[1];
    input[(2*network_input_height + ky)*network_input_width + kx] = original[2];
  }

  std::vector<float> output(get_classes_count());

  nn->forward(output, input);

  return argmax(output);
}



void ImageRecognition::put_pixel(std::vector<float> &image, unsigned int y, unsigned int x, float r, float g, float b)
{
  image[(0*height + y)*width + x] = r;
  image[(1*height + y)*width + x] = g;
  image[(2*height + y)*width + x] = b;
}

std::vector<float> ImageRecognition::get_pixel(std::vector<float> &image, unsigned int y, unsigned int x)
{
  std::vector<float> result(3);

  result[0] = image[(0*height + y)*width + x];
  result[1] = image[(1*height + y)*width + x];
  result[2] = image[(2*height + y)*width + x];

  return result;
}

void ImageRecognition::init_palette()
{
  unsigned int count = get_classes_count();


  float PI = 3.141592654;

  for (unsigned int i = 0; i < count; i++)
  {
    std::vector<float> rgb(3);

    rgb[0] = fabs(sin(i*PI/count + PI*0.0/3.0));
    rgb[1] = fabs(sin(i*PI/count + PI*1.0/3.0));
    rgb[2] = fabs(sin(i*PI/count + PI*2.0/3.0));

    palette.push_back(rgb);
  }
}

unsigned int ImageRecognition::argmax(std::vector<float> &v)
{
  unsigned int res = 0;

  for (unsigned int i = 0; i < v.size(); i++)
    if (v[i] > v[res])
      res = i;

  return res;
}
