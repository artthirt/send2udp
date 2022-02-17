#ifndef CUSTOMHEXEDITOR_H
#define CUSTOMHEXEDITOR_H

#include <QWidget>

class CustomHexEditor : public QWidget
{
    Q_OBJECT
public:
    explicit CustomHexEditor(QWidget *parent = nullptr);

    void insertData(const QByteArray& data);

    QByteArray data() const {
        return mData;
    }

signals:
    void changedData(const QByteArray& data);

    // QWidget interface
protected:
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    QByteArray mData;
    int mTopLine = 0;
    int mCurPos = 0;
    int mEven = 0;
    int mHBottom = 0;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // CUSTOMHEXEDITOR_H
