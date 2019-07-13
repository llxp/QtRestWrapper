#include "../../include/private/qrestwrapperview.h"

#include <QWebEngineProfile>
#include <QLayout>
#include <QCloseEvent>

QRestWrapperView::QRestWrapperView(QWidget *parent)
    : QWebEngineView(parent), m_tempWidget(new TempWidget(parent))
{

}

QRestWrapperView::~QRestWrapperView()
{
    if(parentWidget() != nullptr && parentWidget()->layout() != nullptr) {
        parentWidget()->layout()->removeWidget(this);
        setParent(nullptr);
    }
    stop();
    disconnect();
    page()->disconnect();
    qDebug() << __FUNCTION__;
    //setParent(nullptr);
    //page()->profile()->deleteLater();
    //page()->deleteLater();
    //page()->setParent(nullptr);
    //page()->disconnect();
    //page()->profile()->deleteLater();
    //delete page();
}


//void QRestWrapperView::closeEvent(QCloseEvent *event)
//{
    //QWebEngineView::closeEvent(event);
    //qDebug() << "closed...";
    //emit QRestWrapperView::closedEvent();
    //event->accept();
//}


bool QRestWrapperView::event(QEvent *e)
{
    if(e != nullptr) {
        if(e->type() == QEvent::Close) {
            qDebug() << "close event...";
            //invokeSignal();
            m_tempWidget->invokeEvent();
        }
    }
    return QWebEngineView::event(e);
}
