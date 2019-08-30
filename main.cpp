#include <iostream>
#include <stdlib.h>
#include <time.h>

int main() {
  // Define test constants
  const unsigned short RAW_DATA_SIZE = 200;
  const unsigned short TOP_VALUES_COUNT = 15;

  // Define algorithm variables

  double doubleElement;
  unsigned short elementSize = sizeof doubleElement;
  double rawData [RAW_DATA_SIZE];
  double topData [TOP_VALUES_COUNT];
  double remainingData [RAW_DATA_SIZE - TOP_VALUES_COUNT];
  unsigned short remainingCount;
  struct element {
    unsigned short index;
    double value;
    double* list;
    unsigned short size;
  };

  // Generate test data
  srand(time(NULL));
  for (unsigned short index = 0; index < RAW_DATA_SIZE; index++){
    topData[index] = (double)rand() / (double)rand();
  }

  // Algorithm
  {
    // Copy raw data to filtering arrays
    unsigned short index = 0;
    for (; index < TOP_VALUES_COUNT; index++){
      topData[index] = rawData[index];
    }
    remainingCount = 0;
    for (; index < RAW_DATA_SIZE; index++){
      remainingData[remainingCount++] = rawData[index];
    }

    // Filter out data until TOP_VALUES_COUNT values remain
    while (remainingCount > 0){
      // Get the minimum value in the top data
      struct element min = { 0, topData[0], topData, elementSize };
      for (unsigned short index = 1; index < TOP_VALUES_COUNT; index++){
        double value = topData[index];
        if (value < min.value){
          min.index = index;
          min.value = value;
        }
      } std::cout << std::endl << "min found: " << min.value << std::endl;

      // Go through remaining data, dropping values less than min and updating max
      unsigned short index;
      unsigned short cachedRemainingCount = remainingCount;
      struct element max = { 0, remainingData[0], remainingData, elementSize };
      bool maxCandidateFound = false;
      for (index =  0; index < cachedRemainingCount; index++){
        double value = remainingData[index];
        if (value > min.value){
          if (!maxCandidateFound){
            maxCandidateFound = true;
            max.value = value;
            remainingCount--;
            std::cout << "max found: " << max.value << std::endl;
          } else if (value > max.value) {
            remainingData[index + remainingCount - cachedRemainingCount] = max.value;
            max.value = value;
            std::cout << "new max found: " << max.value << std::endl;
          } else {
            remainingData[index + remainingCount - cachedRemainingCount] = value;
          }
        } else {
          remainingCount--;
          std::cout << "(dropped value: " << value << ")" << std::endl;
        }
      }
      if (!maxCandidateFound){
        std::cout << "no max found" << std::endl;
        break;
      }
      // Replace min with max
      topData[min.index] = max.value;
    }
  }

  // Output

  std::cout << "\n=====Top " << TOP_VALUES_COUNT << " Values=====\n";
  for (unsigned short index = 0; index < TOP_VALUES_COUNT; index++){
    std::cout << '\n' << topData[index];
  }
  std::cout << "\n\n=====Remaining Data=====\n";
  for (unsigned short index = 0; index < remainingCount; index++){
    std::cout << "\n" << remainingData[index];
  }
}
