#include "../include/qrestwrappercertificateerror.h"

QRestWrapperCertificateError::QRestWrapperCertificateError(Error error, QString errorDescription, bool isOverridable, QUrl url)
    : m_error(error),
      m_errorDescription(errorDescription),
      m_isOverridable(isOverridable),
      m_url(url)
{

}

QTRESTWRAPPERSHARED_EXPORT QRestWrapperCertificateError::Error QRestWrapperCertificateError::error() const
{
    return m_error;
}

QString QRestWrapperCertificateError::errorDescription() const
{
    return m_errorDescription;
}

bool QRestWrapperCertificateError::isOverridable() const
{
    return m_isOverridable;
}

QUrl QRestWrapperCertificateError::url() const
{
    return m_url;
}
