#include "customhexeditor.h"

#include <QKeyEvent>

#include <QPainter>

const int lineSize = 16;
const int heightLine = 18;
const int widthSymb = 20;

CustomHexEditor::CustomHexEditor(QWidget *parent)
    : QWidget{parent}
{
    setAutoFillBackground(false);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

void CustomHexEditor::insertData(const QByteArray &data)
{
    mData = data;
    if(mHBottom > height()){
        mTopLine++;
    }
    update();
}

void CustomHexEditor::wheelEvent(QWheelEvent *event)
{
    const int lines = mData.size() / lineSize + 1;
    QPoint pt = event->angleDelta();
    if(pt.y() < 0){
        mTopLine++;
    }else{
        mTopLine--;
    }
    if(mTopLine < 0){
        mTopLine = 0;
    }
    if(mTopLine > lines - 1){
        mTopLine = lines - 1;
    }
    update();
}

void CustomHexEditor::keyPressEvent(QKeyEvent *event)
{
    char ckey = event->key();

    if(!mInputAsHex && std::isprint(ckey)){
        if(!event->modifiers().testFlag(Qt::ShiftModifier)){
            ckey = std::tolower(ckey);
        }
        mData.insert(mCurPos, ckey);
        mCurPos++;
        emit changedData(mData);
        mEven = 0;
        update();
        return;
    }

    bool insert = false;
    int val = 0;
    if(ckey >= '0' && ckey <= '9'){
        val = ckey - '0';
        insert = true;
    }
    if(ckey >= 'A' && ckey <= 'F'){
        val = ckey - 'A' + 0xA;
        insert = true;
    }
    if(ckey >= 'a' && ckey <= 'f'){
        val = ckey - 'A' + 0xA;
        insert = true;
    }
    if(insert){
        if(mEven){
            uchar back = static_cast<uchar>(mData[mCurPos]);
            val += back << 4;
            mData[mCurPos] = val;
            mCurPos += 1;
        }else{
            mData.insert(mCurPos, val);
        }
        mEven = (mEven + 1) % 2;
        emit changedData(mData);
        update();
    }else{
        if(event->key() == Qt::Key_Backspace){
            if(mData.size() && mCurPos){
                mData.remove(--mCurPos, 1);
                mEven = 0;
                mCurPos = std::min<int>(mCurPos, mData.size() - 1);
                update();
            }
        }
        if(event->key() == Qt::Key_Delete){
            if(mData.size()){
                mData.remove(mCurPos, 1);
                mEven = 0;
                mCurPos = std::min<int>(mCurPos, mData.size() - 1);
                update();
            }
        }
        if(event->key() == Qt::Key_Left){
            if(mData.size() && mCurPos > 0){
                mCurPos--;
                mEven = 0;
                update();
            }
        }
        if(event->key() == Qt::Key_Right){
            if(mData.size() && mCurPos < mData.size()){
                mCurPos++;
                mEven = 0;
                update();
            }
        }
        if(event->key() == Qt::Key_Up){
            if(mData.size() && mCurPos - lineSize >= 0){
                mCurPos -= lineSize;
                mEven = 0;
                update();
            }
        }
        if(event->key() == Qt::Key_Down){
            if(mData.size() && mCurPos + lineSize < mData.size() - 1){
                mCurPos += lineSize;
                mEven = 0;
                update();
            }
        }
        const int lines = mData.size() / lineSize + 1;
        if(lines * heightLine < height()){
            mTopLine = 0;
            update();
            return;
        }
        if(mCurPos / lineSize > mTopLine && lines * heightLine > height()){
            mTopLine++;
            update();
        }
        if(mCurPos / lineSize < mTopLine){
            mTopLine--;
            update();
        }
    }

    QWidget::keyPressEvent(event);
}

void CustomHexEditor::keyReleaseEvent(QKeyEvent *event)
{
    QWidget::keyReleaseEvent(event);
}

void CustomHexEditor::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.fillRect(rect(), Qt::white);

    const int lines = mData.size() / lineSize + 1;

    int delim = 7;

    mHBottom = 0;
    for(int i = 0; i < lines; ++i){
        int y = heightLine + i * heightLine;
        painter.drawText(10, y, QString("%1").arg((mTopLine + i) * lineSize, 5, 16, QLatin1Char('0')));
        for(int j = 0; j < lineSize; ++j){
            int off = (mTopLine + i) * lineSize + j;
            if(off < mData.size()){
                int loff = j > delim? 10 : 0;
                uchar val = static_cast<uchar>(mData[off]);
                QPoint pt = QPoint(10 + 2 * widthSymb + loff + j * widthSymb, y);
                painter.drawText(pt, QString("%1").arg(val, 2, 16, QLatin1Char('0')));

                uchar v = static_cast<uchar>(val);
                if(!std::isprint(v)){
                    v = '.';
                }

                QPoint pt1 = QPoint(10 + 2 * widthSymb + loff + j * widthSymb * 0.5, y);
                painter.drawText(pt1 + QPoint((widthSymb + 3) * lineSize, 0), QString(QLatin1Char(v)));

                if(mCurPos == off){
                    painter.drawRect(QRect(pt + QPoint(-2, 2), pt + QPoint(widthSymb - 7, -heightLine + 5)));
                }
                mHBottom = pt.y();
            }else{
                if(off == mCurPos){
                    int loff = j > delim? 10 : 0;
                    QPoint pt = QPoint(10 + 2 * widthSymb + loff + j * widthSymb, y);
                    if(mCurPos == off){
                        painter.drawRect(QRect(pt + QPoint(-2, 2), pt + QPoint(widthSymb - 7, -heightLine + 5)));
                    }
                }
            }
        }
    }
}


void CustomHexEditor::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    const int lines = mData.size() / lineSize + 1;
    if(lines * heightLine < height()){
        mTopLine = 0;
        update();
        return;
    }
    if(mCurPos / lineSize > mTopLine && lines * heightLine > height()){
        mTopLine++;
        update();
    }
    if(mCurPos / lineSize < mTopLine){
        mTopLine--;
        update();
    }
}
