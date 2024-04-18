#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <glibconfig.h>

// Proyecto de IO
// Nelson Alvarado Navarro, Esteban Villavicencio Soto

GtkWidget *ventanaP;

GtkWidget *button1;
GtkWidget *button2;
GtkWidget *button3;
GtkWidget *button4;
GtkWidget *button5;
GtkWidget *button6;

// almacenar la ventana de diálogo

void call_pending() {
    printf("tamos");
    system("./pending &");
    //system("./pending.c &");
    printf("salimos");

}

void on_button3_clicked(GtkButton *button, gpointer user_data) {
    gtk_main_quit();
}

void on_button4_clicked(GtkButton *button, gpointer user_data) {
    printf("asdsd");
    system("./floyA &");
    printf("asdsd2");
}

void on_button1_clicked(GtkButton *button, gpointer user_data) {
    printf("asdsd");
    system("./KnapS &");
    printf("asdsd2");

}
void on_button2_clicked(GtkButton *button, gpointer user_data) {
    system("./ReemplazoEquipos &");


}

void on_button5_clicked(GtkButton *button, gpointer user_data) {
    system("./SeriesDeportivas &");
}

void on_button6_clicked(GtkButton *button, gpointer user_data) {
     system("./Abinarios &");
}

int main(int argc, char **argv) {
    printf("asdssdsdsdsdd");
    GtkBuilder *builder;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "ProyectoIO.glade", NULL);

    ventanaP = GTK_WIDGET(gtk_builder_get_object(builder, "ventanaP"));
    button1 = GTK_WIDGET(gtk_builder_get_object(builder, "button1"));
    button2 = GTK_WIDGET(gtk_builder_get_object(builder, "button2"));

    button3 = GTK_WIDGET(gtk_builder_get_object(builder, "button3"));
    button4 = GTK_WIDGET(gtk_builder_get_object(builder, "button4"));
    button5 = GTK_WIDGET(gtk_builder_get_object(builder, "button5"));
    button6 = GTK_WIDGET(gtk_builder_get_object(builder, "button6"));


    g_signal_connect(button1, "clicked", G_CALLBACK(on_button1_clicked), NULL);
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button2_clicked), NULL);
    g_signal_connect(button3, "clicked", G_CALLBACK(on_button3_clicked), NULL);
    g_signal_connect(button4, "clicked", G_CALLBACK(on_button4_clicked), NULL);
    g_signal_connect(button5, "clicked", G_CALLBACK(on_button5_clicked), NULL);
    g_signal_connect(button6, "clicked", G_CALLBACK(on_button6_clicked), NULL);

    gtk_widget_show_all(ventanaP);
    gtk_main();
}
