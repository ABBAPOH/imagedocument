#ifndef DEFAULTHANDLER_H
#define DEFAULTHANDLER_H

#include "imageiohandler.h"

class DefaultHandler : public ImageIOHandler
{
public:
    DefaultHandler();

    bool canRead() { return true; }
    bool read(ImageContents &contents, const ReadOptions &options) Q_DECL_OVERRIDE;
    bool write(const ImageContents &contents, const WriteOptions &options) Q_DECL_OVERRIDE;
};

class DefaultHandlerPlugin : public ImageIOHandlerPlugin
{
public:
    DefaultHandler *create(QIODevice *device, const QMimeType &mimeType);
    Capabilities capabilities(QIODevice *device, const QMimeType &mimeType) const Q_DECL_OVERRIDE;
};

#endif // DEFAULTHANDLER_H
