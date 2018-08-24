#include "image_recognition_result.h"


ImageRecognitionResult::ImageRecognitionResult()
{
  m_x = 0;
  m_y = 0;
  m_class_id = 0;
}

ImageRecognitionResult::ImageRecognitionResult(ImageRecognitionResult& other)
{
  copy(other);
}

ImageRecognitionResult::ImageRecognitionResult(const ImageRecognitionResult& other)
{
  copy(other);
}

ImageRecognitionResult::ImageRecognitionResult(unsigned int x, unsigned int y, int class_id)
{
  m_x = x;
  m_y = y;
  m_class_id = class_id;
}

ImageRecognitionResult::~ImageRecognitionResult()
{

}

ImageRecognitionResult& ImageRecognitionResult::operator= (ImageRecognitionResult& other)
{
  copy(other);

  return *this;
}

ImageRecognitionResult& ImageRecognitionResult::operator= (const ImageRecognitionResult& other)
{
  copy(other);

  return *this;
}

void ImageRecognitionResult::copy(ImageRecognitionResult& other)
{
  m_x = other.m_x;
  m_y = other.m_y;
  m_class_id = other.m_class_id;
}

void ImageRecognitionResult::copy(const ImageRecognitionResult& other)
{
  m_x = other.m_x;
  m_y = other.m_y;
  m_class_id = other.m_class_id;
}
