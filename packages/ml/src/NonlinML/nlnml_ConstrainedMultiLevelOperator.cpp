/*
#@HEADER
# ************************************************************************
#
#               ML: A Multilevel Preconditioner Package
#                 Copyright (2002) Sandia Corporation
#
# Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
# license for use of this work by or on behalf of the U.S. Government.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# Questions? Contact Jonathan Hu (jhu@sandia.gov) or Ray Tuminaro 
# (rstumin@sandia.gov).
#
# ************************************************************************
#@HEADER
*/
/* ******************************************************************** */
/* See the file COPYRIGHT for a complete copyright notice, contact      */
/* person and disclaimer.                                               */
/* ******************************************************************** */
/*!
 * \file nlnml_preconditioner1.cpp
 *
 * \class ML_Nox_Preconditioner
 *
 * \brief ML nonlinear preconditioner and solver
 *
 * \date Last update do Doxygen: 31-Mar-05
 *
 */
#include "ml_common.h"

#if defined(HAVE_ML_NOX) && defined(HAVE_ML_EPETRA) && defined(HAVE_ML_AZTECOO) && defined(HAVE_ML_TEUCHOS) && defined(HAVE_ML_IFPACK) && defined(HAVE_ML_AMESOS) && defined(HAVE_ML_EPETRAEXT)

// ----------   Includes   ----------
#include <ctime>
#include <iostream>

// this class
#include "nlnml_ConstrainedMultiLevelOperator.H"

/*----------------------------------------------------------------------*
 |  ctor (public)                                             m.gee 3/06|
 *----------------------------------------------------------------------*/
NLNML::NLNML_ConstrainedMultiLevelOperator::NLNML_ConstrainedMultiLevelOperator(
              RefCountPtr<ML_Epetra::MultiLevelOperator> ml_operator,
              RefCountPtr<NLNML::NLNML_CoarseLevelNoxInterface> coarseinterface) :
comm_(ml_operator->Comm()),
coarseinterface_(coarseinterface),
ml_operator_(ml_operator)
{
  label_ = "NLNML_ConstrainedMultiLevelOperator";
  return;
}


/*----------------------------------------------------------------------*
 |                                                            m.gee 3/06|
 *----------------------------------------------------------------------*/
int NLNML::NLNML_ConstrainedMultiLevelOperator::ApplyInverse(
                     const Epetra_MultiVector& X, Epetra_MultiVector& Y) const
{
  int err = ml_operator_->ApplyInverse(X,Y);
#if 0
  Epetra_Vector tmp(View,Y,0);
  coarseinterface_.ApplyAllConstraints(tmp);
#endif
  return err;
}















#endif
