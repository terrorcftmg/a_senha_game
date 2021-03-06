#include <stdlib.h>
#include <string.h>
#include "../headers/interfaceControl.h"
#include "../headers/attemptProcessor.h"
#include "../headers/codeGenerator.h"
#include "../headers/gameCore.h"
#include "../struct/gameData.h"
#include "../struct/gameMacros.h"

gameData *gameInstance;

/**
 * Função responsável por inicializar
 * os dados do jogo corrente.
*/
void initializeGameData(){
    gameInstance = (gameData *)malloc(sizeof(gameData));
    gameInstance->difficulty = 0;
}

/**
 * Função responsável por imprimir o 
 * menu principal e ler a opção
 * escolhida pelo jogador.
 * 
 * @return {int} Opção escolhida pelo
 *  jogador.
*/
int mainMenu(){
    int opt=0;

    do{
        interfacePrintMenu(MAIN_MENU_SIZE, MAIN_MENU);
        interfaceScan("%d", &opt);
    } while(opt<0 || opt>3);

    return opt;
}

/**
 * Função responsável por imprimir o 
 * menu principal e alterar a dificuldade
 * de acordo com a opção escolhida pelo
 * jogador.
*/
void changeDifficult(){
    int opt=0;

    do{
        interfacePrintMenu(DIFFIC_MENU_SIZE, DIFFIC_MENU);
        interfaceScan("%d", &opt);
    } while(opt<1 || opt>4);

    gameInstance->difficulty = opt;
    interfacePrint(DIFFIC_DEFINED_TO, opt);
    waitKeyPress();
}

/**
 * Função responsável por gerar um
 * novo código definindo suas
 * propriedades de acordo com a 
 * dificuldade do jogo escolhido.
 * 
 * @return {gameCode*} ponteiro
 *  para uma estrutura de senha.
*/
gameCode* newCode(){
    gameCode *myCode = getCode();

    switch(gameInstance->difficulty){
        case 1:
            myCode->length = 3;
            myCode->maxRange = 4;
            myCode->allowRepeat = 1;
            break;

        case 2:
            myCode->length = 4;
            myCode->maxRange = 5;
            myCode->allowRepeat = 0;
            break;

        case 3:
        case 4:
            myCode->length = 4;
            myCode->maxRange = 9;
            myCode->allowRepeat = 0;
            break;
    }

    generateCode(myCode);
    return myCode;
}

/**
 * Função responsável por relizar o fluxo principal
 * de checar a tentativa do usuário e imprimir as
 * respostas.
*/
void gameProcedure(){
    if(!gameInstance->difficulty){
        interfacePrint(DIFFIC_NOT_DEFINED);
    } else {
        gameCode *code = newCode();
        if(gameInstance->difficulty == 4){
            interfacePrint(TEST_MODE, code->codeValue);
        }

        interfacePrint(GENERATED_CODE);

        int *attemptResult;
        int attemptCount = 1;
        char input[code->length];
        int exitFlag = 0;
        
        do{
            interfaceScan("%s", input);
            if(!strcmp(input, (char*)GAME_EXIT_CODE)){

                exitFlag = 1;
                interfacePrint(CODE_IS, code->codeValue);
                waitKeyPress();

            } else {

                attemptResult = processAttempt(*code, input);
                if(attemptResult!=NULL){
                    interfacePrint("| %d %d |\n", attemptResult[0], attemptResult[1]);
                    if(attemptResult[0]==code->length){
                        exitFlag = 1;
                        interfacePrint(CONGRAT_FOUND_CODE, attemptCount);
                        waitKeyPress();
                    }
                }

            }

            attemptCount++;
        } while(!exitFlag);
    }
}

/**
 * função responsável por realizar o fluxo
 * do jogo, de selecionar os menus e 
 * definir propriedades do jogo.
 * 
 * @param {int} paramDiff Indicador de uso
 *  de passagem de dificuldade pela 
 *  linha de comando.
*/
void runGame(int paramDiff){
    initializeGameData();

    if(!paramDiff){
        system(CLR_SCR); //removeThat
        interfacePrint(SELECT_DIFF);
        changeDifficult();
    } else {
        gameInstance->difficulty = paramDiff;
    }

    int menuOpt = 0;
    do{
        system(CLR_SCR); //removeThat
        menuOpt = mainMenu();
        switch(menuOpt){
            case 1:
                system(CLR_SCR); //removeThat
                changeDifficult();
                break;
            case 2:
                gameProcedure();
                break;
        }
    } while(menuOpt!=3);
}
