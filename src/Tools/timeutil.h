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

#ifndef TIMEUTIL_H
#define TIMEUTIL_H

#include <QString>
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
#include <QRegExp>
#include <QVector>

static QRegExp timePattern = QRegExp("(\\d+):(\\d+):(\\d+)[:\\.](\\d+)");
#else
#include <QList>
#include <QRegularExpression>

static QRegularExpression timePattern = QRegularExpression("(\\d+):(\\d+):(\\d+)[:\\.](\\d+)");
#endif

class TimeUtil
{
public:
    TimeUtil();

    static qint64 timeStrToPTS(QString s, bool *ok = 0);
    static qint64 timeStrXmlToPTS(QString s, double fps);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    static QRegExp getTimePattern() { return timePattern; }
#else
    static QRegularExpression getTimePattern() { return timePattern; }
#endif
    static QString ptsToTimeStrXml(qint64 pts, double fps);
    static QString ptsToTimeStr(qint64 pts);
    static QString ptsToTimeStrIdx(qint64 pts);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    static QVector<int> msToTime(qint64 ms);
#else
    static QList<int> msToTime(qint64 ms);
#endif
};

#endif // TIMEUTIL_H
