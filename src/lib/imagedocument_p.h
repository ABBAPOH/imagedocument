#ifndef IMAGEDOCUMENT_P
#define IMAGEDOCUMENT_P

#include "imagedocument.h"

#include <QtCore/QFile>
#include <QtCore/QHash>
#include <QtCore/QMap>

class ImageIOHandler;
class ImageIOHandlerPlugin;

class ImageDocumentPrivate
{
    Q_DECLARE_PUBLIC(ImageDocument)

    ImageDocument *q_ptr;

public:
    typedef QMap<ImageResource::Option, QVariant> OptionMap;
    typedef QMap<ImageResource::ExifOption, QVariant> ExifOptionMap;

    explicit ImageDocumentPrivate(ImageDocument *qq) : q_ptr(qq) {}

    void init();
    bool initHandler();
    void killHandler();

    QIODevice *device;
    QString fileName;
    QScopedPointer<QFile> file;
    ImageDocument::OpenMode openMode;
    QMimeType mimeType;
    QString errorString;
    int mipmapCount;
    int frameCount;
    ImageResource::Sides sides;
//    int sliceCount;
    OptionMap options;
    ExifOptionMap exifOptions;
    QByteArray subType;

    QMap<ImageIndex, ImageResource> elements;

    ImageIOHandler *handler;
};

class ImageIOHandlerDatabase
{
public:
    ImageIOHandlerDatabase();
    ~ImageIOHandlerDatabase();

    ImageIOHandler *create(const QMimeType &mimeType);

    static ImageIOHandlerDatabase *instance();

private:

    QHash<QString, ImageIOHandlerPlugin *> map;
};

#endif // IMAGEDOCUMENT_P

