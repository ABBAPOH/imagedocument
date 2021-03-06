#include "imageview.h"
#include "imageview_p.h"

#include <ImageControl>

#include <QtWidgets/QAction>
#include <QtWidgets/QScrollBar>

#include <QtGui/QPainter>
#include <QtGui/QResizeEvent>

#include <QDebug>

void ImageViewPrivate::init()
{
    Q_Q(ImageView);

    q->horizontalScrollBar()->setRange(0, 0);
    q->horizontalScrollBar()->setValue(0);
    q->horizontalScrollBar()->setSingleStep(20);

    q->verticalScrollBar()->setRange(0, 0);
    q->verticalScrollBar()->setValue(0);
    q->verticalScrollBar()->setSingleStep(20);

    control.reset(new ImageControl(q));
    QObject::connect(control.data(), &ImageControl::documentChanged,
                     q, &ImageView::documentChanged);
    QObject::connect(control.data(), &ImageControl::updateRequested,
                     q->viewport(), static_cast<void(QWidget::*)()>(&QWidget::update));

    QObject::connect(q->horizontalScrollBar(), &QAbstractSlider::valueChanged,
                     q, &ImageView::onScrollBarValueChanged);
    QObject::connect(q->verticalScrollBar(), &QAbstractSlider::valueChanged,
                     q, &ImageView::onScrollBarValueChanged);

    QObject::connect(control.data(), &ImageControl::positionChanged,
                     q, &ImageView::onPositionChanged);
    QObject::connect(control.data(), &ImageControl::positionBoundsChanged,
                     q, &ImageView::onPositionBoundsChanged);

    createActions();
    retranslateUi();
}

void ImageViewPrivate::createActions()
{
    Q_Q(ImageView);

    actions[ImageView::ZoomIn].reset(new QAction);
    actions[ImageView::ZoomIn]->setShortcut(QKeySequence::ZoomIn);
    q->addAction(actions[ImageView::ZoomIn].data());
    q->connect(actions[ImageView::ZoomIn].data(), &QAction::triggered, q, &ImageView::zoomIn);

    actions[ImageView::ZoomOut].reset(new QAction);
    actions[ImageView::ZoomOut]->setShortcut(QKeySequence::ZoomOut);
    q->addAction(actions[ImageView::ZoomOut].data());
    q->connect(actions[ImageView::ZoomOut].data(), &QAction::triggered, q, &ImageView::zoomOut);

    actions[ImageView::NormalSize].reset(new QAction);
    actions[ImageView::NormalSize]->setShortcut(QKeySequence("Ctrl+0"));
    q->addAction(actions[ImageView::NormalSize].data());
    q->connect(actions[ImageView::NormalSize].data(),
            &QAction::triggered, q, &ImageView::normalSize);

    actions[ImageView::RotateLeft].reset(new QAction);
    actions[ImageView::RotateLeft]->setShortcut(QKeySequence("Ctrl+L"));
    q->addAction(actions[ImageView::RotateLeft].data());
    q->connect(actions[ImageView::RotateLeft].data(),
            &QAction::triggered, q, &ImageView::rotateLeft);

    actions[ImageView::RotateRight].reset(new QAction);
    actions[ImageView::RotateRight]->setShortcut(QKeySequence("Ctrl+R"));
    q->addAction(actions[ImageView::RotateRight].data());
    q->connect(actions[ImageView::RotateRight].data(),
            &QAction::triggered, q, &ImageView::rotateRight);

    actions[ImageView::FlipHorizontally].reset(new QAction);
    actions[ImageView::FlipHorizontally]->setShortcut(QKeySequence("Ctrl+Shift+H"));
    q->addAction(actions[ImageView::FlipHorizontally].data());
    q->connect(actions[ImageView::FlipHorizontally].data(),
            &QAction::triggered, q, &ImageView::flipHorizontally);

    actions[ImageView::FlipVertically].reset(new QAction);
    actions[ImageView::FlipVertically]->setShortcut(QKeySequence("Ctrl+Shift+V"));
    q->addAction(actions[ImageView::FlipVertically].data());
    q->connect(actions[ImageView::FlipVertically].data(),
            &QAction::triggered, q, &ImageView::flipVertically);
}

void ImageViewPrivate::retranslateUi()
{
    actions[ImageView::ZoomIn]->setText(ImageView::tr("Zoom in"));
    actions[ImageView::ZoomOut]->setText(ImageView::tr("Zoom out"));
    actions[ImageView::NormalSize]->setText(ImageView::tr("Normal size"));
    actions[ImageView::RotateLeft]->setText(ImageView::tr("Rotate Left"));
    actions[ImageView::RotateRight]->setText(ImageView::tr("Rotate Right"));
    actions[ImageView::FlipHorizontally]->setText(ImageView::tr("Flip horizontally"));
    actions[ImageView::FlipVertically]->setText(ImageView::tr("Flip vertically"));
}

ImageView::ImageView(QWidget *parent) :
    QAbstractScrollArea(parent),
    d_ptr(new ImageViewPrivate(this))
{
    Q_D(ImageView);
    d->init();
}

ImageView::~ImageView()
{
}

QAction *ImageView::action(Action action) const
{
    Q_D(const ImageView);

    if (action < 0 || action >= ActionsCount)
        return 0;

    return d->actions[action].data();
}

ImageDocumentPointer ImageView::document() const
{
    Q_D(const ImageView);
    return d->control->document();
}

void ImageView::setDocument(const ImageDocumentPointer &doc)
{
    Q_D(ImageView);
    d->control->setDocument(doc);
}

void ImageView::jumpTo(int index, int level)
{
    Q_D(ImageView);
    d->control->jumpTo(index, level);
}

void ImageView::zoomIn()
{
    Q_D(ImageView);
    d->control->zoomIn();
}

void ImageView::zoomOut()
{
    Q_D(ImageView);
    d->control->zoomOut();
}

void ImageView::normalSize()
{
    Q_D(ImageView);
    d->control->normalSize();
}

void ImageView::rotateLeft()
{
    Q_D(ImageView);
    d->control->rotateLeft();
}

void ImageView::rotateRight()
{
    Q_D(ImageView);
    d->control->rotateRight();
}

void ImageView::flipHorizontally()
{
    Q_D(ImageView);
    d->control->flipHorizontally();
}

void ImageView::flipVertically()
{
    Q_D(ImageView);
    d->control->flipVertically();
}

void ImageView::onScrollBarValueChanged()
{
    Q_D(ImageView);
    d->control->setPosition(QPoint(horizontalScrollBar()->value(), verticalScrollBar()->value()));
}

void ImageView::onPositionChanged(const QPoint &pos)
{
    horizontalScrollBar()->setValue(pos.x());
    verticalScrollBar()->setValue(pos.y());
}

void ImageView::onPositionBoundsChanged()
{
    Q_D(ImageView);

    const auto bounds = d->control->positionBounds();
    horizontalScrollBar()->setMinimum(bounds.topLeft().x());
    horizontalScrollBar()->setMaximum(bounds.bottomRight().x());

    verticalScrollBar()->setMinimum(bounds.topLeft().y());
    verticalScrollBar()->setMaximum(bounds.bottomRight().y());
}

void ImageView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    Q_D(ImageView);
    QPainter painter(viewport());
    d->control->paint(&painter);
}

void ImageView::mousePressEvent(QMouseEvent *event)
{
    Q_D(ImageView);
    d->control->mousePressEvent(event);
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
    Q_D(ImageView);
    d->control->mouseMoveEvent(event);
}

void ImageView::mouseReleaseEvent(QMouseEvent *event)
{
    Q_D(ImageView);
    d->control->mouseReleaseEvent(event);
}

bool ImageView::viewportEvent(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Resize : {
        Q_D(ImageView);
        d->control->setSize(static_cast<QResizeEvent *>(event)->size());
    }
    default:
        break;
    }

    return QAbstractScrollArea::viewportEvent(event);
}
