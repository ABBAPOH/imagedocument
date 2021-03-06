#include <QtTest>

#include <ImageIO>
#include <QMimeDatabase>

class TestJpeg : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void readImage_data();
    void readImage();
    void testHeader_data();
    void testHeader();
};

void TestJpeg::initTestCase()
{
    qApp->addLibraryPath(qApp->applicationDirPath() + ImageIO::pluginsDirPath());
}

void TestJpeg::readImage_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<bool>("success");

    QTest::newRow("beavis") << QString(":/beavis.jpg") << true;
    QTest::newRow("qtbug13653") << QString(":/qtbug13653-no_eoi.jpg") << true;
    QTest::newRow("corrupt") << QString(":/corrupt.jpg") << false;
}

void TestJpeg::readImage()
{
    QFETCH(QString, fileName);
    QFETCH(bool, success);

    ImageIO io(fileName);
    const auto result = io.read();
    const auto &ok = result.first;
    QVERIFY2(ok == success, qPrintable(ok.toString()));
}

void TestJpeg::testHeader_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QSize>("size");
    QTest::addColumn<int>("format");

    QTest::newRow("beavis") << QString(":/beavis.jpg") << QSize(350, 350) << int(QImage::Format_Indexed8);
    QTest::newRow("qtbug13653") << QString(":/qtbug13653-no_eoi.jpg") << QSize(240, 180) << int(QImage::Format_RGB32);
}

void TestJpeg::testHeader()
{
    QFETCH(QString, fileName);
    QFETCH(QSize, size);
    QFETCH(int, format);

    ImageIO io(fileName);
    const auto result = io.readHeader();
    const auto &status = result.first;
    const auto &header = result.second;
    QVERIFY2(status, qPrintable(status.toString()));

    QCOMPARE(header.size(), size);
    QCOMPARE(int(header.imageFormat()), format);
}

QTEST_MAIN(TestJpeg)

#include "tst_jpeg.moc"
