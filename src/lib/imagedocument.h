#ifndef IMAGEDOCUMENT_H
#define IMAGEDOCUMENT_H

#include <QtCore/QObject>
#include <QtGui/QImage>

class ImageDocumentPrivate;
class ImageDocument : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ImageDocument)
    Q_DISABLE_COPY(ImageDocument)

public:
    explicit ImageDocument(QObject *parent = 0);
    ~ImageDocument();

    QIODevice *device() const;
    void setDevice(QIODevice *device);

    QByteArray format() const;
    void setFormat(const QByteArray &format);

    bool hasError() const;
    QString errorString() const;

//    bool read();
//    bool write();

//    int mipmapCount() const;
//    void setMipmapCount(int count);

//    int frameCount() const;
//    void setFrameCount(int count);

//    ImageIndex::Sides sides() const;
//    void setSides(ImageIndex::Sides sides);

//    int sliceCount() const;
//    void setSliceCount(int count);

//    int layerCount() const;
//    void setLayerCount(int count);

//    QImage image(const ImageIndex &index = ImageIndex()) const;

//    ImageMeta meta() const;

protected:
    ImageDocumentPrivate *d_ptr;
};

#endif // IMAGEDOCUMENT_H
