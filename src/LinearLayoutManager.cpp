#include "LinearLayoutManager.h"
#include "RecyclerAdapter.h"
#include <QDebug>
#include <QLayout>

void LinearLayoutManager::addViewHolder(ViewHolder* holder, int position)
{
    if (!container || !holder) return;

    qDebug() << "addViewHolder position:" << position;

    holder->setParent(container);
    // container->layout()->addWidget(holder);
    holder->show();

    // 测量尺寸
    const QSize size = holder->sizeHint();
    const int height = size.height();
    int top = 0;

    if (position > 0 && position - 1 < m_itemRects.size())
    {
        const QRect& prevRect = m_itemRects[position - 1];
        top = prevRect.bottom() + spacing;
    }

    const QRect rect(0, top, container->width(), height);
    if (position >= m_itemRects.size())
        m_itemRects.resize(position + 1);

    m_itemRects[position] = rect;
    holder->setGeometry(rect);

    m_attachedViewHolders[position] = holder;

    qDebug() << "addViewHolder at" << position << "rect:" << rect;
}

void LinearLayoutManager::removeViewHolder(ViewHolder* holder)
{
    if (!holder) return;

    holder->hide();
    holder->setParent(nullptr);

    auto it = m_attachedViewHolders.begin();
    while (it != m_attachedViewHolders.end())
    {
        if (it.value() == holder)
        {
            it = m_attachedViewHolders.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void LinearLayoutManager::layout()
{
    if (!container || m_itemRects.isEmpty()) return;

    const int totalHeight = m_itemRects.last().bottom() + spacing;
    container->resize(container->width(), totalHeight);

    qDebug() << "[LinearLayoutManager] layout total height:" << totalHeight;
}

void LinearLayoutManager::makesureLayout(const int& position)
{
    if (position >= m_itemRects.size())
    {
        qDebug() << "makesureLayout error, out of index, position:" << position << ", rects:" << m_itemRects.size();
        return;
    }

    m_itemRects[position].setWidth(container->width());
    if (ViewHolder* holder = m_attachedViewHolders[position])
    {
        holder->resize(container->width(), holder->height());
        holder->setGeometry(m_itemRects[position]);
    }
}

int LinearLayoutManager::preloadCount()
{
    return 5;
}

QPair<int, int> LinearLayoutManager::computeVisibleRange(int scrollY)
{
    int start = 0;
    int end = 0;

    const int maxY = scrollY + m_viewportSize.height();
    const int itemCount = m_itemRects.size();

    for (int i = 0; i < itemCount; ++i)
    {
        const QRect& rect = m_itemRects[i];
        if (rect.bottom() >= scrollY)
        {
            start = i;
            break;
        }
    }

    for (int i = start; i < itemCount; ++i)
    {
        if (m_itemRects[i].top() > maxY)
        {
            end = i - 1;
            break;
        }
        end = i;
    }

    return qMakePair(start, end);
}

void LinearLayoutManager::prepareLayoutIfNeeded(RecyclerAdapter<QVariant>* adapter, QWidget* itemParent, int viewportHeight)
{
    if (!m_itemRects.isEmpty() || !adapter || viewportHeight <= 0) return;

    int y = 0;
    const int count = adapter->getItemCount();

    for (int i = 0; i < count && y < viewportHeight * 2; i++)
    {
        QString type = adapter->getItemViewType(i);
        ViewHolder* vh = adapter->onCreateViewHolder(itemParent, type);

        qDebug() << "in prepare vh index:" << i;
        vh->hide(); // 不加入 layout
        vh->setParent(itemParent);
        vh->bindData(adapter->getItem(i));
        vh->resize(itemParent->width(), vh->height()); // 固定宽度以触发布局

        if (vh->layout())
            vh->layout()->activate();
        vh->adjustSize(); // 强制计算 sizeHint

        const int h = vh->sizeHint().height();
        QRect rect(0, y, itemParent->width(), h);
        m_itemRects.append(rect);
        y += h + spacing;

        delete vh;
    }
}
