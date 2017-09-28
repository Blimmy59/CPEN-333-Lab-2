#include <iostream>
#include <thread>
#include <vector>
#include <string>

// partitions elements low through high (inclusive)
// around a pivot and returns the pivot index
int partition(std::vector<int>& data, int low, int high) {
  // your code here
	int i = low, j = high;
	int tmp;
	int pivot = data[(low + high) / 2];

	/* partition */
	while (i <= j) {
		while (data[i] < pivot)
			i++;
		while (data[j] > pivot)
			j--;
		if (i <= j) {
			tmp = data[i];
			data[i] = data[j];
			data[j] = tmp;
			i++;
			j--;
		}
	}
	return i;
}

// sorts elements low through high (inclusive) using a single thread
void quicksort(std::vector<int>& data, int low, int high) {
  // your code here
	int index = partition(data, low, high);
	if (low < index - 1)
		quicksort(data, low, index - 1);
	if (index < high)
		quicksort(data, index, high);
}

// sorts elements low through high (inclusive) using multiple threads
void parallel_quicksort(std::vector<int>& data, int low, int high) {
  // your code here
	int p1 = partition(data, low, high);
	int p2 = partition(data, low, p1);
	int p3 = partition(data, p1, high);
	int p4 = partition(data, low, p2);
	int p5 = partition(data, p2, p1);
	int p6 = partition(data, p1, p3);
	int p7 = partition(data, p3, high);

	std::thread* thread1;
	std::thread* thread2;
	std::thread* thread3;
	std::thread* thread4;
	std::thread* thread5;
	std::thread* thread6;
	std::thread* thread7;

	thread1 = new std::thread(quicksort, std::ref(data), low , p4);
	thread2 = new std::thread(quicksort, std::ref(data), p4, p2);
	thread3 = new std::thread(quicksort, std::ref(data), p2, p5);
	thread4 = new std::thread(quicksort, std::ref(data), p5, p1);
	thread5 = new std::thread(quicksort, std::ref(data), p1, p6);
	thread6 = new std::thread(quicksort, std::ref(data), p6, p3);
	thread7 = new std::thread(quicksort, std::ref(data), p3, p7);
	quicksort(data, p7, high);											//thread8 = new std::thread(quicksort, std::ref(data), p7 + 1, high);
	
	thread1->join();
	thread2->join();
	thread3->join();
	thread4->join();
	thread5->join();
	thread6->join();
	thread7->join();

	delete thread1;
	delete thread2;
	delete thread3;
	delete thread4;
	delete thread5;
	delete thread6;
	delete thread7;

}


std::string checkSort(std::vector<int>& sortedData, int low, int high) {
	int errors = 0;
	for (int i = 0; i < high; i++) {
		if (sortedData[i] > sortedData[i+1]) {
			errors += 1;
			return "WARNING!!! Data not sorted";
		}
	}
	if (errors > 0) {
		return "Failed";
	}
	else {
		return "Passed";
	}
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
  auto qt1 = std::chrono::high_resolution_clock::now();
  quicksort(v1, 0, v1.size()-1);
  auto qt2 = std::chrono::high_resolution_clock::now();

  auto qduration = qt2 - qt1;
  auto qduration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(qduration);
  long quicksortTime = qduration_ms.count();

  // sort v2 using parallel algorithm
  auto pt1 = std::chrono::high_resolution_clock::now();
  parallel_quicksort(v2, 0, v2.size()-1);
  auto pt2 = std::chrono::high_resolution_clock::now();

  auto pduration = pt2 - pt1;
  auto pduration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(pduration);
  long parrallelTime = pduration_ms.count();

  double speedupFactor = (double)quicksortTime / (double)parrallelTime;

  std::string quicksortCheck = checkSort(v1, 0, v1.size() - 1);
  std::string parrallelCheck = checkSort(v2, 0, v2.size() - 1);
  std::cout << "QuickSort " << quicksortCheck << "- Time: " << quicksortTime << std::endl;
  std::cout << "Parrallel Sort " << parrallelCheck << "- Time: " << parrallelTime << std::endl;
  std::cout << "Speedup Factor = "<< speedupFactor << std::endl;
  std::cin.get();
  return 0;
}
