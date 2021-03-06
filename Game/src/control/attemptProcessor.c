#include <stdlib.h>
#include <string.h>
#include "../headers/interfaceControl.h"
#include "../headers/attemptProcessor.h"
#include "../struct/gameMacros.h"

/**
 * Função responsável por verificar se 
 * possui na String um caracter ASCII que
 * não representa um digito entre 0 e 9.
 * 
 * @param {char *} str String a ser 
 *  verificada.
 * 
 * @return {int} Retorna 1 se possui um
 *  caracter não numerico e 0 se não.
*/
int hasNaNChar(char *str){
    int hasNaN = 0;

    for(int i = 0; i<strlen(str); i++){
        if(str[i]<'0' || str[i]>'9'){
            hasNaN = 1;
        }
    }
    
    return hasNaN;
}

/**
 * Função responsável por verificar se 
 * possui na String algum digito maior
 * que o máximo permitido na senha.
 * 
 * @param {char *} str String a ser 
 *  verificada.
 * @param {int} maxRange Máximo permitido
 *  para senha gerada.
 * 
 * @return {int} Retorna 1 se possui um
 *  digito maior e 0 se não.
*/
int hasGrThanMaxRange(char *str, int maxRange){
    int hasGreaterThan = 0;

    for (int i = 0; i < strlen(str); i++) { 
        if((str[i]-'0')>maxRange){
            hasGreaterThan = 1;
        }
    } 

    return hasGreaterThan;
}

/**
 * Verifica se a entrada é válida de acordo
 * com os parametros da senha gerada.
 * Retorna:
 *  0 - Válido.
 *  1 - Senha de tamanho diferente.
 *  2 - Caracter não numérico.
 *  3 - Digito maior que o maior permitido.
 * 
 * @param {char} *input String correspondente
 *  a entrada do usuário.
 * @param {gameCode} gCode Senha gerada pelo
 *  sistema.
 * 
 * @return {int} Còdigo de validade do 
 *  parametro testado.
*/
int checkValidInput(char *input, gameCode gCode){
    int validFlag = 0;

    if(strlen(input)!=gCode.length){
        validFlag = 1;
    } else if(hasNaNChar(input)){
        validFlag = 2;
    } else if(hasGrThanMaxRange(input, gCode.maxRange)){
        validFlag = 3;
    }

    return validFlag;
}


/**
 * Função responsável por processar o input
 * válido do usuário e retornar as informações
 * sobre sua tentativa.
 * 
 * @param {char} *generatedCode String correspondente
 *  à senha gerada pelo jogo.
 * @param {char} *inputedCode String correspondente
 *  à senha inserida pelo jogador.
 * 
 * @return {int*} Array de Posições Corretas, Numeros
 *  Corretos.
*/
int* processInputedCode(char *generatedCode, char *inputedCode){
    int correctNum = 0;
    int correctPos = 0;
    int gCodeLen = strlen(generatedCode);
    static int arrayResult[2];

    char toProccessCode[gCodeLen];
    strcpy(toProccessCode, generatedCode);

    for(int i=0; i<gCodeLen; i++){
        if(toProccessCode[i]==inputedCode[i]){
            inputedCode[i] = '@';
            toProccessCode[i] = '@';
            correctPos++;
        }
    }
    
    for(int i=0; i<gCodeLen; i++){
        if(inputedCode[i]!='@' && strchr(toProccessCode, inputedCode[i])){
            correctNum++;
        }
        
    }

    arrayResult[0] = correctPos;
    arrayResult[1] = correctNum;

    return arrayResult;
}

/**
 * Função responsável por verificar
 * se a tentativa inserida pelo usuário é
 * válida. Após a verificação, imprime
 * a mensagem correspondente. 
 * Retorna um ponteiro para um array
 * de inteiros no caso de uma entrada valida, no
 * formato { posicoes_corretas, numeros_corretos },
 * sendo que numeros_corretos estão na posição
 * errada. Caso a entrada seja invalida, retorna
 * NULL e imprime o erro na tela.
 * 
 * @param {gameCode *} gCode Referencia à uma
 *  senha gerada pelo jogo.
 * @param {char *} playerInput Entrada do jogador
 *  no jogo.
 * 
 * @return {int*} Ponteiro para array de respostas
 *  ou NULL, de acordo com a validade do input.
*/
int* processAttempt(gameCode gCode, char playerInput[]){
    int validationCode = checkValidInput(playerInput, gCode);
    int *response = malloc(sizeof(int));

    if(!validationCode){ 
        response = processInputedCode(gCode.codeValue, playerInput);
    } else {
        interfacePrint(INVALID_INPUT);
        
        switch (validationCode){
            case 1:
                interfacePrint(DIFFERENT_SIZE);
                break;

            case 2:
                interfacePrint(HAS_NAN_CHAR);
                break;

            case 3:
                interfacePrint(GREATER_THAN_MAX_RANGE);
                break;
        
            default:
                break;
        }

        response = NULL;
    }

    return response;
}