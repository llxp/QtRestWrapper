#include "../../include/private/qrestwrapperutils.h"

#include <QVector>
#include <QRandomGenerator>

using QtRestWrapper::QRestWrapperUtils;

QRestWrapperUtils::QRestWrapperUtils()
{

}

/*
 * random string utility functions
 */

QVector<QString> QRestWrapperUtils::m_generatedRandomStrings;

int constexpr strLength(const char* str)
{
    return *str ? 1 + strLength(str + 1) : 0;
}

QString QRestWrapperUtils::randomString(int length)
{
    static constexpr char possibleCharacters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    static constexpr int possibleCharactersLength = strLength(possibleCharacters);

    QString randomString;
    while(m_generatedRandomStrings.contains(randomString)) {
        randomString = "";
        for (int i=0; i<length; ++i) {
            auto rnd = QRandomGenerator::global()->generate();
            int index = rnd % possibleCharactersLength;
            QChar nextChar = possibleCharacters[index];
            randomString.append(nextChar);
        }
    }
    m_generatedRandomStrings.append(randomString);
    return randomString;
}

QMap<QString, QString> QRestWrapperUtils::cookieSplit(const QString &cookieString)
{
    QMap<QString, QString> splittedCookies;
    for(auto &kvPairString: cookieString.split('&')) {
        auto kvPairList = kvPairString.split('=');
        splittedCookies[std::move(kvPairList[0])]= std::move(kvPairList[1]);
    }
    return splittedCookies;
}
