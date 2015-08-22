import qbs.base 1.0

Lib {
    Depends { name: "Qt.gui" }
    Export {
        Depends { name: "cpp" }
        cpp.defines: [ "IMAGEDOCUMENT_EXPORT=Q_DECL_IMPORT" ]
    }

    Depends { name: "PNGLib" }
    cpp.defines: base.concat(["QT_STATICPLUGIN=1", "IMAGEDOCUMENT_EXPORT=Q_DECL_EXPORT"])
    name: "ImageDocument"
    files: [ "*.cpp", "*.h" ]
}