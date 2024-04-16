/* Kurkov Ivan, 22.B05-MM, 14.04.2024 */
#include <iostream>
#include <cmath>
#include <conio.h>

#include "fort.hpp"
#include "int.h"
#include "gqr.h"

const char *WeightStr = "|x - 0.5|";
double Weight( double x )
{
  return fabs(x - 0.5);
}

const char *Variant7Str = "sin x";
double Variant7( double x )
{
  return x + 1;
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
  double moments[4], nodes[2], *coefficients = nullptr;
  AcmFVals acm = Accumulate(Weight, ITER, a, b);

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
    switch (_getch())
    {
    case '0':
      run = false;
      break;
    case '1':
    {
      fort::char_table stats;

      std::cout << "Input number of nodes (= 2): ";
      std::cin >> n;
      while (n != 2)
      {
        std::cout << phrases[std::rand() % 4] << '\n';
        std::cout << "Input number of nodes (= 2): ";
        std::cin >> n;
      }
      do
      {
        std::cout << "Input 2 different nodes, integration interval = (0, 1): ";
        std::cin >> nodes[0] >> nodes[1];
      } while (nodes[0] == nodes[1]);

      if (coefficients != nullptr)
        delete[] coefficients;
      coefficients = GetIQRCoefficients(moments, nodes, n);
      acm = Accumulate([]( double x ){ return Weight(x) * Variant7(x); }, ITER, a, b);
      stats << fort::header << 'i' << "x_i" << "m_i" << "A_i" << fort::endr;
      for (size_t i = 0; i < n; i++)
        stats << i << nodes[i] << moments[i] << coefficients[i] << fort::endr;
      std::cout << stats.to_string();
      std::cout << "I_[ * ](" << WeightStr << '*' << Variant7Str << ", " << a << ", " << b  << ") = "
        << QRMiddleRectangles(acm) << '\n';
      std::cout << "I_IQR(" << WeightStr << '*' << Variant7Str << ", " << a << ", " << b << ") = "
        << IQR(Variant7, coefficients, nodes, n) << '\n';
      break;
    }
    default:
      std::cout << "[Error]: Incorrect choice!\n";
    }
  }
  return 0;
}