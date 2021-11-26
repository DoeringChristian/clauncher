#ifndef POP_LAUNCHER_H
#define POP_LAUNCHER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include "darray.h"
#include "jp.h"
#include "util.h"
#include <gtk/gtk.h>

struct icon_src{
    char *name, *mime;
};

struct pl_item{
    int id;
    char *name;
    char *description;
    struct icon_src icon;
    struct icon_src icon_cat;
    size_t window[2];

    GtkWidget *toplevel;
    GtkWidget *icon_img;
    GtkWidget *label;
};

struct pl{
    darray(struct pl_item) items;
    struct process process;
};

struct pl *pl_init(struct pl *dst);
void pl_free(struct pl *dst);

int pl_search(struct pl *dst, const char *str);

struct pl_item *pl_item_init(struct pl_item *dst, struct jp_element json);
void pl_item_free(struct pl_item *dst);

struct icon_src *icon_src_init(struct icon_src *dst, struct jp_member json);
void icon_src_free(struct icon_src *dst);

#endif //POP_LAUNCHER_H
