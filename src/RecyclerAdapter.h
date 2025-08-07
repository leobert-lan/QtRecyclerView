#pragma once
#include <QVector>
#include <QVariant>

#include "RecyclerViewNotifier.h"
#include "ViewHolder.h"

template<typename T>
class RecyclerAdapter {
public:
    virtual ~RecyclerAdapter() = default;
    [[nodiscard]] virtual int getItemCount() const = 0;
    [[nodiscard]] virtual QVariant getItem(int position) const = 0;
    [[nodiscard]] virtual QString getItemViewType(int position) const {
        return "default";
    }
    virtual ViewHolder* onCreateViewHolder(QWidget* parent,const QString& type) = 0;
    virtual void onBindViewHolder(ViewHolder* holder, int position) = 0;

    void setRecyclerView(RecyclerViewNotifier* view) { m_view = view; }

    void notifyDataSetChanged() const;
    void notifyItemInserted(int position) const;
    void notifyItemRemoved(int position) const;
    void notifyItemChanged(int position) const;

protected:
    RecyclerViewNotifier* m_view = nullptr;
};

template<typename T>
void RecyclerAdapter<T>::notifyDataSetChanged() const
{
    if (m_view)
        m_view->notifyDataSetChanged();
}

template<typename T>
void RecyclerAdapter<T>::notifyItemInserted(const int position) const
{
    if (m_view)
        m_view->notifyItemInserted(position);
}

template<typename T>
void RecyclerAdapter<T>::notifyItemRemoved(const int position) const
{
    if (m_view)
        m_view->notifyItemRemoved(position);
}

template<typename T>
void RecyclerAdapter<T>::notifyItemChanged(const int position) const
{
    if (m_view)
        m_view->notifyItemChanged(position);
}
