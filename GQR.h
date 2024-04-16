/* Kurkov Ivan, 22.B05-MM, 14.04.2024 */
#ifndef GQR_H
#define GQR_H

double * GetIQRCoefficients( const double *moments, const double *nodes, size_t size );
double IQR( double (*f)(double), const double *coef, const double *nodes, size_t size );

#endif // !GQR_H

