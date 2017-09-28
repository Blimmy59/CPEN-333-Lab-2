#include <thread>
#include <iostream>
#include <random>

double estimate_pi(int nsamples) {

  // YOUR CODE HERE
	static std::default_random_engine rnd;
	static std::uniform_real_distribution<double> dist(-1.0, 1.0);
	int hits = 0;
	int i = 0;
	while (i < nsamples) {
		double x = dist(rnd);
		double y = dist(rnd);
		double r = pow((x*x + y*y), 0.5);
		if (r <= 1) {
			hits++;
		}
		i++;
	}
	return ((double)hits / (double)nsamples)*4;
}

// generates a random sample and sets hits[idx]
// to true if within the unit circle
void pi_sampler(std::vector<bool>& hits, int idx) {

  // single instance of random engine and distribution
  static std::default_random_engine rnd;
  static std::uniform_real_distribution<double> dist(-1.0, 1.0);

  // YOUR CODE HERE
	  double x = dist(rnd);
	  double y = dist(rnd);
	  double r = pow((x*x + y*y), 0.5);
	  if (r <= 1) {
		  hits[idx] = true;
	  }
}

// naively uses multithreading to try to speed up computations
double estimate_pi_multithread_naive(int nsamples) {
  // stores whether each sample falls within circle
  std::vector<bool> hits(nsamples, false);

  // create and store all threads
  std::vector<std::thread> threads;
  for (int i=0; i<nsamples; ++i) {
    threads.push_back(std::thread(pi_sampler, std::ref(hits), i));
  }

  // wait for all threads to complete
  for (int i=0; i<nsamples; ++i) {
    threads[i].join();
  }

  // estimate our value of PI
  double pi = 0;
  for (int i=0; i<nsamples; ++i) {
    if (hits[i]) {
      pi = pi + 1;
    }
  }
  pi = pi / nsamples*4;

  return pi;
}

// count number of hits using nsamples, populates hits[idx]
void pi_hits(std::vector<int>& hits, int idx, int nsamples) {

  // single instance of random engine and distribution
  static std::default_random_engine rnd;
  static std::uniform_real_distribution<double> dist(-1.0, 1.0);

  // YOUR CODE HERE
  int i = 0;
  while (i < nsamples) {
	  double x = dist(rnd);
	  double y = dist(rnd);
	  double r = pow((x*x + y*y), 0.5);
	  if (r <= 1) {
		  hits[idx] += 1;
	  }
	  i++;
  }
}

// divides work among threads intelligently
double estimate_pi_multithread(int nsamples) {

  // number of available cores
  int nthreads = std::thread::hardware_concurrency();

  // hit counts
  std::vector<int> hits(nthreads, 0);

  // create and store threads
  std::vector<std::thread> threads;
  int msamples = 0; // samples accounted for
  for (int i=0; i<nthreads-1; ++i) {
    threads.push_back(
        std::thread(pi_hits, std::ref(hits), i, nsamples/nthreads));
    msamples += nsamples/nthreads;
  }
  // remaining samples
  threads.push_back(
      std::thread(pi_hits, std::ref(hits), nthreads-1, nsamples-msamples));

  // wait for threads to finish
  for (int i=0; i<nthreads; ++i) {
    threads[i].join();
  }

  // estimate pi
  double pi = 0;
  for (int i=0; i<nthreads; ++i) {
    pi += hits[i];
  }
  pi = pi/nsamples*4;

  return pi;
}

int main() {
	// Simply estimate pi
	int numSamples = 1000000;
	auto t1 = std::chrono::high_resolution_clock::now();
	double pi = estimate_pi(numSamples);
	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = t2 - t1;
	auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
	long ms = duration_ms.count();
	std::cout << "Lame Estimate: " << pi << " Samples: " << numSamples << "  Time: " << ms << " ms." << std::endl;

	// Naively estimate pi
	int numSamples2 = 1000;
	auto t12 = std::chrono::high_resolution_clock::now();
	double pi2 = estimate_pi_multithread_naive(numSamples2);
	auto t22 = std::chrono::high_resolution_clock::now();
	auto duration2 = t22 - t12;
	auto duration_ms2 = std::chrono::duration_cast<std::chrono::milliseconds>(duration2);
	long ms2 = duration_ms2.count();
	std::cout << "Naive Estimation: " << pi2 << " Samples: " << numSamples2 << "  Time: " << ms2 << " ms." << std::endl;

	// Smartly estimate pi
	int numSamples3 = 10000000;
	auto t13 = std::chrono::high_resolution_clock::now();
	double pi3 = estimate_pi_multithread(numSamples3);
	auto t23 = std::chrono::high_resolution_clock::now();
	auto duration3 = t23 - t13;
	auto duration_ms3 = std::chrono::duration_cast<std::chrono::milliseconds>(duration3);
	long ms3 = duration_ms3.count();
	std::cout << "Smart Estimate: " << pi3 << " Samples: " << numSamples3 << "  Time: " << ms3 << " ms." << std::endl;
	std::cin.get();
  return 0;
}