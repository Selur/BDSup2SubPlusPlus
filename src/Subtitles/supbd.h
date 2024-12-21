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

#ifndef SUPBD_H
#define SUPBD_H

#include "bitmap.h"
#include "imageobject.h"
#include "palette.h"
#include "substream.h"
#include "types.h"

#include <QObject>
#include <QScopedPointer>
#include <QString>
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
#include <QVector>
#else
#include <QList>
#endif
class SubtitleProcessor;
class SubPictureBD;
class FileBuffer;

struct ODS;
struct PDS;
struct WDS;
struct PCS;

class SupBD : public QObject, public Substream
{
    Q_OBJECT

  public:
    SupBD(QString fileName, SubtitleProcessor *subtitleProcessor);
    ~SupBD();

    void decode(int index);
    void readAllSupFrames();

    int primaryColorIndex() { return _primaryColorIndex; }
    int numFrames();
    int numForcedFrames() { return _numForcedFrames; }

    qint64 endTime(int index);
    qint64 startTime(int index);
    qint64 startOffset(int index);

    double getFps(int index);

    bool isForced(int index);

    Bitmap &bitmap() { return _bitmap; }

    Palette &palette() { return _palette; }

    QImage image();
    QImage image(Bitmap &bitmap);

    SubPicture *subPicture(int index);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QVector<uchar> createSupFrame(SubPicture *subPicture, Bitmap &bm, Palette &pal, bool forcedOnly);
#else
    QList<uchar> createSupFrame(SubPicture *subPicture, Bitmap &bm, Palette &pal, bool forcedOnly);
#endif

  signals:
    void maxProgressChanged(qint64 maxProgress);
    void currentProgressChanged(qint64 currentProgress);

  private:
    struct SupSegment {
        int type = 0;
        int size = 0;
        qint64 timestamp = 0;
        int offset = 0;
    };

    int _primaryColorIndex = 0;

    Bitmap _bitmap;

    Palette _palette;

    QScopedPointer<FileBuffer> fileBuffer;

    QString supFileName;
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QVector<SubPictureBD> subPictures;
#else
    QList<SubPictureBD> subPictures;
#endif

    SubtitleProcessor *subtitleProcessor = 0;

    void decode(SubPictureBD *subPicture);
    PCS parsePCS(SupSegment *segment, QString &msg);
    WDS parseWDS(SupSegment *segment);

    int getFpsId(double fps);
    PDS parsePDS(SupSegment *segment, QString &msg);

    double getFpsFromID(int id);

    ODS parseODS(SupSegment *segment, QString &msg, bool forceFirst, bool &isFirst);

    Bitmap decodeImage(SubPictureBD *subPicture, int transIdx);

    CompositionState getCompositionState(SupSegment *segment);

    Palette decodePalette(SubPictureBD *subPicture);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QVector<uchar> encodeImage(Bitmap &bm);
#else
    QList<uchar> encodeImage(Bitmap &bm);
#endif
    SupSegment readSegment(int offset);

    void findImageArea(SubPictureBD *subPicture);

    bool imagesAreMergeable(SubPictureBD &currentSub, SubPictureBD &prevSub);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QVector<uchar> packetHeader =
#else
    QList<uchar> packetHeader =
#endif
      {
        0x50, 0x47,              // 0:  "PG"
        0x00, 0x00, 0x00, 0x00,  // 2:  PTS - presentation time stamp
        0x00, 0x00, 0x00, 0x00,  // 6:  DTS - decoding time stamp
        0x00,                    // 10: segment_type
        0x00, 0x00,              // 11: segment_length (bytes following till next PG)
      };
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QVector<uchar> headerPCSStart =
#else
    QList<uchar> headerPCSStart =
#endif
      {
        0x00, 0x00, 0x00, 0x00,  // 0: video_width, video_height
        0x10,                    // 4: hi nibble: frame_rate (0x10=24p), lo nibble: reserved
        0x00, 0x00,              // 5: composition_number (increased by start and end header)
        0x80,                    // 7: composition_state (0x80: epoch start)
        0x00,                    // 8: palette_update_flag (0x80), 7bit reserved
        0x00,                    // 9: palette_id_ref (0..7)
        0x01,                    // 10: number_of_composition_objects (0..2)
        0x00, 0x00,              // 11: 16bit object_id_ref
        0x00,                    // 13: window_id_ref (0..1)
        0x00,                    // 14: object_cropped_flag: 0x80, forced_on_flag = 0x040, 6bit reserved
        0x00, 0x00, 0x00, 0x00   // 15: composition_object_horizontal_position, composition_object_vertical_position
      };
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QVector<uchar> headerPCSNext =
#else
    QList<uchar> headerPCSNext =
#endif
      {
        0x00, 0x01,             // 11: 16bit object_id_ref
        0x01,                   // 13: window_id_ref (0..1)
        0x00,                   // 14: object_cropped_flag: 0x80, forced_on_flag = 0x040, 6bit reserved
        0x00, 0x00, 0x00, 0x00  // 15: composition_object_horizontal_position, composition_object_vertical_position
      };
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QVector<uchar> headerPCSEnd =
#else
    QList<uchar> headerPCSEnd =
#endif
      {
        0x00, 0x00, 0x00, 0x00,  // 0: video_width, video_height
        0x10,                    // 4: hi nibble: frame_rate (0x10=24p), lo nibble: reserved
        0x00, 0x00,              // 5: composition_number (increased by start and end header)
        0x00,                    // 7: composition_state (0x00: normal)
        0x00,                    // 8: palette_update_flag (0x80), 7bit reserved
        0x00,                    // 9: palette_id_ref (0..7)
        0x00,                    // 10: number_of_composition_objects (0..2)
      };

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QVector<uchar> headerODSFirst =
#else
    QList<uchar> headerODSFirst =
#endif
      {
        0x00, 0x00,              // 0: object_id
        0x00,                    // 2: object_version_number
        0xC0,                    // 3: first_in_sequence (0x80), last_in_sequence (0x40), 6bits reserved
        0x00, 0x00, 0x00,        // 4: object_data_length - full RLE buffer length (including 4 bytes size info)
        0x00, 0x00, 0x00, 0x00,  // 7: object_width, object_height
      };
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QVector<uchar> headerODSNext =
#else
    QList<uchar> headerODSNext =
#endif
      {
        0x00, 0x00,  // 0: object_id
        0x00,        // 2: object_version_number
        0x40,        // 3: first_in_sequence (0x80), last_in_sequence (0x40), 6bits reserved
      };
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QVector<uchar> headerWDS =
#else
    QList<uchar> headerWDS =
#endif
      {
        0x00,                    // 0 : number of windows (0..2)
        0x00,                    // 1 : window id (0..1)
        0x00, 0x00, 0x00, 0x00,  // 2 : x-ofs, y-ofs
        0x00, 0x00, 0x00, 0x00   // 6 : width, height
      };
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QVector<uchar> headerWDSNext =
#else
    QList<uchar> headerWDSNext =
#endif
      {
        0x01,                    // 1 : window id (0..1)
        0x00, 0x00, 0x00, 0x00,  // 2 : x-ofs, y-ofs
        0x00, 0x00, 0x00, 0x00   // 6 : width, height
      };
};

#endif  // SUPBD_H
