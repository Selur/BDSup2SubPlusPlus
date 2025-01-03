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

#ifndef SUPXML_H
#define SUPXML_H

#include "substream.h"
#include "bitmap.h"
#include "palette.h"

#include <QObject>
#include <QFile>
#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
#include <QXmlDefaultHandler>
#else
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>
#endif
#include <QStringList>
#include <QString>

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
#include <QVector>
#else
#include <QList>
#endif
#include <QScopedPointer>

class SubtitleProcessor;
class SubPictureXML;
class QImage;
class BitStream;
class XmlHandler;

enum class Resolution : int;

class SupXML : public QObject, public Substream
{
    Q_OBJECT

#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
    class XmlHandler : public QXmlDefaultHandler
#else
    class XmlHandler : public QXmlStreamReader
#endif
    {
    public:
        XmlHandler(SupXML* parent) { this->parent = parent; }

        bool characters(const QString &ch);
        bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
        bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts);
#else
        bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlStreamAttributes &atts);
#endif

    private:

        bool valid = false;

        QStringList xmlStates = { "bdn", "description", "name", "language", "format", "events", "event", "graphic" };

        QString txt;
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
        QVector<int> getResolutions(Resolution resolution);
#else
        QList<int> getResolutions(Resolution resolution);
#endif
        Resolution getResolution (QString string);

        SubPictureXML *subPicture = nullptr;

        SupXML* parent;

        enum class XmlState { BDN, DESCRIPT, NAME, LANGUAGE, FORMAT, EVENTS, EVENT, GRAPHIC, UNKNOWN };

        XmlState state;
        XmlState findState(QString string);
    };

public:
    SupXML(QString fileName, SubtitleProcessor* subtitleProcessor);
    ~SupXML();

    void decode(int index);
    void readAllImages();
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    void writeXml(QString filename, QVector<SubPicture *> pics);
#else
    void writeXml(QString filename, QList<SubPicture *> pics);
#endif
    int primaryColorIndex() { return _primaryColorIndex; }
    int numFrames();
    int numForcedFrames() { return _numForcedFrames; }

    qint64 endTime(int index);
    qint64 startTime(int index);
    qint64 startOffset(int /*index*/) { return 0; }

    double getFps() { return fps; }

    bool isForced(int index);

    Bitmap &bitmap() { return _bitmap; }

    Palette &palette() { return _palette; }

    QImage image();
    QImage image(Bitmap &bitmap);

    QString getLanguage() { return language; }
    QString getPNGname(QString filename, int idx);

    SubPicture *subPicture(int index);

signals:
    void maxProgressChanged(qint64 maxProgress);
    void currentProgressChanged(qint64 currentProgress);

private:
    int _primaryColorIndex = 0;
    int numToImport = 0;
    double fps;
    double fpsXml;

    Bitmap _bitmap;

    Palette _palette;

    QScopedPointer<QFile> xmlFile;

    QString title = "";
    QString pathName;
    QString language = "deu";
    QString xmlFileName;
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QVector<SubPictureXML> subPictures;
#else
    QList<SubPictureXML> subPictures;
#endif
    Resolution resolution;

    SubtitleProcessor* subtitleProcessor = 0;

    double XmlFps(double fps);
};

#endif // SUPXML_H
