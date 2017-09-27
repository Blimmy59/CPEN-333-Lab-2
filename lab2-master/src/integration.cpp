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

int main() {

  // sample usage of functions
  Density1 d1;
  XFunction xd1(d1);  // x*d1(x,y,z)
  YFunction yd1(d1);  // y*d1(x,y,z)
  ZFunction zd1(d1);  // z*d1(x,y,z)

  std::cout << "d1.eval(0.1,0.2,0.3): " << d1.eval(0.1,0.2,0.3) << std::endl;
  std::cout << "xd1.eval(0.1,0.2,0.3): " << xd1.eval(0.1,0.2,0.3) << std::endl;
  std::cout << "yd1.eval(0.1,0.2,0.3): " << yd1.eval(0.1,0.2,0.3) << std::endl;
  std::cout << "zd1.eval(0.1,0.2,0.3): " << zd1.eval(0.1,0.2,0.3) << std::endl;

  // YOUR CODE HERE

  return 0;
}