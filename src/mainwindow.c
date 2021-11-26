#include "mainwindow.h"

struct mainwindow *mainwindow_init(struct mainwindow *dst){
    dst->win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    dst->box0 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    dst->box1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    dst->entry = gtk_entry_new();

    pl_init(&dst->launcher);

    gtk_container_add(GTK_CONTAINER(dst->win), dst->box0);
    gtk_box_pack_start(GTK_BOX(dst->box0), dst->entry, FALSE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(dst->box0), dst->box1, TRUE, TRUE, 0);

    g_signal_connect(G_OBJECT(dst->win), "destroy", G_CALLBACK(mainwindow_close_cb), dst);
    g_signal_connect(G_OBJECT(dst->entry), "changed", G_CALLBACK(mainwindow_changed_cb), dst);

    gtk_widget_show_all(dst->win);
    return dst;
}
void mainwindow_free(struct mainwindow *dst){
    pl_free(&dst->launcher);
    gtk_main_quit();
}

void mainwindow_close_cb(GtkWidget *widget, gpointer data){
    mainwindow_free(data);
}
void mainwindow_changed_cb(GtkWidget *widget, gpointer data){
    struct mainwindow *dst = data;
    g_print("test\n");
    
    pl_search(&dst->launcher, gtk_entry_get_text(GTK_ENTRY(dst->entry)));

    for(size_t i = 0;i < darray_len(&dst->launcher.items);i++){
        gtk_box_pack_start(GTK_BOX(dst->box0), dst->launcher.items[i].toplevel, FALSE, TRUE, 0);
    }

    gtk_widget_show_all(dst->win);
}
