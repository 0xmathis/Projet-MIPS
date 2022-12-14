#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _ANALYSE_H_
#define _ANALYSE_H_

#include "analyse.h"

#endif

#include "utils.h"
#include "constantes.h"


void analyseLine(char *line, Instruction *instruction) {
    // on récupère toutes les infos de l'instruction
    setOperateurFromLine(line, instruction);
    setInfos(line, instruction);



    // setOperateurFormat(instruction);
    // if (!isError(instruction)) { // s'il n'y a pas d'erreur sur l'opérateur
    //     setNbParametersFromLine(instruction);  // si pas d'erreur sur l'opérateur alors pas d'erreur ici
    //     setParametersFromLine(line, instruction);

    //     if (!isError(instruction)) {  // s'il n'y a pas d'erreur sur les paramètres
    //         setOperateurOPcodeOrFunc(instruction);  // si pas d'erreur sur l'opérateur alors pas d'erreur ici
    //         setParametersOrderFromLine(instruction);  // si pas d'erreur sur l'opérateur alors pas d'erreur ici
    //     }
    // }
}

void setInfos(char *line, Instruction *instruction) {
    FILE *file = fopen("data/data.txt", "r");
    int isFound = 0;
//    int i = 0;

    while (!feof(file) && !isFound) {
//        printf("%d\n", i+=1);
        char lineTested[100];
        char *checkError = fgets(lineTested, 100, file);

//        printf("lineTested : %s\n", lineTested);

        if (checkError[0] == '#' || checkError[0] == '\n') {
            continue;
        }

        replaceChar(lineTested, '\n', '\0');

        char operateur[10], format[3], OPcodeOrFunc[10], nbParameters[10], inputFormat[20], orderParameters[15];
        sscanf(lineTested, "%s ; %s ; %s ; %s ; %s ; %s ;", operateur, format, OPcodeOrFunc, nbParameters, inputFormat, orderParameters);
//        printf("inputFormat : %s\n", inputFormat);
//        printf("%s ;; %s ;; %s ; %s ;; %s ;; %s ;;\n", operateur, format, OPcodeOrFunc, nbParameters, inputFormat, orderParameters);

        if (strcmp(instruction->operateur, operateur) == 0) {
            replaceChar(inputFormat, '+', ' ');
            replaceChar(orderParameters, '+', ' ');
            isFound = 1;

            setOperateurFormat(format, instruction);
            setOperateurOPcodeOrFunc(OPcodeOrFunc, instruction);
            setNbParametersFromLine(nbParameters, instruction);
            setParametersFromLine(line, inputFormat, instruction);
            setParametersOrderFromLine(orderParameters, instruction);
        }
    }
}

void setOperateurFromLine(char *line, Instruction *instruction) {
    char operateur[8];  // stock l'adresse de début de l'opétateur
    char *tempOP = operateur;
    char *tempLine = line;

    while (*tempLine != ' ' && *tempLine != '\n') { // tant qu'il n'y a pas d'espace ou de retour à la ligne
        *tempOP = *tempLine;
        tempOP++;
        tempLine++;
    }
    *tempOP = '\0';

    toUpperCase(operateur);
    setOperateur(instruction, operateur);
}

void setOperateurFormat(char *formatChar, Instruction *instruction) {
    int formatInt;
    sscanf(formatChar, "%d", &formatInt);
    setFormat(instruction, formatInt);
}

void setOperateurOPcodeOrFunc(char *OPcodeOrFuncChar, Instruction *instruction) {
    int OPcodeOrFuncInt;
    sscanf(OPcodeOrFuncChar, "%d", &OPcodeOrFuncInt);
    setOPcodeOrFunc(instruction, OPcodeOrFuncInt);
}

void setNbParametersFromLine(char *nbParametersChar, Instruction *instruction) {
    int nbParametersInt;
    sscanf(nbParametersChar, "%d", &nbParametersInt);
    setNbParameters(instruction, nbParametersInt);
}

void setParametersFromLine(char *line, char *inputFormat, Instruction *instruction) {
    /*
    Dans les types I, J, et R les instructions recoivent au maximum 3 paramètres
    Dans le type R, le code binaire contient 4 paramètres : rd, rs, rt et sa
    Mais un des 4 sera nul (le paramètre dépend de l'instruction)
    On ne donne donc pas de valeur à parameters[3] qui sera forcément nul
    */

    char temp[8];
    char strParameters[5][10] = {0};
    int intParameters[5];
//    int nbParameters = splitString(line, strParameters, ' ');
//    removeChar(strParameters[1], ' ');
//    removeChar(strParameters[2], ' ');
//    removeChar(strParameters[3], ' ');
    int nbParameters = sscanf(line, inputFormat, strParameters[0], strParameters[1], strParameters[2], strParameters[3]);

//    printf("%s\n", line);
//    printf("%s\n", inputFormat);
//    printf("nbParameters : %d\n", nbParameters);

    if (instruction->nbParameters != nbParameters - 1) { // si on récupère un mauvais nombre de paramètres
        setNOP(instruction);
        setError(instruction, BAD_NBPARAMETERS);
    } else {
        int format = instruction->format;

        if (format == 1) {
            // si le paramètre 1 est un immédiat
            formatParameter(strParameters[1], &intParameters[0]);
        } else if (format == 2 || format == 3 || format == 9) {
            // si les paramètres 1 et 2 sont des registres et 3 est un immédiat
            checkRegisterExistence(instruction, strParameters[1], &intParameters[0]);  // +1 pour éliminer le $
            checkRegisterExistence(instruction, strParameters[2], &intParameters[1]);
            formatParameter(strParameters[3], &intParameters[2]);
        } else if (format == 4) {
            // si les paramètres 1 et 3 sont des registres et 2 est un immédiat
            checkRegisterExistence(instruction, strParameters[1], &intParameters[0]);
            formatParameter(strParameters[2], &intParameters[1]);
            checkRegisterExistence(instruction, strParameters[3], &intParameters[2]);
        } else if (format == 5 || format == 6) {
            // si le paramètre 1 est un registre et 2 est un immédiat
            checkRegisterExistence(instruction, strParameters[1], &intParameters[0]);
            formatParameter(strParameters[2], &intParameters[1]);
        } else if (format == 7 || format == 8) {
            // si les paramètres 1, 2 et 3 sont des registres
            checkRegisterExistence(instruction, strParameters[1], &intParameters[0]);
            checkRegisterExistence(instruction, strParameters[2], &intParameters[1]);
            checkRegisterExistence(instruction, strParameters[3], &intParameters[2]);
        } else if (format == 10) {
            // si les paramètres 1 et 2 sont des registres
            checkRegisterExistence(instruction, strParameters[1], &intParameters[0]);
            checkRegisterExistence(instruction, strParameters[2], &intParameters[1]);
        } else if (format == 11 || format == 12) {
            // si le paramètre 1 est un registre
            checkRegisterExistence(instruction, strParameters[1], &intParameters[0]);
        }
    }


    if (!isError(instruction)) {
        setParameters(instruction, intParameters);
    }
}

void setParametersOrderFromLine(char *parametersOrderChar, Instruction *instruction) {
    int parametersOrderInt[4];
    sscanf(parametersOrderChar, "%d %d %d %d", &parametersOrderInt[0], &parametersOrderInt[1], &parametersOrderInt[2],
           &parametersOrderInt[3]);
    setParametersOrder(instruction, parametersOrderInt);
}

void formatParameter(char *strParameter, int *intParameter) {
    if (strParameter[0] == '0' && strParameter[1] == 'x') {
        *intParameter = strtol(strParameter, NULL, 16);
    } else {
        sscanf(strParameter, "%d", intParameter);
    }
}

void checkRegisterExistence(Instruction *instruction, char *strParameter, int *intParameter) {
    toLowerCase(strParameter);
//    printf("check : ");

    if ('a' <= strParameter[0] && strParameter[0] <= 'z') {
        *intParameter = findRegisterNumber(strParameter);
    } else {
        sscanf(strParameter, "%d", intParameter);
    }

//    printf("%d\n", *intParameter);

    if (*intParameter < 0 || 31 < *intParameter) {
        setError(instruction, BAD_REGISTER);
    }
}

int findRegisterNumber(char *mnemonic) {
    FILE *file = fopen("data/registers.txt", "r");
    int output = -1;
    int nbLine = 0;

    while (!feof(file) && output == -1) {
        char line[5];
        char *checkError = fgets(line, 5, file);

        if (strcmp(mnemonic, line) == 0) {
            output = nbLine;
        }

        nbLine++;
    }

    return output;
}