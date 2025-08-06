#include "myviewholder.h"

#include <qboxlayout.h>


MyViewHolder::MyViewHolder(QWidget* parent)
    : ViewHolder(parent, "default"), label(new QLabel(this)) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->setContentsMargins(16, 8, 16, 8);
    label->setStyleSheet("background:#f0f0f0; border:1px solid #ccc; padding:8px;");
}

void MyViewHolder::bindData(const QVariant& data) {
    label->setText(data.toString());
}

QSize MyViewHolder::sizeHint() const
{
    return QSize(500, 50);
}
