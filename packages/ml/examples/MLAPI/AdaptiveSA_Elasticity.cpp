
//@HEADER
// ************************************************************************
// 
//               ML: A Multilevel Preconditioner Package
//                 Copyright (2002) Sandia Corporation
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
// ************************************************************************
//@HEADER

// usage: 
// the exe takes the path to the example as input parameter, e.g.
// AdaptiveSA_Elasticity.exe ../ExampleMatrices/sphere

#ifdef HAVE_MPI
#include "mpi.h"
#endif

#include "ml_config.h"
#include "ml_common.h"
#ifdef HAVE_ML_MLAPI
#include "MLAPI.h"

#include "Epetra_Map.h"
#include "Epetra_CrsMatrix.h"

using namespace Teuchos;
using namespace MLAPI;

// ============== //
// example driver //
// ============== //

int main(int argc, char *argv[])
{
  char filename[200];
  int i = 0;

#ifdef HAVE_MPI
  MPI_Init(&argc,&argv);
  Epetra_MpiComm comm(MPI_COMM_WORLD);
#else
  Epetra_SerialComm Comm;
#endif
  int proc  = comm.MyPID();
  int nproc = comm.NumProc();

  // Initialize the workspace and set the output level
  Init();
  
  try {
  // create a Epetra_Map from file
  sprintf(filename,"%s/data_update%d.txt",argv[1],nproc);
  Epetra_Map* map = Epetra_ML_readupdatevector(filename,comm);
  if (!map) {
     cout << "**ERR**: could not read map, number of procs ok?\n"; throw -1; }
     
  // read the Epetra_RowMatrix
  sprintf(filename,"%s/data_matrix.txt",argv[1]);
  Epetra_CrsMatrix* Afine = Epetra_ML_readaztecmatrix(filename,*map,comm);

  if (!Afine) {
     cout << "**ERR**: could not read matrix\n"; throw -1; }

  // read the nullspace
  int dimNS = 6;
  
  // read the rhs
  Epetra_MultiVector* Rhs = new Epetra_MultiVector(*map,1,true);
  sprintf(filename,"%s/data_rhs.txt",argv[1]);
  bool ok = Epetra_ML_readaztecvector(filename,*Rhs,*map,comm,0);
  if (!ok) {
     cout << "**ERR**: could not read rhs\n"; throw -1; }
       
  Space FineSpace(map->NumGlobalElements());
  Operator A(FineSpace, FineSpace, Afine, true);

    Teuchos::ParameterList List;
    List.set("smoother: type", "symmetric Gauss-Seidel");
    List.set("smoother: sweeps", 2);
    List.set("smoother: damping factor", 1.0);
    List.set("coarse: type", "Amesos-KLU");
    List.set("coarse: max size", 32);
    List.set("adapt: max reduction", 0.1);
    List.get("adapt: iters fine", 45);
    List.get("adapt: iters coarse", 5);

    int NumPDEEqns = dimNS;
    int MaxLevels  = 10;
    MultiLevelAdaptiveSA Prec(A, List, NumPDEEqns, MaxLevels);

    // ================================================= //
    // setup the null space, either by default component //
    // or by reading from file. Here the null space has  //
    // 6 components.                                     //
    // Variable UseAdapt toggles the use of adaptation.  //
    // ================================================= //

    bool ReadKernel = true;

    MultiVector NSfine(FineSpace,dimNS);
    if (!ReadKernel) {
      NSfine = 0.0;
      for (int i = 0 ; i < NSfine.GetMyLength() ; ++i)
	    for (int j = 0 ; j < NumPDEEqns ;++j)
		    if (i % NumPDEEqns == j)
  			    NSfine(i,j) = 1.0;
    } 
    else {
      Epetra_MultiVector* Epetra_NSfine = new Epetra_MultiVector(*map,dimNS,true);
      for (int i=0; i<dimNS; i++)
      {
        sprintf(filename,"%s/data_nullsp%d.txt",argv[1],i);
        bool ok = Epetra_ML_readaztecvector(filename,*Epetra_NSfine,*map,comm,i);
        if (!ok) {
          cout << "**ERR**: could not read nullspace\n"; throw -1; }
      }
      for (int i = 0 ; i < NSfine.GetMyLength() ; ++i)
        for (int v = 0 ; v < dimNS ; ++v) 
          NSfine(i, v) = (*Epetra_NSfine)[v][i]; 
    }

    Prec.SetNullSpace(NSfine);
    Prec.Compute();

    cout << "Current candidates = " << NSfine.GetNumVectors() << endl;
    cout << "Enter number of additional candidates = ";
    int NumAdditional;
    cin >> NumAdditional;
    for (int i = 0 ; i < NumAdditional ; ++i) {
      Prec.IncrementNullSpace();
      Prec.Compute();
    }

    // test the solver
    MultiVector LHS(FineSpace);
    MultiVector RHS(FineSpace);

    LHS.Random();
    RHS = 0.0;

    List.set("krylov: type", "cg");
    Krylov(A, LHS, RHS, Prec, List);

    Finalize(); 

  }
  catch (const int e) {
    cerr << "Caught integer exception, code = " << e << endl;
  }
  catch (...) {
    cerr << "Caught exception..." << endl;
  }
     
#ifdef HAVE_MPI
  MPI_Finalize(); 
#endif

  return(0);
}




#endif // #if defined(HAVE_ML_MLAPI)
