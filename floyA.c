#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <glibconfig.h>
#include <cairo.h>
#include <ctype.h>
#define N 1024
#define INF 9999

typedef struct {
    double x;
    double y;
} Point;

Point nodePositions[N];
gboolean graphInitialized = FALSE;

GtkWidget *vFloy;
GtkWidget* matriz[N][N];
GtkWidget* matrizOptima[N][N];

GtkWidget *dialogResultado;
GtkWidget *grafo;
GtkWidget *botonResultado;
GtkWidget *labelResultados;
GtkWidget *textoResultados;

GtkWidget *tablaDtitulo;
GtkWidget *alertas;

// MatrizD
GtkWidget *matrizD;
GtkWidget *matrizboxD1;
GtkWidget *matrizboxD2;

//inputs
GtkWidget *inputCNodos;
GtkWidget *inputArchivoName;

// MatrizP
GtkWidget *matrizP;
GtkWidget *matrizboxP1;
GtkWidget *matrizboxP2;


//botones
GtkWidget *botonCNodos;
GtkWidget *botonSiguienteTabla;
GtkWidget *botoncalcularRuta;
GtkWidget *botonGuardarArchivo;
GtkWidget *botonAbrirArchivo;
GtkWidget *botonSalirFloyd;


FILE *archivo;
FILE *archivoMatriz;

int indiceTablaActual = 0;

int numNodos = 0;
int matrizSize = 0;
int num = 1;
char Dnum[50];
int nodeSize = 60;
int validandoEsSiguienteTabla = 0;
int contador = 1;
int validandoMatrizLeida = 0;
int lendArchivo = 0;
char NodoEntrada;
char NodoSalida;

void llamarResultado(){
    char resultadoRutas[10000];
    char rutaOptima[100];
    snprintf(resultadoRutas, sizeof(resultadoRutas), " ");
    for (int i = 1; i < matrizSize; i++) {
            for (int j = 1; j < matrizSize; j++) {
                if (i == 1){
                    NodoEntrada = 'A';
                } else if(i == 2){
                    NodoEntrada = 'B';
                } else if(i == 3){
                    NodoEntrada = 'C';
                } else if(i == 4){
                    NodoEntrada = 'D';
                } else if(i == 5){
                    NodoEntrada = 'E';
                } else if(i == 6){
                    NodoEntrada = 'F';
                } else if(i == 7){
                    NodoEntrada = 'G';
                }
                if (j == 1){
                    NodoSalida = 'A';
                } else if(j == 2){
                    NodoSalida = 'B';
                } else if(j == 3){
                    NodoSalida = 'C';
                } else if(j == 4){
                    NodoSalida = 'D';
                } else if(j == 5){
                    NodoSalida = 'E';
                } else if(j == 6){
                    NodoSalida = 'F';
                } else if(j == 7){
                    NodoSalida = 'G';
                }
                const char *textoActual = gtk_label_get_text(GTK_LABEL(labelResultados));
                snprintf(resultadoRutas, sizeof(resultadoRutas), "%s", textoActual);
                printf("\nEntrada: %c\n", NodoEntrada);
                const gchar *valorPosicion = gtk_entry_get_text(GTK_ENTRY(matrizOptima[i][j]));
                int valorP = atoi(valorPosicion);
                if (valorP == 0){
                    printf("\nEntramosss");
                    snprintf(rutaOptima, sizeof(rutaOptima), "\n%c -> %c : ruta óptima: directa", NodoEntrada, NodoSalida);
                    strcat(resultadoRutas, rutaOptima);
                    gtk_label_set_text(GTK_LABEL(labelResultados), resultadoRutas);
                }
            }
        }
}

void algoritmoFloy() {
    int k = 0;
    int distancia_ik, distancia_kj, distancia_ij;
    char nuevo_valor[10];
    if (contador < matrizSize) {
        k = contador;
        for (int i = 1; i < matrizSize; i++) {
            for (int j = 1; j < matrizSize; j++) {
                if (indiceTablaActual == 1 || (i != indiceTablaActual && j != indiceTablaActual)) {
                    const gchar *matriz_ik = gtk_entry_get_text(GTK_ENTRY(matriz[i][k]));
                    const gchar *matriz_kj = gtk_entry_get_text(GTK_ENTRY(matriz[k][j]));
                    const gchar *matriz_ij = gtk_entry_get_text(GTK_ENTRY(matriz[i][j]));

                    if (strcmp(matriz_ik, "00") == 0) {
                        distancia_ik = INF;
                    } else {
                        distancia_ik = atoi(matriz_ik);
                    }
                    if (strcmp(matriz_kj, "00") == 0) {
                        distancia_kj = INF;
                    } else {
                        distancia_kj = atoi(matriz_kj);
                    }
                    if (strcmp(matriz_ij, "00") == 0) {
                        distancia_ij = INF;
                    } else {
                        distancia_ij = atoi(matriz_ij);
                    }
                    if (distancia_ij > distancia_ik + distancia_kj) {
                        if (INF <= distancia_kj + distancia_ik) {
                            snprintf(nuevo_valor, sizeof(nuevo_valor), "00");
                        } else {
                            snprintf(nuevo_valor, sizeof(nuevo_valor), "%d", distancia_ik + distancia_kj);
                        }

                        GdkRGBA color;
                        gdk_rgba_parse(&color, "green");
                        gtk_widget_override_color(matriz[i][j], GTK_STATE_FLAG_NORMAL, &color);
                        gtk_widget_override_color(matrizOptima[i][j], GTK_STATE_FLAG_NORMAL, &color);

                        char matriPcontadorCadena[20];
                        snprintf(matriPcontadorCadena, sizeof(matriPcontadorCadena), "%d", num);
                        gtk_entry_set_text(GTK_ENTRY(matriz[i][j]), nuevo_valor);
                        gtk_entry_set_text(GTK_ENTRY(matrizOptima[i][j]), matriPcontadorCadena);

                    }
                }
            }
        }
        char nuevot[50];
        snprintf(nuevot, sizeof(nuevot), "D(%d)", num);
        gtk_label_set_text(GTK_LABEL(tablaDtitulo), nuevot);
    }
    num++;
    indiceTablaActual++;
}

void algoritmoFloySiguienteTabla() {
    algoritmoFloy();
    contador++;
}

void algoritmoFloyCompleto() {
    for(int i = 1; i < matrizSize; i++){
        algoritmoFloy();
        contador++;
    }
    gtk_widget_set_sensitive(botonResultado, TRUE);
}

void escribirArchivo(){
    archivo = fopen("matriz.txt", "w");

    for (int i = 0; i < matrizSize; i++) {
        for (int j = 0; j < matrizSize; j++) {
            const char *valor = gtk_entry_get_text(GTK_ENTRY(matriz[i][j]));
            fprintf(archivo, "%s", valor);
            if (j < matrizSize - 1) {
                fprintf(archivo, "\t");
            }
        }
        fprintf(archivo, "\n");
    }
    fclose(archivo);
}

static gboolean dibujarGrafo(GtkWidget *widget, cairo_t *cr) {
    //printf("\nGrafo entramos\n");
    int tamNodo = 30;
    for (int i = 0; i < matrizSize - 1; i++) {
        double x = i * tamNodo + tamNodo / 0.5;
        double y = (i*2) * tamNodo + tamNodo / 0.5;

        cairo_arc(cr, x, y, nodeSize / 2.0, 1, 2 * G_PI);
        cairo_set_source_rgb(cr, 0, 255, 0);
        cairo_fill(cr);
    }
    return FALSE;
}

void generarMatrizP(GtkWidget *widget) {
        matrizboxP1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

        for (int i = 0; i < matrizSize; i++) {
            matrizboxP2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
            for (int j = 0; j < matrizSize; j++) {
                matrizOptima[i][j] = gtk_entry_new();
                gtk_entry_set_width_chars(GTK_ENTRY(matrizOptima[i][j]), 2);
                gtk_box_pack_start(GTK_BOX(matrizboxP2), matrizOptima[i][j], TRUE, TRUE, FALSE);

                if (i == 0 && j == 0) {
                    gtk_entry_set_text(GTK_ENTRY(matrizOptima[i][j]), "P");
                    gtk_widget_set_sensitive(matrizOptima[i][j], FALSE);
                }
                if (i == 0 && j > 0) {     // etiquetas de nodos
                    char letter = 'A' + (j - 1);
                    char letter_str[2] = {letter, '\0'};
                    gtk_entry_set_text(GTK_ENTRY(matrizOptima[i][j]), letter_str);
                    //gtk_widget_set_sensitive(matrizOptima[i][j], FALSE);
                } else if (i > 0 && j == 0) {
                    char letter = 'A' + (i - 1);
                    char letter_str[2] = {letter, '\0'};
                    gtk_entry_set_text(GTK_ENTRY(matrizOptima[i][j]), letter_str);
                    //gtk_widget_set_sensitive(matrizOptima[i][j], FALSE);
                }
                if (i != 0 && j != 0 ) {
                    gtk_entry_set_text(GTK_ENTRY(matrizOptima[i][j]), "0");
                    gtk_widget_set_sensitive(matrizOptima[i][j], FALSE);
                }
            }
            gtk_box_pack_start(GTK_BOX(matrizboxP1), matrizboxP2, TRUE, TRUE, FALSE);
        }
        gtk_box_pack_start(GTK_BOX(matrizP), matrizboxP1, TRUE, FALSE, 0);
        gtk_widget_show_all(vFloy);
}

void actualizarMatriz(GtkWidget *entry, gpointer user_data) {
    int *indices = (int*)user_data;
    int i = indices[0];
    int j = indices[1];
    const gchar *entrada = gtk_entry_get_text(GTK_ENTRY(entry));
    for (int i = 1; i < matrizSize; i++) {
        for (int j = 1; j < matrizSize; j++) {
            const gchar *entrada = gtk_entry_get_text(GTK_ENTRY(matriz[i][j]));
            //printf("%s ", entrada);
        }
        //printf("\n");
    }
    g_signal_connect(G_OBJECT(grafo), "draw", G_CALLBACK(dibujarGrafo), NULL);
    //llamar a actualizar grafo
}

void generarMatrizD(GtkWidget *widget, gpointer user_data) {
    matrizSize = 0;
    if(validandoMatrizLeida != 1){
        GtkWidget *cnodosIngresado = user_data;
        const gchar *nodosIngresados = gtk_entry_get_text(GTK_ENTRY(cnodosIngresado));
        matrizSize = atoi(nodosIngresados);
    } else {
        matrizSize = lendArchivo;
    }
    if (matrizSize <= 0  ||  matrizSize > 8) {
        matrizSize = 7;
    }
    matrizSize++;
    generarMatrizP(widget);
    matrizboxD1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    for (int i = 0; i < matrizSize; i++) {
        matrizboxD2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

        for (int j = 0; j < matrizSize; j++) {
            matriz[i][j] = gtk_entry_new();
            gtk_entry_set_width_chars(GTK_ENTRY(matriz[i][j]), 2);
            gtk_box_pack_start(GTK_BOX(matrizboxD2), matriz[i][j], TRUE, TRUE, FALSE);

            if (i == j) { // Llenar la diagonal con ceros
                if(i != 0 && j != 0){
                    gtk_entry_set_text(GTK_ENTRY(matriz[i][j]), "0");
                    gtk_widget_set_sensitive(matriz[i][j], FALSE); // Deshabilitar edición
                } else {
                    char nuevot[50];
                    snprintf(nuevot, sizeof(nuevot), "D(%d)", 0);
                    gtk_label_set_text(GTK_LABEL(tablaDtitulo), nuevot);
                    gtk_widget_set_sensitive(matriz[i][j], FALSE);
                }
            }
                // Mostrar etiquetas de nodos
            if (i == 0 && j > 0) {
                char letter = 'A' + (j - 1);
                char letter_str[2] = {letter, '\0'};
                gtk_entry_set_text(GTK_ENTRY(matriz[i][j]), letter_str);
            } else if (i > 0 && j == 0) {
                char letter = 'A' + (i - 1);
                char letter_str[2] = {letter, '\0'};
                gtk_entry_set_text(GTK_ENTRY(matriz[i][j]), letter_str);
            }

            int *indices = malloc(2 * sizeof(int));
            indices[0] = i;
            indices[1] = j;
            g_signal_connect(matriz[i][j], "changed", G_CALLBACK(actualizarMatriz), indices);
        }
        gtk_box_pack_start(GTK_BOX(matrizboxD1), matrizboxD2, TRUE, TRUE, FALSE);
    }
    validandoMatrizLeida = 0;
    gtk_box_pack_start(GTK_BOX(matrizD), matrizboxD1, TRUE, FALSE, 0);
    gtk_widget_show_all(vFloy);
}

void leerArchivo(GtkBuilder *builder, gpointer user_data) {
    gtk_label_set_text(GTK_LABEL(alertas), " ");
    lendArchivo = 0;
    GtkWidget *nombreArchivo = user_data;
    const gchar *nombreA = gtk_entry_get_text(GTK_ENTRY(nombreArchivo));
    printf("El nombre es: %s\n", nombreA);
    char caracter;
    archivo = fopen(nombreA, "r");

    fseek(archivo, 0, SEEK_END);
    long tam = ftell(archivo);

    if (archivo == NULL) {
        char alerta[100];
        snprintf(alerta, sizeof(alerta), "Error al leer el archivo: Archivo no existe ");
        gtk_label_set_text(GTK_LABEL(alertas), alerta);
        printf("Error al leer el archivo: Archivo no existe \n");
    } else {
         if(tam == 0) {
            char alerta[100];
            snprintf(alerta, sizeof(alerta), "Archivo vacio ");
            gtk_label_set_text(GTK_LABEL(alertas), alerta);
            printf("ERROR: Archivo vacio \n");
         } else {
            fseek(archivo, 0, SEEK_SET);
            char caracter;
            while ((caracter = fgetc(archivo)) != '\n') {
               if (caracter != ' ' && caracter != '\t'){
                    //printf("%c\n", caracter);
                    lendArchivo++;
               }
            }
            validandoMatrizLeida = 1; // seteamos para que iguale el lend a matrizsize caso abrir archivo

            gpointer data = GINT_TO_POINTER(lendArchivo);
            generarMatrizD(NULL, data);

            fseek(archivo, lendArchivo, SEEK_SET);
            for (int i = 1; i < matrizSize; i++) {
                for (int j = 1; j < matrizSize; j++) {
                    int entrada = 0;
                    while (!isdigit(caracter = fgetc(archivo))) {
                    }
                    if (caracter == '0'){
                        if((caracter = fgetc(archivo)) == '0'){
                            gtk_entry_set_text(GTK_ENTRY(matriz[i][j]), "00");
                        } else {
                             gtk_entry_set_text(GTK_ENTRY(matriz[i][j]), "0");
                        }
                    } else {
                    ungetc(caracter, archivo);
                    fscanf(archivo, "%d", &entrada);
                    gtk_entry_set_text(GTK_ENTRY(matriz[i][j]), g_strdup_printf("%d", entrada));
                    }
                }
            }
            fclose(archivo);
        }
    }
}

int main(int argc, char **argv) {
    GtkBuilder *builder;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "FloydWindow.glade", NULL);

    vFloy = GTK_WIDGET(gtk_builder_get_object(builder, "vFloy"));
    matrizD= GTK_WIDGET(gtk_builder_get_object(builder, "matrizD"));
    matrizP= GTK_WIDGET(gtk_builder_get_object(builder, "matrizP"));
    inputCNodos = GTK_WIDGET(gtk_builder_get_object(builder, "inputCNodos"));
    botonCNodos = GTK_WIDGET(gtk_builder_get_object(builder, "botonCNodos"));
    inputArchivoName = GTK_WIDGET(gtk_builder_get_object(builder, "inputArchivoName"));

    botonResultado = GTK_WIDGET(gtk_builder_get_object(builder, "botonResultado"));

    tablaDtitulo = GTK_WIDGET(gtk_builder_get_object(builder, "tablaDtitulo"));
    alertas = GTK_WIDGET(gtk_builder_get_object(builder, "alertas"));
    labelResultados = GTK_WIDGET(gtk_builder_get_object(builder, "labelResultados"));
    botonSiguienteTabla = GTK_WIDGET(gtk_builder_get_object(builder, "botonSiguienteTabla"));
    textoResultados = GTK_WIDGET(gtk_builder_get_object(builder, "textoResultados"));
    botoncalcularRuta = GTK_WIDGET(gtk_builder_get_object(builder, "botoncalcularRuta"));
    botonGuardarArchivo = GTK_WIDGET(gtk_builder_get_object(builder, "botonGuardarArchivo"));
    botonAbrirArchivo = GTK_WIDGET(gtk_builder_get_object(builder, "botonAbrirArchivo"));
    grafo = GTK_WIDGET(gtk_builder_get_object(builder, "grafo"));
    botonSalirFloyd = GTK_WIDGET(gtk_builder_get_object(builder, "botonSalirFloyd"));

    g_signal_connect(botonResultado, "clicked", G_CALLBACK(llamarResultado), NULL);
    g_signal_connect(botonSalirFloyd, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(botonCNodos, "clicked", G_CALLBACK(generarMatrizD), inputCNodos);
    g_signal_connect(botonAbrirArchivo, "clicked", G_CALLBACK(leerArchivo), inputArchivoName);
    g_signal_connect(botonGuardarArchivo, "clicked", G_CALLBACK(escribirArchivo), NULL);
    g_signal_connect(botonSiguienteTabla, "clicked", G_CALLBACK(algoritmoFloySiguienteTabla), NULL);
    g_signal_connect(botoncalcularRuta, "clicked", G_CALLBACK(algoritmoFloyCompleto), NULL);

    gtk_widget_set_sensitive(botonResultado, FALSE);
    gtk_widget_show_all(vFloy);

    gtk_main();
}
