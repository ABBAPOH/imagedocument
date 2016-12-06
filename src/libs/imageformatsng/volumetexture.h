#pragma once

#include <QtCore/QSharedDataPointer>
#include <QtGui/QImage>

class Size3D
{
public:
    Size3D() = default;
    Size3D(int w, int h, int d) : width(w), height(h), depth(d) {}

    int width {0};
    int height {0};
    int depth {0};
};

class VolumeTextureData;
class VolumeTexture
{
public:
    VolumeTexture() Q_DECL_NOEXCEPT;
    VolumeTexture(const VolumeTexture &);
    VolumeTexture(VolumeTexture &&) Q_DECL_NOEXCEPT;
    VolumeTexture(int width, int heigth, int depth = 1, QImage::Format format = QImage::Format_ARGB32);
    VolumeTexture(const QVector<QImage> &slices);
    ~VolumeTexture();

    VolumeTexture &operator=(const VolumeTexture &);
    VolumeTexture &operator=(VolumeTexture &&) Q_DECL_NOEXCEPT;

    bool isNull() const Q_DECL_NOEXCEPT;

    int width() const Q_DECL_NOEXCEPT;
    int height() const Q_DECL_NOEXCEPT;
    int depth() const Q_DECL_NOEXCEPT;

    QImage::Format format() const Q_DECL_NOEXCEPT;

    Size3D size() const;

    QRgb pixel(int x, int y, int z);
    void setPixel(int x, int y, int z, uint index_or_rgb);

    void fill(uint value);

    QImage slice(int slice);
    void setSlice(int index, const QImage &image);

    // VolumeTexture scaled(int width, int height, int depth);
    // VolumeTexture transformed(const QTransform &matrix, Qt::TransformationMode mode = Qt::FastTransformation) const

private:
    QSharedDataPointer<VolumeTextureData> d;
};
