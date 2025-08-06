#pragma once

#include <QMap>
#include <QRect>
#include <QSize>
#include <QVector>
#include "AbstractLayoutManager.h"
#include "RecyclerAdapter.h"
#include "ViewHolder.h"

/**
 * @brief 垂直线性布局管理器（仿 Android LinearLayoutManager）
 */
class LinearLayoutManager : public AbstractLayoutManager {
public:
    void setViewportSize(const QSize& size) override { m_viewportSize = size; }

    void setContainer(QWidget* widget) override { container = widget; }

    QWidget* itemParent() override { return container; }

    QPair<int, int> computeVisibleRange(int scrollY) override;
    void prepareLayoutIfNeeded(RecyclerAdapter<QVariant>* adapter, QWidget* itemParent, int viewportHeight);

    void addViewHolder(ViewHolder* holder, int position) override;

    void removeViewHolder(ViewHolder* holder) override;

    void layout() override;

private:
    QSize m_viewportSize;
    QWidget* container = nullptr;

    QMap<int, ViewHolder*> m_attachedViewHolders;
    QVector<QRect> m_itemRects;

    int spacing = 8;
};
