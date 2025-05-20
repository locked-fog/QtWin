#include <qtwButton.h>

QtwButton::QtwButton(QWidget *parent)
    :   QPushButton(parent),
        currentOpacity(1.0),
        bgColorAnimation(nullptr),
        opacityAnimation(nullptr),
        isHovered(false){
    initButtonColors();
    currentBgColor = baseBgColor;
    initAnimations();
    setMouseTracking(true);            
}

QtwButton :: QtwButton(const QString &text, QWidget *parent)
    :   QPushButton(text,parent),
        currentOpacity(1.0),
        bgColorAnimation(nullptr),
        opacityAnimation(nullptr),
        isHovered(false){
    initButtonColors();
    currentBgColor = baseBgColor;
    initAnimations();
    setMouseTracking(true);  
}

QtwButton :: QtwButton(const QIcon &icon, const QString &text, QWidget *parent)
    :   QPushButton(icon,text,parent),
        currentOpacity(1.0),
        opacityAnimation(nullptr),
        bgColorAnimation(nullptr),
        isHovered(false){
    initButtonColors();
    currentBgColor = baseBgColor;
    initAnimations();
    setMouseTracking(true);  
}

QtwButton :: ~QtwButton() {
    delete bgColorAnimation;
    delete opacityAnimation;
}

void QtwButton :: initButtonColors(){
    baseBgColor = palette().color(QPalette::Button);
    hoverBgColor = baseBgColor.darker(HOVER_DARKER_FACTOR);
}

void QtwButton :: initAnimations() {
    bgColorAnimation = new QPropertyAnimation(this,"animatedBgColor",this);
    bgColorAnimation->setDuration(ANIMATION_DURATION_COLOR_MS);
    bgColorAnimation->setEasingCurve(QEasingCurve::InOutQuad);

    opacityAnimation = new QPropertyAnimation(this,"animatedOpacity",this);
    opacityAnimation->setDuration(ANIMATION_DURATION_OPACITY_MS);
    opacityAnimation->setEasingCurve(QEasingCurve::InOutQuad);
}

QColor QtwButton :: animatedBgColor() const{
    return currentBgColor;
}

void QtwButton::setAnimatedBgColor(const QColor &color){
    if(currentBgColor != color){
        currentBgColor = color;
        update();
        emit animatedBgColorChanged(currentBgColor);
    }
}

qreal QtwButton::animatedOpacity() const{
    return currentOpacity;
}

void QtwButton::setAnimatedOpacity(qreal opacity){
    if(!qFuzzyCompare(currentOpacity,opacity)){
        currentOpacity = opacity;
        update();
        emit animatedOpacityChanged(currentOpacity);
    }
}

void QtwButton::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QStyleOptionButton option;
    initStyleOption(&option);

    painter.setOpacity(currentOpacity);
    painter.setBrush(currentBgColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(),CORNER_RADIUS,CORNER_RADIUS);

    if(style()){
        style()->drawControl(QStyle::CE_PushButtonLabel, &option,&painter,this);
    }else{
        throw(new QtwException(QTWBUTTON_PAINTEVENT_NULLPTR));
    }
}

void QtwButton::enterEvent(QEnterEvent *event){
    isHovered = true;
    bgColorAnimation->stop();
    opacityAnimation->stop();

    bgColorAnimation->setStartValue(currentBgColor);
    bgColorAnimation->setEndValue(hoverBgColor);
    bgColorAnimation->start();

    if(!qFuzzyCompare(currentOpacity,1.0)){
        opacityAnimation->setStartValue(currentOpacity);
        opacityAnimation->setEndValue(1.0);
        opacityAnimation->start();
    }

    QPushButton::enterEvent(event);
}

void QtwButton::leaveEvent(QEvent *event){
    isHovered = false;

    bgColorAnimation->stop();
    
    bgColorAnimation->setStartValue(currentBgColor);
    bgColorAnimation->setEndValue(baseBgColor);
    bgColorAnimation->start();

    if(!isDown() && !qFuzzyCompare(currentOpacity,1.0)){
        opacityAnimation->stop();
        opacityAnimation->setStartValue(currentOpacity);
        opacityAnimation->setEndValue(1.0);
        opacityAnimation->start();
    }

    QPushButton::leaveEvent(event);
}

void QtwButton::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        opacityAnimation->stop();

        opacityAnimation->setStartValue(currentOpacity);
        opacityAnimation->setEndValue(PRESSED_OPACITY);
        opacityAnimation->start();
    }
    QPushButton::mousePressEvent(event);
}

void QtwButton::mouseReleaseEvent(QMouseEvent *event){
    if( event->button() == Qt::LeftButton){
        opacityAnimation->stop();

        opacityAnimation->setStartValue(currentOpacity);
        opacityAnimation->setEndValue(1.0);
        opacityAnimation->start();
    }
    QPushButton::mouseReleaseEvent(event);
}
