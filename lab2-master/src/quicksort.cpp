#include <iostream>
#include <thread>
#include <vector>

// partitions elements low through high (inclusive)
// around a pivot and returns the pivot index
int partition(std::vector<int>& data, int low, int high) {
  // your code here




}

// sorts elements low through high (inclusive) using a single thread
void quicksort(std::vector<int>& data, int low, int high) {
  // your code here
	int pivot = data[0];
	int length = high - low - 1;
	for (int i = 1; i < length; i++) {

		if (data[i] <= pivot) {
			
		}
		else {

		}
	}
}

// sorts elements low through high (inclusive) using multiple threads
void parallel_quicksort(std::vector<int>& data, int low, int high) {
  // your code here
}

int main() {

  // create two copies of random data
  const int VECTOR_SIZE = 1000000;
  std::vector<int> v1(VECTOR_SIZE, 0);
  // fill with random integers
  for (int i=0; i<VECTOR_SIZE; ++i) {
    v1[i] = rand();
  }
  std::vector<int> v2 = v1;  // copy all contents

  // sort v1 using sequential algorithm
  quicksort(v1, 0, v1.size()-1);

  // sort v2 using parallel algorithm
  parallel_quicksort(v2, 0, v2.size()-1);

  return 0;
}
