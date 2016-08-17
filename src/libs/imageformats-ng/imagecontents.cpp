#include "imagecontents.h"

#include <QtCore/QDebug>

namespace {

struct FaceOffset
{
    int x, y;
};

static const FaceOffset faceOffsets[2][6] =  {
    { {2, 1}, {0, 1}, {1, 0}, {1, 2}, {1, 1}, {3, 1} },
    // TODO: fix vertical offsets
    { {2, 1}, {0, 1}, {1, 0}, {1, 2}, {1, 1}, {1, 3} }
};

static const FaceOffset multipliers[2] = { {4, 3}, {3, 4} };

} // namespace

class ImageContentsData : public QSharedData
{
public:
    typedef QPair<int, int> ImageIndex;

    ImageHeader header;
    QMap<ImageIndex, QImage> images;
    ImageExifMeta exif;

    QImage image(int side, int index, int level) const;
    void setImage(const QImage &image, int side, int index, int level);
};

QImage ImageContentsData::image(int side, int index, int level) const
{
    if (index < 0 || index >= header.imageCount()) {
        qWarning() << "Attempt to get image with index = " << index
                   << "which is out of bounds" << 0 << header.imageCount();
        return QImage();
    }
    if (header.mipmapCount() > 0) {
        if (level < 0 || level >= header.mipmapCount()) {
            qWarning() << "Attempt to get image with level = " << level
                       << "which is out of bounds" << 0 << header.mipmapCount();
            return QImage();
        }
    }

    const auto realIndex = index * (header.type() == ImageHeader::Cubemap ? 6 : 1) + side;
    return images.value(ImageContentsData::ImageIndex(realIndex, level));
}

void ImageContentsData::setImage(const QImage &image, int side, int index, int level)
{
    if (index < 0 || index >= header.imageCount()) {
        qWarning() << "Attempt to get image with index = " << index
                   << "which is out of bounds" << 0 << header.imageCount();
        return;
    }
    if (header.mipmapCount() > 0) {
        if (level < 0 || level >= header.mipmapCount()) {
            qWarning() << "Attempt to get image with level = " << level
                       << "which is out of bounds" << 0 << header.mipmapCount();
            return;
        }
    }

    if (header.imageFormat() == QImage::Format_Invalid) {
        header.setImageFormat(image.format());
    }
    QImage copy(image);
    if (image.format() != header.imageFormat())
        copy = image.convertToFormat(header.imageFormat());

    const auto realIndex = index * (header.type() == ImageHeader::Cubemap ? 6 : 1) + side;
    images.insert(ImageContentsData::ImageIndex(realIndex, level), copy);
}

/*!
    \class ImageContents
*/

ImageContents::ImageContents() :
    d(new ImageContentsData)
{

}

ImageContents::ImageContents(const QImage &image):
    d(new ImageContentsData)
{
    if (image.isNull())
        return;
    d->header.setType(ImageHeader::Image);
    d->header.setSize(image.size());
    d->header.setImageFormat(image.format());
    d->header.setImageCount(1);
    setImage(image);
}

ImageContents::ImageContents(const ImageContents &other) :
    d(other.d)
{

}

ImageContents &ImageContents::operator=(const ImageContents &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

ImageContents::~ImageContents()
{

}

ImageHeader ImageContents::header() const
{
    return d->header;
}

void ImageContents::setHeader(const ImageHeader &header)
{
    d->header = header;
}

QImage ImageContents::image(int index, int level) const
{
    return d->image(0, index, level);
}

void ImageContents::setImage(const QImage &image, int index, int level)
{
    d->setImage(image, 0, index, level);
}

QImage ImageContents::image(ImageContents::Side side, int index, int level) const
{
    Q_ASSERT(side >= PositiveX && side <= NegativeZ);
    return d->image(int(side), index, level);
}

void ImageContents::setImage(const QImage &image, ImageContents::Side side, int index, int level)
{
    Q_ASSERT(side >= PositiveX && side <= NegativeZ);
    d->setImage(image, int(side), index, level);
}

ImageExifMeta ImageContents::exifMeta() const
{
    return d->exif;
}

void ImageContents::setExifMeta(const ImageExifMeta &exif)
{
    d->exif = exif;
}

void ImageContents::clear()
{
    ImageContents c;
    swap(c);
}

Optional<ImageContents> ImageContents::toProjection(ImageContents::Projection projection) const
{
    if (d->header.type() != ImageHeader::Cubemap)
        return Nothing();

    const auto qSize = d->header.size();
    if (qSize.width() != qSize.height()) {
        qWarning() << "Invalid size for cube texture" << qSize;
        return Nothing();
    }

    ImageContents result;

    const auto size = qSize.width();

    for (int level = 0; level < d->header.mipmapCount(); ++level) {
        for (int index = 0; index < d->header.imageCount(); ++index) {
            QImage image(multipliers[projection].x * size,
                         multipliers[projection].y * size,
                         d->header.imageFormat());

            image.fill(0);

            for (int side = PositiveX; side <= NegativeZ; side++) {
                const auto face = this->image(Side(side), index, level);
                if (face.isNull())
                    continue; // Skip face.

                // Compute face offsets.
                const int offset_x = faceOffsets[projection][side].x * size;
                const int offset_y = faceOffsets[projection][side].y * size;

                // Copy face on the image.
                for (int y = 0; y < size; y++) {
                    const QRgb *src = reinterpret_cast<const QRgb *>(face.scanLine(y));
                    QRgb *dst = reinterpret_cast<QRgb *>(image.scanLine(y + offset_y)) + offset_x;
                    memcpy(dst, src, sizeof(QRgb) * unsigned(size));
                }
            }
            result.setImage(image, index, level);
        }
    }

    return QImage();
}

bool operator==(const ImageContents &lhs, const ImageContents &rhs)
{
    return lhs.d == rhs.d ||
            (lhs.d->header == rhs.d->header
             && lhs.d->images == rhs.d->images
             && lhs.d->exif == rhs.d->exif);
}

bool operator!=(const ImageContents &lhs, const ImageContents &rhs)
{
    return !(lhs == rhs);
}