#include <MyViewHolder.h>
#include <QApplication>
#include <QStringList>
#include <QVBoxLayout>
#include "RecyclerView.h"
#include "qlabel.h"
#include "LinearLayoutManager.h"
#include "ViewHolder.h"
#include "RecyclerAdapter.h"

class MyAdapter : public RecyclerAdapter<QVariant> {
public:
    explicit MyAdapter(const QStringList& items) : m_items(items) {}

    int getItemCount() const override {
        return m_items.size();
    }

    QVariant getItem(int position) const override {
        return m_items.value(position);
    }

    QString getItemViewType(int position) const override {
        return "text";
    }

    ViewHolder* onCreateViewHolder(QWidget* parent, const QString& type) override {
        return new MyViewHolder(parent);
    }

    void onBindViewHolder(ViewHolder* holder, int position) override {
        holder->bindData(getItem(position));
    }

private:
    QStringList m_items;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget mainWindow;
    mainWindow.setWindowTitle("RecyclerView LinearLayoutManager Example");
    mainWindow.resize(400, 600);

    auto* recyclerView = new RecyclerView;
    recyclerView->resize(400,600);

    QStringList items;
    for (int i = 1; i <= 100; ++i) {
        items << QString("Item #%1").arg(i);
    }

    auto* adapter = new MyAdapter(items);
    auto* layoutManager = new LinearLayoutManager;

    recyclerView->setAdapter(adapter);
    recyclerView->setLayoutManager(layoutManager);

    QVBoxLayout* layout = new QVBoxLayout(&mainWindow);
    layout->addWidget(recyclerView);
    layout->setContentsMargins(0, 0, 0, 0);

    mainWindow.show();
    return app.exec();
}
