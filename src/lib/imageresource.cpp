#include "imageresource.h"

#include <QtCore/QMap>

class ImageResourceData : public QSharedData
{
public:
    typedef QMap<ImageResource::Option, QVariant> OptionMap;
    typedef QMap<ImageResource::ExifOption, QVariant> ExifOptionMap;

    ImageResource::Type type;
    QVector<QImage> images;
    QByteArray subType;
    OptionMap options;
    ExifOptionMap exifOptions;
};

ImageResource::ImageResource(Type type) :
    d(new ImageResourceData)
{
    d->type = type;
    switch (type) {
    case Image: d->images.resize(1); break;
    case Cubemap: d->images.resize(6); break;
    }
}

ImageResource::ImageResource(const ImageResource &other) :
    d(other.d)
{
}

ImageResource &ImageResource::operator=(const ImageResource &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

ImageResource::~ImageResource()
{
}

QImage ImageResource::image() const
{
    return d->images.at(0);
}

void ImageResource::setImage(const QImage &image)
{
    d->images[0] = image;
}

static int sideToIndex(ImageResource::Side side)
{
    switch (side) {
    case ImageResource::PositiveX: return 0;
    case ImageResource::NegaviveX: return 1;
    case ImageResource::PositiveY: return 2;
    case ImageResource::NegaviveY: return 3;
    case ImageResource::PositiveZ: return 4;
    case ImageResource::NegaviveZ: return 5;
    }
    return -1;
}

QImage ImageResource::image(ImageResource::Side side)
{
    return d->images.at(sideToIndex(side));
}

void ImageResource::setImage(ImageResource::Side side, const QImage &image)
{
    d->images[sideToIndex(side)] = image;
}

QByteArray ImageResource::subType() const
{
    return d->subType;
}

void ImageResource::setSubType(const QByteArray &subType)
{
    d->subType = subType;
}

QVariant ImageResource::option(ImageResource::Option option) const
{
    return d->options.value(option);
}

void ImageResource::setOption(ImageResource::Option option, const QVariant &value)
{
    d->options.insert(option, value);
}

QVariant ImageResource::exifOption(ImageResource::ExifOption option) const
{
    return d->exifOptions.value(option);
}

void ImageResource::setExifOption(ImageResource::ExifOption option, const QVariant &value)
{
    d->exifOptions.insert(option, value);
}