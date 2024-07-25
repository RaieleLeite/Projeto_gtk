#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdbool.h>

GtkBuilder *builder;
GtkWidget *window;

void on_main_window_destroy(GtkWidget *widget, gpointer data){
    gtk_main_quit();
}

void on_button_login_clicked(GtkWidget *widget, gpointer data){
    char *login = gtk_entry_get_text(gtk_builder_get_object(builder, "Login"));

}

void on_button_cadastrar_inicial_clicked(GtkWidget *widget, gpointer data){
    gtk_main_quit();
}

void on_button_listar_inicial_clicked(GtkWidget *widget, gpointer data){
    gtk_main_quit();
}

void on_button_sair_inicial_clicked(GtkWidget *widget, gpointer data){
    gtk_main_quit();
}

void on_button_listar_voltar_clicked(GtkWidget *widget, gpointer data){
    gtk_main_quit();
}

void on_button_listar_clicked(GtkWidget *widget, gpointer data){
    gtk_main_quit();
}

int main (int argc, char *argv[]){
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("ui.glade");

    gtk_builder_add_callback_symbols(
        builder,
        "on_main_window_destroy",                   G_CALLBACK(on_main_window_destroy),
        "on_button_login_clicked",                  G_CALLBACK(on_button_login_clicked),
        "on_button_cadastrar_inicial_clicked",      G_CALLBACK(on_button_cadastrar_inicial_clicked),
        "on_button_listar_inicial_clicked",         G_CALLBACK(on_button_listar_inicial_clicked),
        "on_button_sair_inicial_clicked",           G_CALLBACK(on_button_sair_inicial_clicked),
        "on_button_listar_voltar_clicked",          G_CALLBACK(on_button_listar_voltar_clicked),
        "on_button_listar_clicked",                 G_CALLBACK(on_button_listar_clicked),
        NULL);
    gtk_builder_connect_signals(builder, NULL);



    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

    gtk_widget_show_all(window);
    gtk_main();
    return 0;


}


