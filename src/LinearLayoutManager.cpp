#include "LinearLayoutManager.h"
#include <algorithm>

#include "RecyclerAdapter.h"

void LinearLayoutManager::addViewHolder(ViewHolder* holder, int position) {
    if (!container || !holder) return;

    holder->setParent(container);
    holder->show();

    // 测量尺寸
    QSize size = holder->sizeHint();
    int height = size.height();
    int top = 0;

    // 计算位置
    if (position > 0 && position - 1 < m_itemRects.size()) {
        const QRect& prevRect = m_itemRects[position - 1];
        top = prevRect.bottom() + spacing;
    }

    QRect rect(0, top, container->width(), height);

    qDebug() << "addViewHolder for postion:" << position <<"rect:[" << 0<<","<<top<<","<<container->width()<<","<<height<<"]";
    if (position >= m_itemRects.size())
        m_itemRects.resize(position + 1);
    m_itemRects[position] = rect;

    holder->setGeometry(rect);
    m_attachedViewHolders[position] = holder;
}

void LinearLayoutManager::removeViewHolder(ViewHolder* holder) {
    if (!holder) return;

    holder->hide();
    holder->setParent(nullptr);

    auto it = m_attachedViewHolders.begin();
    while (it != m_attachedViewHolders.end()) {
        if (it.value() == holder) {
            it = m_attachedViewHolders.erase(it);
        } else {
            ++it;
        }
    }
}

void LinearLayoutManager::layout() {
    if (!container || m_itemRects.isEmpty()) return;

    int totalHeight = m_itemRects.last().bottom() + spacing;
    container->resize(container->width(), totalHeight);

    qDebug() <<"layout:" << totalHeight;
}

QPair<int, int> LinearLayoutManager::computeVisibleRange(int scrollY) {
    int start = 0;
    int end = 0;

    const int maxY = scrollY + m_viewportSize.height();
    const int itemCount = m_itemRects.size();

    for (int i = 0; i < itemCount; ++i) {
        const QRect& rect = m_itemRects[i];
        if (rect.bottom() >= scrollY) {
            start = i;
            break;
        }
    }

    for (int i = start; i < itemCount; ++i) {
        if (m_itemRects[i].top() > maxY) {
            end = i - 1;
            break;
        }
        end = i;
    }

    qDebug() << "computeVisibleRange scrollY:" << scrollY <<" maxY:" << maxY <<" start:" <<start <<" end:" << end;

    return qMakePair(start, end);
}

void LinearLayoutManager::prepareLayoutIfNeeded(RecyclerAdapter<QVariant>* adapter, QWidget* itemParent, int viewportHeight)
{
    if (!m_itemRects.isEmpty() || !adapter || viewportHeight <= 0) return;

    int y = 0;
    int count = adapter->getItemCount();
    for (int i = 0; i < count && y < viewportHeight * 2; ++i) {  // 填满 2x 可视区
        QString type = adapter->getItemViewType(i);
        ViewHolder* vh = adapter->onCreateViewHolder(itemParent, type);
        vh->hide();  // 不显示
        vh->setParent(itemParent);
        vh->bindData(adapter->getItem(i));
        vh->resize(itemParent->width(), vh->sizeHint().height());
        int h = vh->sizeHint().height();
        m_itemRects.append(QRect(0, y, itemParent->width(), h));
        y += h + spacing;
        vh->deleteLater();  // 不加入布局，仅用于测量
    }
}

