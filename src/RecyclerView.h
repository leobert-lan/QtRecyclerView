#pragma once
#include <QScrollArea>
#include <QMap>
#include "RecyclerAdapter.h"
#include "AbstractLayoutManager.h"
#include "RecyclerCachePool.h"
#include "RecyclerViewNotifier.h"

class RecyclerView : public QScrollArea, public RecyclerViewNotifier
{
    Q_OBJECT

public:
    explicit RecyclerView(QWidget* parent = nullptr);

    void setAdapter(RecyclerAdapter<QVariant>* adapter);
    void setLayoutManager(AbstractLayoutManager* layoutManager);

    [[nodiscard]] QString getItemViewType(int position) const {
        return m_adapter->getItemViewType(position);
    }

protected:
    void resizeEvent(QResizeEvent* event) override;
    void scrollContentsBy(int dx, int dy) override;

    void notifyDataSetChanged() override;
    void notifyItemInserted(int position) override;
    void notifyItemRemoved(int position) override;
    void notifyItemChanged(int position) override;

private:
    QWidget* contentWidget;
    RecyclerAdapter<QVariant>* m_adapter = nullptr;
    RecyclerCachePool m_cachePool;
    AbstractLayoutManager* m_layoutManager = nullptr;

    QMap<int, ViewHolder*> m_attachedViewHolders;
    int scrollOffset = 0;

    void updateVisibleItems();
    void recycleAllViews();
};
