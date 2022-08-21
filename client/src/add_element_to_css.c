#include "../inc/client.h"

void add_element_to_css(GtkWidget *element, char *name)
{
    GtkStyleContext *context;
    context = gtk_widget_get_style_context(element);
    gtk_style_context_add_class(context, name);
}

