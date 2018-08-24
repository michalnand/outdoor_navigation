#ifndef _IMAGE_RECOGNTION_RESULT_H_
#define _IMAGE_RECOGNTION_RESULT_H_

class ImageRecognitionResult
{
  private:
    unsigned int m_x, m_y;
    int m_class_id;

  public:
    // Default constructor
    ImageRecognitionResult();

    // Copy constructor
    ImageRecognitionResult(ImageRecognitionResult& other);

    // Copy constructor
    ImageRecognitionResult(const ImageRecognitionResult& other);

    ImageRecognitionResult(unsigned int x, unsigned int y, int class_id);

    // Destructor
    virtual ~ImageRecognitionResult();

    // Copy assignment operator
    ImageRecognitionResult& operator= (ImageRecognitionResult& other);

    // Copy assignment operator
    ImageRecognitionResult& operator= (const ImageRecognitionResult& other);

  protected:
    void copy(ImageRecognitionResult& other);
    void copy(const ImageRecognitionResult& other);

  public:
    unsigned int x()
    {
      return m_x;
    }

    unsigned int y()
    {
      return m_y;
    }

    int class_id()
    {
      return m_class_id;
    }
};

#endif
