//
// Created by Lenovo on 2025/8/5.
//
#ifndef GRIDLAYOUTMANAGER_H
#define GRIDLAYOUTMANAGER_H

#include "AbstractLayoutManager.h"
#include <QVector>
#include <QMap>
#include <QRect>

class GridLayoutManager : public AbstractLayoutManager
{
public:
    explicit GridLayoutManager(int spanCount = 2, int spacing = 8);

    void setSpanCount(int count);

    void setViewportSize(const QSize& size) override;
    void setContainer(QWidget* widget) override;
    QWidget* itemParent() override;

    QPair<int, int> computeVisibleRange(int scrollY) override;
    void addViewHolder(ViewHolder* holder, int position) override;
    void removeViewHolder(ViewHolder* holder) override;
    void layout() override;
    void prepareLayoutIfNeeded(
        RecyclerAdapter<QVariant>* adapter,
        RecyclerCachePool* pool,
        QWidget* itemParent,
        int viewportHeight
    ) override;
    // void prepareLayoutIfNeeded(RecyclerAdapter<QVariant>* adapter, QWidget* itemParent, int viewportHeight) override;
    void makesureLayout(const int& position) override;
    QVector<QRect>& itemRects() override { return m_itemRects; }
    int preloadCount() override;

private:
    int spanCount;
    int spacing;
    QSize m_viewportSize;
    QWidget* m_container = nullptr;
    QVector<QRect> m_itemRects;
    QMap<int, ViewHolder*> m_attachedViewHolders;
};

#endif // GRIDLAYOUTMANAGER_H
