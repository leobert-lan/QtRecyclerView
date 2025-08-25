#pragma once

#include "AbstractLayoutManager.h"
#include "RecyclerAdapter.h"
#include "ViewHolder.h"

/**
 * @brief 垂直线性布局管理器（仿 Android LinearLayoutManager）
 */
class LinearLayoutManager : public AbstractLayoutManager
{
public:
    explicit LinearLayoutManager(const int spacing_ = 0)
        : container(nullptr), spacing(spacing_)
    {
    }

    void setViewportSize(const QSize& size) override { m_viewportSize = size; }

    void setContainer(QWidget* widget) override { container = widget; }

    QWidget* itemParent() override { return container; }

    QPair<int, int> computeVisibleRange(int scrollY) override;

    void prepareLayoutIfNeeded(
        RecyclerAdapter<QVariant>* adapter,
        RecyclerCachePool* pool,
        QWidget* itemParent,
        int viewportHeight
    ) override;

    void addViewHolder(ViewHolder* holder, int position) override;

    void removeViewHolder(ViewHolder* holder) override;

    void layout() override;

    void makesureLayout(const int& position) override;
    QVector<QRect>& itemRects() override { return m_itemRects; }

    int preloadCount() override;

private:
    QSize m_viewportSize;
    QWidget* container;

    QMap<int, ViewHolder*> m_attachedViewHolders;
    QVector<QRect> m_itemRects;

    int spacing = 0;
};
