#include "splash.h"

#define _S(a,b) 0x##b##a
#define SPLASHROW(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,c0,c1,c2,c3,c4,c5,c6,c7,c8,c9,d0,d1,d2,d3,d4,d5,d6,d7,d8,d9,e0,e1,e2,e3,e4,e5,e6,e7,e8,e9,f0,f1,f2,f3,f4,f5,f6,f7,f8,f9,g0,g1,g2,g3,g4,g5,g6,g7,g8,g9,h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,i0,i1,i2,i3,i4,i5,i6,i7,i8,i9,j0,j1,j2,j3,j4,j5,j6,j7) \
    _S(a0,a1),_S(a2,a3),_S(a4,a5),_S(a6,a7),_S(a8,a9),_S(b0,b1),_S(b2,b3),_S(b4,b5),_S(b6,b7),_S(b8,b9),_S(c0,c1),_S(c2,c3),_S(c4,c5),_S(c6,c7),_S(c8,c9),_S(d0,d1),_S(d2,d3),_S(d4,d5),_S(d6,d7),_S(d8,d9),_S(e0,e1),_S(e2,e3),_S(e4,e5),_S(e6,e7),_S(e8,e9), \
    _S(f0,f1),_S(f2,f3),_S(f4,f5),_S(f6,f7),_S(f8,f9),_S(g0,g1),_S(g2,g3),_S(g4,g5),_S(g6,g7),_S(g8,g9),_S(h0,h1),_S(h2,h3),_S(h4,h5),_S(h6,h7),_S(h8,h9),_S(i0,i1),_S(i2,i3),_S(i4,i5),_S(i6,i7),_S(i8,i9),_S(j0,j1),_S(j2,j3),_S(j4,j5),_S(j6,j7),

U8 splashLogoData[SPLASHLOGO_WIDTH*SPLASHLOGO_HEIGHT] = {
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,6,6,6,6,6,6,6,6,6,6,6,6,6,1,1,1,1,1,6,6,6,6,6,6,6,6,6,6,6,6,6,6,1,1,1,1,1,1,1,1,1,1,1,1,6,6,6,6,6,6,6,1,1,1,1,1,1,1,1,1,1,6,6,6,6,6,6,6,6,6,6,6,6,1,1,1,1,1,1,6,6,6,6,6,6,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,6,E,E,E,E,E,E,E,E,E,E,E,E,E,1,1,1,1,6,E,E,E,E,E,E,E,E,E,E,E,E,E,E,6,1,1,1,1,1,1,1,1,1,1,1,E,E,E,E,E,E,E,E,1,1,1,1,1,1,1,1,6,E,E,E,E,E,E,E,E,E,E,E,E,E,1,1,1,1,6,E,E,E,E,E,E,E,1)
    SPLASHROW(1,1,1,1,1,1,1,1,6,6,E,E,E,E,E,E,E,E,E,E,E,E,E,E,1,1,1,1,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,6,1,1,1,1,1,1,1,1,1,6,E,E,E,E,E,E,E,4,1,1,1,1,1,1,6,6,E,E,E,E,E,E,E,E,E,E,E,E,E,4,1,1,1,1,E,E,E,E,E,E,4,4,1)
    SPLASHROW(1,1,1,1,1,1,1,1,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,4,1,1,1,1,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,1,1,1,1,1,1,1,6,E,E,E,E,E,E,E,E,4,1,1,1,1,1,1,E,E,E,E,E,E,E,E,E,E,E,E,E,E,4,4,1,1,1,1,E,E,E,E,E,E,4,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,E,E,E,E,E,E,E,4,4,4,4,4,4,4,4,4,1,1,1,6,E,E,E,E,E,4,4,4,4,4,4,E,E,E,E,E,E,4,1,1,1,1,1,1,1,E,E,E,E,E,E,E,E,4,4,1,1,1,1,1,6,E,E,E,E,E,E,4,4,4,4,4,4,4,4,4,1,1,1,1,1,E,E,E,E,E,E,4,1,1)
    SPLASHROW(1,1,1,1,1,1,1,6,E,E,E,E,E,4,4,4,1,1,1,1,1,1,1,1,1,1,1,E,E,E,E,E,4,4,1,1,1,1,1,E,E,E,E,E,E,4,1,1,1,1,1,1,6,E,E,E,E,E,E,E,E,4,1,1,1,1,1,6,E,E,E,E,E,4,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,E,E,E,E,E,4,4,1,1)
    SPLASHROW(1,1,1,1,1,1,1,E,E,E,E,E,E,4,1,1,1,1,1,1,1,1,1,1,1,1,1,E,E,E,E,E,4,1,1,1,1,1,1,E,E,E,E,E,E,4,1,1,1,1,1,1,E,E,E,E,E,E,E,E,E,4,1,1,1,1,1,E,E,E,E,E,E,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,E,E,E,E,E,4,1,1,1)
    SPLASHROW(1,1,1,1,1,1,6,E,E,E,E,E,4,4,1,1,1,1,1,1,1,1,1,1,1,1,6,E,E,E,E,E,4,1,1,1,1,1,1,E,E,E,E,E,E,4,1,1,1,1,1,6,E,E,E,E,E,E,E,E,E,4,1,1,1,1,6,E,E,E,E,E,E,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,C,C,C,C,C,4,1,1,1)
    SPLASHROW(1,1,1,1,1,1,E,E,E,E,E,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,E,E,E,E,E,E,4,1,1,1,1,1,1,E,E,E,E,E,4,4,1,1,1,1,1,E,E,E,E,E,4,E,E,E,E,4,1,1,1,1,E,E,C,C,C,C,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,C,C,C,C,C,4,4,1,1,1)
    SPLASHROW(1,1,1,1,1,1,E,E,E,E,E,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,E,E,E,E,E,E,1,1,1,1,1,1,1,E,E,E,E,E,4,1,1,1,1,1,6,E,E,E,E,E,6,E,C,C,C,4,1,1,1,1,C,C,C,C,C,C,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,C,C,C,C,C,4,1,1,1,1)
    SPLASHROW(1,1,1,1,1,6,E,E,E,E,E,4,1,1,1,1,1,1,1,1,1,1,1,1,1,6,E,E,E,E,E,E,1,1,1,1,1,1,6,E,E,E,E,E,4,1,1,1,1,1,C,C,C,C,C,4,C,C,C,C,C,4,1,1,1,6,C,C,C,C,C,C,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,C,C,C,C,C,4,1,1,1,1)
    SPLASHROW(1,1,1,1,1,E,E,E,E,E,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,E,E,E,E,E,E,4,1,1,1,1,1,1,E,E,E,E,C,4,4,1,1,1,1,6,C,C,C,C,C,4,C,C,C,C,4,4,1,1,1,C,C,C,C,C,C,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,C,C,C,C,C,4,4,1,1,1,1)
    SPLASHROW(1,1,1,1,1,E,E,E,E,E,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,E,E,E,E,E,E,4,1,1,1,1,1,6,C,C,C,C,C,4,1,1,1,1,1,C,C,C,C,C,4,4,C,C,C,C,4,1,1,1,1,C,C,C,C,C,C,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,C,C,C,C,C,4,1,1,1,1,1)
    SPLASHROW(1,1,1,1,6,E,E,E,E,E,4,1,1,1,1,1,1,1,1,1,1,1,1,1,6,E,E,C,C,C,C,4,1,1,1,1,6,C,C,C,C,C,4,4,1,1,1,1,6,C,C,C,C,C,4,1,C,C,C,C,4,1,1,1,1,C,C,C,C,C,C,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,C,C,C,C,C,4,1,1,1,1,1)
    SPLASHROW(1,1,1,1,E,E,E,E,E,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,C,C,C,C,C,C,4,4,1,1,1,6,C,C,C,C,C,C,4,1,1,1,1,1,C,C,C,C,C,4,4,1,C,C,C,C,4,1,1,1,1,C,C,C,C,C,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,C,C,C,C,C,4,4,1,1,1,1,1)
    SPLASHROW(1,1,1,1,E,E,E,E,E,4,6,6,C,C,C,C,6,6,6,6,6,1,1,1,C,C,C,C,C,C,4,6,6,6,6,C,C,C,C,C,C,4,4,1,1,1,1,1,C,C,C,C,C,4,1,1,C,C,C,C,4,1,1,1,1,C,C,C,C,C,4,6,6,6,6,6,6,6,6,6,6,1,1,1,1,1,C,C,C,C,C,4,1,1,1,1,1,1)
    SPLASHROW(1,1,1,6,E,E,E,E,E,6,E,E,E,E,E,E,C,C,C,C,C,C,1,1,C,C,C,C,C,C,6,C,C,C,C,C,C,C,C,C,4,4,1,1,1,1,1,6,C,C,C,C,4,4,1,6,C,C,C,C,4,1,1,1,6,C,C,C,C,C,6,C,C,C,C,C,C,C,C,C,C,C,1,1,1,6,C,C,C,C,C,4,1,1,1,1,1,1)
    SPLASHROW(1,1,1,E,E,E,E,E,4,E,E,E,E,E,E,C,C,C,C,C,4,4,1,1,C,C,C,C,C,4,C,C,C,C,C,C,C,C,C,C,4,1,1,1,1,1,1,C,C,C,C,C,4,1,1,C,C,C,C,C,4,1,1,1,C,C,C,C,C,4,C,C,C,C,C,C,C,C,C,C,C,4,1,1,1,C,C,C,C,C,4,4,1,1,1,1,1,1)
    SPLASHROW(1,1,1,E,E,E,E,E,4,E,E,E,E,C,C,C,C,C,C,C,4,1,1,1,C,C,C,C,C,4,C,C,C,C,C,C,C,C,C,C,4,1,1,1,1,1,6,C,C,C,C,4,4,1,1,C,C,C,C,C,4,1,1,1,C,C,C,C,C,4,C,C,C,C,C,C,C,C,C,C,C,1,1,1,1,C,C,C,C,C,4,1,1,1,1,1,1,1)
    SPLASHROW(1,1,6,E,E,E,E,E,4,1,4,4,4,4,6,C,C,C,C,C,4,1,1,6,C,C,C,C,C,4,1,4,4,4,C,C,C,C,C,C,4,1,1,1,1,1,C,C,C,C,C,4,1,1,1,C,C,C,C,4,4,1,1,6,C,C,C,C,C,4,1,4,4,4,6,C,C,C,C,C,C,1,1,1,6,C,C,C,C,C,4,1,1,1,1,1,1,1)
    SPLASHROW(1,1,E,E,E,E,E,E,4,1,1,1,1,1,C,C,C,C,C,4,4,1,1,C,C,C,C,C,4,4,1,1,1,1,C,C,C,C,C,C,4,1,1,1,1,6,C,C,C,C,4,4,1,1,1,C,C,C,C,4,1,1,1,C,C,C,C,C,4,4,1,1,1,1,C,C,C,C,C,C,4,1,1,1,C,C,C,C,C,4,4,1,1,1,1,1,1,1)
    SPLASHROW(1,1,E,E,E,E,E,C,1,1,1,1,1,1,C,C,C,C,C,4,1,1,1,C,C,C,C,C,4,1,1,1,1,1,C,C,C,C,C,C,4,1,1,1,1,C,C,C,C,C,4,1,1,1,1,C,C,C,C,4,1,1,1,C,C,C,C,C,4,1,1,1,1,1,C,C,C,C,C,C,4,1,1,1,C,C,C,C,C,4,1,1,1,1,1,1,1,1)
    SPLASHROW(1,6,E,E,E,C,C,C,1,1,1,1,1,6,C,C,C,C,C,4,1,1,6,C,C,C,C,C,4,1,1,1,1,1,C,C,C,C,C,C,4,1,1,1,6,C,C,C,C,6,6,6,6,6,6,C,C,C,C,4,1,1,6,C,C,C,C,C,4,1,1,1,1,6,C,C,C,C,C,C,4,1,1,6,E,E,E,E,E,4,1,1,1,1,1,1,1,1)
    SPLASHROW(1,E,C,C,C,C,C,4,1,1,1,1,1,C,C,C,C,C,4,4,1,1,C,C,C,C,C,4,4,1,1,1,1,1,C,C,C,C,C,C,1,1,1,1,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,4,1,1,C,C,C,C,C,4,4,1,1,1,1,E,E,E,E,E,E,4,4,1,1,E,E,E,E,E,E,4,1,1,1,1,1,1,1,1)
    SPLASHROW(1,C,C,C,C,C,C,4,1,1,1,1,1,C,C,C,C,C,4,1,1,1,C,C,C,C,C,4,1,1,1,1,1,6,C,C,C,C,C,C,1,1,1,6,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,4,1,1,E,E,E,E,E,4,1,1,1,1,1,E,E,E,E,E,E,4,1,1,1,E,E,E,E,E,E,1,1,1,1,1,1,1,1,1)
    SPLASHROW(6,C,C,C,C,C,C,4,1,1,1,1,6,C,C,C,C,C,4,1,1,6,C,C,C,C,C,4,1,1,1,1,1,C,C,C,C,C,C,C,1,1,1,C,C,C,C,C,C,C,C,C,E,E,E,E,E,E,E,4,1,6,E,E,E,E,E,4,1,1,1,1,1,E,E,E,E,E,E,4,1,1,6,E,E,E,E,E,E,1,1,1,1,1,1,1,1,1)
    SPLASHROW(C,C,C,C,C,C,4,4,1,1,1,1,C,C,C,C,C,4,4,1,1,C,C,C,C,C,4,4,1,1,1,1,6,C,C,C,C,C,C,4,1,1,6,C,C,E,E,E,4,4,4,4,4,4,E,E,E,E,4,4,1,E,E,E,E,E,4,4,1,1,1,1,1,E,E,E,E,E,4,4,1,1,E,E,E,E,E,E,4,1,1,1,1,1,1,1,1,1)
    SPLASHROW(C,C,C,C,C,C,4,1,1,1,1,1,C,C,C,C,C,4,1,1,1,C,C,C,C,C,4,1,1,1,1,1,C,C,E,E,E,E,E,4,1,6,E,E,E,E,E,E,1,1,1,1,1,1,E,E,E,E,4,1,1,E,E,E,E,E,4,1,1,1,1,1,1,E,E,E,E,E,4,1,1,1,E,E,E,E,E,E,4,1,1,1,1,1,1,1,1,1)
    SPLASHROW(C,C,C,C,C,C,6,1,1,1,1,6,C,C,C,C,C,4,1,1,6,C,C,C,C,C,4,1,1,1,1,6,E,E,E,E,E,E,4,4,1,E,E,E,E,E,E,4,1,1,1,1,1,6,E,E,E,E,4,1,1,E,E,E,E,E,6,1,1,1,1,1,6,E,E,E,E,E,4,1,1,6,E,E,E,E,E,E,4,1,1,1,1,1,1,1,1,1)
    SPLASHROW(C,C,C,C,C,C,C,6,6,6,6,C,C,C,C,C,4,4,1,1,C,C,C,C,C,6,6,6,6,6,6,E,E,E,E,E,E,4,4,1,6,E,E,E,E,E,E,4,1,1,1,1,1,E,E,E,E,E,4,1,1,E,E,E,E,E,E,6,6,6,6,6,E,E,E,E,E,4,4,1,1,E,E,E,E,E,E,4,4,1,1,1,1,1,1,1,1,1)
    SPLASHROW(C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,4,1,1,1,C,C,C,C,E,E,E,E,E,E,E,E,E,E,E,E,4,4,1,1,E,E,E,E,E,E,4,4,1,1,1,1,1,E,E,E,E,E,4,1,1,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,4,1,1,1,E,E,E,E,E,E,4,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,4,1,1,6,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,4,4,1,1,1,E,E,E,E,E,E,4,1,1,1,1,1,1,E,E,E,E,E,4,1,1,1,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,4,1,1,1,E,E,E,E,E,E,4,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,4,1,1,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,4,4,1,1,1,1,E,E,E,E,E,4,4,1,1,1,1,1,1,E,E,E,E,E,4,1,1,1,E,E,E,E,E,E,E,E,E,E,E,E,E,E,4,4,1,1,1,E,E,E,E,E,4,4,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,4,4,4,4,4,4,4,4,4,4,4,4,4,4,1,1,1,1,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,1,1,1,1,1,4,4,4,4,4,4,1,1,1,1,1,1,1,1,4,4,4,4,4,1,1,1,1,4,4,4,4,4,4,4,4,4,4,4,4,4,4,1,1,1,1,4,4,4,4,4,4,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,6,6,6,6,6,6,6,6,6,1,1,1,1,1,1,1,1,1,1,1,1,6,6,6,6,6,6,6,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,E,E,E,E,E,E,E,E,E,E,1,1,1,1,1,1,1,1,1,1,6,E,E,E,E,E,E,E,E,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,E,E,E,E,E,E,E,E,E,E,E,1,1,1,1,1,1,1,1,6,6,E,E,E,E,E,E,E,E,E,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4,4,4,4,4,E,E,E,E,E,4,1,1,1,1,1,1,6,6,E,E,E,4,4,4,E,E,E,E,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,E,E,E,E,4,1,1,1,1,1,1,6,E,E,E,4,4,1,1,6,E,E,E,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,6,E,E,E,E,4,1,1,1,1,1,6,6,E,E,E,4,1,1,6,6,E,E,E,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,E,E,E,E,4,4,1,1,1,1,1,6,E,E,E,4,4,1,1,6,E,E,E,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,6,E,E,E,E,4,1,1,1,1,1,1,6,E,E,E,4,1,1,1,6,E,E,E,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,E,E,E,E,4,4,1,1,1,1,1,6,6,E,E,E,4,1,1,6,6,E,E,E,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,6,6,6,6,6,6,6,E,E,E,E,E,4,1,1,1,1,1,1,6,E,E,E,E,4,1,1,6,E,E,E,E,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,E,E,E,E,E,E,E,E,E,E,E,4,4,1,1,1,1,1,1,6,E,E,E,E,1,1,1,6,E,E,E,E,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,E,E,E,E,E,E,E,E,E,E,E,4,4,1,1,1,1,1,1,6,6,E,E,E,E,1,1,6,6,E,E,E,E,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,6,E,E,E,4,4,4,4,4,4,4,4,4,1,1,1,1,1,1,1,6,E,E,E,E,4,1,1,6,E,E,E,E,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,E,E,E,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,E,E,E,E,4,1,1,6,E,E,E,E,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,6,E,E,E,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,6,E,E,E,E,4,1,6,6,E,E,E,E,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,E,E,E,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,E,E,E,E,4,4,1,6,E,E,E,E,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,E,E,E,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,E,E,E,E,4,1,6,6,E,E,E,E,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,6,E,E,E,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,6,E,E,E,E,4,1,6,E,E,E,E,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,E,E,E,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,E,E,E,E,E,4,1,6,E,E,E,E,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,E,E,E,4,6,6,6,6,6,6,6,1,1,1,6,6,6,6,1,1,1,E,E,E,E,E,4,6,E,E,E,E,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,6,E,E,E,E,E,E,E,E,E,E,E,E,1,6,6,E,E,E,E,1,1,6,E,E,E,E,E,E,E,E,E,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,E,E,E,E,E,E,E,E,E,E,E,E,4,4,1,E,E,E,E,4,1,1,1,E,E,E,E,E,E,E,E,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
    SPLASHROW(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4,4,4,4,4,4,4,4,4,4,4,4,1,1,1,4,4,4,4,1,1,1,1,4,4,4,4,4,4,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
};
