#include "GridLayoutManager.h"
#include "ViewHolder.h"
#include "RecyclerAdapter.h"
#include <cmath>

GridLayoutManager::GridLayoutManager(int spanCount, int spacing)
    : spanCount(spanCount), spacing(spacing)
{
}

void GridLayoutManager::setSpanCount(int count)
{
    spanCount = std::max(1, count);
}

void GridLayoutManager::setViewportSize(const QSize& size)
{
    m_viewportSize = size;
}

void GridLayoutManager::setContainer(QWidget* widget)
{
    m_container = widget;
}

QWidget* GridLayoutManager::itemParent()
{
    return m_container;
}

void GridLayoutManager::addViewHolder(ViewHolder* holder, int position)
{
    if (!m_container || !holder || position < 0) return;

    holder->setParent(m_container);
    holder->show();

    const QSize hintSize = holder->sizeHint();

    const int totalSpacing = spacing * (spanCount + 1);
    const int availableWidth = m_container->width() - totalSpacing;
    const int cellWidth = availableWidth / spanCount;

    const int column = position % spanCount;
    const int row = position / spanCount;

    // 遍历前面几行，找到当前 row 的最大 Y 值（行高不固定）
    int y = spacing;
    for (int r = 0; r < row; ++r)
    {
        int maxHeightInRow = 0;
        for (int c = 0; c < spanCount; ++c)
        {
            if (const int index = r * spanCount + c; index < m_itemRects.size())
            {
                maxHeightInRow = std::max(maxHeightInRow, m_itemRects[index].height());
            }
        }
        y += maxHeightInRow + spacing;
    }

    const int x = spacing + column * (cellWidth + spacing);
    const int height = hintSize.height();

    const QRect rect(x, y, cellWidth, height);
    holder->setGeometry(rect);

    if (position >= m_itemRects.size())
        m_itemRects.resize(position + 1);
    m_itemRects[position] = rect;

    m_attachedViewHolders[position] = holder;

    qDebug() << "Grid addViewHolder at position:" << position << "rect:" << rect << "row:" << row << "col:" << column;
}


void GridLayoutManager::removeViewHolder(ViewHolder* holder)
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

QPair<int, int> GridLayoutManager::computeVisibleRange(const int scrollY)
{
    if (m_itemRects.isEmpty())
    {
        return qMakePair(0, 0);
    }

    const int viewportTop = scrollY;
    const int viewportBottom = scrollY + m_viewportSize.height();

    int firstVisible = -1;
    int lastVisible = -1;

    // 线性遍历 m_itemRects，找到首尾可见项
    for (int i = 0; i < m_itemRects.size(); ++i)
    {
        // 判断item是否与视口垂直区域相交

        if (const QRect& rect = m_itemRects[i]; !(rect.bottom() < viewportTop || rect.top() > viewportBottom))
        {
            if (firstVisible == -1)
                firstVisible = i;
            lastVisible = i;
        }
        else if (firstVisible != -1 && rect.top() > viewportBottom)
        {
            // 已找到最后可见项，提前终止循环
            break;
        }
    }

    if (firstVisible == -1)
    {
        // 当前视口无任何可见项，可能在空白区域，返回合理空区间
        return qMakePair(0, 0);
    }

    return qMakePair(firstVisible, lastVisible);
}


void GridLayoutManager::layout()
{
    if (!m_container || m_itemRects.isEmpty()) return;

    int totalHeight = 0;
    for (auto m_itemRect : m_itemRects)
    {
        totalHeight = std::max(totalHeight, m_itemRect.bottom());
    }

    totalHeight += spacing;
    m_container->resize(m_container->width(), totalHeight);

    qDebug() << "[GridLayoutManager] layout total height:" << totalHeight << ",width:" << m_container->width();
}

void GridLayoutManager::prepareLayoutIfNeeded(RecyclerAdapter<QVariant>* adapter, RecyclerCachePool* pool, QWidget* itemParent,
                                              int viewportHeight)
{
    if (!adapter || !itemParent || m_itemRects.size() >= adapter->getItemCount()) return;

    const int itemCount = adapter->getItemCount();
    if (m_itemRects.size() != itemCount)
    {
        m_itemRects.resize(itemCount);
    }

    QVector<int> columnHeights(spanCount, spacing);
    const int availableWidth = itemParent->width() - spacing * (spanCount + 1);
    const int cellWidth = availableWidth / spanCount;

    int maxColumnHeight = 0;

    for (int i = 0; i < itemCount; ++i)
    {
        if (m_itemRects[i].isValid())
        {
            const int col = i % spanCount;
            columnHeights[col] = std::max(columnHeights[col], m_itemRects[i].bottom() + spacing);
            continue;
        }

        QString type = adapter->getItemViewType(i);
        ViewHolder* vh = pool->getRecycledView(type);
        if (!vh)
        {
            vh = adapter->onCreateViewHolder(itemParent, type);
        }
        vh->hide();
        vh->setParent(itemParent);
        vh->bindData(adapter->getItem(i));
        vh->resize(cellWidth, vh->sizeHint().height());

        const int height = vh->sizeHint().height();
        pool->putRecycledView(type, vh);

        const int column = i % spanCount;
        const int x = spacing + column * (cellWidth + spacing);
        const int y = columnHeights[column];

        m_itemRects[i] = QRect(x, y, cellWidth, height);
        columnHeights[column] += height + spacing;

        maxColumnHeight = *std::max_element(columnHeights.begin(), columnHeights.end());

        // 到达2屏高度则提前返回（惰性构建）
        if (maxColumnHeight >= 2 * viewportHeight) break;
    }
}


void GridLayoutManager::makesureLayout(const int& position)
{
    if (position >= m_itemRects.size())
    {
        qDebug() << "makesureLayout error, out of index, position:" << position << ", rects:" << m_itemRects.size();
        return;
    }

    const auto holder = m_attachedViewHolders[position];
    if (!holder)
    {
        return;
    }

    const QSize hintSize = holder->sizeHint();
    const int totalSpacing = spacing * (spanCount + 1);
    const int availableWidth = m_container->width() - totalSpacing;
    const int cellWidth = availableWidth / spanCount;

    const int column = position % spanCount;
    const int row = position / spanCount;

    // 遍历前面几行，找到当前 row 的最大 Y 值（行高不固定）
    int y = spacing;
    for (int r = 0; r < row; ++r)
    {
        int maxHeightInRow = 0;
        for (int c = 0; c < spanCount; ++c)
        {
            if (const int index = r * spanCount + c; index < m_itemRects.size())
            {
                maxHeightInRow = std::max(maxHeightInRow, m_itemRects[index].height());
            }
        }
        y += maxHeightInRow + spacing;
    }

    const int x = spacing + column * (cellWidth + spacing);
    const int height = hintSize.height();
    auto rect = m_itemRects[position];
    rect.setX(x);
    rect.setRect(x, y, cellWidth, height);
    holder->setGeometry(rect);
}

int GridLayoutManager::preloadCount()
{
    return 3 * spanCount;
}
