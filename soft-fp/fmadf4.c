/* Implement fma using soft-fp.
   Copyright (C) 2013-2015 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   In addition to the permissions in the GNU Lesser General Public
   License, the Free Software Foundation gives you unlimited
   permission to link the compiled version of this file into
   combinations with other programs, and to distribute those
   combinations without any restriction coming from the use of this
   file.  (The Lesser General Public License restrictions do apply in
   other respects; for example, they cover modification of the file,
   and distribution when not linked into a combine executable.)

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <libc-internal.h>
#include <math.h>
#include "soft-fp.h"
#include "double.h"

double
__fma (double a, double b, double c)
{
  FP_DECL_EX;
  FP_DECL_D (A);
  FP_DECL_D (B);
  FP_DECL_D (C);
  FP_DECL_D (R);
  double r;

  FP_INIT_ROUNDMODE;
  FP_UNPACK_D (A, a);
  FP_UNPACK_D (B, b);
  FP_UNPACK_D (C, c);
  FP_FMA_D (R, A, B, C);
  /* R_e is not set in cases where it is not used in packing, but the
     compiler does not see that it is set in all cases where it is
     used, resulting in warnings that it may be used
     uninitialized.  */
  DIAG_PUSH_NEEDS_COMMENT;
#if __GNUC_PREREQ (4, 7)
  DIAG_IGNORE_NEEDS_COMMENT (4.9, "-Wmaybe-uninitialized");
#else
  DIAG_IGNORE_NEEDS_COMMENT (4.9, "-Wuninitialized");
#endif
  FP_PACK_D (r, R);
  DIAG_POP_NEEDS_COMMENT;
  FP_HANDLE_EXCEPTIONS;

  return r;
}
#ifndef __fma
weak_alias (__fma, fma)
#endif

#ifdef NO_LONG_DOUBLE
strong_alias (__fma, __fmal)
weak_alias (__fmal, fmal)
#endif
