#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtk/gtk.h>
#include "pop_launcher.h"

struct mainwindow{
    GtkWidget *win;
    GtkWidget *box0, *box1;
    GtkWidget *entry;

    struct pl launcher;
};

struct mainwindow *mainwindow_init(struct mainwindow *dst);
void mainwindow_free(struct mainwindow *dst);

void mainwindow_close_cb(GtkWidget *widget, gpointer data);
void mainwindow_changed_cb(GtkWidget *widget, gpointer data);

#endif //MAINWINDOW_H
