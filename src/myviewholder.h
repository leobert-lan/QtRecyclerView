#ifndef MYVIEWHOLDER_H
#define MYVIEWHOLDER_H

#include <ViewHolder.h>
#include <qlabel.h>


class MyViewHolder : public ViewHolder {
    Q_OBJECT
public:
    explicit MyViewHolder(QWidget* parent = nullptr);

    void bindData(const QVariant& data) override;

    QSize sizeHint() const override;

private:
    QLabel* label;
};

#endif // MYVIEWHOLDER_H
