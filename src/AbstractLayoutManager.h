//
// Created by Lenovo on 2025/8/5.
//

#ifndef ABSTRACTLAYOUTMANAGER_H
#define ABSTRACTLAYOUTMANAGER_H

#include <QPair>
#include <QSize>
#include "RecyclerAdapter.h"


class ViewHolder;
class QWidget;

/**
 * @brief 抽象布局管理器，类似 Android 中的 LayoutManager 接口
 */
class AbstractLayoutManager
{
public:
    virtual ~AbstractLayoutManager() = default;

    /**
     * @brief 设置可视区域的大小（由 RecyclerView 提供）
     * @param size 当前 viewport 尺寸
     */
    virtual void setViewportSize(const QSize& size) = 0;

    virtual void setContainer(QWidget* widget) = 0;
    /**
     * @brief 计算在当前 scrollY 情况下可见的 item position 范围
     * @param scrollY 垂直滚动偏移
     * @return 起始和结束 position（闭区间）
     */
    virtual QPair<int, int> computeVisibleRange(int scrollY) = 0;

    /**
     * @brief 返回用于放置 ViewHolder 的父容器（通常是 RecyclerView 的 container）
     */
    virtual QWidget* itemParent() = 0;

    /**
     * @brief 添加一个 ViewHolder 到布局中
     * @param holder 被添加的 ViewHolder
     * @param position 数据 position
     */
    virtual void addViewHolder(ViewHolder* holder, int position) = 0;

    /**
     * @brief 从布局中移除 ViewHolder（一般不会删除其内存）
     */
    virtual void removeViewHolder(ViewHolder* holder) = 0;

    /**
     * @brief 根据当前状态重新布局所有元素（可更新 container 尺寸）
     */
    virtual void layout() = 0;
    virtual void prepareLayoutIfNeeded(RecyclerAdapter<QVariant>* recycler_adapter, QWidget* widget, int height) = 0;

    virtual void makesureLayout(const int& position) = 0;

    virtual QVector<QRect>& itemRects() = 0;

    /// 滑动过程中，提前预加载的 item 个数
    virtual int preloadCount() = 0;
};

#endif // ABSTRACTLAYOUTMANAGER_H
