//
// Created by Lenovo on 2025/8/7.
//

#ifndef RECYCLERVIEWNOTIFIER_H
#define RECYCLERVIEWNOTIFIER_H

class RecyclerViewNotifier {
public:
    virtual ~RecyclerViewNotifier() = default;

    virtual void notifyDataSetChanged() = 0;
    virtual void notifyItemInserted(int position) = 0;
    virtual void notifyItemRemoved(int position) = 0;
    virtual void notifyItemChanged(int position) = 0;
};


#endif //RECYCLERVIEWNOTIFIER_H
