#include "qrestwrapperurlinterceptor.h"
#include "qrestwrapper.h"

#include <QRegularExpression>
#include <QWebEngineUrlRequestInterceptor>

namespace QtRestWrapper {

// temp class to get access to the filter lists stored in the QRestWrapper class
class QRestWrapperPrivate : public QRestWrapper {
public:
    QVector<QString> getAllowedUrlPatterns() const {
        return m_allowedUrlPatterns;
    }
    QVector<QString> getForbiddenUrlPatterns() const {
        return m_forbiddenUrlPatterns;
    }
};

QRestWrapperUrlInterceptor::QRestWrapperUrlInterceptor(QObject *parent) : QWebEngineUrlRequestInterceptor(parent)
{

}

// testable
void QRestWrapperUrlInterceptor::setRestWrapper(QRestWrapper *restWrapper)
{
    m_restWrapper = restWrapper;
}

// not testable, because QWebEngineUrlRequestInfo is not instantiable
void QRestWrapperUrlInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info)
{
    bool shouldBeAllowed = false;

    // cast to the temp class to get access to the filter lists
    QRestWrapperPrivate *ptr = static_cast<QRestWrapperPrivate *>(m_restWrapper);

    shouldBeAllowed = checkForAllowedList(ptr->getAllowedUrlPatterns(), info.requestUrl());
    shouldBeAllowed = checkForForbiddenList(ptr->getForbiddenUrlPatterns(), info.requestUrl());

    // check if the request should be blocked
    if(shouldBeAllowed == false) {
        // block the request
        info.block(true);
    }
    // invoke the signal, to check, if the current
    // url is the successful authentication url
    checkAuthenticationPage(info.requestUrl());
}

// testable
bool QRestWrapperUrlInterceptor::checkForAllowedList(const QVector<QString> &allowedList, const QUrl &url) const
{
    // match for the allowed patterns
    // and set to shouldBeAllowed = true,
    // if on the allowed list
    for(auto &allowedUrlPattern : allowedList) {
        QRegularExpression re(allowedUrlPattern, QRegularExpression::DotMatchesEverythingOption);
        QRegularExpressionMatch match = re.match(QUrl::fromPercentEncoding(url.toString().toUtf8()));
        if(match.hasMatch()) {
            return true;
        }
    }
    return false;
}

// testable
bool QRestWrapperUrlInterceptor::checkForForbiddenList(const QVector<QString> &forbiddenList, const QUrl &url) const
{
    // match for the forbidden patterns
    // and set to shouldBeAllowed = false,
    // if on the forbidden list
    // a way to implement gray listing
    for(auto &forbiddenUrlPattern : forbiddenList) {
        QRegularExpression re(forbiddenUrlPattern, QRegularExpression::DotMatchesEverythingOption);
        QRegularExpressionMatch match = re.match(QUrl::fromPercentEncoding(url.toString().toUtf8()));
        if(match.hasMatch()) {
            return false;
        }
    }
    return true;
}

} // namespace QtRestWrapper
