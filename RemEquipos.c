#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <glibconfig.h>
#include <cairo.h>
#include <ctype.h>
#define N 1024
#include <stdbool.h>

GtkWidget *vReemplazoEquipos;
GtkWidget *inputCostoInicial;
GtkWidget *comboBoxPlazoProy;
GtkWidget *comboBoxVidaUtil;
GtkWidget *botonGenerar;
GtkWidget *boxOpciones;
GtkWidget *matrizPrincipal[N][N];
GtkWidget *matrizResultado[N][N];

GtkWidget *boxMatrizPrimera;
GtkWidget *boxPrimera1;
GtkWidget *boxPrimera2;

GtkWidget *boxMatrizResultado;
GtkWidget *boxMatrizResultado1;
GtkWidget *boxMatrizResultado2;


GtkWidget *botonGuardarArchivo;
GtkWidget *botonAbrirArchivo;
GtkWidget *botonSalir;
GtkWidget *boxBotonCalcular;
GtkWidget *botonCalcular;

GtkWidget *labelCostoInicial;
GtkWidget *labelPlazo;
GtkWidget *labelVida;

GtkWidget *labelRutasOptimas;

const gchar *juegos[21];

int costoInicial = 0;
int plazoProyecto = 0;
int vidaUtil = 0;
int matrizSizePrincipal = 0;
int matrizSizePrimera = 0;
int cantCostos = 0;

const gchar *mantenimientos[N];
const gchar *ventas[N];
const gchar *GA[N];
const gchar *G[N];
const gchar *aniosSolucion[N];
const gchar *aniosSolucionAux[N];

char costoGuardar[50] = "";
char plazoGuardar[50] = "";
char vidaUtilGuardar[50] = "";

FILE *archivo;

struct Costo {
    int t;
    int x;
    int valor;
    char *nombre;
    int aniosD;
    int inflacion;
};
struct Costo costos[9999];

char solucionTotal[9999];

void obtenerDatos() {
    //g_print("Obtener datos \n");
    const gchar *costoInicialExtraer = gtk_entry_get_text(GTK_ENTRY(inputCostoInicial));
    costoInicial = atoi(costoInicialExtraer);
    plazoProyecto = atoi(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(comboBoxPlazoProy)));
    vidaUtil = atoi(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(comboBoxVidaUtil)));

    //g_print("Valor plazo Proyecto: %d\n", plazoProyecto);
    //g_print("Valor vida util: %d\n", vidaUtil);
    //g_print("CostoInicial: %d\n", costoInicial);

}

void actualizarMatrizPrincipal(GtkWidget *entry, gpointer user_data) {
    int *indices = (int*)user_data;
    int i = indices[0];
    int j = indices[1];
    const gchar *entrada = gtk_entry_get_text(GTK_ENTRY(entry));
    for (int i = 0; i < vidaUtil+1; i++){
        for (int j = 0; j < 3; j++) {
            const gchar *entrada = gtk_entry_get_text(GTK_ENTRY(matrizPrincipal[i][j]));
        }
    }
    for (int filaActual = 1; filaActual < vidaUtil+1; filaActual++) {
        const gchar *mantenimiento = gtk_entry_get_text(GTK_ENTRY(matrizPrincipal[filaActual][1]));
        const gchar *venta = gtk_entry_get_text(GTK_ENTRY(matrizPrincipal[filaActual][2]));

        mantenimientos[filaActual-1] = mantenimiento;
        ventas[filaActual-1] = venta;

        printf("Contenido del arreglo 'mantenimiento':\n");
        for (int i = 0; i < vidaUtil; i++) {
            printf("%s ", mantenimientos[i]);
        }
        printf("\n");

        printf("Contenido del arreglo 'ventas':\n");
        for (int i = 0; i < vidaUtil; i++) {
            printf("%s ", ventas[i]);
        }
        printf("\n");
    }
}

void generarMatrizPrincipal() {
    int numero = 1;
    boxPrimera1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    for (int i = 0; i < vidaUtil+1; i++) {
        boxPrimera2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        for (int j = 0; j < 3; j++) {
            matrizPrincipal[i][j] = gtk_entry_new();
            gtk_entry_set_width_chars(GTK_ENTRY(matrizPrincipal[i][j]), 1);
            gtk_box_pack_start(GTK_BOX(boxPrimera2), matrizPrincipal[i][j], TRUE, TRUE, FALSE);
            if (i == 0 && j == 0){
                gtk_entry_set_text(GTK_ENTRY(matrizPrincipal[i][j]), "Año");
                gtk_widget_set_sensitive(matrizPrincipal[i][j], FALSE);
            }
            if (i == 0 && j == 1){
                gtk_entry_set_text(GTK_ENTRY(matrizPrincipal[i][j]), "Mantenimiento");
                gtk_widget_set_sensitive(matrizPrincipal[i][j], FALSE);
            }
            if (i == 0 && j == 2){
                gtk_entry_set_text(GTK_ENTRY(matrizPrincipal[i][j]), "Venta");
                gtk_widget_set_sensitive(matrizPrincipal[i][j], FALSE);
            }
            if (j == 0 && i > 0){
                char contador[50];
                snprintf(contador, sizeof(contador), "%d", numero);
                gtk_entry_set_text(GTK_ENTRY(matrizPrincipal[i][j]), contador);
                gtk_widget_set_sensitive(matrizPrincipal[i][j], FALSE);
                numero++;
            }
            int *indices = malloc(2 * sizeof(int));
            indices[0] = i;
            indices[1] = j;
            g_signal_connect(matrizPrincipal[i][j], "changed", G_CALLBACK(actualizarMatrizPrincipal), indices);

        }
        gtk_box_pack_start(GTK_BOX(boxPrimera1), boxPrimera2, TRUE, TRUE, FALSE);
    }
    gtk_box_pack_start(GTK_BOX(boxMatrizPrimera), boxPrimera1, TRUE, FALSE, 0);
    gtk_widget_show_all(vReemplazoEquipos);

}

void escribirArchivo(){
    archivo = fopen("EjemRE.txt", "w");
    sprintf(costoGuardar, "%d", costoInicial);
    fprintf(archivo, "%s", costoGuardar);
    fprintf(archivo, " ");
    sprintf(plazoGuardar, "%d", plazoProyecto);
    fprintf(archivo, "%s", plazoGuardar);
    fprintf(archivo, " ");
    sprintf(vidaUtilGuardar, "%d", vidaUtil);
    fprintf(archivo, "%s", vidaUtilGuardar);
    fprintf(archivo, "\n");

    //g_print("Valor plazo ProyectoAAA: %s\n", plazoGuardar);
    //g_print("Valor vida utilAAA: %s\n", vidaUtilGuardar);
    //g_print("CostoInicialAAA: %s\n", costoGuardar);

    for (int i = 1; i < vidaUtil+1; i++) {
        for (int j = 1; j < 3; j++) {
            const char *entrada = gtk_entry_get_text(GTK_ENTRY(matrizPrincipal[i][j]));
            fprintf(archivo, "%s", entrada);
            if (j < (vidaUtil+1) - 1) {
                fprintf(archivo, "\t");
            }
        }
        fprintf(archivo, "\n");
    }
    fclose(archivo);
}

void leerArchivo(){
    int validandoEsCosto = 0;
    int validandoEsPlazo = 0;
    int validandoEsVidaEquipo = 0;

    char costoLeido[50] = "";
    char plazoLeido[50] = "";
    char vidaEquipoLeido[50] = "";

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
            if(validandoEsCosto == 0){
                //printf("ESTAMOS CON 1 \n");
                if(caracter != ' '){
                    costoLeido[a] = caracter;
                    a++;
                } else {
                     validandoEsCosto = 1;
                     a = 0;
                 }
            } else if(validandoEsPlazo == 0){
                 //printf("ESTAMOS CON 2 \n");
                if(caracter != ' '){
                    plazoLeido[a] = caracter;
                    a++;
                } else {
                     validandoEsPlazo = 1;
                     a = 0;
                 }
            } else if(validandoEsVidaEquipo == 0){
                 //printf("ESTAMOS CON 3 \n");
                if(caracter != ' '){
                    vidaEquipoLeido[a] = caracter;
                    a++;
                } else {
                     validandoEsVidaEquipo = 1;
                     a = 0;
                 }
            }
        }
        costoInicial = atoi(costoLeido);
        plazoProyecto = atoi(plazoLeido);
        vidaUtil = atoi(vidaEquipoLeido);

        g_print("costo leido:  %d\n", costoInicial);
        g_print("plazo leido : %d\n", validandoEsPlazo);
        g_print("vidautil leido : %d\n", vidaUtil);


        generarMatrizPrincipal(NULL);

        char dato[50] = "";

        for (int i = 1; i < vidaUtil+1; i++) {
            for (int j = 1; j < 3; j++) {
                fscanf(archivo, "%s", dato);
                //g_print("buenas : %s\n", dato);
                gtk_entry_set_text(GTK_ENTRY(matrizPrincipal[i][j]), dato);
                }
            }
        fclose(archivo);
        gtk_widget_hide(GTK_WIDGET(boxOpciones));
        gtk_widget_hide(GTK_WIDGET(labelCostoInicial));
        gtk_widget_hide(GTK_WIDGET(labelPlazo));
        gtk_widget_hide(GTK_WIDGET(labelVida));
    }
}

void genearMatrizResultado() {
    int numero = 0;
    int NUM = 0;
    int NUM2 = 0;
    boxMatrizResultado1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    for (int i = 0; i < plazoProyecto+2; i++) {
        boxMatrizResultado2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        for (int j = 0; j < 3; j++) {
            matrizResultado[i][j] = gtk_entry_new();
            gtk_entry_set_width_chars(GTK_ENTRY(matrizResultado[i][j]), 1);
            gtk_box_pack_start(GTK_BOX(boxMatrizResultado2), matrizResultado[i][j], TRUE, TRUE, FALSE);
            if (i == 0 && j == 0){
                gtk_entry_set_text(GTK_ENTRY(matrizResultado[i][j]), "t");
                gtk_widget_set_sensitive(matrizResultado[i][j], FALSE);
            }
            if (i == 0 && j == 1){
                gtk_entry_set_text(GTK_ENTRY(matrizResultado[i][j]), "G(t)");
                gtk_widget_set_sensitive(matrizResultado[i][j], FALSE);
            }
            if (i == 0 && j == 2){
                gtk_entry_set_text(GTK_ENTRY(matrizResultado[i][j]), "Años");
                gtk_widget_set_sensitive(matrizResultado[i][j], FALSE);
            }
            if (j == 0 && i > 0){
                char contador[50];
                snprintf(contador, sizeof(contador), "%d", numero);
                gtk_entry_set_text(GTK_ENTRY(matrizResultado[i][j]), contador);
                gtk_widget_set_sensitive(matrizResultado[i][j], FALSE);
                numero++;
            }
            if (j == 1 && i > 0){
                gtk_entry_set_text(GTK_ENTRY(matrizResultado[i][j]), G[NUM]);
                NUM++;
            }
            if (j == 2 && i > 0){
                gtk_entry_set_text(GTK_ENTRY(matrizResultado[i][j]), aniosSolucion[NUM2]);
                NUM2++;
            }

        }
        gtk_box_pack_start(GTK_BOX(boxMatrizResultado1), boxMatrizResultado2, TRUE, TRUE, FALSE);
    }
    gtk_box_pack_start(GTK_BOX(boxMatrizResultado), boxMatrizResultado1, TRUE, FALSE, 0);
    gtk_widget_show_all(vReemplazoEquipos);

}

void solucionRutasOptiomas(){
    g_print("Estamos en solucion");

    for(int i = 0; i < plazoProyecto; i++){
        aniosSolucionAux[i] = aniosSolucion[i];
    }
    aniosSolucionAux[plazoProyecto] = "0";

    /*
    printf("Contenido del AñosAUX: \n");
    for (int i = 0; i < plazoProyecto+1; i++) {
        printf("%s ", aniosSolucionAux[i]);
    }
    printf("\n");
    */

    int numeroPlan = 1;
    for(int i = 0; i < plazoProyecto; i++){
        char solucion[1000] = "";
        char numPlan[9999] = "";
        strcat(solucion, "Plan");
        snprintf(numPlan, sizeof(numPlan), "%d", numeroPlan);
        strcat(solucion, numPlan);
        strcat(solucion, ": ");
        int lendPosicion = strlen(aniosSolucion[i]);
        if(lendPosicion == 1){
            char val1[9999] = "";
            snprintf(val1, sizeof(val1), "%d", i);
            strcat(solucion, val1);
            strcat(solucion, "--->");
            int b = atoi(aniosSolucion[i]);
            char val2[9999] = "";
            snprintf(val2, sizeof(val2), "%d", b);
            strcat(solucion, val2);
            if (b == plazoProyecto){
                strcat(solucion, " ");
            } else {
                strcat(solucion, "--->");
                int blend = strlen(aniosSolucion[b]);
                int c = atoi(aniosSolucion[b]);
                if(c == plazoProyecto){
                    char val3[9999] = "";
                    snprintf(val3, sizeof(val3), "%d", c);
                    strcat(solucion, val3);
                }
                g_print("lend valor %d es; lendnuevob es: %d   c es:   %dn", b , blend, c);
            }
        } else {
            char val4[9999] = "";
            snprintf(val4, sizeof(val4), "%d", i);
            strcat(solucion, val4);
            strcat(solucion, "--->");
            const gchar *conector = aniosSolucion[i];
            char conectores[1000];
            strcpy(conectores, conector);
            for (int j = 0; conectores[j] != '\0'; j++) {
                char caracter = conectores[j];
                if(caracter != ',') {
                char caracterAux[2];
                caracterAux[1] = '\0';
                caracterAux[0] = caracter;
                const gchar *conectorAux = aniosSolucion[caracter];
                char conectoresAux[1000];
                strcpy(conectores, conector);
                strcat(solucion, caracterAux);
                strcat(solucion, "--->");
                }
            }
        }
        numeroPlan++;
        //g_print("Estamos en solucion año es: %d y cantidad es: %d\n", i , lendPosicion);
        strcat(solucionTotal, solucion);
        strcat(solucionTotal, "\n");
        //if(sizeof())
        //for
    g_print("%s\n", solucion);
    }
    gtk_label_set_text(GTK_LABEL(labelRutasOptimas), solucionTotal);

}

int algoritmoRE(){
    int numG = plazoProyecto;
    int e = 1;
    int eAux = 1;
    int a = 0;
    for(int i = 0; i < plazoProyecto; i++){
        for(int j = 0; j < vidaUtil;j++){
        if(e < plazoProyecto+1){
            char valorCompleto[9999] = "";
            strcat(valorCompleto, "C");
            char valor1[9999] = "";
            snprintf(valor1, sizeof(i), "%d", i);
            strcat(valorCompleto, valor1);
            char valor2[9999] = "";
            snprintf(valor2, sizeof(e), "%d", e);
            strcat(valorCompleto, valor2);
            costos[a].t = i;
            costos[a].x = e;
            costos[a].nombre = strdup(valorCompleto);
            int cantAnios = e - i;
            costos[a].aniosD = cantAnios;
            int mantenimientoVariable = 0;
            for(int m = 0; m < cantAnios; m++) {
                mantenimientoVariable += atoi(mantenimientos[m]);
            }
            costos[a].valor = costoInicial + mantenimientoVariable - atoi(ventas[j]);
            cantCostos++;
            a++;
            e++;
            }
         }
        eAux++;
        e = eAux;
    }
    for (int i = 0; i < cantCostos; i++) {
        printf("Costo[%d]: t=%d, x=%d, nombre=%s, valor=%d, años=%d\n", i, costos[i].t, costos[i].x, costos[i].nombre, costos[i].valor, costos[i].aniosD);
    }
    printf("\n");

    G[plazoProyecto] = "0";
    for (int i = plazoProyecto; i >= 0; i--) {
        int valorMin = 0;
        int validando = 0;
        char valorAnios[9999] = "";
        for (int j = 0; j < cantCostos; j++) {
            if(costos[j].t == i){
                if(validando != 1){
                    valorMin = costos[j].valor + atoi(G[costos[j].x]);
                    validando = 1;
                }
                int valorActual = costos[j].valor + atoi(G[costos[j].x]);
                //printf("G: %s\n ", G[costos[j].x]);
                if(valorActual <= valorMin){
                    valorMin = valorActual;
                }
            }
        }
        //printf("sal2\n ");
        validando = 0;
        char GAux[9999] = "";
        snprintf(GAux, 10, "%d", valorMin);
        G[i] = strdup(GAux);
    }

    aniosSolucion[plazoProyecto] = "0";
    for(int i = 0; i < plazoProyecto; i++){
        int validandoComa = 0;
        char ValAux[9999] = "";
        for (int j = 0; j < cantCostos; j++) {
            if(costos[j].t == i){
                int valorActual = costos[j].valor + atoi(G[costos[j].x]);
                if(valorActual == atoi(G[i])){
                    int valorAnio = costos[j].x;
                    //printf("valorActual es: %d, nombre=%s , ANIO: %d\n", valorActual, costos[j].nombre, valorAnio);
                    char anio[9999] = "";
                    snprintf(anio, sizeof(valorAnio), "%d", valorAnio);
                    if(validandoComa == 1){
                        strcat(ValAux, ",");
                    }
                    strcat(ValAux, anio);
                    validandoComa = 1;
                }
            }
        }
    aniosSolucion[i] = strdup(ValAux);
    }
    //printf("terminamos");

    printf("Contenido del arreglo 'G':\n");
    for (int i = 0; i < plazoProyecto+1; i++) {
        printf("%s ", G[i]);
    }
    printf("\n");

    printf("Contenido del Años: \n");
    for (int i = 0; i < plazoProyecto+1; i++) {
        printf("%s ", aniosSolucion[i]);
    }
    printf("\n");

    genearMatrizResultado();
    solucionRutasOptiomas();
    }



int main(int argc, char **argv) {
    GtkBuilder *builder;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "ReemplazoEquipos.glade", NULL);

    vReemplazoEquipos = GTK_WIDGET(gtk_builder_get_object(builder, "vReemplazoEquipos"));
    inputCostoInicial = GTK_WIDGET(gtk_builder_get_object(builder, "inputCostoInicial"));
    comboBoxPlazoProy = GTK_WIDGET(gtk_builder_get_object(builder, "comboBoxPlazoProy"));
    comboBoxVidaUtil = GTK_WIDGET(gtk_builder_get_object(builder, "comboBoxVidaUtil"));
    botonGenerar = GTK_WIDGET(gtk_builder_get_object(builder, "botonGenerar"));
    boxOpciones = GTK_WIDGET(gtk_builder_get_object(builder, "boxOpciones"));
    boxMatrizPrimera = GTK_WIDGET(gtk_builder_get_object(builder, "boxMatrizPrimera"));
    botonGuardarArchivo = GTK_WIDGET(gtk_builder_get_object(builder, "botonGuardarArchivo"));
    botonAbrirArchivo = GTK_WIDGET(gtk_builder_get_object(builder, "botonAbrirArchivo"));
    botonSalir = GTK_WIDGET(gtk_builder_get_object(builder, "botonSalir"));
    labelCostoInicial = GTK_WIDGET(gtk_builder_get_object(builder, "labelCostoInicial"));
    labelPlazo = GTK_WIDGET(gtk_builder_get_object(builder, "labelPlazo"));
    labelVida = GTK_WIDGET(gtk_builder_get_object(builder, "labelVida"));
    boxBotonCalcular = GTK_WIDGET(gtk_builder_get_object(builder, "boxBotonCalcular"));
    botonCalcular = GTK_WIDGET(gtk_builder_get_object(builder, "botonCalcular"));
    boxMatrizResultado = GTK_WIDGET(gtk_builder_get_object(builder, "boxMatrizResultado"));
    labelRutasOptimas = GTK_WIDGET(gtk_builder_get_object(builder, "labelRutasOptimas"));

    g_signal_connect(botonGenerar, "clicked", G_CALLBACK(obtenerDatos), NULL);
    g_signal_connect(botonGenerar, "clicked", G_CALLBACK(generarMatrizPrincipal), NULL);
    g_signal_connect(botonAbrirArchivo, "file-set", G_CALLBACK(leerArchivo), NULL);
    g_signal_connect(botonSalir, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(botonGuardarArchivo, "clicked", G_CALLBACK(escribirArchivo), NULL);
    g_signal_connect(botonCalcular, "clicked", G_CALLBACK(algoritmoRE), NULL);

    gtk_widget_show_all(vReemplazoEquipos);
    gtk_widget_hide(GTK_WIDGET(boxBotonCalcular));

    gtk_main();
}
