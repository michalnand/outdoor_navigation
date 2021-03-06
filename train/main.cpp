#include <iostream>
#include <dataset_images.h>

#include <classification_experiment.h>

int main()
{
    srand(time(NULL));

    DatasetImages dataset("./dataset_parameters.json");

    /*
    {
      ClassificationExperiment experiment(dataset, "net_0/");
      experiment.run();
    }

    {
      ClassificationExperiment experiment(dataset, "net_1/");
      experiment.run();
    }

    {
      ClassificationExperiment experiment(dataset, "net_2/");
      experiment.run();
    }

    {
      ClassificationExperiment experiment(dataset, "net_3/");
      experiment.run();
    }
    */

    /*
    {
      ClassificationExperiment experiment(dataset, "net_4/");
      experiment.run();
    }
    */

    {
      ClassificationExperiment experiment(dataset, "net_5/");
      experiment.run();
    }
 

    std::cout << "program done\n";

    return 0;
}
