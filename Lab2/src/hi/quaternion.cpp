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

#include "hi/quaternion.hh"

Quaternion::Quaternion(){
  m_aData[R]=0;
  m_aData[I]=0;
  m_aData[J]=0;
  m_aData[K]=0;
}

//Assign
Quaternion&
Quaternion::operator=(const Quaternion& i_Q){
  m_aData[R]=i_Q.m_aData[R];
  m_aData[I]=i_Q.m_aData[I];
  m_aData[J]=i_Q.m_aData[J];
  m_aData[K]=i_Q.m_aData[K];
  return *this;
}

//Self modifying operators
Quaternion&
Quaternion::operator+=(const Quaternion& i_Q){
  m_aData[R]+=i_Q.m_aData[R];
  m_aData[I]+=i_Q.m_aData[I];
  m_aData[J]+=i_Q.m_aData[J];
  m_aData[K]+=i_Q.m_aData[K];
  return *this;
}

Quaternion&
Quaternion::operator-=(const Quaternion& i_Q){
  m_aData[R]-=i_Q.m_aData[R];
  m_aData[I]-=i_Q.m_aData[I];
  m_aData[J]-=i_Q.m_aData[J];
  m_aData[K]-=i_Q.m_aData[K];
  return *this;
}

Quaternion&
Quaternion::operator*=(const Quaternion& i_Q){
  m_aData[R] =
    (m_aData[R]*i_Q.m_aData[R] -m_aData[I]*i_Q.m_aData[I])+
    (-m_aData[J]*i_Q.m_aData[K] -m_aData[K]*i_Q.m_aData[J]);
  //-----------------------------------------------------------
  m_aData[I] =
    (m_aData[R]*i_Q.m_aData[I] +m_aData[I]*i_Q.m_aData[R])+
    (m_aData[J]*i_Q.m_aData[J] -m_aData[K]*i_Q.m_aData[K]);
  //-----------------------------------------------------------
  m_aData[J] =
    (m_aData[R]*i_Q.m_aData[K] -m_aData[I]*i_Q.m_aData[J]) +
    (m_aData[J]*i_Q.m_aData[R] +m_aData[K]*i_Q.m_aData[I]);
  //-----------------------------------------------------------
  m_aData[K] =
    (m_aData[R]*i_Q.m_aData[J] +m_aData[I]*i_Q.m_aData[K])+
    (-m_aData[J]*i_Q.m_aData[I] +m_aData[K]*i_Q.m_aData[R]);
  return *this;
}

// Scalar operators
Quaternion
Quaternion::operator*(const float i_fF){
  Quaternion l_oQ = Quaternion();
  l_oQ.m_aData[R] = i_fF*m_aData[R];
  l_oQ.m_aData[I] = i_fF*m_aData[I];
  l_oQ.m_aData[J] = i_fF*m_aData[J];
  l_oQ.m_aData[K] = i_fF*m_aData[K];
  return l_oQ;
}

Quaternion
Quaternion::operator/(const float i_fF){
  Quaternion l_oQ = Quaternion();
  l_oQ.m_aData[R] = m_aData[R]/i_fF;
  l_oQ.m_aData[I] = m_aData[I]/i_fF;
  l_oQ.m_aData[J] = m_aData[J]/i_fF;
  l_oQ.m_aData[K] = m_aData[K]/i_fF;
  return l_oQ;
}

//Binary operators
Quaternion
Quaternion::operator+(const Quaternion& i_Q){
  Quaternion l_oQ = Quaternion();
  l_oQ.m_aData[R]=m_aData[R]+i_Q.m_aData[R];
  l_oQ.m_aData[I]=m_aData[R]+i_Q.m_aData[I];
  l_oQ.m_aData[J]=m_aData[R]+i_Q.m_aData[J];
  l_oQ.m_aData[K]=m_aData[R]+i_Q.m_aData[K];
  return l_oQ;
}

Quaternion
Quaternion::operator-(const Quaternion& i_Q){
  Quaternion l_oQ = Quaternion();
  l_oQ.m_aData[R]=m_aData[R]-i_Q.m_aData[R];
  l_oQ.m_aData[I]=m_aData[R]-i_Q.m_aData[I];
  l_oQ.m_aData[J]=m_aData[R]-i_Q.m_aData[J];
  l_oQ.m_aData[K]=m_aData[R]-i_Q.m_aData[K];
  return l_oQ;
}

Quaternion
Quaternion::operator*(const Quaternion& i_Q){
  Quaternion l_oQ = Quaternion();
  l_oQ.m_aData[R] =
    (m_aData[R]*i_Q.m_aData[R] -m_aData[I]*i_Q.m_aData[I])+
    (-m_aData[J]*i_Q.m_aData[K] -m_aData[K]*i_Q.m_aData[J]);
  //-----------------------------------------------------------
  l_oQ.m_aData[I] =
    (m_aData[R]*i_Q.m_aData[I] +m_aData[I]*i_Q.m_aData[R])+
    (m_aData[J]*i_Q.m_aData[J] -m_aData[K]*i_Q.m_aData[K]);
  //-----------------------------------------------------------
  l_oQ.m_aData[J] =
    (m_aData[R]*i_Q.m_aData[K] -m_aData[I]*i_Q.m_aData[J]) +
    (m_aData[J]*i_Q.m_aData[R] +m_aData[K]*i_Q.m_aData[I]);
  //-----------------------------------------------------------
  l_oQ.m_aData[K] =
    (m_aData[R]*i_Q.m_aData[J] +m_aData[I]*i_Q.m_aData[K])+
    (-m_aData[J]*i_Q.m_aData[I] +m_aData[K]*i_Q.m_aData[R]);
  return l_oQ;
}

//Comparison
bool
Quaternion::operator==(const Quaternion& i_Q){
  return (m_aData[R]==i_Q.m_aData[R] &&
	  m_aData[I]==i_Q.m_aData[I] &&
	  m_aData[J]==i_Q.m_aData[J] &&
	  m_aData[K]==i_Q.m_aData[K]);
}


// norm
float
Quaternion::Norm(void){
  return sqrt(m_aData[R]*m_aData[R]+
	      m_aData[I]*m_aData[I]+
	      m_aData[J]*m_aData[J]+
	      m_aData[K]*m_aData[K]);
}

//Conjugate
Quaternion
Quaternion::Conjugate(void){
  Quaternion l_oQ = Quaternion();
  l_oQ.m_aData[R]= m_aData[R];
  l_oQ.m_aData[I]=-m_aData[I];
  l_oQ.m_aData[J]=-m_aData[J];
  l_oQ.m_aData[K]=-m_aData[K];
  return l_oQ;
}

//Conjugate
Quaternion
Quaternion::Unit(void){
  Quaternion l_oQ = Quaternion();
  float l_fNorm = this->Norm();
  l_oQ.m_aData[R]= m_aData[R]/l_fNorm;
  l_oQ.m_aData[I]= m_aData[I]/l_fNorm;
  l_oQ.m_aData[J]= m_aData[J]/l_fNorm;
  l_oQ.m_aData[K]= m_aData[K]/l_fNorm;
  return l_oQ;
}

//Reciprocal
Quaternion
Quaternion::Reciprocal(void){
  Quaternion l_oQ = Quaternion();
  float l_fNorm = this->Norm();
  l_oQ.m_aData[R]= m_aData[R]/(l_fNorm*l_fNorm);
  l_oQ.m_aData[I]=-m_aData[I]/(l_fNorm*l_fNorm);
  l_oQ.m_aData[J]=-m_aData[J]/(l_fNorm*l_fNorm);
  l_oQ.m_aData[K]=-m_aData[K]/(l_fNorm*l_fNorm);
}


// Constructor from Euler angles
void inline
Quaternion::AssignFromEuler(float i_fRoll, float i_fPitch, float i_fYaw){
    // Compute trigonometric functions only once. 
  float l_fCR = (float) cos(0.5f*i_fRoll);
  float l_fSR = (float) sin(0.5f*i_fRoll);
  float l_fCP = (float) cos(0.5f*i_fPitch);
  float l_fSP = (float) sin(0.5f*i_fPitch);
  float l_fCY = (float) cos(0.5f*i_fYaw);
  float l_fSY = (float) sin(0.5f*i_fYaw);

  m_aData[R] = l_fCR * l_fCP * l_fCY + l_fSR * l_fSP * l_fSY;
  
  m_aData[I] = l_fCY * l_fSR * l_fCP - l_fSY * l_fCR * l_fSP;
  m_aData[J] = l_fCY * l_fCR * l_fSP + l_fSY * l_fSR * l_fCP;
  m_aData[K] = l_fSY * l_fCR * l_fCP - l_fCY * l_fSR * l_fSP;
}

Quaternion::Quaternion(EulerAngle_t i_stEulerAngle){
  AssignFromEuler(i_stEulerAngle.m_fRoll, i_stEulerAngle.m_fPitch, i_stEulerAngle.m_fYaw);
}


Quaternion::Quaternion(float i_fRoll, float i_fPitch, float i_fYaw){
  AssignFromEuler(i_fRoll, i_fPitch, i_fYaw);
}

// Get Euler Angles
EulerAngle_t
Quaternion::GetEuler(void){
  EulerAngle_t o_stEulerAngle;
  // Roll------------------------------------------------------------------------
  float l_fSR = 2.0f        * (m_aData[R] * m_aData[I] + m_aData[J] * m_aData[K]);
  float l_fCR = 1.0f - 2.0f * (m_aData[I] * m_aData[I] + m_aData[J] * m_aData[J]);
  o_stEulerAngle.m_fRoll = (float) atan2(l_fSR, l_fCR);
  //Pitch------------------------------------------------------------------------
  float l_fSP = 2.0f * (m_aData[R] * m_aData[J] - m_aData[K] * m_aData[I]);
  if (fabs(l_fSP) >=1.0f){
    // just in case there are any accumulated rounding errors
    o_stEulerAngle.m_fPitch = copysign(0.5f*M_PI, l_fSP);
  }
  else{
    o_stEulerAngle.m_fPitch = (float) asin(l_fSP);
  }
  //Yaw--------------------------------------------------------------------------
  float l_fSY = 2.0f        * (m_aData[R] * m_aData[K] + m_aData[I] * m_aData[J]);
  float l_fCY = 1.0f - 2.0f * (m_aData[J] * m_aData[J] + m_aData[K] * m_aData[K]);
  o_stEulerAngle.m_fYaw = (float) atan2(l_fSY, l_fCY);

  return o_stEulerAngle;
}

