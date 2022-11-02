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

#ifndef SUBSTREAMDVD_H
#define SUBSTREAMDVD_H

#include <QtCore/QScopedPointer>
#include <QList>
#include <QImage>

#include <Subtitles/bitmap.h>
#include <Subtitles/palette.h>

class SubPictureDVD;
class FileBuffer;
class SubtitleProcessor;

class SubstreamDVD
{

public:
    SubstreamDVD();
    virtual ~SubstreamDVD();

    virtual void setSrcPalette(Palette &palette) = 0;
    void decode(SubPictureDVD &pic, SubtitleProcessor* subtitleProcessor);

    virtual int languageIdx() = 0;

    virtual Palette &getSrcPalette() = 0;
    static Palette decodePalette(SubPictureDVD &pic, Palette &palette, int alphaCrop);

    QList<uchar> encodeLines(Bitmap &bitmap, bool even);
    virtual QList<int> &getFrameAlpha(int index) = 0;
    virtual QList<int> &getFramePal(int index) = 0;
    virtual QList<int> getOriginalFrameAlpha(int index) = 0;
    virtual QList<int> getOriginalFramePal(int index) = 0;

protected:
    Bitmap _bitmap;

    Palette srcPalette;
    Palette _palette;

    SubtitleProcessor* subtitleProcessor = 0;

    QScopedPointer<FileBuffer> fileBuffer;

    QList<int> lastAlpha = { 0, 0xf, 0xf, 0xf };
    QList<SubPictureDVD> subPictures;

    int screenWidth = 720;
    int screenHeight = 576;
    int ofsXglob = 0;
    int ofsYglob = 0;
    int delayGlob = 0;
    int _languageIdx = 0;
    int _primaryColorIndex = 0;

private:
    void decodeLine(QList<uchar> src, int srcOfs, int srcLen, QImage &trg, int trgOfs, int width, int maxPixels);

    Bitmap decodeImage(SubPictureDVD &pic, int transIdx);
};

#endif // SUBSTREAMDVD_H
