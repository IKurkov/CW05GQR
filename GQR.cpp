/* Kurkov Ivan, 22.B05-MM, 14.04.2024 */
#include "gqr.h"

double * GetIQRCoefficients( const double *moments, const double *nodes, size_t size )
{
  double *coef = new double[size];

  coef[0] = (moments[0] * nodes[1] - moments[1]) / (nodes[1] - nodes[0]);
  coef[1] = (moments[1] - moments[0] * nodes[0]) / (nodes[1] - nodes[0]);
  return coef;
}

double IQR( double (*f)(double), const double *coef, const double *nodes, size_t size )
{
  double sum = 0;

  for (size_t i = 0; i < size; i++)
    sum += coef[i] * f(nodes[i]);
  return sum;
}