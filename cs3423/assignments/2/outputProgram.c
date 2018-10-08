/**
author:     some student
**/
#include <stdio.h>

int main() {
    int numberIn;

    printf("Enter a number: ");

    scanf("%d", &numberIn);

    if (numberIn > 10) {
    //    add  two
        return numberIn + 2;
    } else if (numberIn < 5){
    //    subtract two   
        return numberIn - 2;

    return numberIn * 2;
}
