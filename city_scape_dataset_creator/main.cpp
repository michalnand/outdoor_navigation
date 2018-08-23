#include <iostream>

#include <cityscapes/cs_label_test.h>
#include <cityscapes/cs_parse_file.h>
#include <cityscapes/cs_dataset_create.h>

#include <image_load.h>

/*
#include <CImg.h>

void image_save(std::string file_name, ImageLoad &input_image)
{
  cimg_library::CImg<float> output_image(input_image.width(), input_image.height(), 1, 3, 0);

  std::vector<float> pixel(3);

  if (input_image.grayscale())
  {
    for (unsigned int y = 0; y < input_image.height(); y++)
    for (unsigned int x = 0; x < input_image.width(); x++)
    {
      pixel[0] = input_image.get()[y*input_image.width() + x];
      pixel[1] = pixel[0];
      pixel[2] = pixel[0];

      output_image.draw_point(x, y, &pixel[0]);
    }
  }
  else
  {
      for (unsigned int y = 0; y < input_image.height(); y++)
      for (unsigned int x = 0; x < input_image.width(); x++)
      {
        pixel[0] = input_image.get()[(0*input_image.height() + y)*input_image.width() + x];
        pixel[1] = input_image.get()[(1*input_image.height() + y)*input_image.width() + x];
        pixel[2] = input_image.get()[(2*input_image.height() + y)*input_image.width() + x];

        output_image.draw_point(x, y, &pixel[0]);
      }
  }

  output_image.normalize(0, 255);
  output_image.save(file_name.c_str());
}
*/

int main()
{
  srand(time(NULL));

  {
    std::string config_file_name = "testing_config.json";
    CSDatasetCreate dataset_create(config_file_name);

    dataset_create.process();
  }

  {
    std::string config_file_name = "training_config.json";
    CSDatasetCreate dataset_create(config_file_name);

    dataset_create.process();
  }


  printf("program done\n");
  return 0;
}
