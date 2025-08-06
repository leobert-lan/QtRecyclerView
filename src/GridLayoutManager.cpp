// GridLayoutManager.cpp
#include "GridLayoutManager.h"

GridLayoutManager::GridLayoutManager(int spanCount)
    : m_spanCount(spanCount)
{
}

void GridLayoutManager::setContainer(QWidget* container)
{
    m_container = container;
    if (!m_gridLayout) {
        m_gridLayout = new QGridLayout(container);
        m_gridLayout->setContentsMargins(0, 0, 0, 0);
        m_gridLayout->setSpacing(spacing);
    }
}

QWidget* GridLayoutManager::itemParent()
{
    return m_container;
}

void GridLayoutManager::setViewportSize(const QSize& size)
{
    m_viewportSize = size;
}

QPair<int, int> GridLayoutManager::computeVisibleRange(int scrollY)
{
    int itemHeight = 0;
    int totalItems = m_itemRects.size();
    int start = 0, end = totalItems - 1;

    for (int i = 0; i < totalItems; ++i) {
        const QRect& rect = m_itemRects[i];
        if (rect.bottom() >= scrollY) {
            start = i;
            break;
        }
    }

    int maxY = scrollY + m_viewportSize.height();
    for (int i = start; i < totalItems; ++i) {
        if (m_itemRects[i].top() > maxY) {
            end = i - 1;
            break;
        }
    }

    return qMakePair(start, end);
}

void GridLayoutManager::addViewHolder(ViewHolder* holder, int position)
{
    if (!m_container || !m_gridLayout) return;

    holder->setParent(m_container);
    holder->show();

    // 测量
    const QSize itemSize = holder->sizeHint();
    const int itemWidth = itemSize.width();
    const int itemHeight = itemSize.height();

    const int row = position / m_spanCount;
    const int column = position % m_spanCount;

    m_gridLayout->addWidget(holder, row, column);
    m_attachedViewHolders[position] = holder;

    // 记录每个 item 的几何信息用于 layout 计算
    QRect rect(column * (itemWidth + spacing),
               row * (itemHeight + spacing),
               itemWidth,
               itemHeight);

    if (position >= m_itemRects.size()) {
        m_itemRects.resize(position + 1);
    }
    m_itemRects[position] = rect;
}

void GridLayoutManager::removeViewHolder(ViewHolder* holder)
{
    if (holder) {
        m_gridLayout->removeWidget(holder);
        holder->setParent(nullptr);
    }
}

void GridLayoutManager::layout()
{
    if (!m_container || m_itemRects.isEmpty())
        return;

    const QRect& last = m_itemRects.last();
    const int newHeight = last.bottom() + spacing;
    m_container->resize(m_container->width(), newHeight);
}
