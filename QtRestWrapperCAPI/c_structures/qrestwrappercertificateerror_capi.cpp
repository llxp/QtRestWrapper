#include "qrestwrappercertificateerror_c.h"

#include <qrestwrappercertificateerror.h>

struct QRestWrapperCertificateError_CAPI qRestWrapperCertificateErrorCapiFromQRestWrapperCertificateError(const QRestWrapperCertificateError & qRestWrapperCertificateError)
{
    struct QRestWrapperCertificateError_CAPI newObj;
    QByteArray urlData = qRestWrapperCertificateError.url().toString().toUtf8();
    newObj.m_url = qurlCapiFromString(urlData.data());
    newObj.m_error = static_cast<Error_CAPI>(static_cast<int>(qRestWrapperCertificateError.error()));
    newObj.m_isOverridable = qRestWrapperCertificateError.isOverridable();
    newObj.m_errorDescription = static_cast<char *>(malloc(sizeof(char) * static_cast<size_t>(qRestWrapperCertificateError.errorDescription().length()) + 1));
    strcpy_s(newObj.m_errorDescription, static_cast<size_t>(qRestWrapperCertificateError.errorDescription().length()), qRestWrapperCertificateError.errorDescription().toUtf8().data());
    newObj.m_errorDescription[qRestWrapperCertificateError.errorDescription().length()] = '\0';
    return newObj;
}
