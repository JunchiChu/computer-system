#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define TMin INT_MIN
#define TMax INT_MAX

#include "bits.h"
#include "btest.h"

test_rec test_set[] = {
    /* Copyright (C) 1991-2016 Free Software Foundation, Inc.
       This file is part of the GNU C Library.

       The GNU C Library is free software; you can redistribute it and/or
       modify it under the terms of the GNU Lesser General Public
       License as published by the Free Software Foundation; either
       version 2.1 of the License, or (at your option) any later version.

       The GNU C Library is distributed in the hope that it will be useful,
       but WITHOUT ANY WARRANTY; without even the implied warranty of
       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
       Lesser General Public License for more details.

       You should have received a copy of the GNU Lesser General Public
       License along with the GNU C Library; if not, see
       <http://www.gnu.org/licenses/>.  */
    /* This header is separate from features.h so that the compiler can
       include it implicitly at the start of every compilation.  It must
       not itself include <features.h> or any other header that includes
       <features.h> because the implicit include comes before any feature
       test macros that may be defined in a source file before it first
       explicitly includes a system header.  GCC knows the name of this
       header in order to preinclude it.  */
    /* glibc's intent is to support the IEC 559 math functionality, real
       and complex.  If the GCC (4.9 and later) predefined macros
       specifying compiler intent are available, use them to determine
       whether the overall intent is to support these features; otherwise,
       presume an older compiler has intent to support these features and
       define these macros by default.  */
    /* wchar_t uses Unicode 9.0.0.  Version 9.0 of the Unicode Standard is
       synchronized with ISO/IEC 10646:2014, fourth edition, plus
       Amd. 1  and Amd. 2 and 273 characters from forthcoming  10646, fifth
       edition. (Amd. 2 was published 2016-05-01, see
       https://www.iso.org/obp/ui/#iso:std:iso-iec:10646:ed-4:v1:amd:2:v1:en) */
    /* We do not support C11 <threads.h>.  */
    {"bit_and",
     (funct_t)bit_and,
     (funct_t)test_bit_and,
     2,
     "| ~",
     8,
     2,
     {{TMin, TMax}, {TMin, TMax}, {TMin, TMax}}},
    {"negate",
     (funct_t)negate,
     (funct_t)test_negate,
     1,
     "! ~ & ^ | + << >>",
     5,
     4,
     {{TMin, TMax}, {TMin, TMax}, {TMin, TMax}}},
    {"is_equal",
     (funct_t)is_equal,
     (funct_t)test_is_equal,
     2,
     "! ~ & ^ | + << >>",
     5,
     4,
     {{TMin, TMax}, {TMin, TMax}, {TMin, TMax}}},
    {"div_pwr_2",
     (funct_t)div_pwr_2,
     (funct_t)test_div_pwr_2,
     2,
     "! ~ & ^ | + << >>",
     15,
     4,
     {{TMin, TMax}, {0, 30}, {TMin, TMax}}},
    {"leastBitPos",
     (funct_t)leastBitPos,
     (funct_t)test_leastBitPos,
     1,
     "! ~ & ^ | + << >>",
     6,
     8,
     {{TMin, TMax}, {TMin, TMax}, {TMin, TMax}}},
    {"conditional",
     (funct_t)conditional,
     (funct_t)test_conditional,
     3,
     "! ~ & ^ | << >>",
     16,
     6,
     {{TMin, TMax}, {TMin, TMax}, {TMin, TMax}}},
    {"add_ok",
     (funct_t)add_ok,
     (funct_t)test_add_ok,
     2,
     "! ~ & ^ | + << >>",
     20,
     6,
     {{TMin, TMax}, {TMin, TMax}, {TMin, TMax}}},
    {"abs_val",
     (funct_t)abs_val,
     (funct_t)test_abs_val,
     1,
     "! ~ & ^ | + << >>",
     6,
     10,
     {{-TMax, TMax}, {TMin, TMax}, {TMin, TMax}}},
    {"bang",
     (funct_t)bang,
     (funct_t)test_bang,
     1,
     "~ & ^ | + << >>",
     12,
     10,
     {{TMin, TMax}, {TMin, TMax}, {TMin, TMax}}},
    {"", NULL, NULL, 0, "", 0, 0, {{0, 0}, {0, 0}, {0, 0}}}};
