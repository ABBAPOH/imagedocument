#pragma once

#include "imageformatsng_global.h"

#include <QtCore/QHash>
#include <QtCore/QRect>
#include <QtCore/QVariant>
#include <QtCore/QSet>
#include <QtGui/QColor>

class IMAGEFORMATSNG_EXPORT ImageOptions
{
    Q_GADGET
    Q_PROPERTY(QRect clipRect READ clipRect WRITE setClipRect)
    Q_PROPERTY(QSize scaledSize READ scaledSize WRITE setScaledSize)
    Q_PROPERTY(QRect scaledCliptRect READ scaledCliptRect WRITE setScaledCliptRect)
    Q_PROPERTY(int inputQuality READ inputQuality WRITE setInputQuality)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(bool skipMipmaps READ skipMipmaps WRITE setSkipMipmaps)
    Q_PROPERTY(int compression READ compression WRITE setCompression)
    Q_PROPERTY(float gamma READ gamma WRITE setGamma)
    Q_PROPERTY(int quality READ quality WRITE setQuality)
public:
    ImageOptions();
    ImageOptions(const ImageOptions &other);
    ImageOptions(ImageOptions &&other);
    ~ImageOptions();

    ImageOptions &operator=(const ImageOptions &other);
    ImageOptions &operator=(ImageOptions &&other);

    enum Option {
        // read options
        ClipRect,
        ScaledSize,
        ScaledClipRect,
        InputQuality,
        BackgroundColor,
        SkipMipmaps,

        // write options
        CompressionRatio,
        Gamma,
        Quality,
        OptionsCount
    };
    Q_ENUM(Option)

    using OptionsSet = QSet<ImageOptions::Option>;

    bool isEmpty() const;

    bool contains(Option option) const;
    void remove(Option option);

    QRect clipRect() const;
    void setClipRect(const QRect &rect);

    QSize scaledSize() const;
    void setScaledSize(const QSize &size);

    QRect scaledCliptRect() const;
    void setScaledCliptRect(const QRect &rect);

    int inputQuality(int defaultValue = 100) const;
    void setInputQuality(int quality);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

    bool skipMipmaps() const;
    void setSkipMipmaps(bool skip);

    int compression(int defaultValue = 0) const;
    void setCompression(int compression);

    float gamma(float defaultValue = 0.0) const;
    void setGamma(float gamma);

    int quality(int defaultValue = 100) const;
    void setQuality(int quality);

    inline void swap(ImageOptions &other) Q_DECL_NOEXCEPT
    {
        _options.swap(other._options);
    }

    static QString optionToString(ImageOptions::Option option);
    static QString optionsToString(const OptionsSet &options);

private:
    QHash<Option, QVariant> _options;

    friend bool IMAGEFORMATSNG_EXPORT operator==(const ImageOptions &lhs, const ImageOptions &rhs);
    friend bool IMAGEFORMATSNG_EXPORT operator!=(const ImageOptions &lhs, const ImageOptions &rhs);
};

bool IMAGEFORMATSNG_EXPORT operator==(const ImageOptions &lhs, const ImageOptions &rhs);
bool IMAGEFORMATSNG_EXPORT operator!=(const ImageOptions &lhs, const ImageOptions &rhs);

using ImageOptionsSet = ImageOptions::OptionsSet;

Q_DECLARE_METATYPE(ImageOptions);
