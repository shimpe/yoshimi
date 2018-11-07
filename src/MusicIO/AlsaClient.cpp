/*
    AlsaClient.cpp - Alsa audio / Alsa midi

    Copyright 2009, Alan Calvert

    This file is part of yoshimi, which is free software: you can
    redistribute it and/or modify it under the terms of the GNU General
    Public License as published by the Free Software Foundation, either
    version 3 of the License, or (at your option) any later version.

    yoshimi is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with yoshimi.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>

using namespace std;

#include "Misc/Config.h"
#include "MusicIO/AlsaClient.h"

bool AlsaClient::openAudio(void)
{
    if (alsaEngine.openAudio())
    {
        Runtime.settings.Samplerate = getSamplerate();
        Runtime.settings.Buffersize = getBuffersize();
        return true;
    }
    cerr << "Error, AlsaClient audio open failed" << endl;
    return false;
}

bool AlsaClient::openMidi(void)
{
    if (alsaEngine.openMidi())
        return true;
    cerr << "Error, AlsaClient midi open failed" << endl;
    return false;
}