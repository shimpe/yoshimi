/*
    Util.cpp - Miscellaneous functions

    Original ZynAddSubFX author Nasca Octavian Paul
    Copyright (C) 2002-2005 Nasca Octavian Paul
    Copyright 2009, Alan Calvert

    This file is part of yoshimi, which is free software: you can redistribute
    it and/or modify it under the terms of version 2 of the GNU General Public
    License as published by the Free Software Foundation.

    yoshimi is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.   See the GNU General Public License (version 2 or
    later) for more details.

    You should have received a copy of the GNU General Public License along with
    yoshimi; if not, write to the Free Software Foundation, Inc., 51 Franklin
    Street, Fifth Floor, Boston, MA  02110-1301, USA.

    This file is a derivative of the ZynAddSubFX original, modified October 2009
*/

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

#include "Misc/Util.h"

// Transform the velocity according the scaling parameter (velocity sensing)
float VelF(float velocity, unsigned char scaling)
{
    float x = powf(VELOCITY_MAX_SCALE, (64.0 - scaling) / 64.0);
    if (scaling == 127 || velocity > 0.99)
        return 1.0;
    else
        return powf(velocity, x);

//      return (scaling != 127 && velocity <= 0.99) ? powf(velocity, x) : 1.0;
}


// Get the detune in cents
float getdetune(unsigned char type, unsigned short int coarsedetune,
                      unsigned short int finedetune)
{
    float det = 0.0;
    float octdet = 0.0;
    float cdet = 0.0;
    float findet = 0.0;
    // Get Octave
    int octave = coarsedetune / 1024;
    if (octave >= 8)
        octave -= 16;
    octdet = octave * 1200.0;

    // Coarse and fine detune
    int cdetune = coarsedetune % 1024;
    if (cdetune > 512)
        cdetune -= 1024;

    int fdetune = finedetune - 8192;

    switch (type)
    {
    //	case 1: is used for the default (see below)
        case 2:
            cdet = fabsf(cdetune * 10.0);
            findet = fabsf(fdetune / 8192.0) * 10.0;
            break;
        case 3:
            cdet = fabsf(cdetune * 100);
            findet = powf(10, fabsf(fdetune / 8192.0) * 3.0) / 10.0 - 0.1;
            break;
        case 4:
            cdet = fabsf(cdetune * 701.95500087); // perfect fifth
            findet = (powf(2, fabsf(fdetune / 8192.0) * 12.0) - 1.0) / 4095 * 1200;
            break;
            // case ...: need to update N_DETUNE_TYPES, if you'll add more
        default:
            cdet = fabsf(cdetune * 50.0);
            findet = fabsf(fdetune / 8192.0) * 35.0; // almost like "Paul's Sound Designer 2"
            break;
    }
    if (finedetune < 8192)
        findet = -findet;
    if (cdetune < 0)
        cdet = -cdet;

    det = octdet + cdet + findet;
    return det;
}


bool isRegFile(string chkpath)
{
    struct stat st;
    if (!lstat(chkpath.c_str(), &st))
        if (S_ISREG(st.st_mode))
            return true;
    return false;
}


bool isDirectory(string chkpath)
{
    struct stat st;
    if (!lstat(chkpath.c_str(), &st))
        if (S_ISDIR(st.st_mode))
            return true;
    return false;
}


bool isFifo(string chkpath)
{
    struct stat st;
    if (!lstat(chkpath.c_str(), &st))
        if (S_ISFIFO(st.st_mode))
            return true;
    return false;
}


string asString(const float& number)
{
   ostringstream oss;
   oss.precision(3);
   oss.width(3);
   oss << number;
   return string(oss.str());
}


string asString(const int& number)
{
   ostringstream oss;
   oss << number;
   return string(oss.str());
}


string asString(const unsigned int& number)
{
   ostringstream oss;
   oss << number;
   return string(oss.str());
}


float string2float(const string& str)
{
    istringstream machine(str);
    float fval;
    machine >> fval;
    return fval;
}


int string2int(const string& str)
{
    istringstream machine(str);
    int intval;
    machine >> intval;
    return intval;
}


// make a filename legal
void legit_filename(string& fname)
{
    for (unsigned int i = 0; i < fname.size(); ++i)
    {
        char c = fname.at(i);
        if (!((c >= '0' && c <= '9')
              || (c >= 'A' && c <= 'Z')
              || (c >= 'a' && c <= 'z')
              || c == '-'
              || c == ' '
              || c == '.'))
            fname.at(i) = '_';
    }
}