/* Kurkov Ivan, 22.B05-MM, 14.04.2024 */
#include <cstring>
#include <utility>

#include "int.h"
#include "gqr.h"

IQR::IQR( const double *x, const double *m, size_t n ) : Nodes(new double[n]), Coeffs(new double[n]), Size(n)
{
  memcpy(Nodes, x, n * sizeof(double));
  Coeffs[0] = (m[0] * x[1] - m[1]) / (x[1] - x[0]);
  Coeffs[1] = (m[1] - m[0] * x[0]) / (x[1] - x[0]);
}

void IQR::swap( IQR &R )
{
  std::swap(Nodes, R.Nodes);
  std::swap(Coeffs, R.Coeffs);
  std::swap(Size, R.Size);
}

IQR::IQR( const IQR &R ) : Nodes(new double[R.Size]), Coeffs(new double[R.Size]), Size(R.Size)
{
  memcpy(Nodes, R.Nodes, Size * sizeof(double));
  memcpy(Coeffs, R.Coeffs, Size * sizeof(double));
}

IQR & IQR::operator=( const IQR &R )
{
  if (this != &R)
    IQR(R).swap(*this);
  return *this;
}

IQR::IQR( IQR &&R ) { swap(R); }

IQR & IQR::operator=( IQR &&R )
{
  swap(R);
  return *this;
}

IQR::~IQR( void )
{
  delete[] Nodes;
  delete[] Coeffs;
  Size = 0;
}

double IQR::Integrate( double (*f)(double) )
{
  double sum = 0;

  for (size_t i = 0; i < Size; i++)
    sum += Coeffs[i] * f(Nodes[i]);
  return sum;
}