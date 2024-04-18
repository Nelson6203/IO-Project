#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <glibconfig.h>
#include <cairo.h>
#include <ctype.h>
#define N 1024
#include <stdbool.h>

GtkWidget *vArbolBinario;
GtkWidget *vGrafo;

// Grafo
GtkWidget *labelgrafo;
GtkWidget *grafo;
//

GtkWidget *botonLlamarGrafo;
GtkWidget *comboBoxLlaves;
GtkWidget *botonGenerar;
GtkWidget *boxOpciones;
GtkWidget *matrizLlaves[N][N];
GtkWidget *matrizPrincipal[N][N];
GtkWidget *matrizResultado[N][N];
GtkWidget *matrizTablaR[N][N];

GtkWidget *boxMatrizLlaves;
GtkWidget *boxMatrizLlaves1;
GtkWidget *boxMatrizLlaves2;

GtkWidget *boxMatrizTablaR;
GtkWidget *boxMatrizTablaR1;
GtkWidget *boxMatrizTablaR2;

GtkWidget *boxMatrizResultado;
GtkWidget *boxMatrizResultado1;
GtkWidget *boxMatrizResultado2;

GtkWidget *botonGuardarArchivo;
GtkWidget *botonAbrirArchivo;
GtkWidget *botonSalir;
GtkWidget *boxBotonCalcular;
GtkWidget *botonCalcular;

GtkWidget *labelCantidad;
GtkWidget *labelCostoResultado;
GtkWidget *labelTablaA;
GtkWidget *labelTablaR;

int llaves = 0;
char nombres[N][50];
char nombresAux[N][50];
const gchar *pesos[N];
const gchar *pesosAux[N];
const gchar *probabilidades[N];
int matrixSize = 0;
float valorMenor = 0;


char llavesGuardar[50] = "";
FILE *archivo;

void obtenerDatos() {
    llaves = atoi(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(comboBoxLlaves)));
}

void llamarGrafo(){
    g_print("Entramos en grafo");
    gtk_widget_show_all(vGrafo);

}

void mostrarSolucion(){
    char solucion[9999] = "";
    strcat(solucion, "El costo óptimo es: ");
    char costosSolucion[9999] = "";
    float costoOpSolucion = atof(gtk_entry_get_text(GTK_ENTRY(matrizResultado[1][matrixSize-1])));
    snprintf(costosSolucion, sizeof(costosSolucion), "%.2f", costoOpSolucion);
    strcat(solucion, costosSolucion);
    gtk_label_set_text(GTK_LABEL(labelCostoResultado), solucion);
}

void ordenar() {
    int i, j;
    char temp[50];
    const gchar *temp_value;

    for (i = 0; i < llaves; i++) {
        strcpy(temp, nombres[i]);
        temp_value = pesos[i];
        j = i - 1;

        while (j >= 0 && strcmp(nombres[j], temp) > 0) {
            strcpy(nombres[j + 1], nombres[j]);
            pesos[j + 1] = pesos[j];
            j--;
        }

        strcpy(nombres[j + 1], temp);
        pesos[j + 1] = temp_value;

    }
    /*
    printf("\nArreglo ordenado: \n");
    for (int i = 0; i < llaves; i++) {
        printf("Key: %s, Value: %s\n", nombres[i], pesos[i]);
    }
    */
}

void escribirArchivo(){
    archivo = fopen("EjemA.txt", "w");
    sprintf(llavesGuardar, "%d", llaves);
    fprintf(archivo, "%s", llavesGuardar);
    fprintf(archivo, "\n");

    for (int i = 1; i < llaves+1; i++) {
        for (int j = 1; j < 3; j++) {
            const char *valor = gtk_entry_get_text(GTK_ENTRY(matrizLlaves[i][j]));
            fprintf(archivo, "%s", valor);
            if (j < llaves+1 - 1) {
                fprintf(archivo, "\t");
            }
        }
        fprintf(archivo, "\n");
    }
    fclose(archivo);
}

void leerArchivo(){
    int validandoEsCantLlaves  = 0;

    char cantLlavesLeido[50] = "";

    const gchar *direccionArchivo = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(botonAbrirArchivo));
    gchar *nombreArchivo = g_filename_display_basename(direccionArchivo);
    g_print("Nombre del archivo seleccionado: %s\n", nombreArchivo);
    archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Error al leer el archivo: Archivo no existe \n");
    } else {
        char caracter;
        int a = 0;
        while ((caracter = fgetc(archivo)) != '\n') {
                if(caracter != ' '){
                    cantLlavesLeido[a] = caracter;
                    a++;
                }
        }
        llaves = atoi(cantLlavesLeido);

        g_print("llaves leido:  %d\n", llaves);

        generarMatrizLlaves();

        char dato[50] = "";

        for (int i = 1; i < llaves+1; i++) {
            for (int j = 1; j < 3; j++) {
                fscanf(archivo, "%s", dato);
                //g_print("buenas : %s\n", dato);
                gtk_entry_set_text(GTK_ENTRY(matrizLlaves[i][j]), dato);
                }
            }
        fclose(archivo);
    }
}

void algoritmo(){

    int validandoTermino = 0;
    int validandoI = 0;
    float valoryes = 10;
    int a = 1;
    int b = 1;
    int j = 1;
    int h = 0;
    int d = 2;
    int val = 0;
    int valorP = 0;
    int diferenciaAux = 0;
    int e = 0;
    while(validandoTermino != 1){
        a = 1;
        diferenciaAux = 0;
        h = 0;
        e = 0;
        for (int i = 1; i < matrixSize; i++) {
            //printf("for principal %d\n : ", i);
            int validando = 0;
            j = a;
            for (j; j < matrixSize; j++) {
                char valores[50] = "";
                int valorPosicion = i;
                int diferencia = (j-i)+h;
                diferenciaAux = diferencia + e;
                if(diferencia == d){
                    //printf("fila i: %d , columna j;%d  diferencia es: %d\n", i,j, diferenciaAux+1);
                    for (int n = i; n < diferenciaAux+1; n++) {
                      //  printf("estamos en la fila: %d , estamos en la columna %d y nuestro j es: %d\n", i,n, j);
                        if(validando == 0){
                            //valorMenor = 0;
                            const gchar *entradaA = gtk_entry_get_text(GTK_ENTRY(matrizResultado[i][n]));
                            const gchar *entradaB = gtk_entry_get_text(GTK_ENTRY(matrizResultado[valorPosicion+1][j]));
                            //printf("a1: %s , b1: %s\n", entradaA,entradaB);
                            valorMenor = atof(entradaA) + atof(entradaB);
                            valorP = n;
                            //printf("valor menor eseeeeee: %f\n", valorMenor);
                            validando = 1;
                            valorPosicion++;
                        } else {
                            float valor = 0;
                            const gchar *entradaA = gtk_entry_get_text(GTK_ENTRY(matrizResultado[i][n]));
                            const gchar *entradaB = gtk_entry_get_text(GTK_ENTRY(matrizResultado[valorPosicion+1][j]));
                            //printf("a2: %s , b2: %s\n", entradaA,entradaB);
                            valor = atof(entradaA) + atof(entradaB);
                            //printf("valor menor es: %f , valor nuevo es: %f\n", valorMenor, valor);
                            if(valor < valorMenor){
                                //printf("encontramos mejor:\n");
                                valorP = n;
                              valorMenor = valor;
                            }
                            valorPosicion++;
                        }
                    }
                    //printf("estamos llegando?\n");
                    e++;
                    int k = i;
                    //printf("vamos a sumar chunces %d \n", k, i, j);
                    for (k; k < j; k++) {
                        //printf("k es: %d i es: %d j es: %d \n", k, i, j);
                        //printf("valor sumando es: %s\n", probabilidades[k-1]);
                        valorMenor += atof(probabilidades[k-1]);
                    }
                    //printf("\n");
                    char contador[50];
                    snprintf(contador, sizeof(contador), "%.2f", valorMenor);
                    gtk_entry_set_text(GTK_ENTRY(matrizResultado[i][j]), contador);
                    char contadorAux[50];
                    snprintf(contadorAux, sizeof(contadorAux), "%d", valorP);
                    gtk_entry_set_text(GTK_ENTRY(matrizTablaR[i][j]), contadorAux);
                }
            }
            a++;
        }
        d++;
        h++;
        //valoryes++;
        const gchar *entradafinal = gtk_entry_get_text(GTK_ENTRY(matrizResultado[1][matrixSize-1]));
        //printf("entradafinal es: %s\n", entradafinal);
        if(strcmp(entradafinal, "") != 0) {
            //printf("TERMINAMOS\n");
            validandoTermino =1;
        }
        val++;
        }
        mostrarSolucion();
}

void generarTablaR(){

    int numero = 1;
    int posicion = 0;
    int posicion1 = 0;
    boxMatrizTablaR1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    for (int i = 0; i < matrixSize; i++) {
        boxMatrizTablaR2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

        for (int j = 0; j < matrixSize; j++) {
            matrizTablaR[i][j] = gtk_entry_new();
            gtk_entry_set_width_chars(GTK_ENTRY(matrizTablaR[i][j]), 1);
            gtk_box_pack_start(GTK_BOX(boxMatrizTablaR2), matrizTablaR[i][j], TRUE, TRUE, FALSE);

            if(i == 0 && j == 0 ){
                gtk_widget_set_sensitive(matrizTablaR[i][j], FALSE);
            } else if(i == 0 && j == 1){
                gtk_entry_set_text(GTK_ENTRY(matrizTablaR[i][j]), "0");
                gtk_widget_set_sensitive(matrizTablaR[i][j], FALSE);
            } else if(j == 0 && i > 0) {
                char contador[50];
                snprintf(contador, sizeof(contador), "%d", numero);
                gtk_entry_set_text(GTK_ENTRY(matrizTablaR[i][j]), contador);
                gtk_widget_set_sensitive(matrizTablaR[i][j], FALSE);
                numero++;
            } else if (i == 0 && j > 1){
                gtk_entry_set_text(GTK_ENTRY(matrizTablaR[i][j]), nombres[posicion]);
                gtk_widget_set_sensitive(matrizTablaR[i][j], FALSE);
                posicion++;
            } else if (i > j) {
                gtk_entry_set_text(GTK_ENTRY(matrizTablaR[i][j]), "-");
                gtk_widget_set_sensitive(matrizTablaR[i][j], FALSE);

            } else if (i == j) {
                gtk_entry_set_text(GTK_ENTRY(matrizTablaR[i][j]), "0");
                gtk_widget_set_sensitive(matrizTablaR[i][j], FALSE);
            } else if (i > 0 && j == i+1) {
                char contadorPos[50];
                snprintf(contadorPos, sizeof(contadorPos), "%d", i);
                gtk_entry_set_text(GTK_ENTRY(matrizTablaR[i][j]), contadorPos);
                gtk_widget_set_sensitive(matrizTablaR[i][j], FALSE);
            }

        }

        gtk_box_pack_start(GTK_BOX(boxMatrizTablaR1), boxMatrizTablaR2, TRUE, TRUE, FALSE);
    }

    gtk_box_pack_start(GTK_BOX(boxMatrizTablaR), boxMatrizTablaR1, TRUE, FALSE, 0);
    gtk_widget_show_all(vArbolBinario);
    gtk_widget_hide(GTK_WIDGET(comboBoxLlaves));
    gtk_widget_hide(GTK_WIDGET(botonGenerar));
    gtk_widget_hide(GTK_WIDGET(labelCantidad));
}

void generarMatrizResultado() {
    ordenar();
    int i = 0;
    float sumaTotal = 0;
    while (i < llaves) {
        sumaTotal += atoi(pesos[i]);
    i++;
    }
    //printf("suma es':%f\n", sumaTotal);
    ordenar();
    for (int e = 0; e < llaves; e++) {
        float valor = 0;
        valor = atof(pesos[e])/sumaTotal;
        //printf("float tal es':%f\n", valor);
        char contador[50];
        snprintf(contador, sizeof(contador), "%.2f", valor);
        probabilidades[e] = strdup(contador);
    }

    /*
    printf("Contenido del arreglo 'probabilidades':\n");
    for (int k = 0; k < llaves; k++) {
            printf("%s ", probabilidades[k]);
    }
    printf("\n");
    */

    matrixSize = llaves + 2;
    int numero = 1;
    int posicion = 0;
    int posicion1 = 0;
    boxMatrizResultado1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    for (int i = 0; i < matrixSize; i++) {
        boxMatrizResultado2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

        for (int j = 0; j < matrixSize; j++) {
            matrizResultado[i][j] = gtk_entry_new();
            gtk_entry_set_width_chars(GTK_ENTRY(matrizResultado[i][j]), 1);
            gtk_box_pack_start(GTK_BOX(boxMatrizResultado2), matrizResultado[i][j], TRUE, TRUE, FALSE);

            if(i == 0 && j == 0 ){
                gtk_widget_set_sensitive(matrizResultado[i][j], FALSE);
            } else if(i == 0 && j == 1){
                gtk_entry_set_text(GTK_ENTRY(matrizResultado[i][j]), "0");
                gtk_widget_set_sensitive(matrizResultado[i][j], FALSE);
            } else if(j == 0 && i > 0) {
                char contador[50];
                snprintf(contador, sizeof(contador), "%d", numero);
                gtk_entry_set_text(GTK_ENTRY(matrizResultado[i][j]), contador);
                gtk_widget_set_sensitive(matrizResultado[i][j], FALSE);
                numero++;
            } else if (i == 0 && j > 1){
                gtk_entry_set_text(GTK_ENTRY(matrizResultado[i][j]), nombres[posicion]);
                gtk_widget_set_sensitive(matrizResultado[i][j], FALSE);
                posicion++;
            } else if (i > 0 && j == i+1) {
                gtk_entry_set_text(GTK_ENTRY(matrizResultado[i][j]), probabilidades[posicion1]);
                gtk_widget_set_sensitive(matrizResultado[i][j], FALSE);
                GdkRGBA color;
                gdk_rgba_parse(&color, "green");
                gtk_widget_override_color(matrizResultado[i][j], GTK_STATE_FLAG_NORMAL, &color);
                posicion1++;

            } else if (i > j) {
                gtk_entry_set_text(GTK_ENTRY(matrizResultado[i][j]), "-");
                gtk_widget_set_sensitive(matrizResultado[i][j], FALSE);

            } else if (i == j) {
                gtk_entry_set_text(GTK_ENTRY(matrizResultado[i][j]), "0");
                gtk_widget_set_sensitive(matrizResultado[i][j], FALSE);
                GdkRGBA color;
                gdk_rgba_parse(&color, "yellow");
                gtk_widget_override_color(matrizResultado[i][j], GTK_STATE_FLAG_NORMAL, &color);
            }
        }

        gtk_box_pack_start(GTK_BOX(boxMatrizResultado1), boxMatrizResultado2, TRUE, TRUE, FALSE);
    }

    gtk_box_pack_start(GTK_BOX(boxMatrizResultado), boxMatrizResultado1, TRUE, FALSE, 0);
    generarTablaR();
    algoritmo();
    gtk_widget_show_all(vArbolBinario);
    gtk_widget_hide(GTK_WIDGET(comboBoxLlaves));
    gtk_widget_hide(GTK_WIDGET(botonGenerar));
    gtk_widget_hide(GTK_WIDGET(labelCantidad));
}

void actualizarMatrizLlaves(GtkWidget *entry, gpointer user_data) {
    int *indices = (int*)user_data;
    int i = indices[0];
    int j = indices[1];
    const gchar *entrada = gtk_entry_get_text(GTK_ENTRY(entry));
    for (int i = 0; i < llaves+1; i++){
        for (int j = 0; j < 3; j++) {
            const gchar *entrada = gtk_entry_get_text(GTK_ENTRY(matrizLlaves[i][j]));
        }
    }
    for (int filaActual = 1; filaActual < llaves+1; filaActual++) {

        const gchar *nombre = gtk_entry_get_text(GTK_ENTRY(matrizLlaves[filaActual][1]));
        const gchar *peso = gtk_entry_get_text(GTK_ENTRY(matrizLlaves[filaActual][2]));

        strcpy(nombres[filaActual-1], nombre);
        pesos[filaActual-1] = peso;
        /*
        printf("Contenido del arreglo 'nombre':\n");
        for (int i = 0; i < llaves; i++) {
            printf("%s ", nombres[i]);
        }
        printf("\n");

        printf("Contenido del arreglo 'probabilidad':\n");
        for (int i = 0; i < llaves; i++) {
            printf("%s ", pesos[i]);
        }
        printf("\n");
        */
    }
}

void generarMatrizLlaves() {
    int numero = 1;
    boxMatrizLlaves1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    for (int i = 0; i < llaves+1; i++) {
        boxMatrizLlaves2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        for (int j = 0; j < 3; j++) {
            matrizLlaves[i][j] = gtk_entry_new();
            gtk_entry_set_width_chars(GTK_ENTRY(matrizLlaves[i][j]), 1);
            gtk_box_pack_start(GTK_BOX(boxMatrizLlaves2), matrizLlaves[i][j], TRUE, TRUE, FALSE);
            if (i == 0 && j == 0){
                gtk_entry_set_text(GTK_ENTRY(matrizLlaves[i][j]), "Llave");
                gtk_widget_set_sensitive(matrizLlaves[i][j], FALSE);
            }
            if (i == 0 && j == 1){
                gtk_entry_set_text(GTK_ENTRY(matrizLlaves[i][j]), "Nombre");
                gtk_widget_set_sensitive(matrizLlaves[i][j], FALSE);
            }
            if (i == 0 && j == 2){
                gtk_entry_set_text(GTK_ENTRY(matrizLlaves[i][j]), "Peso");
                gtk_widget_set_sensitive(matrizLlaves[i][j], FALSE);
            }
            if (j == 0 && i > 0){
                char contador[50];
                snprintf(contador, sizeof(contador), "%d", numero);
                gtk_entry_set_text(GTK_ENTRY(matrizLlaves[i][j]), contador);
                gtk_widget_set_sensitive(matrizLlaves[i][j], FALSE);
                numero++;
            }
            int *indices = malloc(2 * sizeof(int));
            indices[0] = i;
            indices[1] = j;
            g_signal_connect(matrizLlaves[i][j], "changed", G_CALLBACK(actualizarMatrizLlaves), indices);

        }
        gtk_box_pack_start(GTK_BOX(boxMatrizLlaves1), boxMatrizLlaves2, TRUE, TRUE, FALSE);
    }
    gtk_box_pack_start(GTK_BOX(boxMatrizLlaves), boxMatrizLlaves1, TRUE, FALSE, 0);
    gtk_widget_show_all(vArbolBinario);
    gtk_widget_hide(GTK_WIDGET(comboBoxLlaves));
    gtk_widget_hide(GTK_WIDGET(botonGenerar));
    gtk_widget_hide(GTK_WIDGET(labelCantidad));
    gtk_widget_hide(GTK_WIDGET(labelTablaA));
    gtk_widget_hide(GTK_WIDGET(labelTablaR));
    gtk_widget_hide(GTK_WIDGET(botonLlamarGrafo));

}

int main(int argc, char **argv) {
    GtkBuilder *builder;
    GtkBuilder *builderGrafo;
    gtk_init(&argc, &argv);

    GtkDrawingArea *drawing_area;

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "ArbolesBinarios.glade", NULL);

    builderGrafo = gtk_builder_new();
    gtk_builder_add_from_file(builderGrafo, "grafo.glade", NULL);

    vArbolBinario = GTK_WIDGET(gtk_builder_get_object(builder, "vArbolBinario"));

    comboBoxLlaves = GTK_WIDGET(gtk_builder_get_object(builder, "comboBoxLlaves"));
    botonGenerar = GTK_WIDGET(gtk_builder_get_object(builder, "botonGenerar"));
    boxOpciones = GTK_WIDGET(gtk_builder_get_object(builder, "boxOpciones"));
    boxMatrizLlaves = GTK_WIDGET(gtk_builder_get_object(builder, "boxMatrizLlaves"));
    botonGuardarArchivo = GTK_WIDGET(gtk_builder_get_object(builder, "botonGuardarArchivo"));
    botonAbrirArchivo = GTK_WIDGET(gtk_builder_get_object(builder, "botonAbrirArchivo"));
    botonSalir = GTK_WIDGET(gtk_builder_get_object(builder, "botonSalir"));
    boxMatrizResultado = GTK_WIDGET(gtk_builder_get_object(builder, "boxMatrizResultado"));
    boxMatrizTablaR = GTK_WIDGET(gtk_builder_get_object(builder, "boxMatrizTablaR"));
    labelCantidad = GTK_WIDGET(gtk_builder_get_object(builder, "labelCantidad"));
    boxBotonCalcular = GTK_WIDGET(gtk_builder_get_object(builder, "boxBotonCalcular"));
    botonCalcular = GTK_WIDGET(gtk_builder_get_object(builder, "botonCalcular"));
    labelCostoResultado = GTK_WIDGET(gtk_builder_get_object(builder, "labelCostoResultado"));
    labelTablaA = GTK_WIDGET(gtk_builder_get_object(builder, "labelTablaA"));
    labelTablaR = GTK_WIDGET(gtk_builder_get_object(builder, "labelTablaR"));
    botonLlamarGrafo = GTK_WIDGET(gtk_builder_get_object(builder, "botonLlamarGrafo"));

    g_signal_connect(botonGenerar, "clicked", G_CALLBACK(obtenerDatos), NULL);
    g_signal_connect(botonGenerar, "clicked", G_CALLBACK(generarMatrizLlaves), NULL);
    g_signal_connect(botonCalcular, "clicked", G_CALLBACK(generarMatrizResultado), NULL);
    g_signal_connect(botonAbrirArchivo, "file-set", G_CALLBACK(leerArchivo), NULL);
    g_signal_connect(botonSalir, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(botonGuardarArchivo, "clicked", G_CALLBACK(escribirArchivo), NULL);
    g_signal_connect(botonLlamarGrafo, "clicked", G_CALLBACK(llamarGrafo), NULL);


    // grafo
    vGrafo = GTK_WIDGET(gtk_builder_get_object(builderGrafo, "vGrafo"));
    drawing_area = GTK_DRAWING_AREA(gtk_builder_get_object(builderGrafo, "grafo"));
    grafo = GTK_WIDGET(gtk_builder_get_object(builder, "grafo"));

    labelgrafo = GTK_WIDGET(gtk_builder_get_object(builderGrafo, "labelgrafo"));

    gtk_widget_show_all(vArbolBinario);
    gtk_widget_hide(GTK_WIDGET(boxBotonCalcular));
    gtk_widget_hide(GTK_WIDGET(labelTablaA));
    gtk_widget_hide(GTK_WIDGET(labelTablaR));
    gtk_widget_hide(GTK_WIDGET(botonLlamarGrafo));

    gtk_main();
}
