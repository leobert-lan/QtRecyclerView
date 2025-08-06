#pragma once
#include <QWidget>

class ViewHolder : public QWidget
{
    Q_OBJECT

public:
    explicit ViewHolder(QWidget* parent = nullptr, QString viewType = "default") : QWidget(parent), m_type(std::move(viewType))
    {
    }

    virtual void bindData(const QVariant& data) = 0;


    [[nodiscard]] QString viewType() const { return m_type; }

private:
    QString m_type;
};
