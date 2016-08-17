#ifndef IMAGEHEADER_H
#define IMAGEHEADER_H

#include "imageio_global.h"

#include <QtCore/QSharedDataPointer>
#include <QtGui/QImage>

class ImageHeaderData;
class IMAGEIO_EXPORT ImageHeader
{
    Q_GADGET
public:
    enum Type {
        Invalid,
        Image,
        Animation,
        ImageArray,
        Cubemap,
        VolumeTexture
    };
    Q_ENUM(Type)

    ImageHeader();
    ImageHeader(const ImageHeader &other);
    ImageHeader(ImageHeader &&other);
    ~ImageHeader();

    ImageHeader &operator=(const ImageHeader &other);
    ImageHeader &operator=(ImageHeader &&other);

    bool isNull() const;

    Type type() const;
    void setType(Type t);

    QSize size() const;
    void setSize(QSize size);

    int width() const;
    int height() const;

    QImage::Format imageFormat() const;
    void setImageFormat(QImage::Format format);

    QString name() const;
    void setName(const QString &name);

    int imageCount() const;
    void setImageCount(int count);

    int mipmapCount() const;
    void setMipmapCount(int count);

    int imageDelay() const; // TODO: should we support separate delays for each frame?
    void setImageDelay(int delay); // TODO: this could be only called by handler. Move to DocData?

    int loopCount() const;
    void setLoopCount(int count);

    inline void swap(ImageHeader &other) { qSwap(d, other.d); }

private:
    QSharedDataPointer<ImageHeaderData> d;

    friend bool IMAGEIO_EXPORT operator==(const ImageHeader &lhs, const ImageHeader &rhs);
    friend bool IMAGEIO_EXPORT operator!=(const ImageHeader &lhs, const ImageHeader &rhs);
};

bool IMAGEIO_EXPORT operator==(const ImageHeader &lhs, const ImageHeader &rhs);
bool IMAGEIO_EXPORT operator!=(const ImageHeader &lhs, const ImageHeader &rhs);

#endif // IMAGEHEADER_H