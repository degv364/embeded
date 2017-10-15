/**
 Copyright 2017 Daniel Garcia Vaglio <degv364@gmail.com> Esteban Zamora Alvarado <estebanzacr.20@gmail.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 **/

#include <math.h>

#ifndef QUATERNION_
#define QUATERNION_

#define R 0 //real part
#define I 1
#define J 2
#define K 3

class Quaternion{
private:
public:
  // Is this considered a magic number?
  // If yes, then FIXME, use a define
  // It is public to avoid overhad of get and set functions (and is for internal use ONLY)
  float m_aData[4];
  
  Quaternion();

  // Assign
  Quaternion& operator=(const Quaternion& i_Q);
  
  // Self modifying operators
  Quaternion& operator+=(const Quaternion& i_Q);
  Quaternion& operator-=(const Quaternion& i_Q);
  Quaternion& operator*=(const Quaternion& i_Q);

  //Scalar operators
  Quaternion operator*(const float i_fF);
  Quaternion operator/(const float i_fF);

  // Binary operators
  Quaternion operator+(const Quaternion& i_Q);
  Quaternion operator-(const Quaternion& i_Q);
  Quaternion operator*(const Quaternion& i_Q);

  // Comparison
  bool operator==(const Quaternion& i_Q);

  // norm
  float Norm(void);
  //Conjugate
  Quaternion Conjugate(void);
  // Unit quaternion q/norm
  Quaternion Unit(void);
  // Reciprocal  q*rec = 1
  Quaternion Reciprocal(void);
};


#endif
