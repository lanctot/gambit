//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Interface to LP solvers
//
// This file is part of Gambit
// Copyright (c) 2002, The Gambit Project
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//

#ifndef LPSOLVE_H
#define LPSOLVE_H

#include "libgambit/libgambit.h"
#include "lptab.h"
#include "bfs.h"

//
// This class implements a LP solver.  Its constructor takes as input a
// LP problem of the form maximize c x subject to A x<=b, x >= 0. 
// The last k equations can represent equalities (indicated by the 
// parameter "nequals").  
//
// All computation is done in the class constructor; when the constructor
// returns the computation has completed.  OptimumVector() returns the 
// solution.  The components are indexed by the columns of A, with the 
// excess columns representing the artificial and slack variables.   
//

template <class T> class LPSolve {
private:
  int  well_formed, feasible, bounded, aborted, flag, nvars, neqns,nequals;
  T total_cost,eps1,eps2,eps3,tmin;
  BFS<T> opt_bfs,dual_bfs;
  LPTableau<T> tab;
  gbtArray<bool> *UB, *LB;
  gbtArray<T> *ub, *lb;
  gbtVector<T> *xx, *cost; 
  gbtVector<T> y, x, d;

  void Solve(int phase = 0);
  int Enter(void);
  int Exit(int);
public:
  LPSolve(const gbtMatrix<T> &A, const gbtVector<T> &B, const gbtVector<T> &C,
	  int nequals);   // nequals = number of equalities (last nequals rows)
//  LPSolve(const gbtMatrix<T> &A, const gbtVector<T> &B, 
//	  const gbtVector<T> &C,  const gbtVector<int> &sense, 
//	  const gbtVector<int> &LB,  const gbtVector<T> &lb, 
//	  const gbtVector<int> &UB, const gbtVector<T> &ub);
  ~LPSolve();
  
  T OptimumCost(void) const;
  const gbtVector<T> &OptimumVector(void) const;
  const gbtList< BFS<T> > &GetAll(void);
  const LPTableau<T> &GetTableau();
  
  int IsAborted(void) const;
  int IsWellFormed(void) const;
  int IsFeasible(void) const;
  int IsBounded(void) const;
  long NumPivots(void) const;
  void OptBFS(BFS<T> &b) const;
  T Epsilon(int i = 2) const;
};

#endif   // LPSOLVE_H







