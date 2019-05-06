#include "qrestwrapperurlinterceptor.h"
#include "qtrestwrapper.h"

#include <QRegularExpression>

namespace QtRestWrapper {

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

void QRestWrapperUrlInterceptor::setRestWrapper(QRestWrapper *restWrapper)
{
    m_restWrapper = restWrapper;
}

void QRestWrapperUrlInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info)
{
    bool shouldBeAllowed = false;
    QRestWrapperPrivate *ptr = static_cast<QRestWrapperPrivate *>(m_restWrapper);
    for(auto &allowedUrlPattern : ptr->getAllowedUrlPatterns()) {
        QRegularExpression re(allowedUrlPattern, QRegularExpression::DotMatchesEverythingOption);
        QRegularExpressionMatch match = re.match(QUrl::fromPercentEncoding(info.requestUrl().toString().toUtf8()));
        if(match.hasMatch()) {
            shouldBeAllowed = true;
            break;
        }
    }
    for(auto &forbiddenUrlPattern : ptr->getForbiddenUrlPatterns()) {
        QRegularExpression re(forbiddenUrlPattern, QRegularExpression::DotMatchesEverythingOption);
        QRegularExpressionMatch match = re.match(QUrl::fromPercentEncoding(info.requestUrl().toString().toUtf8()));
        if(match.hasMatch()) {
            shouldBeAllowed = false;
            break;
        }
    }
    if(shouldBeAllowed == false) {
        info.block(true);
    }
    checkAuthenticationPage(info.requestUrl());
}

} // namespace QtRestWrapper
