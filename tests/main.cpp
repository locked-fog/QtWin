// QtWin/tests/main.cpp

#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QImage>

// 包含我们自己库的头文件
#include <QtWin/QWPalette.h>
#include <QtWin/QWApplication.h>
#include <QtWin/QWSettings.h>
#include <QtWin/QWLogger.h>
#include <QtWin/QWWindow.h>

QWLOGGER(testApp)
QWLOGNAME(testApp,"test.main.core")

int main(int argc, char *argv[])
{
    QtWin::QWApplication app(argc, argv, "Locked_Fog","test", "0.0.1");
    app.toggleDarkMode();
    QtWin::QWLogger::clearLogFile();
    qwLogger(QtWin::LogLevel::Debug,testApp)<<"Debug test info";


    // qCInfo(testApp)<<"Creat image...";
    // QImage* image = new QImage;
    // image->load(QString("C:\\Users\\AboxY\\OneDrive\\图片\\屏幕截图 2022-08-30 174745.png"));
    
    // std::vector<QtWin::HCTColor> colorSet;
    // QtWin::extractSeedColor(*image,colorSet);
    // QtWin::HCTColor seed = colorSet[4];
    // // 创建调色板
    // QtWin::RGBColor seedColor = QtWin::HCT2RGB(seed);
    // QtWin::QWPalette palette(seed);

    // qCDebug(testApp)<<"test Windows";
    // QWidget window;
    // window.setFixedWidth(500);
    // QVBoxLayout *layout = new QVBoxLayout(&window);

    // const char* colorNames[] = {
    //     "mainColor",
    //     "subColor",
    //     "neutralColor",
    //     "neutralAccent",
    //     "accentColor"
    // };  

    // for (int i = 0; i < 5; ++i) {
    //     QtWin::QWPalette::QWColor colorType = static_cast<QtWin::QWPalette::QWColor>(i);

    //     // ====== tone 设置位置开始 ======
    //     QColor bg = palette.getQColor(colorType, 10); // 背景色 tone = 50
    //     QColor fg = palette.getQColor(colorType, 80); // 前景色 tone = 95（一般是白或浅色）
    //     // ====== tone 设置位置结束 ======

    //     QLabel *label = new QLabel(colorNames[i]);
    //     QPalette pal = label->palette();
    //     pal.setColor(QPalette::Window, bg);
    //     pal.setColor(QPalette::WindowText, fg);
    //     label->setAutoFillBackground(true);
    //     label->setPalette(pal);
    //     label->setAlignment(Qt::AlignCenter);
    //     label->setFixedHeight(100);
    //     QFont fon = label->font();
    //     fon.setFamily(QString("Source Code Pro"));
    //     fon.setPixelSize(20);
    //     label->setFont(fon);

    //     layout->addWidget(label);
    // }

    // QLabel *label = new QLabel(QString("Seed Color"));
    // QPalette pal = label->palette();
    // pal.setColor(QPalette::Window, QColor(seedColor.red,seedColor.green,seedColor.blue));
    // pal.setColor(QPalette::WindowText,QColor(255,255,255));
    // label->setAutoFillBackground(true);
    // label->setPalette(pal);
    // label->setAlignment(Qt::AlignCenter);
    // label->setFixedHeight(100);
    // QFont fon = label->font();
    // fon.setFamily(QString("Source Code Pro"));
    // fon.setPixelSize(20);
    // label->setFont(fon);
    // layout->addWidget(label);

    // window.setLayout(layout);
    // window.setWindowTitle("QWPalette 色板展示");
    // window.show();

    QtWin::QWWindow window;
    QLabel *label = new QLabel(&window);
    window.setCentralWidget(label);
    
    qwLogger(QtWin::LogLevel::Debug,testApp)<<"windowcreated";
    
    label->setText(QString("Test"));
    QFont fon = label->font();
    fon.setFamily("Source Code Pro");
    fon.setPixelSize(64);
    fon.setBold(true);
    label->setFont(fon);
    window.setSeedColor(QColor(19, 149, 192));
    window.setDarkTone(30);
    window.setMaterial(QtWin::QWWindow::Default);
    window.show();

    return app.exec();
}