#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <glibconfig.h>
#include <cairo.h>
#include <ctype.h>
#define N 1024
#include <stdbool.h>

GtkWidget *vKnapsack;
GtkWidget* matrizObj[N][N];
GtkWidget* matrizPrin[N][N];

GtkWidget *botonMatriz;
GtkWidget *boxPesos;
GtkWidget *boxCantidadObjetos;
GtkWidget *botonAbrirArchivo;
GtkWidget *botonGuardarArchivo;
GtkWidget *botonCalAlgoritmo;

GtkWidget *labelTituloPeso;
GtkWidget *labelSolucion;
GtkWidget *labelTituloCantidadO;
GtkWidget *labelProblemaMatematico;
GtkWidget *botonMostrarTablaResultado;
GtkWidget *botonSalir;

GtkWidget *pruebas;

// Matriz Objetos
GtkWidget *boxMatrizObjetos;
GtkWidget *boxMatrizO1;
GtkWidget *boxMatrizO2;

// Matriz Principal
GtkWidget *boxMatrizPrincipal;
GtkWidget *boxMatriz1;
GtkWidget *boxMatriz2;

int validandoMatrizLeida = 0;
const gchar *pesoSeleccionado = 0;
const gchar *objetosCSeleccionado = 0;
int matrizSizeObjetos = 0;
int matrizSizePrincipal = 0;

int cantidadObjetosUsados[N];
const gchar *nombres[N];
const gchar *costos[N];
const gchar *valores[N];
const gchar *cantidadDisponible[N];
const gchar *cantidadDisponibleTemp[N];
int *matrizResultado[N][N];
char variablesPesoCantObj[50];
char pesoLeido[50];
char cantObjLeida[50];
char dato[50];
int validandoSolucion = 0;

int matrizTemp[N][N];
int objetoSeleccionado[N][N];

const gchar *mObjetoObjetos = "Objetos";
const gchar *mObjetoCosto = "Peso";
const gchar *mObjetoValor = "Valor";
const gchar *mObjetoCantidad = "CantidadDisponible";

FILE *archivo;

void mostrarSolucion(){
    char solucion[9999] = "";
    strcat(solucion, "Solución:     ");
    printf("Cantidad de cada objeto utilizado en la solución máxima:\n");
    int contador = 1;
    for (int i = 0; i < matrizSizeObjetos; i++) {
        char solucionTemp[100];
        strcat(solucion, "X");
        char numContador[50] = "";
        snprintf(numContador, sizeof(numContador), "%d", contador);
        strcat(solucion, numContador);
        strcat(solucion, " = ");
        char numContador2[50] = "";
        snprintf(numContador2, sizeof(numContador2), "%d", cantidadObjetosUsados[i]);
        strcat(solucion, numContador2);
        strcat(solucion, "  ");
        contador++;
    }
    gtk_label_set_text(GTK_LABEL(labelSolucion), solucion);
}

void acomodarResultados(){
    int valor = 0;
    for (int i = 1; i <= atoi(objetosCSeleccionado); i++) {
        for (int j = 1; j <= atoi(pesoSeleccionado)+1; j++) {
            char numContador[50] = "";
            snprintf(numContador, sizeof(numContador), "%d", matrizTemp[i][j-1]);
            gtk_entry_set_text(GTK_ENTRY(matrizPrin[j][i]), numContador);
            if (objetoSeleccionado[i][j-1] == 1) {
                GdkRGBA color;
                gdk_rgba_parse(&color, "green");
                gtk_widget_override_color(matrizPrin[j][i], GTK_STATE_FLAG_NORMAL, &color);
            } else {
                GdkRGBA color;
                gdk_rgba_parse(&color, "red");
                gtk_widget_override_color(matrizPrin[j][i], GTK_STATE_FLAG_NORMAL, &color);
            }
            //printf("%d ", objetoSeleccionado[i][j-1]);
        }
        //printf("\n");
    }

}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int knapSack() {
    matrizTemp[atoi(objetosCSeleccionado)][atoi(pesoSeleccionado)];
    objetoSeleccionado[atoi(objetosCSeleccionado)][atoi(pesoSeleccionado)];
    int validandoDisponible = 0;
    for (int i = 0; i <= atoi(objetosCSeleccionado); i++) {
        for (int j = 0; j <= atoi(pesoSeleccionado); j++) {
            validandoDisponible = 0;
            if (i == 0) {
                matrizTemp[i][j] = 0;
                objetoSeleccionado[i][j] = 0;
            } else if (j >= atoi(costos[i-1])) {
                int maxCopias = atoi(cantidadDisponible[i-1]);
                int maxValor = matrizTemp[i-1][j];

                for (int k = 0; k <= maxCopias; k++) {
                    int valorRevisarNuevo = k * atoi(valores[i-1]) + matrizTemp[i-1][j-k*atoi(costos[i-1])];
                    maxValor = max(maxValor, valorRevisarNuevo);
                }
                matrizTemp[i][j] = maxValor;
                if (atoi(cantidadDisponible[i-1]) >= 1) {
                    objetoSeleccionado[i][j] = (maxValor > matrizTemp[i-1][j]) ? 1 : 1;
                } else {
                    objetoSeleccionado[i][j] = 0;
                     matrizTemp[i][j] = matrizTemp[i - 1][j];
                }
            } else {
                matrizTemp[i][j] = matrizTemp[i - 1][j];
                objetoSeleccionado[i][j] = 0;
            }
            printf("%d ", matrizTemp[i][j]);
        }

    printf("\n");
    }

    printf("\n");
    printf("REVISANDO EL NUEVO CANT del arreglo 'cantidadDisponible':\n");

    for (int i = 0; i < matrizSizeObjetos; i++) {
        printf("%s ", cantidadDisponible[i]);
    }
    int cantidadObjetosUsados[N];
    memset(cantidadObjetosUsados, 0, sizeof(cantidadObjetosUsados));
    int i = atoi(objetosCSeleccionado);
    int j = atoi(pesoSeleccionado);

    while (i > 0 && j > 0) {
        if (objetoSeleccionado[i][j]) {
            cantidadObjetosUsados[i - 1]++;
            j -= atoi(costos[i - 1]);
        } else {
            i--;
        }
    }
    acomodarResultados();
    mostrarSolucion();
}

void mostrarFormulaMatematica(){
    char maximizar[1000] = "";
    char sujeto[1000] = "";
    char formulaMatematica[10000] = "";
    char solucionOptima[10000];
    //char letter = 'A' + (j - 1);
    //char letter_str[2] = {letter, '\0'};
    int a = 0;
    int b = 0;
    int c = 0;
    strcat(formulaMatematica, "");
    strcat(maximizar, "MAX Z = ");
    strcat(sujeto, "\n");
    strcat(sujeto, "\n                          ");

    for (int i = 1; i < atoi(objetosCSeleccionado)+1; i++) {
        for (int j = 1; j < 4; j++) {
            const char *valor = gtk_entry_get_text(GTK_ENTRY(matrizObj[i][j]));
            if (valor != NULL){
                if (j == 1 && i > 0){
                    //printf("es peso: %s\n", valor);
                    strcat(sujeto, valor);
                    char letter = 'A' + (i - 1);
                    char letter_str[2] = {letter, '\0'};
                    strcat(sujeto, "X");
                    strcat(sujeto, letter_str);
                    if(i != atoi(objetosCSeleccionado)){
                        strcat(sujeto, " + ");
                    }
                    letter++;
                    // aqui va sujeto
                } else if (j == 2 && i > 0){
                    //printf("es valor: %s\n", valor);
                    strcat(maximizar, valor);
                    char letter = 'A' + (i - 1);
                    char letter_str[2] = {letter, '\0'};
                    strcat(maximizar, "X");
                    strcat(maximizar, letter_str);
                    if(i != atoi(objetosCSeleccionado)){
                        strcat(maximizar, " + ");
                    }
                    letter++;
                }
            }
        }
    }
    strcat(formulaMatematica, maximizar);
    strcat(formulaMatematica, sujeto);
    strcat(formulaMatematica, " ≤ ");
    strcat(formulaMatematica, pesoSeleccionado);
    //printf("%s\n", formulaMatematica);
    gtk_label_set_text(GTK_LABEL(labelProblemaMatematico), formulaMatematica);
}

void escribirArchivo(){
    archivo = fopen("Ejem.txt", "w");
    fprintf(archivo, "%s", pesoLeido);
    fprintf(archivo, " ");
    fprintf(archivo, "%s", cantObjLeida);
    fprintf(archivo, "\n");
    for (int i = 1; i < atoi(cantObjLeida)+1; i++) {
        for (int j = 0; j < 4; j++) {
            const char *valor = gtk_entry_get_text(GTK_ENTRY(matrizObj[i][j]));
            fprintf(archivo, "%s", valor);
            if (j < (atoi(cantObjLeida)+1) - 1) {
                fprintf(archivo, "\t");
            }
        }
        fprintf(archivo, "\n");
    }
    fclose(archivo);
}

void obtenerDatos() {
    pesoSeleccionado = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(boxPesos));
    g_print("Valor seleccionado en comboPesos: %s\n", pesoSeleccionado);
    objetosCSeleccionado = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(boxCantidadObjetos));
    g_print("Valor seleccionado en comboPesos: %s\n", objetosCSeleccionado);
}

void actualizarMatrizObjeto(GtkWidget *entry, gpointer user_data) {
    int *indices = (int*)user_data;
    int i = indices[0];
    int j = indices[1];
    const gchar *entrada = gtk_entry_get_text(GTK_ENTRY(entry));
    for (int i = 1; i < matrizSizeObjetos+1; i++) {
        for (int j = 0; j < 4; j++) {
            const gchar *entrada = gtk_entry_get_text(GTK_ENTRY(matrizObj[i][j]));
        }
    }
    for (int filaActual = 1; filaActual < matrizSizeObjetos+1; filaActual++) {
        const gchar *nombre = gtk_entry_get_text(GTK_ENTRY(matrizObj[filaActual][0]));
        const gchar *costo = gtk_entry_get_text(GTK_ENTRY(matrizObj[filaActual][1]));
        const gchar *valor = gtk_entry_get_text(GTK_ENTRY(matrizObj[filaActual][2]));
        const gchar *cantidad = gtk_entry_get_text(GTK_ENTRY(matrizObj[filaActual][3]));

        nombres[filaActual-1] = nombre;
        costos[filaActual-1] = costo;
        valores[filaActual-1] = valor;
        cantidadDisponible[filaActual-1] = cantidad;
        cantidadDisponibleTemp[filaActual-1] = cantidad;
    /*
        printf("Contenido del arreglo 'nombres':\n");
        for (int i = 0; i < matrizSizeObjetos; i++) {
            printf("%s ", nombres[i]);
        }
        printf("\n");

        printf("Contenido del arreglo 'costos':\n");
        for (int i = 0; i < matrizSizeObjetos; i++) {
            printf("%s ", costos[i]);
        }
        printf("\n");

        printf("Contenido del arreglo 'valores':\n");
        for (int i = 0; i < matrizSizeObjetos; i++) {
            printf("%s ", valores[i]);
        }
        printf("\n");

        printf("Contenido del arreglo 'cantidadDisponible':\n");
        for (int i = 0; i < matrizSizeObjetos; i++) {
            printf("%s ", cantidadDisponible[i]);
        }
        printf("\n");
     */
    }
    mostrarFormulaMatematica();

}

void actualizarMatrizPrincipal(GtkWidget *entry, gpointer user_data) {
    int *indices = (int*)user_data;
    int j = indices[1];
    const gchar *entrada = gtk_entry_get_text(GTK_ENTRY(entry));
    for (int i = 1; i < matrizSizePrincipal + 1; i++) {
        gtk_entry_set_text(GTK_ENTRY(matrizPrin[i][j]), entrada);
    }
}

void generarMatrizPrincipal(GtkWidget *widget) {
    boxMatriz1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    int contador = 0;
    for (int i = 0; i < atoi(pesoSeleccionado) + 2; i++) {
        GtkWidget *boxMatriz2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

        for (int j = 0; j < atoi(objetosCSeleccionado) + 1; j++) {
            GtkWidget *entrada = gtk_entry_new();
            gtk_entry_set_width_chars(GTK_ENTRY(entrada), 2);
            gtk_box_pack_start(GTK_BOX(boxMatriz2), entrada, TRUE, TRUE, FALSE);

            if (i == 0 && j == 0) {
                gtk_entry_set_text(GTK_ENTRY(entrada), "peso");
                gtk_widget_set_sensitive(entrada, FALSE);
            }
            if (j == 0 && i > 0){
                char numContador[50];
                snprintf(numContador, sizeof(numContador), "%d", contador);
                gtk_entry_set_text(GTK_ENTRY(entrada), numContador);
                gtk_widget_set_sensitive(entrada, FALSE);
                contador++;
            }
            if (i == 0 && j < atoi(objetosCSeleccionado) + 1 && j != 0){
                gtk_entry_set_text(GTK_ENTRY(entrada), gtk_entry_get_text(GTK_ENTRY(matrizObj[j][i])));
                gtk_widget_set_sensitive(entrada, FALSE);
            }
            matrizPrin[i][j] = entrada;
        }
        gtk_box_pack_start(GTK_BOX(boxMatriz1), boxMatriz2, TRUE, TRUE, FALSE);
    }
    knapSack();
    gtk_box_pack_start(GTK_BOX(boxMatrizPrincipal), boxMatriz1, TRUE, FALSE, 0);
    gtk_widget_hide(GTK_WIDGET(boxPesos));
    gtk_widget_hide(GTK_WIDGET(boxCantidadObjetos));
    gtk_widget_hide(GTK_WIDGET(botonMatriz));
    gtk_widget_hide(GTK_WIDGET(labelTituloPeso));
    gtk_widget_hide(GTK_WIDGET(labelTituloCantidadO));
    gtk_widget_show_all(vKnapsack);
}

void generarMatrizObjetos(GtkWidget *widget) {

    if(validandoMatrizLeida != 1){
        matrizSizeObjetos = atoi(objetosCSeleccionado);
    } else {
        matrizSizeObjetos = atoi(cantObjLeida);
    }
    boxMatrizO1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    //generarMatrizPrincipal(widget); despues vamos a buscar la manera de ponerla cuando le demos calcular algoritmo y la llenamos automaticamente
    for (int i = 0; i < matrizSizeObjetos+1; i++) {
        boxMatrizO2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        for (int j = 0; j < 4; j++) {
            matrizObj[i][j] = gtk_entry_new();
            gtk_entry_set_width_chars(GTK_ENTRY(matrizObj[i][j]), 2);
            gtk_box_pack_start(GTK_BOX(boxMatrizO2), matrizObj[i][j], TRUE, TRUE, FALSE);

            if (i == 0 && j == 0) {
                gtk_entry_set_text(GTK_ENTRY(matrizObj[i][j]), mObjetoObjetos);
                gtk_widget_set_sensitive(matrizObj[i][j], FALSE); // Deshabilitar edición
            }
            if (i == 0 && j == 1) {
                gtk_entry_set_text(GTK_ENTRY(matrizObj[i][j]), mObjetoCosto);
                gtk_widget_set_sensitive(matrizObj[i][j], FALSE); // Deshabilitar edición
            }
            if (i == 0 && j == 2) {
                gtk_entry_set_text(GTK_ENTRY(matrizObj[i][j]), mObjetoValor);
                gtk_widget_set_sensitive(matrizObj[i][j], FALSE); // Deshabilitar edición
            }
            if (i == 0 && j == 3) {
                gtk_entry_set_text(GTK_ENTRY(matrizObj[i][j]), mObjetoCantidad);
                gtk_widget_set_sensitive(matrizObj[i][j], FALSE);
            }
            int *indices = malloc(2 * sizeof(int));
            indices[0] = i;
            indices[1] = j;
            g_signal_connect(matrizObj[i][j], "changed", G_CALLBACK(actualizarMatrizObjeto), indices);
        }
        gtk_box_pack_start(GTK_BOX(boxMatrizO1), boxMatrizO2, TRUE, TRUE, FALSE);
    }
    //validandoMatrizLeida = 0;
    gtk_box_pack_start(GTK_BOX(boxMatrizObjetos), boxMatrizO1, TRUE, FALSE, 0);
     /*
     printf("Contenido del arreglo 'nombres':\n");

        for (int i = 0; i < matrizSizeObjetos; i++) {
            printf("%s ", nombres[i]);
        }
        printf("\n");

        printf("Contenido del arreglo 'costos':\n");
        for (int i = 0; i < matrizSizeObjetos; i++) {
            printf("%s ", costos[i]);
        }
        printf("\n");

        printf("Contenido del arreglo 'valores':\n");
        for (int i = 0; i < matrizSizeObjetos; i++) {
            printf("%s ", valores[i]);
        }
        printf("\n");

        printf("Contenido del arreglo 'cantidadDisponible':\n");
        for (int i = 0; i < matrizSizeObjetos; i++) {
            printf("%s ", cantidadDisponible[i]);
        }

        printf("\n");
    */
    gtk_widget_set_sensitive(botonCalAlgoritmo, TRUE);
    mostrarFormulaMatematica();
    gtk_widget_show_all(vKnapsack);
}

void leerArchivo(GtkBuilder *builder) {
    int validandoEsPeso = 0;
    int validandoEsCantObj = 0;
    const gchar *direccionArchivo = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(botonAbrirArchivo));
    gchar *nombreArchivo = g_filename_display_basename(direccionArchivo);
    g_print("Nombre del archivo seleccionado: %s\n", nombreArchivo);
    archivo = fopen(nombreArchivo, "r");
    fseek(archivo, 0, SEEK_END);
    long tam = ftell(archivo);
    if (archivo == NULL) {
        printf("Error al leer el archivo: Archivo no existe \n");
    } else {
        if(tam == 0) {
            printf("ERROR: Archivo vacio \n");
         } else {
            fseek(archivo, 0, SEEK_SET);
            char caracter;
            int e = 0;
            int j = 0;
            // leemos el peso, cant obj
            while ((caracter = fgetc(archivo)) != '\n') {
               if (caracter == ' '){
                    validandoEsPeso = 1;
               }
               if(validandoEsPeso == 0) {
                pesoLeido[e] = caracter;
                e++;
               }
               if(validandoEsPeso == 1) {
                  if (caracter != ' '){
                    cantObjLeida[j] = caracter;
                   j++;
                  }
               }
            }
            validandoMatrizLeida = 1;
            pesoSeleccionado = pesoLeido;
            objetosCSeleccionado = cantObjLeida;
            //g_print("Valor leido peso: %s\n", pesoLeido);
            //g_print("Valor leido cant objs: %s\n", cantObjLeida);
            generarMatrizObjetos(NULL);
            int r = 0;
            for (int i = 1; i < atoi(cantObjLeida)+1; i++) {
                for (int j = 0; j < 4; j++) {
                    fscanf(archivo, "%s", dato);
                    gtk_entry_set_text(GTK_ENTRY(matrizObj[i][j]), dato);
                }
            }
        }
        fclose(archivo);
        gtk_widget_set_sensitive(botonCalAlgoritmo, TRUE);
        gtk_widget_hide(GTK_WIDGET(boxPesos));
        gtk_widget_hide(GTK_WIDGET(boxCantidadObjetos));
        gtk_widget_hide(GTK_WIDGET(botonMatriz));
        gtk_widget_hide(GTK_WIDGET(labelTituloPeso));
        gtk_widget_hide(GTK_WIDGET(labelTituloCantidadO));
        mostrarFormulaMatematica();
    }
}

int main(int argc, char **argv) {
    GtkBuilder *builder;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "KnapsackWIndow.glade", NULL);

    vKnapsack = GTK_WIDGET(gtk_builder_get_object(builder, "vKnapsack"));
    botonMatriz = GTK_WIDGET(gtk_builder_get_object(builder, "botonMatriz"));
    boxPesos = GTK_WIDGET(gtk_builder_get_object(builder, "boxPesos"));
    boxCantidadObjetos = GTK_WIDGET(gtk_builder_get_object(builder, "boxCantidadObjetos"));
    boxMatrizObjetos = GTK_WIDGET(gtk_builder_get_object(builder, "boxMatrizObjetos"));
    boxMatrizPrincipal = GTK_WIDGET(gtk_builder_get_object(builder, "boxMatrizPrincipal"));
    pruebas = GTK_WIDGET(gtk_builder_get_object(builder, "pruebas"));
    botonAbrirArchivo = GTK_WIDGET(gtk_builder_get_object(builder, "botonAbrirArchivo"));
    labelTituloPeso = GTK_WIDGET(gtk_builder_get_object(builder, "labelTituloPeso"));
    labelTituloCantidadO = GTK_WIDGET(gtk_builder_get_object(builder, "labelTituloCantidadO"));
    botonGuardarArchivo = GTK_WIDGET(gtk_builder_get_object(builder, "botonGuardarArchivo"));
    botonCalAlgoritmo = GTK_WIDGET(gtk_builder_get_object(builder, "botonCalAlgoritmo"));
    labelProblemaMatematico = GTK_WIDGET(gtk_builder_get_object(builder, "labelProblemaMatematico"));
    botonMostrarTablaResultado = GTK_WIDGET(gtk_builder_get_object(builder, "botonMostrarTablaResultado"));
    labelSolucion = GTK_WIDGET(gtk_builder_get_object(builder, "labelSolucion"));
    botonSalir = GTK_WIDGET(gtk_builder_get_object(builder, "botonSalir"));

    g_signal_connect(botonMatriz, "clicked", G_CALLBACK(obtenerDatos), NULL);
    g_signal_connect(botonAbrirArchivo, "file-set", G_CALLBACK(leerArchivo), NULL);
    g_signal_connect(botonGuardarArchivo, "clicked", G_CALLBACK(escribirArchivo), NULL);
    g_signal_connect(botonMatriz, "clicked", G_CALLBACK(generarMatrizObjetos), NULL);
    g_signal_connect(botonCalAlgoritmo, "clicked", G_CALLBACK(generarMatrizPrincipal), NULL);
    g_signal_connect(botonMostrarTablaResultado, "clicked", G_CALLBACK(mostrarFormulaMatematica), NULL);
    g_signal_connect(botonSalir, "clicked", G_CALLBACK(gtk_main_quit), NULL);


    gtk_widget_set_sensitive(botonCalAlgoritmo, FALSE);

    gtk_widget_show_all(vKnapsack);
    gtk_main();
}
