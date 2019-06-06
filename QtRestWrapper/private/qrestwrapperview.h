#ifndef QRESTWRAPPERVIEW_H
#define QRESTWRAPPERVIEW_H

#include <QWebEngineView>
#include <QWidget>

#include "../qtrestwrapper_global.h"

class QTRESTWRAPPERSHARED_EXPORT TempWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TempWidget(QWidget *parent = nullptr) : QWidget(parent) {
    }

public slots:
    void invokeEvent() {
        closedEvent();
    }

signals:
    void closedEvent();
};

class QTRESTWRAPPERSHARED_EXPORT QRestWrapperView : public QWebEngineView
{
    Q_OBJECT
public:
    explicit QRestWrapperView(QWidget *parent = nullptr);
    virtual ~QRestWrapperView() override;
    TempWidget *tempWidget() {
        return m_tempWidget;
    }

    // QWidget interface
//protected:
    //virtual void closeEvent(QCloseEvent *event) override;

    // QObject interface
public:
    virtual bool event(QEvent *event) override;

private:
    TempWidget *m_tempWidget;
};

#endif // QRESTWRAPPERVIEW_H
