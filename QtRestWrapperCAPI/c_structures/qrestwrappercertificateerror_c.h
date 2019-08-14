#ifndef QRESTWRAPPERCERTIFICATEERROR_C_H
#define QRESTWRAPPERCERTIFICATEERROR_C_H

#include "qurl_c.h"

enum Error_CAPI : int {
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

struct QRestWrapperCertificateError_CAPI
{
    Error_CAPI m_error;
    char *m_errorDescription;
    bool m_isOverridable;
    struct qurlCAPI *m_url;
};

class QRestWrapperCertificateError;
struct QRestWrapperCertificateError_CAPI *qRestWrapperCertificateErrorCapiFromQRestWrapperCertificateError(const QRestWrapperCertificateError & qRestWrapperCertificateError);

#endif // QRESTWRAPPERCERTIFICATEERROR_C_H
