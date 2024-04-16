/* Kurkov Ivan, 22.B05-MM, 14.04.2024 */
#ifndef GQR_H
#define GQR_H

struct IQR
{
  double *Nodes = nullptr, *Coeffs = nullptr;
  size_t Size = 0;

  IQR( void ) = default;
  IQR( const double *x, const double *m, size_t n);
  IQR( const IQR &Rule );
  IQR & operator=( const IQR &R );
  IQR( IQR &&R );
  IQR & operator=( IQR &&R );
  ~IQR( void );

  void swap( IQR &R );

  double Integrate( double (*f)(double) );
};

#endif // !GQR_H

