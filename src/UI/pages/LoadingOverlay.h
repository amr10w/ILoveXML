#pragma once
#include <QWidget>
#include <QTimer>
#include <QtConcurrent>
#include <QFutureWatcher>

class ModernSpinner : public QWidget
{
    Q_OBJECT
public:
    ModernSpinner(QWidget *parent = nullptr) : QWidget(parent), angle(0)
    {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [this]()
                {
            angle = (angle + 10) % 360;
            update(); });
        timer->start(16);
    }

protected:
    void paintEvent(QPaintEvent *) override;

private:
    int angle;
    QTimer *timer;
};

namespace Ui
{
    class LoadingOverlay;
}

class LoadingOverlay : public QWidget
{
    Q_OBJECT

public:
    explicit LoadingOverlay(QWidget *parent = nullptr);
    ~LoadingOverlay();

protected:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *ev) override;

private:
    Ui::LoadingOverlay *ui;
    ModernSpinner *spinner;
};