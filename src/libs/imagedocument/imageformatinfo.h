#ifndef IMAGEFORMATINFO_H
#define IMAGEFORMATINFO_H

#include <ImageOptions>
#include <QtCore/QMimeType>
#include <QtCore/QVector>
#include <QtCore/QSharedDataPointer>

class ImageFormatInfoData;
class ImageFormatInfo
{
public:
    ImageFormatInfo();
    ImageFormatInfo(const ImageFormatInfo &other);
    ImageFormatInfo(ImageFormatInfo &&other);
    ~ImageFormatInfo();

    ImageFormatInfo &operator=(const ImageFormatInfo &);
    ImageFormatInfo &operator=(ImageFormatInfo &&);

    QByteArray name() const;
    QMimeType mimeType() const;

    QVector<QByteArray> supportedSubTypes() const;

    QVector<ImageOptions::Option> supportedOptions(const QByteArray &subType = QByteArray()) const;
    bool supportsOption(ImageOptions::Option option) const;
    bool supportsOption(const QByteArray &subType, ImageOptions::Option option) const;

private:
    explicit ImageFormatInfo(const ImageFormatInfoData &data);

private:
    QSharedDataPointer<ImageFormatInfoData> d;

    friend class ImageIOHandlerDatabase;
};

#endif // IMAGEFORMATINFO_H
