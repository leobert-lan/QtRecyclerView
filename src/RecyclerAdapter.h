#pragma once
#include <QVector>
#include <QVariant>
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
};
