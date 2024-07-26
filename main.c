#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *entry_login;
GtkWidget *entry_senha;
GtkWidget *entry_nome_cad;
GtkWidget *entry_login_cad;
GtkWidget *entry_senha_cad;
GtkWidget *horario_chegada;
GtkWidget *horario_saida;
GtkStack  *stack;
GtkListStore *modelodearmazenamento;

void mensagem(const gchar *texto, const gchar *texto_secundario) {
    GtkWidget *mensagem_dialogo = gtk_message_dialog_new(
        GTK_WINDOW(window),
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "%s", texto
    );
    gtk_window_set_title(GTK_WINDOW(mensagem_dialogo), texto_secundario);
    gtk_dialog_run(GTK_DIALOG(mensagem_dialogo));
    gtk_widget_destroy(mensagem_dialogo);
}

void save_to_file_login(const gchar *login_text, const gchar *senha_text, const gchar *horario_text, const gchar *buffer) {
    FILE *pArquivo_horario = fopen("Horario_de_entrada.txt", "a");
    if (pArquivo_horario == NULL) {
        g_print("Erro ao abrir o arquivo.\n");
        return;
    }

    fprintf(pArquivo_horario, "Login: %s\n", login_text);
    fprintf(pArquivo_horario, "Senha: %s\n", senha_text);
    fprintf(pArquivo_horario, "%s: %s\n", horario_text, buffer);

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

bool verificar_login(const gchar *login_text, const gchar *senha_text) {
    FILE *pArquivo_usuarios = fopen("Usuarios_registrados.txt", "r");
    if (pArquivo_usuarios == NULL) {
        g_print("Erro ao abrir o arquivo de usuários.\n");
        return false;
    }

    char line[256];
    char login[256];
    char senha[256];
    bool found = false;

    while (fgets(line, sizeof(line), pArquivo_usuarios)) {
        if (sscanf(line, "Login: %s", login) == 1) {
            continue;
        }
        if (sscanf(line, "Senha: %s", senha) == 1) {
            if (strcmp(login_text, login) == 0 && strcmp(senha_text, senha) == 0) {
                found = true;
                break;
            }
        }
    }

    fclose(pArquivo_usuarios);
    return found;
}

void login(const gchar *login_text, const gchar *senha_text) {
    if ((strcmp(login_text, "admin") == 0) && (strcmp(senha_text, "12345678") == 0)) {
        gtk_stack_set_visible_child_name(stack, "view_admin");
        const gchar *texto = "Bem Vindo, Usuario logado com sucesso!";
        const gchar *texto_secundario = "Login Sucesso";
        mensagem(texto, texto_secundario);
    } else if (verificar_login(login_text, senha_text)) {
        const gchar *texto = "Bem Vindo, Usuario logado com sucesso!";
        const gchar *texto_secundario = "Login Sucesso";
        mensagem(texto, texto_secundario);
    } else {
        const gchar *texto = "Usuario ou senha invalidos.";
        const gchar *texto_secundario = "Erro de Login";
        mensagem(texto, texto_secundario);
    }
}

void escrever_login() {
    const gchar *login_text = gtk_entry_get_text(GTK_ENTRY(entry_login));
    const gchar *senha_text = gtk_entry_get_text(GTK_ENTRY(entry_senha));

    gboolean chegada = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(horario_chegada));
    gboolean saida = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(horario_saida));

    time_t t;
    struct tm *tm_info;
    char buffer[26];
    time(&t);

    tm_info = localtime(&t);
    strftime(buffer, 26, "%d-%m-%Y %H:%M:%S", tm_info);

    const gchar *horario_text = chegada ? "Horário de chegada" : (saida ? "Horário de saída" : "Horário desconhecido");

    login(login_text, senha_text);
    save_to_file_login(login_text, senha_text, horario_text, buffer);

    // Limpar campos após o login
    gtk_entry_set_text(GTK_ENTRY(entry_login), "");
    gtk_entry_set_text(GTK_ENTRY(entry_senha), "");

    // Limpar os checkboxes
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(horario_chegada), FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(horario_saida), FALSE);
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
    const gchar *texto = "Usuario cadastrado com sucesso!";
    const gchar *texto_secundario = "Cadastro Sucesso";
    mensagem(texto, texto_secundario);
}

void on_button_listar_voltar_clicked(GtkWidget *widget, gpointer data) {
    gtk_stack_set_visible_child_name(stack, "view_admin");
}

void on_button_listar_clicked(GtkWidget *widget, gpointer data) {
    FILE *pArquivo_usuarios = fopen("Usuarios_registrados.txt", "r");
    if (pArquivo_usuarios == NULL) {
        g_print("Erro ao abrir o arquivo de usuários.\n");
        return;
    }

    GtkTreeIter iter;
    gtk_list_store_clear(modelodearmazenamento); // Limpa os itens existentes no modelo

    char line[256];
    while (fgets(line, sizeof(line), pArquivo_usuarios)) {
        // Remove o caractere de nova linha no final da string, se presente
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }

        // Adiciona a linha ao GtkListStore
        gtk_list_store_append(modelodearmazenamento, &iter);
        gtk_list_store_set(modelodearmazenamento, &iter, 0, line, -1);
    }

    fclose(pArquivo_usuarios);
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
    modelodearmazenamento = GTK_LIST_STORE(gtk_builder_get_object(builder, "data"));

    entry_login = GTK_WIDGET(gtk_builder_get_object(builder, "entry_login"));
    entry_senha = GTK_WIDGET(gtk_builder_get_object(builder, "entry_senha"));
    entry_nome_cad = GTK_WIDGET(gtk_builder_get_object(builder, "entry_nome_cad"));
    entry_login_cad = GTK_WIDGET(gtk_builder_get_object(builder, "entry_login_cad"));
    entry_senha_cad = GTK_WIDGET(gtk_builder_get_object(builder, "entry_senha_cad"));
    horario_chegada = GTK_WIDGET(gtk_builder_get_object(builder, "horario_chegada"));
    horario_saida = GTK_WIDGET(gtk_builder_get_object(builder, "horario_saida"));

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
