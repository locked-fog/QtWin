#ifndef QTWBUTTON_H
#define QTWBUTTON_H
#include "QtWin.h"

#include <qtwException.h>

#include <QPushButton>
#include <QColor>
#include <QPropertyAnimation>
#include <QEnterEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

#include <QStyle>
#include <QStyleOptionButton>

class QTWIN_API QtwButton : public QPushButton {
    Q_OBJECT

    //动画部分声明Q_PROPERTY
    Q_PROPERTY(QColor animatedBgColor READ animatedBgColor WRITE setAnimatedBgColor NOTIFY animatedBgColorChanged)
    Q_PROPERTY(qreal animatedOpacity READ animatedOpacity WRITE setAnimatedOpacity NOTIFY animatedOpacityChanged)

public:
    explicit QtwButton(QWidget *parent = nullptr);
    explicit QtwButton(const QString &text, QWidget *parent = nullptr);
    explicit QtwButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
    ~QtwButton() override;

signals:
    //属性变化时的通知信号
    void animatedBgColorChanged(const QColor &color);
    void animatedOpacityChanged(qreal opacity);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QColor animatedBgColor() const;
    void setAnimatedBgColor(const QColor &color);
    qreal animatedOpacity() const;
    void setAnimatedOpacity(qreal opacity);

    //初始化
    void initButtonColors();
    void initAnimations();

    //颜色和不透明度
    QColor baseBgColor;
    QColor hoverBgColor;
    QColor currentBgColor;
    qreal currentOpacity;

    //动画参数（可修改）
    static constexpr qreal PRESSED_OPACITY = 0.9;
    static constexpr int HOVER_DARKER_FACTOR = 120;
    static constexpr int ANIMATION_DURATION_COLOR_MS = 20;
    static constexpr int ANIMATION_DURATION_OPACITY_MS = 20;
    static constexpr int CORNER_RADIUS = 6;

    //动画对象
    QPropertyAnimation *bgColorAnimation;
    QPropertyAnimation *opacityAnimation;

    bool isHovered;

    //TODO:缩放动画

    //TODO:莫奈取色
};

#endif