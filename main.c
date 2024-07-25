#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *entry_login;
GtkWidget *entry_senha;
GtkWidget *entry_nome_cad;
GtkWidget *entry_login_cad;
GtkWidget *entry_senha_cad;
GtkStack  *stack;

void save_to_file_login(const gchar *login_text, const gchar *senha_text) {
    FILE *pArquivo_horario = fopen("Horario_de_entrada.txt", "a");
    if (pArquivo_horario == NULL) {
        g_print("Erro ao abrir o arquivo.\n");
        return;
    }

    fprintf(pArquivo_horario, "Login: %s\n", login_text);
    fprintf(pArquivo_horario, "Senha: %s\n", senha_text);
    fprintf(pArquivo_horario, "\n");

    fclose(pArquivo_horario);
}

void save_to_file_cadastro(const gchar *cad_nome_text, const gchar *cad_login_text, const gchar *cad_senha_text) {
    FILE *pArquivo_usuarios = fopen("Usuarios_registrados.txt", "a");
    if (pArquivo_usuarios == NULL) {
        g_print("Erro ao abrir o arquivo.\n");
        return;
    }

    fprintf(pArquivo_usuarios, "Nome: %s\n", cad_nome_text);
    fprintf(pArquivo_usuarios, "Login: %s\n", cad_login_text);
    fprintf(pArquivo_usuarios, "Senha: %s\n", cad_senha_text);
    fprintf(pArquivo_usuarios, "\n");

    fclose(pArquivo_usuarios);
}

void login(const gchar *login_text, const gchar *senha_text) {
    if ((strcmp(login_text, "admin") == 0) && (strcmp(senha_text, "12345678") == 0)) {
        gtk_stack_set_visible_child_name(stack, "view_admin");
    }
}

void escrever_login() {
    const gchar *login_text = gtk_entry_get_text(GTK_ENTRY(entry_login));
    const gchar *senha_text = gtk_entry_get_text(GTK_ENTRY(entry_senha));

    login(login_text, senha_text);
    save_to_file_login(login_text, senha_text);

    // Limpar campos após o login
    gtk_entry_set_text(GTK_ENTRY(entry_login), "");
    gtk_entry_set_text(GTK_ENTRY(entry_senha), "");
}

void escrever_cad() {
    const gchar *cad_nome_text = gtk_entry_get_text(GTK_ENTRY(entry_nome_cad));
    const gchar *cad_login_text = gtk_entry_get_text(GTK_ENTRY(entry_login_cad));
    const gchar *cad_senha_text = gtk_entry_get_text(GTK_ENTRY(entry_senha_cad));

    save_to_file_cadastro(cad_nome_text, cad_login_text, cad_senha_text);

    // Limpar campos após o cadastro
    gtk_entry_set_text(GTK_ENTRY(entry_nome_cad), "");
    gtk_entry_set_text(GTK_ENTRY(entry_login_cad), "");
    gtk_entry_set_text(GTK_ENTRY(entry_senha_cad), "");
}

void on_main_window_destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

void on_button_login_clicked(GtkWidget *widget, gpointer data) {
    escrever_login();
}

void on_button_cadastrar_inicial_clicked(GtkWidget *widget, gpointer data) {
    gtk_stack_set_visible_child_name(stack, "view_cadastro");
}

void on_button_listar_inicial_clicked(GtkWidget *widget, gpointer data) {
    gtk_stack_set_visible_child_name(stack, "view_listar");
}

void on_button_sair_inicial_clicked(GtkWidget *widget, gpointer data) {
    gtk_stack_set_visible_child_name(stack, "view_login");
}

void on_button_cad_voltar_clicked(GtkWidget *widget, gpointer data) {
    gtk_stack_set_visible_child_name(stack, "view_admin");
}

void on_button_cadastar_clicked(GtkWidget *widget, gpointer data) {
    escrever_cad();
}

void on_button_listar_voltar_clicked(GtkWidget *widget, gpointer data) {
    gtk_stack_set_visible_child_name(stack, "view_admin");
}

void on_button_listar_clicked(GtkWidget *widget, gpointer data) {
    // Implementar a funcionalidade para listar se necessário
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("ui.glade");

    gtk_builder_add_callback_symbols(
        builder,
        "on_main_window_destroy",               G_CALLBACK(on_main_window_destroy),
        "on_button_login_clicked",              G_CALLBACK(on_button_login_clicked),
        "on_button_cadastrar_inicial_clicked",  G_CALLBACK(on_button_cadastrar_inicial_clicked),
        "on_button_listar_inicial_clicked",     G_CALLBACK(on_button_listar_inicial_clicked),
        "on_button_sair_inicial_clicked",       G_CALLBACK(on_button_sair_inicial_clicked),
        "on_button_cad_voltar_clicked",         G_CALLBACK(on_button_cad_voltar_clicked),
        "on_button_cadastar_clicked",           G_CALLBACK(on_button_cadastar_clicked),
        "on_button_listar_voltar_clicked",      G_CALLBACK(on_button_listar_voltar_clicked),
        "on_button_listar_clicked",             G_CALLBACK(on_button_listar_clicked),
        NULL);
    gtk_builder_connect_signals(builder, NULL);

    // Obtenção do objeto da janela principal e dos campos de entrada
    stack = GTK_STACK(gtk_builder_get_object(builder, "stack"));
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));


    entry_login = GTK_WIDGET(gtk_builder_get_object(builder, "entry_login"));
    entry_senha = GTK_WIDGET(gtk_builder_get_object(builder, "entry_senha"));
    entry_nome_cad = GTK_WIDGET(gtk_builder_get_object(builder, "entry_nome_cad"));
    entry_login_cad = GTK_WIDGET(gtk_builder_get_object(builder, "entry_login_cad"));
    entry_senha_cad = GTK_WIDGET(gtk_builder_get_object(builder, "entry_senha_cad"));



    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
