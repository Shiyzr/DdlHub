#ifndef ACHIEVEMENTWIDGET_H
#define ACHIEVEMENTWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QString>

class AchievementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AchievementWidget(QWidget *parent = nullptr);
    AchievementWidget(QString title, QString subtitle, QPixmap pixmap, QWidget *parent = nullptr);
    void setIcon(const QPixmap &icon);
    void setTitle(const QString &title);
    void setSubtitle(const QString &subtitle);
    void setCompleted(bool completed);

    void setConditionText(const QString &text);  // 设置提示内容
    void mousePressEvent(QMouseEvent* event) override;
    bool completed;


protected:
    void paintEvent(QPaintEvent *event) override;
    QSize minimumSizeHint() const override;

private:
    QPixmap icon;
    QString title;
    QString subtitle;
    QString conditionText;  // 成就条件文本
};

#endif // ACHIEVEMENTWIDGET_H
