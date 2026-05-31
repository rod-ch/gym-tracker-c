#include <stdio.h>
#include <string.h>
#include <time.h>
#include <direct.h>
#include <sys/stat.h>
#include <windows.h>

#define PESO_NULL NULL

#define CSV_FOLDER_NAME "examples"
#define CSV_FILE_NAME "archivogin.csv"

char CSV_FOLDER[MAX_PATH];
char CSV_PATH[MAX_PATH];

//mostrar_detalle, 1 = si, 0 = no
float volumen(int mostrar_detalle, float *peso_salida, int *set_ejer,int *reps_ejer);
void menu();
void mul_ejer();
void sentinel_ejer();
void promedio_peso();
void buscar_max_min_peso();
void buscar_peso();
void quick_sort(float array[], int lowIndex, int highIndex);
void swap(float array[], int index1, int index2);
int binary_search(float a[], float e, int l, int r);
void nombre_ejer();
void name_peso();
void archivo_ejer();
void leer_archivo();
int obtener_ultimo_id(const char*);
int asegurar_carpeta(const char *ruta);
int inicializar_rutas_csv();


int asegurar_carpeta(const char *ruta) {
    struct stat info;

    if (stat(ruta, &info) == 0) {
        if (info.st_mode & S_IFDIR) {
            return 1;
        }

        printf("Error: existe un archivo llamado %s, pero no es una carpeta.\n", ruta);
        return 0;
    }

    if (_mkdir(ruta) == 0) {
        return 1;
    }

    printf("Error: no se pudo crear la carpeta %s.\n", ruta);
    return 0;
}





int main() {
	  if (!inicializar_rutas_csv()) {
        return 1;
    }

	menu();
	return 0;
}
void menu(){
	int num;
	
	printf("\033[31m===== GYM TRACKER =====\033[0m\n"); 
	printf("1) Calcular volumen de un ejercicio\n");
	printf("2) Calcular volumen de N ejercicios\n");
	printf("3) Ingresar ejercicios hasta id = 0\n");
	printf("4) Cargar pesos y calcular promedio\n");
	printf("5) Cargar pesos y mostrar maximo y minimo\n");
	printf("6) Buscar un peso en el array\n");
	printf("7) Nombre de Ejercicio\n");
	printf("8) Nombre y peso de Ejercicio\n");
	printf("9) Escribir un archivo de ejercicio\n");
	printf("10) Leer un archivo de ejercicio\n");
	printf("11) Salir\n");

	printf("Seleccione una opcion:");
	scanf("%d",&num);
	
	
	switch(num){
	case 1:
		volumen(1,PESO_NULL, PESO_NULL,PESO_NULL);
		break;
	case 2:
		mul_ejer();
		break;
	case 3:
		sentinel_ejer();
		break;	
	case 4:
		promedio_peso();
		break;
	case 5:
		buscar_max_min_peso();
		break;
	case 6:
		buscar_peso();
		break;
	case 7:
		nombre_ejer();
		break;
	case 8:
		name_peso();
		break;
	case 9:
		archivo_ejer();
		break;
	case 10:
		leer_archivo();
		break;	
	case 11:
		printf("Saliendo...\n");
		break;	
	default:
		printf("Opcion invalida\n");
		break;
		
	}
	
	
	
	
}
int inicializar_rutas_csv() {
    char exe_path[MAX_PATH];

    DWORD length = GetModuleFileNameA(NULL, exe_path, MAX_PATH);

    if (length == 0 || length == MAX_PATH) {
        printf("Error: no se pudo obtener la ruta del ejecutable.\n");
        return 0;
    }

    char *ultima_barra = strrchr(exe_path, '\\');

    if (ultima_barra == NULL) {
        printf("Error: ruta del ejecutable invalida.\n");
        return 0;
    }

    *ultima_barra = '\0';

    snprintf(CSV_FOLDER, sizeof(CSV_FOLDER), "%s\\%s", exe_path, CSV_FOLDER_NAME);
    snprintf(CSV_PATH, sizeof(CSV_PATH), "%s\\%s", CSV_FOLDER, CSV_FILE_NAME);

    return 1;
}
void sentinel_ejer() {
    int id = 100;
    float v = 0;
    float total = 0;
    int mostrar_detalle = 0;

    while (id != 0) {
        printf("Ingrese id del ejercicio (0 para terminar): ");
        scanf("%d", &id);

        if (id == 0) {
            printf("Entrenamiento finalizado.\n");
            printf("Total volume: %.1f\n", total);
            return;
        }

        v = volumen(mostrar_detalle, NULL, NULL, NULL);
        total += v;
    }
}


void mul_ejer() {
	int num;
	int mostrar_detalle = 0;
	float v;
	float total = 0;
	
	printf("Cuantos ejercicios hiciste? \n"); 
	scanf("%d",&num);
	if (num < 1){
	printf("Cantidad invalida\n"); 
	return;
	}
	
	for (int i = 0; i< num; i++){
		v = volumen(mostrar_detalle,PESO_NULL, PESO_NULL,PESO_NULL);
		total += v;
		
	}

	printf("Total volume: %.1f\n", total);

	
}


float volumen(int mostrar_detalle, float *peso_salida, int *set_ejer,int *reps_ejer){
	int set;
	int reps;
	float peso;
	float sum = 0;
	
	printf("Cuantas series hiciste? \n"); 
	scanf("%d",&set);
	if (set<= 0){
	printf("Error de input"); 
	return 0;
	}
	printf("Cuantas repeticiones hiciste? \n"); 
	scanf("%d",&reps);
	if (reps<= 0){	
	printf("Error de input\n"); 
	return 0;
	}

	printf("Cuanto peso usaste?\n"); 
	scanf("%f",&peso);	
	if (peso< 0){	
	printf("Peso no valido"); 

	return 0 ;
	}
	if (mostrar_detalle== 1){
	printf("Set: %d\n", set); 
	printf("Reps: %d\n", reps); 
	printf("Peso: %.1fkg \n", peso); 	
	}
	sum = set*reps*peso;
	printf("Volumen total de Ejercicio: %.1fkg \n", sum); 
	
if (peso_salida != NULL) {
    *peso_salida = peso;
}
if (set_ejer != NULL) {
    *set_ejer = set;
}
if (reps_ejer != NULL) {
    *reps_ejer = reps;
}
	
	
	
	
	return sum;

}
void promedio_peso(){
	int N;
	float sum = 0;
	printf("Numero de pesos:");
	scanf("%d", &N);
	if (N < 1 || N > 1000){
		printf("Cantidad invalida\n"); 
		return;
	}
	
	float pesos [N];
	for (int i = 0; i< N; i++){
		printf("Cual es el peso #%d\n",i+1);
		scanf("%f", &pesos[i]);

	}
	for (int i = 0; i< N; i++){
		sum += pesos[i];

	} 
	int indice_high = N - 1;
	int indice_low = 0;

	quick_sort(pesos, indice_low, indice_high);

	printf("Con orden:");
	
	
	for (int i = 0; i< N; i++){
		printf("#%d %.1f ", i + 1,pesos[i]); 

	}
	
	
	printf("\nTu peso promedio es %.1f\n",(sum/N)); 

}

void buscar_max_min_peso(){
	int N;
	float max = 0;
	float min = 0;
	int indice_max = 0;
	int indice_min = 0;
	printf("Numero de pesos:");
	scanf("%d", &N);
	if (N < 1 || N > 1000){
		printf("Cantidad invalida\n"); 
		return;
	}
	
	float pesos [N];
	for (int i = 0; i< N; i++){
		printf("Cual es el peso #%d\n",i+1);
		scanf("%f", &pesos[i]);

	}
	max = pesos[0];
	min = pesos[0];

	for (int i =1; i< N; i++){
		if (max < pesos[i]){
			max = pesos[i];
			indice_max = i;
		}
		else if (min > pesos[i]){
			min = pesos[i];
			indice_min = i;
		}
	} 
	printf("Tu peso maximo es %.1f en la posicion #%d\n",max, indice_max + 1); 
	printf("Tu peso minimo es %.1f en la posicion #%d\n",min, indice_min + 1); 

}

void quick_sort(float array[], int lowIndex, int highIndex){
	if (lowIndex >= highIndex) return;
	
	float pivot = array[highIndex];
	int leftPointer = lowIndex;
	int rightPointer = highIndex;
	
	while(leftPointer < rightPointer){
		while(array[leftPointer] <= pivot && leftPointer < rightPointer)leftPointer = leftPointer + 1;	
		while(array[rightPointer] >= pivot && leftPointer < rightPointer)rightPointer = rightPointer - 1;
		
		swap(array,leftPointer, rightPointer);

	}
	swap(array,leftPointer, highIndex);
	
	quick_sort(array, lowIndex, leftPointer - 1 );
	quick_sort(array, leftPointer + 1, highIndex);


	
}


void swap(float array[], int index1, int index2){
	
	float temp = array[index1];
	array[index1] = array[index2];
	array[index2] = temp;
	
}
	
int binary_search(float a[], float e, int l, int r){
	
	int mid = l + (r - l) / 2 ;
	if (l > r) return -1;
	
	if (a[mid] == e)
		return mid;
	else if (a[mid] > e)
		return binary_search(a, e, l, mid -1 );
	else
		return binary_search(a, e, mid + 1, r);
	
}


void buscar_peso(){
	int N;
	int indice_low = 0;
	float mynum;

	printf("Numero de pesos:");
	scanf("%d", &N);
	if (N < 1 || N > 1000){
		printf("Cantidad invalida\n"); 
		return;
	}	
	int indice_high = N - 1;
	float pesos [N];
	for (int i = 0; i< N; i++){
		printf("Cual es el peso #%d\n",i+1);
		scanf("%f", &pesos[i]);

	}
	quick_sort(pesos, indice_low, indice_high);
	
	
	
	printf("Que peso deseas buscar?:");
	scanf("%f", &mynum);
	int start = 0;
	int index = binary_search(pesos, mynum, start, N -1);
	if (index == -1) {
		printf("Peso no encontrado \n");
		return;
	}
	
		
	printf("Esta en la posicion #%d: \n", index + 1);


		for (int i = 0; i< N; i++){
		printf("%1.f, ", pesos[i]);

	}
	
}

void nombre_ejer(){
	char name[128];
	
	printf("Nombre de ejercicio sin espacio: \n");
	scanf("%127s", name);
	
	printf("Nombre de ejercicio: %s", name);

}

void name_peso(){
	int num;
	float v;
	float total = 0;
	int mostrar_detalles = 0;

	
	printf("Cuantos ejercicios hiciste? \n"); 
	scanf("%d",&num);
	if (num < 1){
	printf("Cantidad invalida\n"); 
	return;
	}
	char nombres[num][128];
	float pesos[num];
for (int i = 0; i< num; i++){
	float peso_temporal;
	printf("#%d Nombre de tu ejercicio\n", i+1); 
	scanf("%127s", nombres[i]);
	v = volumen(mostrar_detalles, &peso_temporal, PESO_NULL,PESO_NULL);
	pesos[i] = peso_temporal;
	total += v;		
}
	printf("\n");

//	printf("Total volume: %.1f\n", total);
	
for (int i = 0; i< num; i++){
	printf("%s ", nombres[i]);
	printf("%.1f Kg \n", pesos[i]);
}	
}


// Abre el archivo CSV. Si no existe, lo crea con encabezado.
void archivo_ejer() {
    if (!asegurar_carpeta(CSV_FOLDER)) {
        return;
    }

    FILE *file = fopen(CSV_PATH, "r");

    if (file == NULL) {
        file = fopen(CSV_PATH, "a+");

        if (!file) {
            printf("Error: could not open file.\n");
            return;
        }

        char format[] = "id,fecha,ejercicio,sets,reps,peso,nota";
        fprintf(file, "%s\n", format);
    }

    fclose(file);

    file = fopen(CSV_PATH, "a");

    if (!file) {
        printf("Error: could not open file.\n");
        return;
    }

    int num;
    int mostrar_detalle = 0;
    float peso;
    int sets;
    int reps;
    float v;
    float total = 0;
    char nombre_ejer[50];

    printf("Cuantos ejercicios hiciste? \n");
    scanf("%d", &num);
    getchar();

    if (num < 1) {
        printf("Cantidad invalida\n");
        fclose(file);
        return;
    }

    time_t t = time(NULL);
    struct tm date = *localtime(&t);

    int ultimo_id = obtener_ultimo_id(CSV_PATH);

    for (int i = 0; i < num; i++) {
        printf("Nombre de el ejercicio:");
        fgets(nombre_ejer, sizeof(nombre_ejer), stdin);
        nombre_ejer[strcspn(nombre_ejer, "\n")] = '\0';

        v = volumen(mostrar_detalle, &peso, &sets, &reps);
        getchar();

        total += v;

        int id_actual = ultimo_id + i + 1;

        fprintf(file, "%d,%02d/%02d/%d,%s,%d,%d,%.2f,%.2f\n",
                id_actual,
                date.tm_mday,
                date.tm_mon + 1,
                date.tm_year + 1900,
                nombre_ejer,
                sets,
                reps,
                peso,
                v);
				
    }

    printf("Total volume: %.1f\n", total);
    fprintf(file, "TOTAL,,,,%.2f\n", total);
	

    fclose(file);
}
void leer_archivo() {
    if (!asegurar_carpeta(CSV_FOLDER)) {
        return;
    }

    FILE *file = fopen(CSV_PATH, "r");

    if (file == NULL) {
        file = fopen(CSV_PATH, "a");

        if (!file) {
            printf("Error: could not open file.\n");
            return;
        }

        char format[] = "id,fecha,ejercicio,sets,reps,peso,nota";
        fprintf(file, "%s\n", format);
        fclose(file);

        file = fopen(CSV_PATH, "r");

        if (!file) {
            printf("Error: could not open file.\n");
            return;
        }
    }

    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), file)) {
        char *token = strtok(buffer, ",");

        while (token) {
            printf("%s\t", token);
            token = strtok(NULL, ",");
        }

        printf("\n");
    }

    fclose(file);
}
int obtener_ultimo_id(const char *nombre_archivo) {
    FILE *file = fopen(nombre_archivo, "r");

    if (file == NULL) {
        return 0; // si no existe, arrancamos desde 1
    }

    char linea[300];
    int ultimo_id = 0;
    int id;

    // Saltar encabezado
    fgets(linea, sizeof(linea), file);

    while (fgets(linea, sizeof(linea), file)) {
        // Ignorar línea TOTAL
        if (strncmp(linea, "TOTAL", 5) == 0) {
            continue;
        }

        // Intentar leer el primer número antes de la primera coma
        if (sscanf(linea, "%d,", &id) == 1) {
            ultimo_id = id;
        }
    }

    fclose(file);
    return ultimo_id;
}
	




