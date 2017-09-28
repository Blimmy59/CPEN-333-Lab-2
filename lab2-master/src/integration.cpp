#include <iostream>
#include <random>
#include <thread>
#include <cmath>   // for exp, abs


// three-dimensional point
struct Point {
  double x,y,z;
};

// virtual base class for functions
class Function {
 public:
  virtual double eval(double x, double y, double z) = 0;
};

// computes x*fn(x,y,z)
class XFunction : public Function {
  Function& fn;
 public:
  XFunction(Function& fn) : fn(fn){};
  double eval(double x, double y, double z) {
    return x*fn.eval(x,y,z);
  }
};

// computes y*fn(x,y,z)
class YFunction : public Function {
  Function& fn;
 public:
  YFunction(Function& fn) : fn(fn){};
  double eval(double x, double y, double z) {
    return y*fn.eval(x,y,z);
  }
};

// computes z*fn(x,y,z)
class ZFunction : public Function {
  Function& fn;
 public:
  ZFunction(Function& fn) : fn(fn){};
  double eval(double x, double y, double z) {
    return z*fn.eval(x,y,z);
  }
};

// new function for representing density 1
class Density1 : public Function {
 public:
  double eval(double x, double y, double z) {
    double norm2 = x*x+y*y+z*z;
    if (norm2 > 1) {
      return 0;
    }
    return std::exp(-norm2);
  }
};

class Density2 : public Function {
public:
	double eval(double x, double y, double z) {
		double sum = x + y + z;
		return abs(sum);
	}
};

class Density3 : public Function {
public:
	double eval(double x, double y, double z) {
		return pow(x - 1, 2) + pow(y - 2, 2) + pow(z - 3, 2);
	}
};

void pi_hits(std::vector<int>& hits, int idx, int nsamples) {

	// single instance of random engine and distribution
	static std::default_random_engine rnd;
	static std::uniform_real_distribution<double> dist(-1.0, 1.0);

	// YOUR CODE HERE
	int count = 0;
	int HITS = 0;
	double density = 0;
	double xsum = 0;
	double ysum = 0;
	double zsum = 0;

	Density1 d1;
	Density2 d2;
	Density3 d3;


	while (count < nsamples) {
		double x = dist(rnd);
		double y = dist(rnd);
		double z = dist(rnd);

		double pyth = pow((x*x + y*y + z*z), 0.5);

		//check if its in the circle
		if (pyth <= 1)
		{
			density += d3.eval(x, y, z);	//compute density sum
			xsum += x*d3.eval(x, y, z);		//compute x*density respectivley
			ysum += y*d3.eval(x, y, z);
			zsum += z*d3.eval(x, y, z);
			count++;
		}
	}

	//hits[idx] = density;
	if (idx == 0) {
		hits[idx] = density;
		hits[idx + 1] = xsum;
		hits[idx + 2] = ysum;
		hits[idx + 3] = zsum;
		hits[idx + 4] = count;
	}
	if (idx == 1) {
		idx = 0;
		hits[idx + 5] = density;
		hits[idx + 6] = xsum;
		hits[idx + 7] = ysum;
		hits[idx + 8] = zsum;
		hits[idx + 9] = count;
	}

	if (idx == 2) {
		idx = 0;
		hits[idx + 10] = density;
		hits[idx + 11] = xsum;
		hits[idx + 12] = ysum;
		hits[idx + 13] = zsum;
		hits[idx + 14] = count;
	}

	if (idx == 3) {
		idx = 0;
		hits[idx + 15] = density;
		hits[idx + 16] = xsum;
		hits[idx + 17] = ysum;
		hits[idx + 18] = zsum;
		hits[idx + 19] = count;
	}


}

////////////////////
// divides work among threads intelligently
void estimate_pi_multithread(int nsamples) {

	// number of available cores
	int nthreads = std::thread::hardware_concurrency();

	// hit counts
	std::vector<int> hits(20, 0);

	// create and store threads
	std::vector<std::thread> threads;
	int msamples = 0; // samples accounted for
	for (int i = 0; i<nthreads - 1; ++i) {
		threads.push_back(
			std::thread(pi_hits, std::ref(hits), i, nsamples / nthreads));
		msamples += nsamples / nthreads;
	}
	// remaining samples
	threads.push_back(
		std::thread(pi_hits, std::ref(hits), nthreads - 1, nsamples - msamples));

	// wait for threads to finish
	for (int i = 0; i<nthreads; ++i) {
		threads[i].join();
	}

	double density = 0;
	double xsum = 0;
	double ysum = 0;
	double zsum = 0;
	int count = 0;
	int bleh = 0;

	while (bleh < nthreads) {
		density += hits[bleh];
		xsum += hits[bleh * 5 + 1];
		ysum += hits[bleh * 5 + 2];
		zsum += hits[bleh * 5 + 3];
		count += hits[bleh * 5 + 4];
		bleh++;
	}

	double cx = xsum / density;
	double cy = ysum / density;
	double cz = zsum / density;

	std::cout << "cx:  " << "" << cx << "  cy:  " << "" << cy << "  cz:  " << "" << cz << std::endl;


}

int main() {

  // sample usage of functions
  Density1 d1;
  XFunction xd1(d1);  // x*d1(x,y,z)
  YFunction yd1(d1);  // y*d1(x,y,z)
  ZFunction zd1(d1);  // z*d1(x,y,z)

  /*std::cout << "d1.eval(0.1,0.2,0.3): " << d1.eval(0.1,0.2,0.3) << std::endl;
  std::cout << "xd1.eval(0.1,0.2,0.3): " << xd1.eval(0.1,0.2,0.3) << std::endl;
  std::cout << "yd1.eval(0.1,0.2,0.3): " << yd1.eval(0.1,0.2,0.3) << std::endl;
  std::cout << "zd1.eval(0.1,0.2,0.3): " << zd1.eval(0.1,0.2,0.3) << std::endl;*/

  // YOUR CODE HERE


  int nsamples = 10000;
  double PI = 3.141592653589793;
  estimate_pi_multithread(nsamples);
  std::cin.get();

  return 0;
}