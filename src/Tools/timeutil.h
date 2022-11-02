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
#include <QRegularExpression>
#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
template <typename T> class QList;
#endif

static QRegularExpression timePattern = QRegularExpression("(\\d+):(\\d+):(\\d+)[:\\.](\\d+)");

class TimeUtil
{
public:
    TimeUtil();

    static qint64 timeStrToPTS(QString s, bool *ok = 0);
    static qint64 timeStrXmlToPTS(QString s, double fps);

    static QRegularExpression getTimePattern() { return timePattern; }

    static QString ptsToTimeStrXml(qint64 pts, double fps);
    static QString ptsToTimeStr(qint64 pts);
    static QString ptsToTimeStrIdx(qint64 pts);

    static QList<int> msToTime(qint64 ms);
};

#endif // TIMEUTIL_H
