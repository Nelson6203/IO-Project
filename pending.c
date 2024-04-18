#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <glibconfig.h>

GtkWidget *vPending;

void window_pending() {
    printf("en pending");
}

int main(int argc, char **argv) {
    printf("asdssdsdsdsdd");
    GtkBuilder *builder;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "pendingWindow.glade", NULL);

    vPending = GTK_WIDGET(gtk_builder_get_object(builder, "vPending"));
    //button1 = GTK_WIDGET(gtk_builder_get_object(builder, "button1"));

    //g_signal_connect(button6, "clicked", G_CALLBACK(on_button6_clicked), NULL);

    gtk_widget_show_all(vPending);
    gtk_main();
}
