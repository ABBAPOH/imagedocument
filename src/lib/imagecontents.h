#ifndef IMAGECONTENTS_H
#define IMAGECONTENTS_H

#include <ImageExifMeta>

#include <QtCore/QSharedDataPointer>
#include <QtGui/QImage>

class ImageContentsData;

class ImageContents
{
public:
    enum Type {
        Image,
        Animation,
        ImageArray,
        Cubemap,
        VolumeTexture
    };

    ImageContents();
    ImageContents(const ImageContents &other);
    ImageContents &operator=(const ImageContents &other);
    ~ImageContents();

    Type type() const;
    void setType(Type t);

    QSize size() const;
    void setSize(QSize size);

    QImage::Format imageFormat() const;
    void setImageFormat(QImage::Format format);

    QString name() const;
    void setName(const QString &name);

    int imageCount() const;
    void setImageCount(int count);

    int mipmapCount() const;
    void setMipmapCount(int count);

    QImage image(int index = 0, int level = 0) const;
    void setImage(const QImage &image, int index = 0, int level = 0);

    int imageDelay(); // TODO: should we support separate delays for each frame?
    void setImageDelay(int delay); // TODO: this could be only called by handler. Move to DocData?

    int loopCount() const;
    void setLoopCount(int count);

    ImageExifMeta exifMeta() const;
    void setExifMeta(const ImageExifMeta &exif);

    void clear();
    void swap(ImageContents &other);

private:
    QSharedDataPointer<ImageContentsData> d;

    friend bool operator==(const ImageContents &lhs, const ImageContents &rhs);
};

bool operator==(const ImageContents &lhs, const ImageContents &rhs);

inline bool operator!=(const ImageContents &lhs, const ImageContents &rhs)
{
    return !(lhs == rhs);
}

#endif // IMAGECONTENTS_H
