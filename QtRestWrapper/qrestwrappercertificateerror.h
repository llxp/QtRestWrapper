#ifndef QRESTWRAPPERCERTIFICATEERROR_H
#define QRESTWRAPPERCERTIFICATEERROR_H

#include <QObject>
#include <QUrl>
#include <QString>

#include "qtrestwrapper_global.h"

class QTRESTWRAPPERSHARED_EXPORT QRestWrapperCertificateError
{
public:
    enum Error : int {
        SslPinnedKeyNotInCertificateChain = -150,
        CertificateCommonNameInvalid = -200,
        CertificateDateInvalid = -201,
        CertificateAuthorityInvalid = -202,
        CertificateContainsErrors = -203,
        CertificateNoRevocationMechanism = -204,
        CertificateUnableToCheckRevocation = -205,
        CertificateRevoked = -206,
        CertificateInvalid = -207,
        CertificateWeakSignatureAlgorithm = -208,
        CertificateNonUniqueName = -210,
        CertificateWeakKey = -211,
        CertificateNameConstraintViolation = -212
    };
public:
    QRestWrapperCertificateError(){}
    QRestWrapperCertificateError(Error error, QString errorDescription, bool isOverridable, QUrl url);
    ~QRestWrapperCertificateError(){}
    Error error();
    QString errorDescription() const;
    bool isOverridable() const;
    QUrl url() const;

private:
    Error m_error;
    QString m_errorDescription;
    bool m_isOverridable;
    QUrl m_url;
};

Q_DECLARE_METATYPE(QRestWrapperCertificateError)

#endif // QRESTWRAPPERCERTIFICATEERROR_H
