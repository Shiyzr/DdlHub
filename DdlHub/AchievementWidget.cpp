#include "AchievementWidget.h"
#include <QPainter>
#include <QFont>
#include <QLinearGradient>
#include <QImage>
#include <QStyleOption>
#include <QMouseEvent>
#include <QMessageBox>

AchievementWidget::AchievementWidget(QWidget *parent)
    : QWidget(parent), completed(false) {}
AchievementWidget::AchievementWidget(QString title, QString subtitle, QPixmap pix, QWidget *parent)
    : QWidget(parent), completed(false) {
    setSubtitle(subtitle);
    setTitle(title);
    setIcon(pix);
    this->setFixedHeight(70);

}

void AchievementWidget::setIcon(const QPixmap &icon_) {
    icon = icon_;
    update();
}

void AchievementWidget::setTitle(const QString &title_) {
    title = title_;
    update();
}

void AchievementWidget::setSubtitle(const QString &subtitle_) {
    subtitle = subtitle_;
    update();
}

void AchievementWidget::setCompleted(bool completed_) {
    completed = completed_;
    update();
}

QSize AchievementWidget::minimumSizeHint() const {
    return QSize(300, 100);  // 宽大于高
}

void AchievementWidget::setConditionText(const QString &text) {
    conditionText = text;
}

void AchievementWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        QMessageBox::information(this, "成就条件", conditionText.isEmpty() ? "暂无条件说明。" : conditionText);
    }
    QWidget::mousePressEvent(event);  // 继续传递事件（如有父类处理）
}

void AchievementWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 圆角矩形背景
    QRectF rect = this->rect().adjusted(1, 1, -1, -1);
    QColor bgColor = completed ? QColor("#ffffff") : QColor("#aaaaaa");
    painter.setBrush(bgColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect, 15, 15);


    // 绘制图标
    int padding = 15;
    int iconSize = height() - 2 * padding;
    QRect iconRect(padding, padding, iconSize, iconSize);

    QPixmap displayIcon;
    if (!icon.isNull()) {
        displayIcon = completed ? icon : QPixmap::fromImage(icon.toImage().convertToFormat(QImage::Format_Grayscale8));
    }

    painter.drawPixmap(iconRect, displayIcon.scaled(iconRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // 绘制文字
    int textX = iconRect.right() + 15;
    int textWidth = width() - textX - padding;

    // 标题
    QFont titleFont = painter.font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.setPen(completed ? Qt::black : Qt::white);
    painter.drawText(QRect(textX, padding, textWidth, iconSize / 2), Qt::AlignLeft | Qt::AlignVCenter, title);

    // 副标题
    QFont subtitleFont = painter.font();
    subtitleFont.setPointSize(10);
    subtitleFont.setBold(false);
    painter.setFont(subtitleFont);
    painter.drawText(QRect(textX, padding + iconSize / 2, textWidth, iconSize / 2),
                     Qt::AlignLeft | Qt::AlignVCenter, subtitle);
}



