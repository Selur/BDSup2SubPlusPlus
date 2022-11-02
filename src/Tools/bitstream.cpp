/*
 * BDSup2Sub++ (C) 2012 Adam T.
 * Based on code from BDSup2Sub by Copyright 2009 Volker Oth (0xdeadbeef)
 * and Copyright 2012 Miklos Juhasz (mjuhasz)
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "bitstream.h"

BitStream::BitStream(QList<uchar> &buffer) :
    byteOfs(0),
    b(buffer[0] & 0xff),
    bits(8),
    buf(buffer)
{
}

int BitStream::readBits(int n)
{
    int retval = 0;
    while (n > 0)
    {
        // bit by bit
        retval <<= 1;
        if ((b & 0x80) == 0x80)
        {
            retval |= 1; // get next bit
        }
        b <<= 1;
        n--;
        if (--bits == 0)
        {
            if (byteOfs < (buf.size() - 1))
            {
                b = buf[++byteOfs] & 0xff;
                bits = 8;
            }
            else
            {
                bits = 0;
            }
        }
    }
    return retval;
}

void BitStream::syncToByte()
{
    if (bits !=8)
    {
        if (byteOfs < (buf.size() - 1))
        {
            b = buf[++byteOfs] & 0xff;
            bits = 8;
        }
        else
        {
            bits = 0;
        }
    }
}
