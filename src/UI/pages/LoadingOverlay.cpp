#include "LoadingOverlay.h"
#include "ui_LoadingOverlay.h"
#include <QPainter>
#include <QEvent>
#include <QVBoxLayout>

void ModernSpinner::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int size = qMin(width(), height());
    p.translate(width() / 2, height() / 2);
    p.rotate(angle);

    QPen pen;
    pen.setWidth(4);
    pen.setColor(QColor(0, 120, 215));
    pen.setCapStyle(Qt::RoundCap);
    p.setPen(pen);

    int r = (size / 2) - 5;
    p.drawArc(QRect(-r, -r, 2 * r, 2 * r), 0 * 16, 270 * 16);
}

LoadingOverlay::LoadingOverlay(QWidget *parent)
    : QWidget(parent), ui(new Ui::LoadingOverlay)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground);

    QVBoxLayout *layout = new QVBoxLayout(ui->spinnerWidget);
    layout->setContentsMargins(0, 0, 0, 0);

    spinner = new ModernSpinner(ui->spinnerWidget);
    layout->addWidget(spinner);

    if (parent)
    {
        parent->installEventFilter(this);
        resize(parent->size());
    }
    hide();
}

LoadingOverlay::~LoadingOverlay()
{
    delete ui;
}

void LoadingOverlay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.fillRect(rect(), QColor(255, 255, 255, 128));
}

bool LoadingOverlay::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj == parent() && ev->type() == QEvent::Resize)
    {
        resize(static_cast<QWidget *>(obj)->size());
    }
    return QWidget::eventFilter(obj, ev);
}