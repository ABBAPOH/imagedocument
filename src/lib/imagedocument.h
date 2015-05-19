#ifndef IMAGEDOCUMENT_H
#define IMAGEDOCUMENT_H

#include "imageresource.h"

#include <QtCore/QMimeType>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtGui/QImage>

class ImageDocumentPrivate;
class ImageDocument : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ImageDocument)
    Q_DISABLE_COPY(ImageDocument)

public:
    enum OpenModeFlag { NotOpen = 0x0, Read = 0x1, Write = 0x2, ReadWrite = Read | Write };
    Q_DECLARE_FLAGS(OpenMode, OpenModeFlag)

    enum CapabilityFlag {
        NoCapabilities = 0x0,
        SupportMipmaps = 0x1,
        SupportFrames = 0x2,
        SupportSides = 0x4,
        SupportSlices = 0x8,
        SupportLayers = 0x10,
        SupportSubTypes = 0x20, // exclusive options - either we have same subtype for
        SupportElementSubTypes = 0x40, // subimages (dds, blp) or different (icns)
        SupportExif = 0x80
    };
    Q_DECLARE_FLAGS(Capabilities, CapabilityFlag)

    explicit ImageDocument(QObject *parent = 0);
    ~ImageDocument();

    QIODevice *device() const;
    void setDevice(QIODevice *device);

    QString fileName() const;
    void setFileName(const QString &fileName);

    QMimeType mimeType() const;
    void setMimeType(const QMimeType &mimeType);
    void setMimeType(const QString &name);

    bool hasError() const;
    QString errorString() const;

    bool open(OpenMode mode);
    void close();
    OpenMode openMode() const;
    bool isOpen() const;

    Capabilities capabilities() const;

    bool read();
    bool write();

    int mipmapCount() const;
    void setMipmapCount(int count);

    int frameCount() const;
    void setFrameCount(int count);

    ImageResource::Sides sides() const;
    void setSides(ImageResource::Sides sides);

//    int layerCount() const;
//    void setLayerCount(int count);

    // TODO: use class ImageType {QByteArray id; QString name; QString description;};
    // instead of bytearrays
    QVector<QByteArray> subTypes() const;
    QByteArray subType() const;
    void setSubType(const QByteArray &subType);

    bool supportsOption(ImageResource::Option option);
    bool supportsElementOption(ImageResource::Option option, const QByteArray subType = QByteArray());

    QVariant option(ImageResource::Option option) const;
    void setOption(ImageResource::Option option, const QVariant &value);

    QVariant exifOption(ImageResource::ExifOption option) const;
    void setExifOption(ImageResource::ExifOption option, const QVariant &value);

    ImageResource resource(int frame = 0, int mipmap = 0);
    void setResource(const ImageResource &resource, int frame = 0, int mipmap = 0);

//    ImageMeta meta() const;

protected:
    ImageDocumentPrivate *d_ptr;
};

#endif // IMAGEDOCUMENT_H
