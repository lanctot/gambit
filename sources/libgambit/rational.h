//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION;
// Interface to a rational number class
//
// This file is part of Gambit
// Modifications copyright (c) 2002, The Gambit Project
//
// The original copyright and license are included below.

// This may look like C code, but it is really -*- C++ -*-

/* 
Copyright (C) 1988 Free Software Foundation
    written by Doug Lea (dl@rocky.oswego.edu)

This file is part of the GNU C++ Library.  This library is free
software; you can redistribute it and/or modify it under the terms of
the GNU Library General Public License as published by the Free
Software Foundation; either version 2 of the License, or (at your
option) any later version.  This library is distributed in the hope
that it will be useful, but WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the GNU Library General Public License for more details.
You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the Free Software
Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef LIBGAMBIT_RATIONAL_H
#if defined(__GNUG__) && !defined(__APPLE_CC__)
#pragma interface
#endif
#define LIBGAMBIT_RATIONAL_H

#include "integer.h"
#include <math.h>

namespace Gambit {

class Rational {
protected:
  Integer num, den;

  void normalize();

public:
  Rational(void);
  Rational(double);
  Rational(int n);
  Rational(long n);
  Rational(int n, int d);
  Rational(long n, long d);
  Rational(long n, unsigned long d);
  Rational(unsigned long n, long d);
  Rational(unsigned long n, unsigned long d);
  Rational(const Integer& n);
  Rational(const Integer& n, const Integer& d);
  Rational(const Rational&);
  ~Rational();

  Rational &operator =  (const Rational& y);

  bool operator==(const Rational &y) const;
  bool operator!=(const Rational &y) const;
  bool operator< (const Rational &y) const;
  bool operator<=(const Rational &y) const;
  bool operator> (const Rational &y) const;
  bool operator>=(const Rational &y) const;

  Rational operator+(const Rational &y) const;
  Rational operator-(const Rational &y) const;
  Rational operator*(const Rational &y) const;
  Rational operator/(const Rational &y) const;

  Rational &operator+=(const Rational& y);
  Rational &operator-=(const Rational& y);
  Rational &operator*=(const Rational& y);
  Rational &operator/=(const Rational& y);

  Rational operator-(void) const;


  // builtin Rational functions


  void             negate();                      // x = -x
  void             invert();                      // x = 1/x

  friend int       sign(const Rational& x);             // -1, 0, or +1
  friend Rational  abs(const Rational& x);              // absolute value
  friend Rational  sqr(const Rational& x);              // square
  friend Rational  pow(const Rational& x, long y);
  friend Rational  pow(const Rational& x, const Integer& y);
  const Integer&   numerator() const;
  const Integer&   denominator() const;

  // coercion & conversion

  operator double() const;
  friend Integer   floor(const Rational& x);
  friend Integer   ceil(const Rational& x);
  friend Integer   trunc(const Rational& x);
  friend Integer   round(const Rational& x);

  friend std::istream &operator>>(std::istream &s, Rational& y);
  friend std::ostream &operator<<(std::ostream &s, const Rational& y);

  int		   fits_in_float() const;
  int		   fits_in_double() const;

  // procedural versions of operators

  friend int       compare(const Rational& x, const Rational& y);
  friend void      add(const Rational& x, const Rational& y, Rational& dest);
  friend void      sub(const Rational& x, const Rational& y, Rational& dest);
  friend void      mul(const Rational& x, const Rational& y, Rational& dest);
  friend void      div(const Rational& x, const Rational& y, Rational& dest);

  // error detection

  void    error(const char* msg) const;
  int              OK() const;

};

std::string ToText(const Rational &);
Rational ToRational(const std::string &);
void gEpsilon(Rational &v, int i = 8);

//
// This is implemented so as to convert rational numbers to their
// floating point equivalent.
//
inline double ToDouble(const std::string &s)
{
  return (double) ToRational(s);
}

Rational ToNumber(const std::string &p_string);

} // end namespace Gambit

#endif // LIBGAMBIT_RATIONAL_H

