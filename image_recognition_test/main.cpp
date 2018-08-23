#include <iostream>

#include <terrain_recognition.h>


    
int main()
{
  std::string file_name = "recognition_config.json";
  TerrainRecognition recognition(file_name);

  recognition.run();

    std::cout << "program done\n";

    return 0;
}
