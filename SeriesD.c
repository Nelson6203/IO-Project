#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <glibconfig.h>
#include <cairo.h>
#include <ctype.h>
#define N 1024
#include <stdbool.h>


GtkWidget *vSeriesDeportivas;
GtkWidget *matrizJuegos[N][N];
GtkWidget *matrizPrincipal[N][N];
GtkWidget *matrizLugares[N][N];
GtkWidget *comboBoxMaxJuegos;
GtkWidget *comboBoxProbabilidadACasa;
GtkWidget *comboBoxtProbabilidadAVisita;
GtkWidget *botonGenerar;
GtkWidget *labelCantidadJuegos;
GtkWidget *labelProbabilidadCA;
GtkWidget *labelProbabilidadVA;
GtkWidget *botonCalcular;
GtkWidget *boxElegirOpciones;
GtkWidget *LabelMejorDe;

const gchar *juegos[21];
const gchar *tempLugares[N];
GtkWidget *boxOrdenJuegos;
GtkWidget *boxOrden1;
GtkWidget *boxOrden2;

GtkWidget *boxMatrizLugares;
GtkWidget *boxLugares1;
GtkWidget *boxLugares2;

GtkWidget *boxMatrizPrincipal;
GtkWidget *boxPrincipal1;
GtkWidget *boxPrincipal2;

GtkWidget *labelSolucion;
GtkWidget *botonSalir;

GtkWidget *inputCantMejores;


GtkWidget *botonGuardarArchivo;
GtkWidget *botonAbrirArchivo;


int cantidadMaxJuegosSeleccionada = 0;
float probabilidaACasa = 0;
float probabilidadBCasa = 0;
float probabilidadAVisita = 0;
float probabilidadBVisita = 0;
int matrizSizeJuegos = 0;
int validandoMatrizLeida = 0;
int cantidadLeidaJuegos = 0;
int cantMaximaGanar = 0;

int mejorDe = 0;
const gchar *mejorDeCant = "";

FILE *archivo;
char cantMaxJuegosGuardar[50];
int valorProbACGuardar = 0;
int valorProbAVGuardar = 0;
int valorProbBCGuardar = 0;
int valorProbBVGuardar = 0;
char cantProbACJuegosGuardar[50];
char GvalorProbACGuardar[50] = "";
char GvalorProbAVGuardar[50] = "";
char GvalorMejorDe[50] = "";

void obtenerDatos() {
    cantidadMaxJuegosSeleccionada = atoi(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(comboBoxMaxJuegos)));
    g_print("Valor seleccionado max juegos: %d\n", cantidadMaxJuegosSeleccionada);
    probabilidaACasa = ((atoi(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(comboBoxProbabilidadACasa))))/100.0);
    probabilidadAVisita = ((atoi(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(comboBoxtProbabilidadAVisita))))/100.0);
    probabilidadBCasa = 1-probabilidadAVisita;
    probabilidadBVisita = 1-probabilidaACasa;

    // Para que guardemos archivo
    valorProbACGuardar = atoi(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(comboBoxProbabilidadACasa)));
    valorProbAVGuardar = atoi(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(comboBoxtProbabilidadAVisita)));
    //valorProbBCGuardar = 100-valorProbAVGuardar;
    //valorProbBVGuardar = 100-valorProbACGuardar;

    g_print("Valor Casa A: %f\n", probabilidaACasa);
    g_print("Valor Casa B: %f\n", probabilidadBCasa);
    g_print("Valor Visita A %f\n", probabilidadAVisita);
    g_print("Valor Visita B: %f\n", probabilidadBVisita);
}

void obttenerDatosMejorDe() {
    g_print("Estamos viendo cosas");
    const gchar *cantidadJuegosGanar = gtk_entry_get_text(GTK_ENTRY(inputCantMejores));
    cantMaximaGanar = atoi(cantidadJuegosGanar);
    g_print("Valor de inputCantMejoresaaaaaAA: %d\n", cantMaximaGanar);
}

void generarMatrizLugares(GtkWidget *widget) {
    int numero = 0;
    int numero1 = 0;

    boxLugares1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    for (int i = 0; i < cantMaximaGanar+2; i++) {
        boxLugares2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        for (int j = 0; j < cantMaximaGanar+2; j++) {
            matrizLugares[i][j] = gtk_entry_new();
            gtk_entry_set_width_chars(GTK_ENTRY(matrizLugares[i][j]), 2);
            gtk_box_pack_start(GTK_BOX(boxLugares2), matrizLugares[i][j], TRUE, TRUE, FALSE);
            if (i == 0 && j == 0){
                gtk_entry_set_text(GTK_ENTRY(matrizLugares[i][j]), "Lugares");
                gtk_widget_set_sensitive(matrizLugares[i][j], FALSE);
            }
            if (i == 0 && j > 0){
                char contador1[50];
                snprintf(contador1, sizeof(contador1), "%d", numero1);
                gtk_entry_set_text(GTK_ENTRY(matrizLugares[i][j]), contador1);
                gtk_widget_set_sensitive(matrizLugares[i][j], FALSE);
                numero1++;
            }
            if (j == 0 && i > 0){
                char contador[50];
                snprintf(contador, sizeof(contador), "%d", numero);
                gtk_entry_set_text(GTK_ENTRY(matrizLugares[i][j]), contador);
                gtk_widget_set_sensitive(matrizLugares[i][j], FALSE);
                numero++;
            }


            int *indices = malloc(2 * sizeof(int));
            indices[0] = i;
            indices[1] = j;
            //g_signal_connect(matrizJuegos[i][j], "changed", G_CALLBACK(actualizarMatrizJuegos), indices);
        }
        gtk_box_pack_start(GTK_BOX(boxLugares1), boxLugares2, TRUE, TRUE, FALSE);
    }
    gtk_box_pack_start(GTK_BOX(boxMatrizLugares), boxLugares1, TRUE, FALSE, 0);
    gtk_widget_show_all(vSeriesDeportivas);
    gtk_widget_hide(GTK_WIDGET(boxElegirOpciones));
    gtk_widget_hide(GTK_WIDGET(labelCantidadJuegos));
    gtk_widget_hide(GTK_WIDGET(labelProbabilidadCA));
    gtk_widget_hide(GTK_WIDGET(labelProbabilidadVA));
    gtk_widget_hide(GTK_WIDGET(botonCalcular));
}

void generarMatrizP(GtkWidget *widget) {
    int numero = 0;
    int numero1 = 0;
    boxPrincipal1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    for (int i = 0; i < cantMaximaGanar+2; i++) {
        boxPrincipal2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        for (int j = 0; j < cantMaximaGanar+2; j++) {
            matrizPrincipal[i][j] = gtk_entry_new();
            gtk_entry_set_width_chars(GTK_ENTRY(matrizPrincipal[i][j]), 2);
            gtk_box_pack_start(GTK_BOX(boxPrincipal2), matrizPrincipal[i][j], TRUE, TRUE, FALSE);

            if (i == 0 && j == 0){
                gtk_entry_set_text(GTK_ENTRY(matrizPrincipal[i][j]), "Partidos");
                gtk_widget_set_sensitive(matrizPrincipal[i][j], FALSE);
            }
            if (j == 0 && i > 0){
                char contador[50];
                snprintf(contador, sizeof(contador), "%d", numero);
                gtk_entry_set_text(GTK_ENTRY(matrizPrincipal[i][j]), contador);
                gtk_widget_set_sensitive(matrizPrincipal[i][j], FALSE);
                numero++;
            }
            if (i == 0 && j > 0){
                char contador1[50];
                snprintf(contador1, sizeof(contador1), "%d", numero1);
                gtk_entry_set_text(GTK_ENTRY(matrizPrincipal[i][j]), contador1);
                gtk_widget_set_sensitive(matrizPrincipal[i][j], FALSE);
                numero1++;
            }


            int *indices = malloc(2 * sizeof(int));
            indices[0] = i;
            indices[1] = j;
            //g_signal_connect(matrizJuegos[i][j], "changed", G_CALLBACK(actualizarMatrizJuegos), indices);
        }
        gtk_box_pack_start(GTK_BOX(boxPrincipal1), boxPrincipal2, TRUE, TRUE, FALSE);
    }
    validandoMatrizLeida = 0;
    gtk_box_pack_start(GTK_BOX(boxMatrizPrincipal), boxPrincipal1, TRUE, FALSE, 0);
    gtk_widget_show_all(vSeriesDeportivas);
    gtk_widget_hide(GTK_WIDGET(boxElegirOpciones));
    gtk_widget_hide(GTK_WIDGET(labelCantidadJuegos));
    gtk_widget_hide(GTK_WIDGET(labelProbabilidadCA));
    gtk_widget_hide(GTK_WIDGET(labelProbabilidadVA));
    gtk_widget_hide(GTK_WIDGET(botonCalcular));
}

void actualizarMatrizJuegos(GtkWidget *entry, gpointer user_data) {
    int *indices = (int*)user_data;
    int i = indices[0];
    int j = indices[1];
    const gchar *entrada = gtk_entry_get_text(GTK_ENTRY(entry));
    for (int i = 1; i < matrizSizeJuegos+1; i++) {
        for (int j = 0; j < 2; j++) {
            const gchar *entrada = gtk_entry_get_text(GTK_ENTRY(matrizJuegos[i][j]));
        }
    }
    for (int filaActual = 1; filaActual < matrizSizeJuegos+1; filaActual++) {
        const gchar *juego = gtk_entry_get_text(GTK_ENTRY(matrizJuegos[filaActual][1]));
        juegos[filaActual-1] = juego;

        /*
        printf("Contenido del arreglo 'juegos':\n");
        for (int i = 0; i < matrizSizeJuegos; i++) {
            printf("%s ", juegos[i]);
        }
        printf("\n");
    */

    }
    int contadorTemp = 0;
    for (i = matrizSizeJuegos-1; i >= 0; i--) {
        tempLugares[contadorTemp] = juegos[i];
        contadorTemp++;
    }

    printf("Contenido del nuevo arreglo temp de juegos 'juegos':\n");
        for (int i = 0; i < matrizSizeJuegos; i++) {
            printf("%s ", tempLugares[i]);
        }
        printf("\n");

}

void generarMatrizCV(GtkWidget *widget) {
    int numero = 1;
    matrizSizeJuegos = cantidadMaxJuegosSeleccionada;
    boxOrden1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    for (int i = 0; i < matrizSizeJuegos+1; i++) {
        boxOrden2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        for (int j = 0; j < 2; j++) {
            matrizJuegos[i][j] = gtk_entry_new();
            gtk_entry_set_width_chars(GTK_ENTRY(matrizJuegos[i][j]), 1);
            gtk_box_pack_start(GTK_BOX(boxOrden2), matrizJuegos[i][j], TRUE, TRUE, FALSE);

            if (i == 0 && j == 0){
                gtk_entry_set_text(GTK_ENTRY(matrizJuegos[i][j]), "Numero");
                gtk_widget_set_sensitive(matrizJuegos[i][j], FALSE);
            }
            if (i == 0 && j == 1){
                gtk_entry_set_text(GTK_ENTRY(matrizJuegos[i][j]), "Juego");
                gtk_widget_set_sensitive(matrizJuegos[i][j], FALSE);
            }
            if (j == 0 && i > 0){
                char contador[50];
                snprintf(contador, sizeof(contador), "%d", numero);
                gtk_entry_set_text(GTK_ENTRY(matrizJuegos[i][j]), contador);
                gtk_widget_set_sensitive(matrizJuegos[i][j], FALSE);
                numero++;
            }
            int *indices = malloc(2 * sizeof(int));
            indices[0] = i;
            indices[1] = j;
            g_signal_connect(matrizJuegos[i][j], "changed", G_CALLBACK(actualizarMatrizJuegos), indices);
        }
        gtk_box_pack_start(GTK_BOX(boxOrden1), boxOrden2, TRUE, TRUE, FALSE);
    }
    validandoMatrizLeida = 0;
    gtk_box_pack_start(GTK_BOX(boxOrdenJuegos), boxOrden1, TRUE, FALSE, 0);
    gtk_widget_hide(GTK_WIDGET(botonCalcular));
    gtk_widget_show_all(vSeriesDeportivas);
    gtk_widget_hide(GTK_WIDGET(boxElegirOpciones));
    gtk_widget_hide(GTK_WIDGET(labelCantidadJuegos));
    gtk_widget_hide(GTK_WIDGET(labelProbabilidadCA));
    gtk_widget_hide(GTK_WIDGET(labelProbabilidadVA));
}

int algoritmoSD() {
    generarMatrizP(NULL);
    generarMatrizLugares(NULL);

    g_print("Estamos EN ALGORITMO\n");
    int contador2 = 0;
    int posicion = 0;
    posicion  = 0;
    int validando = 0;
    for (int i = 1; i < cantMaximaGanar+2; i++) {
        boxPrincipal2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        for (int j = 1; j < cantMaximaGanar+2; j++) {
            if(i == 1 && j > 0){
                gtk_entry_set_text(GTK_ENTRY(matrizPrincipal[i][j]), "1");
                validando = 0;
            } else if(j == 1 && i > 1) {
                gtk_entry_set_text(GTK_ENTRY(matrizPrincipal[i][j]), "0");
                validando = 0;
            }
            if(i > 1 && j > 1){
                validando = 1;
                int validarDondeEstamos = 0;
                g_print("Estamos EN DONDE DEBEMOS ENTRAMOS\n");
                //g_print("Valor seleccionado max juegos: %s\n", juegos[contador2]);
                g_print("Estamos en i = %d  j = %d  posicion es = %d y el lugar es =: %s\n",i,j, posicion, tempLugares[posicion]);
                if (strcmp(tempLugares[posicion], "C") == 0) {
                    // ESTAMOS HABLANDO DE A ESTA EN CASA
                    //float probabilidaACasa = 0;
                    //float probabilidadBVisita = 0;
                    //g_print("Estamos A jugando en casa\n");
                    const gchar *entradaIzquierda = gtk_entry_get_text(GTK_ENTRY(matrizPrincipal[i-1][j]));
                    const gchar *entradaArriba = gtk_entry_get_text(GTK_ENTRY(matrizPrincipal[i][j-1]));
                    float calculo = (probabilidaACasa * atof(entradaIzquierda)) + (probabilidadBVisita * atof(entradaArriba));
                    char NUM[50];
                    snprintf(NUM, sizeof(NUM), "%f", calculo);
                    gtk_entry_set_text(GTK_ENTRY(matrizPrincipal[i][j]), NUM);
                    gtk_entry_set_text(GTK_ENTRY(matrizLugares[i][j]), tempLugares[posicion]);
                    GdkRGBA color;
                    gdk_rgba_parse(&color, "green");
                    gtk_widget_override_color(matrizPrincipal[j][i], GTK_STATE_FLAG_NORMAL, &color);
                    gtk_widget_override_color(matrizLugares[j][i], GTK_STATE_FLAG_NORMAL, &color);
                    //g_print("entrada izquierda: %d\n", atoi(entradaIzquierda));
                    //g_print("entrada arriba: %d\n", atoi(entradaArriba));
                    //g_print("Valor A casa: %f\n", probabilidaACasa);
                    //g_print("Valor B visita %f\n", probabilidadBVisita);

                } else if(strcmp(tempLugares[posicion], "V") == 0) {
                    // ESTAMOS HABLANDO DE A ESTA EN VISITA
                    //float probabilidadBCasa = 0;
                    //float probabilidadAVisita = 0;
                    //g_print("Estamos A jugando en visita\n");
                    const gchar *entradaIzquierda = gtk_entry_get_text(GTK_ENTRY(matrizPrincipal[i-1][j]));
                    const gchar *entradaArriba = gtk_entry_get_text(GTK_ENTRY(matrizPrincipal[i][j-1]));
                    float calculo = (probabilidadAVisita*atof(entradaIzquierda)) + (probabilidadBCasa*atof(entradaArriba));
                    char NUM[50];
                    snprintf(NUM, sizeof(NUM), "%f", calculo);
                    gtk_entry_set_text(GTK_ENTRY(matrizPrincipal[i][j]), NUM);
                    gtk_entry_set_text(GTK_ENTRY(matrizLugares[i][j]), tempLugares[posicion]);
                    GdkRGBA color;
                    gdk_rgba_parse(&color, "blue");
                    gtk_widget_override_color(matrizPrincipal[j][i], GTK_STATE_FLAG_NORMAL, &color);
                    gtk_widget_override_color(matrizLugares[j][i], GTK_STATE_FLAG_NORMAL, &color);
                }
                posicion++;
            }
        }
        if (validando == 1){
            contador2++;
            posicion  = contador2;
            //posicion -= contador2;
        }

    }
    solucion();

}

void solucion(){
    char solucion[9999] = "";
    strcat(solucion, "La probabilidad de que el Equipo A ganen ");
    char numJuegos[9999] = "";
    snprintf(numJuegos, sizeof(numJuegos), "%d", cantMaximaGanar);
    strcat(solucion, numJuegos);
    strcat(solucion, " partidos es: ");
    char valorFinal[9999] = "";
    float esvalor = atof(gtk_entry_get_text(GTK_ENTRY(matrizPrincipal[cantMaximaGanar+1][cantMaximaGanar+1])));
    g_print("Estamos en el valor de la chunca = %f",esvalor);
    snprintf(valorFinal, sizeof(valorFinal), "%f", esvalor);
    strcat(solucion, valorFinal);
    gtk_label_set_text(GTK_LABEL(labelSolucion), solucion);
}

void escribirArchivo(){
    archivo = fopen("EjemP.txt", "w");
    sprintf(cantMaxJuegosGuardar, "%d", cantidadMaxJuegosSeleccionada);
    fprintf(archivo, "%s", cantMaxJuegosGuardar);
    fprintf(archivo, " ");
    sprintf(GvalorProbACGuardar, "%d", valorProbACGuardar);
    fprintf(archivo, "%s", GvalorProbACGuardar);
    fprintf(archivo, " ");
    sprintf(GvalorProbAVGuardar, "%d", valorProbAVGuardar);
    fprintf(archivo, "%s", GvalorProbAVGuardar);
    fprintf(archivo, " ");
    sprintf(GvalorMejorDe, "%d", cantMaximaGanar);
    fprintf(archivo, "%s", GvalorMejorDe);
    fprintf(archivo, "\n");
    for (int i = 0; i < cantidadMaxJuegosSeleccionada+1; i++) {
        if(juegos[i] != NULL) {
            fprintf(archivo, "%s", juegos[i]);
        }
        fprintf(archivo, "\n");
    }
    fclose(archivo);
}

void leerArchivo(){
    int validandoEsCantJuegos = 0;
    int validandoEsProbAC = 0;
    int validandoEsProbAV = 0;
    int validandoEsMejorde = 0;

    char maxJuegosLeido[50] = "";
    char probACLeido[50] = "";
    char probAVLeido[50] = "";
    char esMejorDeLeido[50] = "";
    char valor[50] = "";

    const gchar *direccionArchivo = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(botonAbrirArchivo));
    gchar *nombreArchivo = g_filename_display_basename(direccionArchivo);
    g_print("Nombre del archivo seleccionado: %s\n", nombreArchivo);
    archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Error al leer el archivo: Archivo no existe \n");
    } else {
        char caracter;
        int a = 0;
        // leemos nuestras cantidades
        while ((caracter = fgetc(archivo)) != '\n') {
            if(validandoEsCantJuegos == 0){ // cantMaxuegos
                        //printf("ESTAMOS CON 1 \n");
                if(caracter != ' '){
                    maxJuegosLeido[a] = caracter;
                    a++;
                } else {
                     validandoEsCantJuegos = 1;
                     a = 0;
                 }
            } else if(validandoEsProbAC == 0){
                 //printf("ESTAMOS CON 2 \n");
                if(caracter != ' '){
                    probACLeido[a] = caracter;
                    a++;
                } else {
                     validandoEsProbAC = 1;
                     a = 0;
                 }
            } else if(validandoEsProbAV == 0){
                 //printf("ESTAMOS CON 3 \n");
                if(caracter != ' '){
                    probAVLeido[a] = caracter;
                    a++;
                } else {
                     validandoEsProbAV = 1;
                     a = 0;
                 }
            }  else if(validandoEsMejorde == 0){
                 //printf("ESTAMOS CON 4 \n");
                if(caracter != ' '){
                    esMejorDeLeido[a] = caracter;
                    a++;
                } else {
                     validandoEsMejorde = 1;
                     a = 0;
                 }
            }
        }
        cantidadMaxJuegosSeleccionada = atoi(maxJuegosLeido);
        probabilidaACasa = (atoi(probACLeido))/100.0;
        probabilidadAVisita = (atoi(probAVLeido))/100.0;
        probabilidadBCasa = 1-probabilidadAVisita;
        probabilidadBVisita = 1-probabilidaACasa;
        cantMaximaGanar = atoi(esMejorDeLeido);
        generarMatrizCV(NULL);

        snprintf(valor, sizeof(valor), "%d", cantMaximaGanar);
        gtk_entry_set_text(inputCantMejores, valor);

        char dato[50] = "";

        for (int i = 0; i < matrizSizeJuegos; i++) {
            fscanf(archivo, "%s", dato);
            juegos[i] = strdup(dato);
            gtk_entry_set_text(GTK_ENTRY(matrizJuegos[i+1][1]), dato);
        }
        fclose(archivo);

        /*
        g_print("ABrimos y nuestra cantMaxJUEgoe es %d\n", cantidadMaxJuegosSeleccionada);
        g_print("probAC es : %f\n", probabilidaACasa);
        g_print("probAV es : %f\n", probabilidadAVisita);
        g_print("probBC es : %f\n", probabilidadBCasa);
        g_print("probBV es : %f\n", probabilidadBVisita);
        g_print("es mejor de es : %d\n", cantMaximaGanar);
        */
    }
}

int main(int argc, char **argv) {
    GtkBuilder *builder;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "SeriesDeportivas.glade", NULL);

    vSeriesDeportivas = GTK_WIDGET(gtk_builder_get_object(builder, "vSeriesDeportivas"));
    comboBoxMaxJuegos = GTK_WIDGET(gtk_builder_get_object(builder, "comboBoxMaxJuegos"));
    comboBoxProbabilidadACasa = GTK_WIDGET(gtk_builder_get_object(builder, "comboBoxProbabilidadACasa"));
    comboBoxtProbabilidadAVisita = GTK_WIDGET(gtk_builder_get_object(builder, "comboBoxtProbabilidadAVisita"));
    botonGenerar = GTK_WIDGET(gtk_builder_get_object(builder, "botonGenerar"));
    labelCantidadJuegos = GTK_WIDGET(gtk_builder_get_object(builder, "labelCantidadJuegos"));
    labelProbabilidadCA = GTK_WIDGET(gtk_builder_get_object(builder, "labelProbabilidadCA"));
    labelProbabilidadVA = GTK_WIDGET(gtk_builder_get_object(builder, "labelProbabilidadVA"));
    boxOrdenJuegos = GTK_WIDGET(gtk_builder_get_object(builder, "boxOrdenJuegos"));
    botonCalcular = GTK_WIDGET(gtk_builder_get_object(builder, "botonCalcular"));
    boxElegirOpciones = GTK_WIDGET(gtk_builder_get_object(builder, "boxElegirOpciones"));
    boxMatrizPrincipal = GTK_WIDGET(gtk_builder_get_object(builder, "boxMatrizPrincipal"));
    boxMatrizLugares = GTK_WIDGET(gtk_builder_get_object(builder, "boxMatrizLugares"));
    inputCantMejores = GTK_WIDGET(gtk_builder_get_object(builder, "inputCantMejores"));
    LabelMejorDe = GTK_WIDGET(gtk_builder_get_object(builder, "LabelMejorDe"));
    labelSolucion = GTK_WIDGET(gtk_builder_get_object(builder, "labelSolucion"));
    botonGuardarArchivo = GTK_WIDGET(gtk_builder_get_object(builder, "botonGuardarArchivo"));
    botonAbrirArchivo = GTK_WIDGET(gtk_builder_get_object(builder, "botonAbrirArchivo"));
    botonSalir = GTK_WIDGET(gtk_builder_get_object(builder, "botonSalir"));
    g_signal_connect(botonGenerar, "clicked", G_CALLBACK(obtenerDatos), NULL);
    g_signal_connect(botonGenerar, "clicked", G_CALLBACK(generarMatrizCV), NULL);
    g_signal_connect(botonCalcular, "clicked", G_CALLBACK(obttenerDatosMejorDe), NULL);
    g_signal_connect(botonCalcular, "clicked", G_CALLBACK(algoritmoSD), NULL);
    g_signal_connect(botonGuardarArchivo, "clicked", G_CALLBACK(escribirArchivo), NULL);
    g_signal_connect(botonAbrirArchivo, "file-set", G_CALLBACK(leerArchivo), NULL);
    g_signal_connect(botonSalir, "clicked", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(vSeriesDeportivas);
    gtk_widget_hide(GTK_WIDGET(botonCalcular));
    gtk_widget_hide(GTK_WIDGET(inputCantMejores));
    gtk_widget_hide(GTK_WIDGET(LabelMejorDe));

    gtk_main();
}
