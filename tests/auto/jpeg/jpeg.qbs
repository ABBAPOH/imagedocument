import qbs.base 1.0

AutoTest {
    Depends { name: "Qt.gui" }
    Depends { name: "ImageDocument" }

    name: "tst_jpeg"
    files: [ "*.cpp", "*.h", "*.qrc" ]
}