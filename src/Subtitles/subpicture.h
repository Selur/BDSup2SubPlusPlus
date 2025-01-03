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

#ifndef SUBPICTURE_H
#define SUBPICTURE_H

#include "erasepatch.h"

#include <QVector>
#include <QRect>
#include <QMap>

class SubPicture
{
public:
    SubPicture();
    SubPicture(const SubPicture& other);
    SubPicture(const SubPicture* other);
    virtual ~SubPicture() { }

    int screenWidth() { return _screenWidth; }
    void setScreenWidth(int width) { _screenWidth = width; }

    int screenHeight() { return _screenHeight; }
    void setScreenHeight(int height) { _screenHeight = height; }

    virtual int imageWidth()
    {
        int width;
        if (numberCompObjects == 1)
        {
            width = scaledImageRects[scaledImageRects.keys()[0]].width();
        }
        else
        {
            int left = scaledImageRects[scaledImageRects.keys()[0]].x() < scaledImageRects[scaledImageRects.keys()[1]].x() ?
                       scaledImageRects[scaledImageRects.keys()[0]].x() : scaledImageRects[scaledImageRects.keys()[1]].x();
            int right = scaledImageRects[scaledImageRects.keys()[0]].right() > scaledImageRects[scaledImageRects.keys()[1]].right() ?
                        scaledImageRects[scaledImageRects.keys()[0]].right() : scaledImageRects[scaledImageRects.keys()[1]].right();
            width = (right - left) + 1;
        }
        return width;
    }

    virtual int imageHeight()
    {
        if (numberCompObjects == 1)
        {
            return scaledImageRects[scaledImageRects.keys()[0]].height();
        }
        else if (scaledImageRects[scaledImageRects.keys()[0]].y() < scaledImageRects[scaledImageRects.keys()[1]].y())
        {
            return ((scaledImageRects[scaledImageRects.keys()[1]].y() +
                    scaledImageRects[scaledImageRects.keys()[1]].height()) - scaledImageRects[scaledImageRects.keys()[0]].y());
        }
        else
        {
            return ((scaledImageRects[scaledImageRects.keys()[0]].y() +
                    scaledImageRects[scaledImageRects.keys()[0]].height()) - scaledImageRects[scaledImageRects.keys()[1]].y());
        }
    }

    virtual int x()
    {
        if (numberCompObjects == 1)
        {
            return scaledImageRects[scaledImageRects.keys()[0]].x();
        }
        return scaledImageRects[scaledImageRects.keys()[0]].x() < scaledImageRects[scaledImageRects.keys()[1]].x() ?
                scaledImageRects[scaledImageRects.keys()[0]].x() : scaledImageRects[scaledImageRects.keys()[1]].x();
    }

    virtual int y()
    {
        if (numberCompObjects == 1)
        {
            return scaledImageRects[scaledImageRects.keys()[0]].y();
        }
        return scaledImageRects[scaledImageRects.keys()[0]].y() < scaledImageRects[scaledImageRects.keys()[1]].y() ?
                scaledImageRects[scaledImageRects.keys()[0]].y() : scaledImageRects[scaledImageRects.keys()[1]].y();
    }

    qint64 startTime() { return start; }
    void setStartTime(qint64 startTime) { start = startTime; }

    qint64 endTime() { return end; }
    void setEndTime(qint64 endTime) { end = endTime; }

    int compNum() { return compositionNumber; }
    void setCompNum(int compNum) { compositionNumber = compNum; }

    int numCompObjects() { return numberCompObjects; }
    void setNumCompObjects(int numCompObjects) { numberCompObjects = numCompObjects; }

    int numberOfWindows() { return numWindows; }
    void setNumberOfWindows(int num) { numWindows = num; }

    virtual bool isForced() { return forced; }
    virtual void setForced(bool isForced)
    {
        forced = isForced;
        forcedFlags[0] = forced ? 0x40 : 0;
        forcedFlags[1] = forced ? 0x40 : 0;
    }

    bool wasDecoded() { return decoded; }
    void setDecoded(bool wasDecoded) { decoded = wasDecoded; }

    bool exclude() { return excluded; }
    void setExclude(bool exclude) { excluded = exclude; }

    QMap<int, QRect> &imageSizes() { return scaledImageRects; }
    void setImageSizes(QMap<int, QRect> rects) { imageRects = rects; scaledImageRects = rects; }

    QMap<int, QRect> &windowSizes() { return scaledWindowRects; }
    void setWindowSizes(QMap<int, QRect> rects) { windowRects = rects; scaledWindowRects = rects; }
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QVector<int>& objectIDs() { return objectIds; }
#else
    QList<int>& objectIDs() { return objectIds; }
#endif

    QMap<int, int> forcedFlags;

    virtual SubPicture* copy();
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QVector<ErasePatch*> erasePatch;
#else
    QList<ErasePatch*> erasePatch;
#endif

  protected:
    int _imageWidth = 0;
    int _imageHeight = 0;
    int xOfs = 0;
    int yOfs = 0;

    QMap<int, QRect> scaledImageRects;
    QMap<int, QRect> imageRects;
    QMap<int, QRect> scaledWindowRects;
    QMap<int, QRect> windowRects;

    int _screenWidth = 0;
    int _screenHeight = 0;
    qint64 start = -1;
    qint64 end = 0;
    int compositionNumber = 0;
    int numberCompObjects = 0;
    int numWindows = 0;

    bool forced = false;
    bool decoded = false;
    bool excluded = false;
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QVector<int> objectIds;
#else
    QList<int> objectIds;
#endif
};

#endif // SUBPICTURE_H
