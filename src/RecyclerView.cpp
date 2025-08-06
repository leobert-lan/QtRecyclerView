#include "RecyclerView.h"
#include <QVBoxLayout>
#include <QScrollBar>
#include <QDebug>

RecyclerView::RecyclerView(QWidget* parent)
    : QScrollArea(parent), contentWidget(new QWidget(this))
{
    setWidgetResizable(false);
    // scrollArea->setWidgetResizable(false);
    setWidget(contentWidget);
    contentWidget->setMinimumHeight(0);
    contentWidget->setLayout(new QVBoxLayout());
    contentWidget->layout()->setContentsMargins(0, 0, 0, 0);
    // contentWidget->layout()->setMaximumSize()
}

void RecyclerView::setAdapter(RecyclerAdapter<QVariant>* adapter)
{
    m_adapter = adapter;
    recycleAllViews();
    updateVisibleItems();
}

void RecyclerView::setLayoutManager(AbstractLayoutManager* layoutManager)
{
    m_layoutManager = layoutManager;
    layoutManager->setContainer(contentWidget);
    updateVisibleItems();
}

void RecyclerView::resizeEvent(QResizeEvent* event)
{
    QScrollArea::resizeEvent(event);
    updateVisibleItems();
}

void RecyclerView::scrollContentsBy(int dx, int dy)
{
    QScrollArea::scrollContentsBy(dx, dy);
    scrollOffset = verticalScrollBar()->value();
    updateVisibleItems();
}

void RecyclerView::updateVisibleItems()
{
    if (!m_adapter || !m_layoutManager)
        return;

    const QSize viewSize = viewport()->size();
    const int scrollY = verticalScrollBar()->value();

    m_layoutManager->setViewportSize(viewSize);

    if (viewSize.height() <= 0)
        return;  // 防止尺寸未初始化导致 prepare 无效

    qDebug() <<"check1 viewport width:" << viewSize.width();
    // 设置 contentWidget 宽度
    contentWidget->resize(viewSize.width(), contentWidget->height());

    m_layoutManager->prepareLayoutIfNeeded(m_adapter, m_layoutManager->itemParent(), viewSize.height());

    // 第二步：重新计算可视区域（这次才有数据）
    auto [fst, snd] = m_layoutManager->computeVisibleRange(scrollY);
    const int start = fst;
    // int end = range.second;
    // 追加一段使其可滑动 todo:追加的count应当由layoutmanager给出
    const int end = std::min(snd + 5, m_adapter->getItemCount());

    qDebug() <<"updateVisibleItems: start" << start <<" end" <<end;

    // 回收不在范围内的 ViewHolder
    QSet<int> visibleNow;
    for (int i = start; i <= end; ++i) visibleNow.insert(i);

    QList<int> keys = m_attachedViewHolders.keys();
    for (int i : keys)
    {
        if (!visibleNow.contains(i))
        {
            ViewHolder* holder = m_attachedViewHolders.take(i);
            QString type = m_adapter->getItemViewType(i);
            m_cachePool.putRecycledView(type, holder);
            m_layoutManager->removeViewHolder(holder);
        }
    }

    // 添加当前可见的，以及预加载的 ViewHolder
    for (int i = start; i < end; i++)
    {
        if (m_attachedViewHolders.contains(i)) {
            //todo make sure it's size
            qDebug() << "should resize for:" <<i;
            m_layoutManager->makesureLayout(i);
            continue;
        }

        QString type = m_adapter->getItemViewType(i);
        ViewHolder* holder = m_cachePool.getRecycledView(type);
        if (!holder)
            holder = m_adapter->onCreateViewHolder(m_layoutManager->itemParent(), type);

        m_attachedViewHolders[i] = holder;
        m_layoutManager->addViewHolder(holder, i);
        m_adapter->onBindViewHolder(holder, i);
    }

    qDebug() <<"check2 viewport height:" << viewport()->height() <<" " <<contentWidget->height();
    contentWidget->resize(viewport()->width(),contentWidget->height());

    m_layoutManager->layout(); // 更新内容尺寸
    qDebug() <<"check3 viewport height:" << viewport()->height() <<" " <<contentWidget->height();

    // // 触发 layout 更新内容区域高度
    // m_layoutManager->layout();
    //
    // // 保证 contentWidget 的高度满足滚动需求
    // contentWidget->resize(viewport()->width(), contentWidget->height());
}


void RecyclerView::recycleAllViews()
{
    for (auto it = m_attachedViewHolders.begin(); it != m_attachedViewHolders.end(); ++it)
    {
        int pos = it.key();
        ViewHolder* vh = it.value();
        QString type = m_adapter->getItemViewType(pos);
        m_cachePool.putRecycledView(type, vh);
        vh->setParent(nullptr);
    }
    m_attachedViewHolders.clear();
}
