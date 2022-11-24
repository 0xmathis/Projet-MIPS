#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _FONCTION_FILES_C_
#define _FONCTION_FILES_C_
#include "../include/fonctionFiles.h"
#include "constantes.c"
#include "../include/utils.h"
#endif


// TODO: voir pour utiliser strcmp de string.h à la place de areStringsEqual

void readLine(FILE* file, char* line, int size) {
        fgets(line, size, file);
}

void read(FILE* file, int registres[32]) {
    while(!feof(file)) {
        // printf("ici\n");
        char line[50];
        readLine(file, line, 50);
        printf("%s\n", line);
        Instruction* instruction = (Instruction*) malloc(sizeof(Instruction));
        analyseLine(line, instruction);
        printInfos(instruction);
        printf("\n");

        free(instruction);

        //break;
    }
}

void analyseLine(char* line, Instruction* instruction) {
    // on récupère l'instruction
    setOperateurFromLine(line, instruction);
    // printf("got operateur\n");
    setOperateurFormat(instruction);
    // printf("got format\n");
    setNbParametersFromLine(instruction);
    setParametersFromLine(line, instruction);
    // printf("got paramatres\n");
    setOperateurOPcodeOrFunc(instruction);
    // printf("got opcode\n");
}

void setNbParametersFromLine(Instruction* instruction) {
    FILE * file = fopen("../data/operateursNbParametres.txt", "r");
    int nbParametres = -1;

    while(!feof(file) && nbParametres == -1) {
        char line[15];
        char mot[8];
        int temp;

        fgets(line, 15, file);
        sscanf(line, "%s %d", mot, &temp);

        if (areStringsEqual(instruction->operateur, mot)) {
            nbParametres = temp;
        }
    }

    fclose(file);
    setNbParametres(instruction, nbParametres);
}

void setParametersFromLine(char* line, Instruction* instruction) {
    char format[20];

    switch(instruction->format) {
        case FORMAT_1:
            copyStrings(FORMAT_1_STR, format);
            break;
        case FORMAT_2:
            copyStrings(FORMAT_2_STR, format);
            break;
        case FORMAT_3:
            copyStrings(FORMAT_3_STR, format);
            break;
        case FORMAT_4:
            copyStrings(FORMAT_4_STR, format);
            break;
        case FORMAT_5:
            copyStrings(FORMAT_5_STR, format);
            break;
        case FORMAT_6:
            copyStrings(FORMAT_6_STR, format);
            break;
        case FORMAT_7:
            copyStrings(FORMAT_7_STR, format);
            break;
        case FORMAT_8:
            copyStrings(FORMAT_8_STR, format);
            break;
    }


//    if (areStringsEqual(instruction->operateur, "NOP\0") || areStringsEqual(instruction->operateur, "SYSCALL\0")) {
//        copyStrings(instruction->operateur, format);
//    } else {
//        getFormatStr(format, instruction);
//    }
    // *(format+1) = '\0';

    printf("%s\n", format);

    char temp[8];
    int parametres[4];
    sscanf(line, format, temp, &parametres[0], &parametres[1], &parametres[2], &parametres[3]);
    // printf("%d %d %d %d\n", x1, x2, x3, x4);
    setParametres(instruction, parametres);

    // switch (instruction->format) {
    //     case FORMAT_R:
    //         setParametersFromLineR(line, instruction);
    //         break;
    //     case FORMAT_I:
    //         setParametersFromLineI(line, instruction);
    //         break;
    //     case FORMAT_J:
    //         setParametersFromLineJ(line, instruction);
    //         break;
    // }
}

void getFormatStr(char* output, Instruction* instruction) {
    FILE * file = fopen("../data/instructionsFormatStr.txt", "r");

    char operateur[30];
    copyStrings(instruction->operateur, operateur);
    int isFound = 0;

    while (!feof(file) && !isFound) {
        char operateurFound[10], temp[20], line[30];

        fgets(line, 30, file);
        sscanf(line, "%s %s", operateurFound, temp);
        printf("temp : %s\n", temp);

        if (areStringsEqual(operateur, operateurFound)) {
            copyStrings(line, output);
            isFound = 1;
        }

    }

    fclose(file);
}

void setOperateurFromLine(char* line, Instruction* instruction){
    char operateur[8];  // stock l'adresse de début de l'opétateur
    char* temp = operateur;
    while (*line != ' ' && *line != '\n') { // tant qu'il n'y a pas d'espace
        *temp = *line;
        temp++;
        line++;
    }
    *temp = '\0';

    setOperateur(instruction, operateur);
}

void setOperateurFormat(Instruction* instruction) {
    FILE * file = fopen("../data/operateursFormat.txt", "r");
    int format = -1;
    
    while(!feof(file) && format == -1) {
        char line[15];
        char mot[8];
        int temp;

        fgets(line, 15, file);
        sscanf(line, "%s %d", mot, &temp);

        if (areStringsEqual(instruction->operateur, mot)) {
            format = temp;
        }
    }

    fclose(file);
    setFormat(instruction, format);
}

// void setParametersFromLineR(char* line, Instruction* instruction){
//     char temp[8];
//     int r1, r2, r3;

//     sscanf(line, FORMAT_R_STR, temp, &r1, &r2, &r3);
//     setParametres(instruction, r1);
//     setParametres(instruction, r2);
//     setParametres(instruction, r3);
// }

// void setParametersFromLineI(char* line, Instruction* instruction){
//     char temp[8];
//     int r1, r2, r3;

//     sscanf(line, FORMAT_I_STR, temp, &r1, &r2, &r3);
//     setParametres(instruction, r1);
//     setParametres(instruction, r2);
//     setParametres(instruction, r3);
// }

// void setParametersFromLineJ(char* line, Instruction* instruction){
//     char temp[8];
//     int r1;

//     sscanf(line, FORMAT_J_STR, temp, &r1);
//     setParametres(instruction, r1);
// }

void setOperateurOPcodeOrFunc(Instruction* instruction) {
    FILE * file = fopen("../data/operateursOPcodeFunc2.txt", "r");
    char OPcodeOrFunc[7];
    char mot[8];


    while(!feof(file) && !areStringsEqual(instruction->operateur, mot)) {
        char line[20];

        fgets(line, 20, file);
        sscanf(line, "%s %s", mot, OPcodeOrFunc);
    }

    fclose(file);
    setOPcodeOrFunc(instruction, OPcodeOrFunc);
}
