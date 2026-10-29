// calculadora opensource wow
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    GtkWidget *display;
    double acumun;
    char operator; 
    gboolean new_num;
    gboolean erro;

} Calc;

static void mostrar(Calc *c, const char *texto)
{
    gtk_label_set_text(GTK_LABEL(c->display), texto);
}

static void mostrar_numero(Calc *c, double valor)
{
    char *s = g_strdup_printf("%.10g", valor);
    mostrar(c, s);
    g_free(s);
}

static void resetar(Calc *reset)
{
    reset->acumun = 0;
    reset->operator = 0;
    reset->new_num = TRUE;
    reset->erro = FALSE;
    mostrar(reset, "0");
}

static double aplicar(Calc *c, double a, char op, double b)
{
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (b == 0) {
            c->erro = TRUE;
            return 0;
        }
        return a / b;
    default:  return b;
    }
}

static void pendent_operation(Calc *c)
{
    double atual = atof(gtk_label_get_text(GTK_LABEL(c->display)));

    if (c->operator)
        c->acumun = aplicar(c, c->acumun, c->operator, atual); else
        c->acumun = atual;

    if (c->erro)
        mostrar(c, "Erro");
    else
        mostrar_numero(c, c->acumun);
}

static void on_botao_clicado(GtkButton *botao, gpointer user_data)
{
    Calc *c = user_data;
    const char *rotulo = gtk_button_get_label(botao);
    char ch = rotulo[0];

    if (ch == 'C') {
        resetar(c);
        return;
    }

    if (c->erro)
        resetar(c);

    if ((ch >= '0' && ch <= '9') || ch == '.') {
        const char *atual = gtk_label_get_text(GTK_LABEL(c->display));

        if (c->new_num) {
            if (ch == '.')
                mostrar(c, "0.");
            else
                mostrar(c, rotulo);
            c->new_num = FALSE;
        } else {
            if (ch == '.' && strchr(atual, '.'))
                return;
            if (strcmp(atual, "0") == 0 && ch != '.') {
                mostrar(c, rotulo);
            } else {
                char *s = g_strconcat(atual, rotulo, NULL);
                mostrar(c, s);
                g_free(s);
            }
        }
        return;
    }

    if (ch == '=') {
        if (c->operator && !c->new_num) {
            pendent_operation(c);
            c->operator = 0;
            c->new_num = TRUE;
        }
        return;
    }

    // operadores
    if (!c->new_num) {
        pendent_operation(c);
        if (c->erro) {
            c->operator = 0;
            c->new_num = TRUE;
            return;
        }
    } else if (!c->operator) {
        c->acumun = atof(gtk_label_get_text(GTK_LABEL(c->display)));
    }
    c->operator = ch;
    c->new_num = TRUE;
}

static void activate(GtkApplication *app)
{
    GtkWidget *window, *caixa, *grade;
    Calc *c = g_new0(Calc, 1);

    static const char *botoes[5][4] = {
        { "7", "8", "9", "/" },
        { "4", "5", "6", "*" },
        { "1", "2", "3", "-" },
        { "0", ".", "=", "+" },
        { "C", NULL, NULL, NULL },
    };

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 260, 360);

    //optimizators
    g_signal_connect_swapped(window, "destroy", G_CALLBACK(g_free), c);

    caixa = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_margin_top(caixa, 8);
    gtk_widget_set_margin_bottom(caixa, 8);
    gtk_widget_set_margin_start(caixa, 8);
    gtk_widget_set_margin_end(caixa, 8);
    gtk_window_set_child(GTK_WINDOW(window), caixa);

    c->display = gtk_label_new("0");
    gtk_label_set_xalign(GTK_LABEL(c->display), 1.0);
    gtk_widget_add_css_class(c->display, "title-1");
    gtk_box_append(GTK_BOX(caixa), c->display);

    //grid
    grade = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grade), 6);
    gtk_grid_set_column_spacing(GTK_GRID(grade), 6);
    gtk_grid_set_row_homogeneous(GTK_GRID(grade), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grade), TRUE);
    gtk_widget_set_vexpand(grade, TRUE);
    gtk_box_append(GTK_BOX(caixa), grade);

    for (int linha = 0; linha < 5; linha++) {
        for (int col = 0; col < 4; col++) {
            const char *rotulo = botoes[linha][col];
            if (!rotulo)
                continue;

            GtkWidget *b = gtk_button_new_with_label(rotulo);
            g_signal_connect(b, "clicked", G_CALLBACK(on_botao_clicado), c);

            int largura = (rotulo[0] == 'C') ? 4 : 1;
            gtk_grid_attach(GTK_GRID(grade), b, col, linha, largura, 1);
        }
    }

    resetar(c);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("br.calculis.teste", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}