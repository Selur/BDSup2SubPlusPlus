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

#ifndef SUPDVD_H
#define SUPDVD_H

#include "substream.h"
#include "substreamdvd.h"

#include <QObject>
#include <QString>
#include <QFile>

class Palette;
class SubtitleProcessor;

class SupDVD : public QObject, public Substream, public SubstreamDVD
{
    Q_OBJECT

public:
    SupDVD(QString supFileName, QString ifoFileName, SubtitleProcessor* subtitleProcessor);
    ~SupDVD();

    void decode(int index);
    void readIfo();
    void writeIfo(QString filename, SubPicture &subPicture, Palette &palette);
    void readAllSupFrames();
    void setSrcPalette(Palette &palette);

    int languageIdx() { return _languageIdx; }
    int primaryColorIndex() { return _primaryColorIndex; }
    int numFrames();
    int numForcedFrames() { return _numForcedFrames; }

    qint64 endTime(int index);
    qint64 startTime(int index);
    qint64 startOffset(int index);

    bool isForced(int index);

    Bitmap &bitmap() { return _bitmap; }

    Palette &palette() { return _palette; }
    Palette &getSrcPalette() { return srcPalette; }

    QImage image();
    QImage image(Bitmap &bitmap);

    QList<uchar> createSupFrame(SubPictureDVD &subPicture, Bitmap &bitmap);
    QList<int> &getFrameAlpha(int index);
    QList<int> &getFramePal(int index);
    QList<int> getOriginalFrameAlpha(int index);
    QList<int> getOriginalFramePal(int index);

    SubPicture *subPicture(int index);

signals:
    void maxProgressChanged(qint64 maxProgress);
    void currentProgressChanged(qint64 currentProgress);

private:
    QString supFileName;
    QString ifoFileName;

    QList<SubPictureDVD> subPictures;

    const QList<uchar> IFOheader = { 0x44, 0x56, 0x44, 0x56, 0x49, 0x44, 0x45, 0x4F, 0x2D, 0x56, 0x54, 0x53 };

    QList<uchar> controlHeader = {
            0x00,													//  dummy byte (for shifting when forced)
            0x00, 0x00,												//  0: offset to end sequence
            0x01,													//  2: CMD 1: start displaying
            0x03, 0x32, 0x10,										//  3: CMD 3: Palette Info
            0x04, 0xff, 0xff,                                       //  6: CMD 4: Alpha Info
            0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,				//  9: CMD 5: sub position
            0x06, 0x00, 0x00, 0x00, 0x00,							// 16: CMD 6: rle offsets
            0xff,                                                   // 21: End of control header
            0x00, 0x00,												// 22: display duration in 90kHz/1024
            0x00, 0x00,												// 24: offset to end sequence (again)
            0x02, 0xff,                                             // 26: CMD 2: stop displaying
        };

    qint64 readSupFrame(qint64 ofs);
};

#endif // SUPDVD_H
