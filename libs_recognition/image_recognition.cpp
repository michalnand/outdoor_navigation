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

  if (min_x < network_input_width)
    min_x = network_input_width;
  if (min_y < network_input_height)
    min_y = network_input_height;

  if (max_x > width - network_input_width)
    max_x = width - network_input_width;
  if (max_y > height - network_input_height)
    max_y = height - network_input_height;

  this->classes_count = 4;

  width_crop  = this->max_x - this->min_x;
  height_crop = this->max_y - this->min_y;

  result.resize((height_crop+stride_y)/stride_y);

  ImageRecognitionResult item;
  for (unsigned int j = 0; j < result.size(); j++)
  {
    result[j].resize((width_crop+stride_x)/stride_x);

    for (unsigned int i = 0; i < result[j].size(); i++)
      result[j][i] = item;
  }

  printf(">>>> %u %u \n", result.size(), result[0].size());

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
  unsigned int result_x = 0;
  unsigned int result_y = 0;
  for (unsigned int y = min_y; y < max_y; y+= stride_y)
  {
    result_x = 0;
    for (unsigned int x = min_x; x < max_x; x+= stride_x)
    {
      int class_id = nn_process(image, y, x);

      ImageRecognitionResult item(x, y, class_id);

      result[result_y][result_x] = item;

      result_x++;
    }

    result_y++;
  }

/*
  for (unsigned int y = 0; y < result.size(); y++)
    for (unsigned int x = 0; x < result[y].size(); x++)
    {
      ImageRecognitionResult item = result[y][x];

      auto color = palette[item.class_id()];

      for (unsigned int ky = item.y(); ky < item.y() + network_input_height; ky++)
      for (unsigned int kx = item.x(); kx < item.x() + network_input_width; kx++)
      {
        auto original = get_pixel(image, ky, kx);

        float k = 0.5;

        put_pixel(image, ky, kx,  ((1.0 - k)*color[0] + k*original[0]),
                                  ((1.0 - k)*color[1] + k*original[1]),
                                  ((1.0 - k)*color[2] + k*original[2]));
      }
    }
*/


  for (unsigned int y = 0; y < result.size(); y++)
    for (unsigned int x = 0; x < result[y].size(); x++)
    {
      ImageRecognitionResult point_a, point_b, point_c, point_d;

      point_a = result[y+0][x+0];
      point_b = point_a;
      point_c = point_a;
      point_d = point_a;

      if ((x+1) < result[y].size())
        point_b = result[y+0][x+1];

      if ((y+1) < result.size())
        point_c = result[y+1][x+0];

      if ((y+1) < result.size())
      if ((x+1) < result[y].size())
        point_d = result[y+1][x+1];


      unsigned int ofs_x = point_a.x();
      unsigned int ofs_y = point_a.y();

      for (unsigned int ky = 0; ky < network_input_height; ky++)
      for (unsigned int kx = 0; kx < network_input_width; kx++)
      {
        auto color = interpolate(point_a, point_b, point_c, point_d, ky*1.0/network_input_height, kx*1.0/network_input_width);
        auto original = get_pixel(image, ky + ofs_y, kx + ofs_x);

        float k = 0.5;

        put_pixel(image, ky + ofs_y, kx + ofs_x,  ((1.0 - k)*color[0] + k*original[0]),
                                                              ((1.0 - k)*color[1] + k*original[1]),
                                                              ((1.0 - k)*color[2] + k*original[2]));
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


std::vector<float> ImageRecognition::interpolate( ImageRecognitionResult &point_a,
                                                  ImageRecognitionResult &point_b,
                                                  ImageRecognitionResult &point_c,
                                                  ImageRecognitionResult &point_d,
                                                  float y,
                                                  float x)
{
    std::vector<float> result(3);

    auto color_a = palette[point_a.class_id()];
    auto color_b = palette[point_b.class_id()];
    auto color_c = palette[point_c.class_id()];
    auto color_d = palette[point_d.class_id()];


    float wa = (1.0 - x)*(1.0 - y);
    float wb = x*(1.0 - y);
    float wc = (1.0 - x)*y;
    float wd = x*y;

    for (unsigned int i = 0; i < result.size(); i++)
      result[i] = 0.0;

    for (unsigned int i = 0; i < result.size(); i++)
      result[i]+= wa*color_a[i];

    for (unsigned int i = 0; i < result.size(); i++)
      result[i]+= wb*color_b[i];

    for (unsigned int i = 0; i < result.size(); i++)
      result[i]+= wc*color_c[i];

    for (unsigned int i = 0; i < result.size(); i++)
      result[i]+= wd*color_d[i];

    return result;
}
