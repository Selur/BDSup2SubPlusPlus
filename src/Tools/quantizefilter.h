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

#ifndef QUANTIZEFILTER_H
#define QUANTIZEFILTER_H

#include <QColor>
#include <QImage>
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
#include <QVector>
#else
#include <QList>
#endif
/**
 * Floyd-Steinberg dithering matrix.
 */
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
static QVector<int> matrix = {
#else
static QList<int> matrix = {
#endif
  0, 0, 0, 0, 0, 7, 3, 5, 1,
};

class QuantizeFilter
{
    class OctTreeQuantizer
    {
        struct OctTreeNode
        {
            int children = 0;
            int level = 0;
            OctTreeNode* parent = 0;
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
            QVector<OctTreeNode*> leaf = QVector<OctTreeNode*>(16);
#else
            QList<OctTreeNode*> leaf = QList<OctTreeNode*>(16);
#endif
            bool isLeaf = false;
            int count = 0;
            int totalAlpha = 0;
            int totalRed = 0;
            int totalGreen = 0;
            int totalBlue = 0;
            int index = 0;
        };

    public:
        ~OctTreeQuantizer()
        {
            for (int i = 0; i < colorList.size(); ++i)
            {
                for (int j = 0; j < colorList[i].size(); ++j)
                {
                    delete colorList[i][j];
                    colorList[i][j] = 0;
                }
            }
        }
        void addPixels(QImage &image);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
        void buildColorTable(QVector<QRgb> inPixels, QVector<QRgb>& table);
#else
        void buildColorTable(QList<QRgb> inPixels, QList<QRgb>& table);
#endif
        void setup(int numColors);

        int indexForColor(QRgb argb);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
        QVector<QRgb> buildColorTable();
#else
        QList<QRgb> buildColorTable();
#endif
      private:
        static constexpr int MAX_LEVEL = 5;

        int colors = 0;
        int maximumColors = 256;
        int nodes = 0;
        int reduceColors = 512;

        OctTreeNode* root = new OctTreeNode;
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
        QVector<QVector<OctTreeNode*>> colorList = QVector<QVector<OctTreeNode*>>(MAX_LEVEL + 1);
#else
        QList<QList<OctTreeNode*>> colorList = QList<QList<OctTreeNode*>>(MAX_LEVEL + 1);
#endif
        void insertColor(QRgb rgb);
        void reduceTree(int numColors);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
        int buildColorTable(OctTreeNode* node, QVector<QRgb>& table, int index);
    } quantizer;
#else
        int buildColorTable(OctTreeNode* node, QList<QRgb>& table, int index);
    } quantizer;
#endif

  public:
    void setNumColors(int numColors);
    void setDither(
      bool dither)
    {
      this->dither = dither;
    }
    void setSerpentine(
      bool serpentine)
    {
      this->serpentine = serpentine;
    }

    int clamp(
      int c)
    {
      if (c < 0) {
        return 0;
      }
      if (c > 255) {
        return 255;
      }
      return c;
    }

    int numColors() { return _numColors; }

    bool getDither() { return dither; }
    bool getSerpentine() { return serpentine; }
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QVector<QRgb> quantize(QImage inImage, QImage* outImage, int width, int height, int numColors, bool dither, bool serpentine);
#else
    QList<QRgb> quantize(QImage inImage, QImage* outImage, int width, int height, int numColors, bool dither, bool serpentine);
#endif
  private:
    static constexpr int sum = 3 + 5 + 7 + 1;
    int _numColors = 255;

    bool dither = false;
    bool serpentine = true;
};

#endif // QUANTIZEFILTER_H
