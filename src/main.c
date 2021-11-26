#include "pop_launcher.h"
#include <gtk/gtk.h>
#include "mainwindow.h"

int main(int argc, char **argv){
    gtk_init(&argc, &argv);

    struct mainwindow w;
    mainwindow_init(&w);

    gtk_main();
#if 0
    struct pl pl;
    if(pl_init(&pl) == NULL)
        return 0;

    pl_search(&pl, "= [1, 1, 1] + [2, 2, 3]");
    printf("\n\n\n");
    pl_search(&pl, "test");

    pl_free(&pl);
#endif
    return 0;
}

