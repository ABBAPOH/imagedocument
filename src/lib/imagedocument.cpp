#include "imagedocument.h"
#include "imagedocument_p.h"

#include "defaulthandler.h"
#include "imageiohandler.h"
#include "dds/ddshandler.h"
#include "jpeg/jpeghandler_p.h"

#include <QtCore/QMimeDatabase>

ImageIOHandlerDatabase::ImageIOHandlerDatabase()
{
    map.insert("image/png", new DefaultHandlerPlugin());
#ifdef Q_OS_LINUX
    map.insert("image/jpeg", new JpegHandlerPlugin());
#endif
    map.insert("image/gif", new DefaultHandlerPlugin());
    map.insert("image/x-dds", new DdsHandlerPlugin());
}

ImageIOHandlerDatabase::~ImageIOHandlerDatabase()
{
    qDeleteAll(map);
}

ImageIOHandler *ImageIOHandlerDatabase::create(const QMimeType &mimeType)
{
    auto plugin = map.value(mimeType.name());
    if (!plugin)
        return 0;
    return plugin->create();
}

Q_GLOBAL_STATIC(ImageIOHandlerDatabase, static_instance)
ImageIOHandlerDatabase *ImageIOHandlerDatabase::instance()
{
    return static_instance();
}

void ImageDocumentPrivate::init()
{
    device = Q_NULLPTR;
    mipmapCount = 1;
    frameCount = 1;
    sides = ImageResource::NoSides;

    handler = 0;
}

bool ImageDocumentPrivate::initHandler()
{
    Q_Q(ImageDocument);

    if (!mimeType.isValid()) {
        error = ImageError(ImageError::MimeTypeError, ImageDocument::tr("Mime type is invalid"));
        return false;
    }

    auto db = ImageIOHandlerDatabase::instance();
    handler = db->create(mimeType);
    if (!handler) {
        error = ImageError(ImageError::UnsupportedFormatError, ImageDocument::tr("Unsupported format"));
        return false;
    }

    handler->setDocument(q);
    handler->setDevice(device);
    handler->setMimeType(mimeType);

    return true;
}

bool ImageDocumentPrivate::ensureHandlerInitialised() const
{
    if (handler)
        return true;
    return const_cast<ImageDocumentPrivate *>(this)->initHandler();
}

bool ImageDocumentPrivate::ensureDeviceOpened(QIODevice::OpenMode mode)
{
    if (!device) {
        error = ImageError(ImageError::DeviceError, ImageDocument::tr("Device error"));
        return false;
    }

    if ((device->openMode() & mode) != mode) {
        device->close();
        if (!device->open(mode)) {
            error = ImageError(ImageError::DeviceError, ImageDocument::tr("Device error"));
            return false;
        }
    }
    return true;
}

void ImageDocumentPrivate::killHandler()
{
    delete handler;
    handler = 0;
}

ImageDocument::ImageDocument(QObject *parent) :
    QObject(parent),
    d_ptr(new ImageDocumentPrivate(this))
{
    Q_D(ImageDocument);
    d->init();
}

ImageDocument::ImageDocument(const QString &fileName, QObject *parent) :
    QObject(parent),
    d_ptr(new ImageDocumentPrivate(this))
{
    Q_D(ImageDocument);
    d->init();
    setFileName(fileName);
}

ImageDocument::~ImageDocument()
{
    delete d_ptr;
}

QIODevice *ImageDocument::device() const
{
    Q_D(const ImageDocument);
    return d->device;
}

void ImageDocument::setDevice(QIODevice *device)
{
    Q_D(ImageDocument);
    if (d->device == device)
        return;
    d->file.reset();
    d->killHandler();
    d->device = device;
}

QString ImageDocument::fileName() const
{
    Q_D(const ImageDocument);
    return d->fileName;
}

void ImageDocument::setFileName(const QString &fileName)
{
    Q_D(ImageDocument);
    if (d->fileName == fileName)
        return;

    d->file.reset(new QFile(fileName));
    d->device = d->file.data();
    d->killHandler();

    setMimeType(QMimeDatabase().mimeTypeForFile(fileName));
}

QMimeType ImageDocument::mimeType() const
{
    Q_D(const ImageDocument);
    return d->mimeType;
}

void ImageDocument::setMimeType(const QMimeType &mimeType)
{
    Q_D(ImageDocument);
    if (d->mimeType == mimeType)
        return;
    d->mimeType = mimeType;
    d->killHandler();
}

void ImageDocument::setMimeType(const QString &name)
{
    auto type = QMimeDatabase().mimeTypeForName(name);
    setMimeType(type);
}

bool ImageDocument::hasError() const
{
    Q_D(const ImageDocument);
    return d->error.errorCode() != ImageError::NoError;
}

ImageError ImageDocument::error() const
{
    Q_D(const ImageDocument);
    return d->error;
}

ImageDocument::Capabilities ImageDocument::capabilities() const
{
    Q_D(const ImageDocument);

    if (!d->ensureHandlerInitialised())
        return NoCapabilities;

    return d->handler->capabilities();
}

bool ImageDocument::read()
{
    Q_D(ImageDocument);

    if (!d->ensureHandlerInitialised())
        return false;

    if (!d->ensureDeviceOpened(QIODevice::ReadOnly))
        return false;

    if (!d->handler->read()) {
        d->error = ImageError(ImageError::HandlerError, ImageDocument::tr("Handler error"));
        return false;
    }

    return true;
}

bool ImageDocument::write()
{
    Q_D(ImageDocument);

    if (!d->ensureHandlerInitialised())
        return false;

    if (!d->ensureDeviceOpened(QIODevice::WriteOnly))
        return false;

    if (!d->handler->write()) {
        d->error = ImageError(ImageError::HandlerError, ImageDocument::tr("Handler error"));
        return false;
    }

    return true;
}

int ImageDocument::mipmapCount() const
{
    Q_D(const ImageDocument);
    return d->mipmapCount;
}

void ImageDocument::setMipmapCount(int count)
{
    Q_D(ImageDocument);
    if (d->mipmapCount == count)
        return;
    d->mipmapCount = count;
}

int ImageDocument::frameCount() const
{
    Q_D(const ImageDocument);
    return d->frameCount;
}

void ImageDocument::setFrameCount(int count)
{
    Q_D(ImageDocument);
    if (d->frameCount == count)
        return;
    d->frameCount = count;
}

ImageResource::Sides ImageDocument::sides() const
{
    Q_D(const ImageDocument);
    return d->sides;
}

void ImageDocument::setSides(ImageResource::Sides sides)
{
    Q_D(ImageDocument);
    if (d->sides == sides)
        return;
    d->sides = sides;
}

QVector<QByteArray> ImageDocument::subTypes() const
{
    Q_D(const ImageDocument);

    if (!d->ensureHandlerInitialised())
        return QVector<QByteArray>();

    return d->handler->subTypes();
}

bool ImageDocument::supportsOption(ImageMeta::Option option)
{
    Q_D(const ImageDocument);

    if (!d->ensureHandlerInitialised())
        return false;

    return d->handler->supportsDocumentOption(option);
}

bool ImageDocument::supportsResourceOption(ImageMeta::Option option, const QByteArray subType)
{
    Q_D(const ImageDocument);

    if (!d->ensureHandlerInitialised())
        return false;

    return d->handler->supportsResourceOption(option, subType);
}

ImageMeta ImageDocument::meta() const
{
    Q_D(const ImageDocument);
    return d->meta;
}

void ImageDocument::setMeta(const ImageMeta &meta)
{
    Q_D(ImageDocument);
    d->meta = meta;
}

ImageResource ImageDocument::resource(int frame, int mipmap)
{
    Q_D(const ImageDocument);
    return d->elements.value(qMakePair(frame, mipmap));
}

void ImageDocument::setResource(const ImageResource &resource, int frame, int mipmap)
{
    Q_D(ImageDocument);
    d->elements.insert(qMakePair(frame, mipmap), resource);
}
