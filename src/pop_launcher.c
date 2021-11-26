#include "pop_launcher.h"

struct pl *pl_init(struct pl *dst){
    process_open(&dst->process, "pop-launcher");

    darray_init(&dst->items, 16);

    return dst;
}
void pl_free(struct pl *dst){
    process_close(&dst->process);

    for(size_t i = 0;i < darray_len(&dst->items);i++){
        pl_item_free(&dst->items[i]);
    }
    darray_free(&dst->items);
}

int pl_search(struct pl *dst, const char *str){
    // clear results

    for(;darray_len(&dst->items) > 0;darray_pop_back(&dst->items)){
        pl_item_free(&dst->items[darray_len(&dst->items)-1]);
    }


    process_printf(&dst->process, "{\"Search\":\"%s\"}\n", str);

    g_print("{\"Search\":%s}\n", str);

    darray(char) response;
    darray_init(&response, 100);

    char buf[100];
    size_t i = 0;
    while((i = process_read(&dst->process, buf, 100)) > 0){
        darray_append(&response, buf, i);
        if(buf[i-1] == '\n')
            break;
    }
    darray_push_back(&response, 0);

    char *c = strchr(response, '{');
    if(c >= &response[darray_len(&response) -1])
        goto cleanup;
    //printf("%s\n", c);

    struct jp_element main = jp_element(c);

    struct jp_member update = jp_element_member(main);

    if(!jp_member_namecomp(update, "Update"))
        goto cleanup;

    struct jp_array results = jp_member_array(update);

    jp_array_foreach(results, elem){
        g_print("%i\n", elem.state.valid);
        struct pl_item item;

        pl_item_init(&item, elem);

        g_print("item: (id: %i, name: %s, description: %s)\n", item.id, item.name, item.description);

        darray_push_back(&dst->items, item);
    }

cleanup:

    darray_free(&response);

    return 0;
}

struct pl_item *pl_item_init(struct pl_item *dst, struct jp_element json){
    *dst = (struct pl_item){0};

    struct jp_member members = jp_element_member(json);
    struct jp_member id = jp_members_search(members, "id");
    struct jp_member name = jp_members_search(members, "name");
    struct jp_member description = jp_members_search(members, "description");
    struct jp_member icon = jp_members_search(members, "icon");
    struct jp_member icon_cat = jp_members_search(members, "category_icon");
    struct jp_member window = jp_members_search(members, "window");


    size_t i;

    if(id.state.valid){
        double d = 0;
        jp_member_number(id, &d);
        dst->id = (int)d;
    }

    if(name.state.valid){
        i = jp_member_strlen(name);
        dst->name = malloc(sizeof(char) * (i+1));
        jp_member_string(name, dst->name, i);
        dst->name[i] = 0;
    }

    if(description.state.valid){
        i = jp_member_strlen(description);
        dst->description = malloc(sizeof(char) * (i+1));
        jp_member_string(description, dst->description, i);
        dst->description[i] = 0;
    }

    if(icon.state.valid){
        icon_src_init(&dst->icon, icon);
    }

    if(icon_cat.state.valid){
        icon_src_init(&dst->icon_cat, icon_cat);
    }

    if(window.state.valid){
        struct jp_element elements = jp_array_element(jp_member_array(window));
        struct jp_element first = elements;
        struct jp_element second = jp_element_next(first);

        double d;

        jp_element_number(first, &d);
        dst->window[0] = (int)d;

        jp_element_number(second, &d);
        dst->window[1] = (int)d;
    }

    // gtk init
    
    dst->toplevel = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    if(dst->icon.name != NULL){
        dst->icon_img = gtk_image_new_from_icon_name(dst->icon.name, 40);
        gtk_image_set_pixel_size(GTK_IMAGE(dst->icon_img), 40);
        gtk_box_pack_start(GTK_BOX(dst->toplevel), dst->icon_img, FALSE, FALSE, 0);
    }

    if(dst->name != NULL){
        dst->label = gtk_label_new(dst->name);
        gtk_box_pack_start(GTK_BOX(dst->toplevel), dst->label, TRUE, TRUE, 0);
    }

    return dst;
}

void pl_item_free(struct pl_item *dst){
    free(dst->name);
    free(dst->description);
    dst->name = NULL;
    dst->description = NULL;

    icon_src_free(&dst->icon);
    icon_src_free(&dst->icon_cat);

    gtk_widget_destroy(dst->toplevel);
    dst->toplevel = NULL;

    dst->id = 0;
}

struct icon_src *icon_src_init(struct icon_src *dst, struct jp_member json){
    size_t i = 0;
    dst->name = NULL;
    dst->mime = NULL;

    if(!json.state.valid)
        return NULL;

    struct jp_member icon_members = jp_member_member(json);
    struct jp_member icon_name = jp_members_search(icon_members, "Name");
    struct jp_member icon_mime = jp_members_search(icon_members, "Mime");

    if(icon_name.state.valid){
        i = jp_member_strlen(icon_name);
        dst->name = malloc(sizeof(char) * (i+1));
        jp_member_string(icon_name, dst->name, i);
        dst->name[i] = 0;
    }

    if(icon_mime.state.valid){
        i = jp_member_strlen(icon_mime);
        dst->mime = malloc(sizeof(char) * (i+1));
        jp_member_string(icon_mime, dst->mime, i);
        dst->mime[i] = 0;
    }

    return dst;
}
void icon_src_free(struct icon_src *dst){
    free(dst->name);
    free(dst->mime);
    dst->name = NULL;
    dst->mime = NULL;
}
