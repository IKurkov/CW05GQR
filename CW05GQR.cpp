/* Kurkov Ivan, 22.B05-MM, 14.04.2024 */
#include <iostream>
#include <cmath>
#include <conio.h>

#include "fort.hpp"
#include "int.h"
#include "gqr.h"
#include "test_functions.h"

const char *WeightStr = "|x - 0.5|";
double Weight( double x )
{
  return fabs(x - 0.5);
}

const double a = 0, b = 1;
const size_t ITER = 10000;
const char *phrases[] =
{"Sorry, current version of program work only with n = 2.",
 "Please try again, n must be 2.",
 "It seems to me that you made a mistake with the key.",
 "Please read statement carefully and try again."};

int main( void )
{
  bool run = true;
  size_t n;
  int key, FId;
  double moments[4], nodes[2], a0, a1, det, I1, I2;
  AcmFVals acm = Accumulate(Weight, ITER, a, b);
  IQR Rule;

  moments[0] = QRMiddleRectangles(acm);
  for (size_t i = 1; i < 4; i++)
  {
    acm = Accumulate([i]( double x ){ return Weight(x) * std::pow(x, i); }, ITER, a, b);
    moments[i] = QRMiddleRectangles(acm);
  }
  
  while (run)
  {
    std::cout << "Interpolation and Gaussian quadrature rules menu [variant #7]:\n"
      "0 - exit\n"
      "1 - interpolation quadrature rules\n"
      "2 - Gaussian quadrature rules\n";
    key = _getch();

    if (key == '0')
      run = false;
    else if (key == '1' || key == '2')
    {
      fort::char_table stats;

      std::cout << "Choose function to integrate:\n";
      for (size_t i = 0; i < NumOfFunctions; i++)
        std::cout << i << " - f(x) = " << FunctionsList[i].Expr << '\n';
      while ((FId = _getch() - '0') < 0 || FId >= NumOfFunctions)
        std::cout << "[Error]: Incorrect choice, try again!\n";

      std::cout << "Input number of nodes (= 2): ";
      std::cin >> n;
      while (n != 2)
      {
        std::cout << phrases[std::rand() % 4] << '\n';
        std::cout << "Input number of nodes (= 2): ";
        std::cin >> n;
      }
      if (key == '1')
        do
        {
          std::cout << "Input 2 different nodes, integration interval = (0, 1): ";
          std::cin >> nodes[0] >> nodes[1];
        } while (nodes[0] == nodes[1]);
      else
      {
        det = moments[1] * moments[1] - moments[0] * moments[2];
        a0 = (moments[2] * moments[2] - moments[1] * moments[3]) / det;
        a1 = (moments[0] * moments[3] - moments[1] * moments[2]) / det;
        nodes[0] = (-a1 - sqrt(a1 * a1 - 4 * a0)) / 2;
        nodes[1] = (-a1 + sqrt(a1 * a1 - 4 * a0)) / 2;
      }

      Rule = IQR(nodes, moments, n);
      acm = Accumulate([FId]( double x ){ return Weight(x) * FunctionsList[FId].Func(x); }, ITER, a, b);

      stats << fort::header << 'i' << "x_i" << "m_i" << "A_i" << fort::endr;
      for (size_t i = 0; i < n; i++)
        stats << i << nodes[i] << moments[i] << Rule.Coeffs[i] << fort::endr;
      std::cout << stats.to_string();
      if (key == '2')
        std::cout << "Orthogonal polynomial: w(x) = x^2"
          << (a1 < 0 ? " - " : " + ") << fabs(a1)
          << "x" << (a0 < 0 ? " - " : " + ") << fabs(a0) << '\n';
      I1 = QRMiddleRectangles(acm);
      I2 = Rule.Integrate(FunctionsList[FId].Func);
      std::cout << "I_[ * ](" << WeightStr << " * [" << FunctionsList[FId].Expr << "], " << a << ", " << b  << ") = "
        << std::setprecision(15) << I1 << '\n';
      std::cout << "I_" << (key == '1' ? 'I' : 'G') << "QR("
        << WeightStr << " * [" << FunctionsList[FId].Expr << "], " << a << ", " << b << ") = "
        << std::setprecision(15) << I2 << '\n';
      std::cout << "Delta: " << fabs(I1 - I2) << '\n';
    }
    else
      std::cout << "[Error]: Incorrect choice!\n";
  }
  return 0;
}