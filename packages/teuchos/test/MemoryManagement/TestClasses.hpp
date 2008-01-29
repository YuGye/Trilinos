// @HEADER
// ***********************************************************************
// 
//                    Teuchos: Common Tools Package
//                 Copyright (2004) Sandia Corporation
// 
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
// 
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//  
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
// Questions? Contact Michael A. Heroux (maherou@sandia.gov) 
// 
// ***********************************************************************
// @HEADER

#ifndef TEUCHOS_TEST_CLASSES_HPP
#define TEUCHOS_TEST_CLASSES_HPP


#include "Teuchos_RCP.hpp"


// Return constants from class functions
const int A_g_return  = 1;
const int A_f_return  = 2;
const int B1_g_return = 3;
const int B1_f_return = 4;
const int B2_g_return = 5;
const int B2_f_return = 6;
const int C_g_return  = 7;
const int C_f_return  = 8;
const int D_g_return  = 9;
const int D_f_return  = 10;
const int E_g_return  = 11;
const int E_f_return  = 12;


/*

 Polymorphic multiple inheritance example

            -----
           |  A  |
            -----
             /|\
              | 
         ------------
        |            |
      -----        ------
     |  B1 |      |  B2  |
      -----        ------
       /|\          /|\
        |            |
         ------------
              |
            -----
           |  C  |
            -----

*/


class C;


class A {
	int A_g_, A_f_;
public:
	A() : A_g_(A_g_return), A_f_(A_f_return) {}
	virtual ~A(); // See below
	virtual int A_g() { return A_g_; }
	virtual int A_f() const { return A_f_; }
private:
  Teuchos::RCP<C> c_;
public:
  void set_C(const Teuchos::RCP<C> &c ) { c_ = c; }
};


class B1 : virtual public A {
	int B1_g_, B1_f_;
public:
	B1() : B1_g_(B1_g_return), B1_f_(B1_f_return) {}
	~B1() { B1_g_ = -1; B1_f_ = -1; }
	virtual int B1_g() { return B1_g_; }
	virtual int B1_f() const { return B1_f_; }
};


class B2 : virtual public A {
	int B2_g_, B2_f_;
public:
	B2() : B2_g_(B2_g_return), B2_f_(B2_f_return) {}
	~B2() { B2_g_ = -1; B2_f_ = -1; }
	virtual int B2_g() { return B2_g_; }
	virtual int B2_f() const { return B2_f_; }
};


class C : virtual public B1, virtual public B2
{
	int C_g_, C_f_;
public:
	C() : C_g_(C_g_return),C_f_(C_f_return) {}
	~C() { C_g_ = -1; C_f_ = -1; }
	virtual int C_g() { return C_g_; }
	virtual int C_f() const { return C_f_; }
private:
  Teuchos::RCP<A> a_;
public:
  void set_A(const Teuchos::RCP<A> &a ) { a_ = a; }
};


// Need to put this here if we have circular references
A::~A() { A_g_ = -1; A_f_ = -1; }

class Get_A_f_return {
  const A *a_;
  int *a_f_return_;
  Get_A_f_return();
public:
  Get_A_f_return( const A *a, int *a_f_return ) : a_(a), a_f_return_(a_f_return) {}
  ~Get_A_f_return() { *a_f_return_ = a_->A_f(); }
};


void deallocA(A* ptr)
{
  std::cout << "\nCalled deallocA(...)!\n";
  delete ptr;
}


void deallocHandleA(A** handle)
{
  std::cout << "\nCalled deallocHandleA(...)!\n";
  A *ptr = *handle;
  delete ptr;
  *handle = 0;
}


/*

 Non-polymophic classes hiearchy examlpe

            -----
           |  D  |
            -----
             /|\
              | 
            -----
           |  E  |
            -----

*/


class D 
{
	int D_g_, D_f_;
public:
	D() : D_g_(D_g_return), D_f_(D_f_return) {}
	int D_g() { return D_g_; }
	int D_f() const { return D_f_; }
};


class E : public D
{
	int E_g_, E_f_;
public:
	E() : E_g_(E_g_return), E_f_(E_f_return) {}
	int E_g() { return E_g_; }
	int E_f() const { return E_f_; }
};


#endif // TEUCHOS_TEST_CLASSES_HPP
