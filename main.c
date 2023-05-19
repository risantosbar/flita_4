#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef struct vert {
    int number_vert;
    int count_degrees;
} Vert;


void  counting_sort(int numbers, int count){
    FILE *inputfile;
    int roads, count_verts, count_edges, count_all_elems = 0;
    char first_string[100];
    inputfile = fopen("array.txt", "r");
    fgets(first_string, 100, inputfile);
    count_edges = strlen(first_string) / 2;
    fseek(inputfile, 0, SEEK_SET);
    while (fscanf(inputfile, "%d", &roads) != EOF) {
        count_all_elems++;
    }
    count_verts = count_all_elems / count_edges;
    fseek(inputfile, 0, SEEK_SET);
    int **matrix = (int**) malloc(count_verts*sizeof(int*));
    for (int i = 0; i <count_verts; i++){
        matrix[i] = (int*) malloc(count_edges*sizeof(int));
    }
    int count_deg;
    for (int i = 0; i < count_verts; i++) {
        for (int j = 0; j < (count_edges); j++) {
            fscanf(inputfile, "%d", &matrix[i][j]);
        }
    }
    fclose(inputfile);

    Vert even_verts[count_verts];
    for (int i = 0; i < count_verts; i++) {
        even_verts[i].number_vert = -1;
        even_verts[i].count_degrees = -1;
    }
    double worktime;
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    for (int i = 0; i < count_verts; i++) {
        count_deg = 0;
        for (int j = 0; j < (count_edges); j++) {
            if (matrix[i][j] == 1) count_deg++;
        }
        if (count_deg % 2 == 0) {
            even_verts[i].number_vert = i;
            even_verts[i].count_degrees = count_deg;
        }
    }
    int location;
    Vert new_elem;

    for (int i = 1; i < count_verts; i++)
    {
        new_elem = even_verts[i];
        location = i - 1;
        while(location >= 0 && even_verts[location].count_degrees > new_elem.count_degrees)
        {
            even_verts[location+1] = even_verts[location];
            location = location - 1;
        }
        even_verts[location+1] = new_elem;
    }
    /*for (int i = 0; i < count_verts; i++) {
        for (int j = i + 1; j < count_verts - i; j++) {
            if (even_verts[j - 1].count_degrees > even_verts[j].count_degrees) {
                Vert buff;
                buff = even_verts[j - 1];
                even_verts[j - 1] = even_verts[j];
                even_verts[j] = buff;
            }
        }
    }*/
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    worktime = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
    printf("%.7f", worktime);
    printf("\n\n");
    fclose(inputfile);
    FILE* file_out=fopen("info.txt","a+");
    fprintf(file_out,"%d %.7f",numbers*numbers, worktime);
    if(count != 6){
        fprintf(file_out, "\n");
    }
    free(matrix);
    fclose(file_out);
}

int main() {
    int numbers = 50;
    int number, count = 0;
    FILE* file = fopen("array.txt", "w");
    fclose(file);
    FILE* file_out=fopen("info.txt","w");
    fclose(file_out);
    for (int i =0 ; i<7;i++){
        file = fopen("array.txt", "w");
        srand(time(NULL));
        for (int j = 0; j < numbers; j++){
            number = rand() % 1;
            fprintf(file, "%d", number);
            for(int i = 0; i < numbers - 1; i++){
                number = rand() % 1;
                fprintf(file, " %d", number);
            }
            fprintf(file, "\n");
        }

        fclose(file);
        counting_sort(numbers, count);
        count++;
        numbers*=2;
    }
    system("gnuplot -e \"plot 'info.txt' u 1:2 with lines, '' u 1:2 with points; pause -1\" && echo Plot generated successfully");
    return 0;
}