//
// Created by Lenovo on 2025/8/5.
//

#ifndef GRIDLAYOUTMANAGER_H
#define GRIDLAYOUTMANAGER_H

#include <QGridLayout>
#include <QMap>
#include <QWidget>
#include <QSize>

#include "AbstractLayoutManager.h"
#include "ViewHolder.h"

class GridLayoutManager : public AbstractLayoutManager {
public:
    explicit GridLayoutManager(int spanCount);

    void setViewportSize(const QSize& size) override;
    QPair<int, int> computeVisibleRange(int scrollY) override;
    void addViewHolder(ViewHolder* holder, int position) override;
    void removeViewHolder(ViewHolder* holder) override;
    void layout() override;
    QWidget* itemParent() override;

    void setContainer(QWidget* container) override;

private:
    int m_spanCount;
    QSize m_viewportSize;
    QWidget* m_container = nullptr;
    QGridLayout* m_gridLayout = nullptr;

    QVector<QRect> m_itemRects;
    QMap<int, ViewHolder*> m_attachedViewHolders;

    int spacing = 8;
};


#endif //GRIDLAYOUTMANAGER_H
