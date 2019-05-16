#ifndef QRESTWRAPPERUTILS_H
#define QRESTWRAPPERUTILS_H

#include <QMap>
#include <QString>



namespace QtRestWrapper {

class QRestWrapperUtils
{
public:
    QRestWrapperUtils();

public:
    // generates a unique random string, unique only, when only this function will be used to generate the random string
    static QString randomString(int length = 12);
    // utility function to split a cookie string into individual raw cookies
    static QMap<QString, QString> cookieSplit(const QString &cookieString);

private:
    static QVector<QString> m_generatedRandomStrings;
};

} // namespace QtRestWrapper

#endif // QRESTWRAPPERUTILS_H
