#include<stdio.h>

/**
 * \file
 * \warning numbers are considered integer, so their average is double
 * \warning main(void) is there only for ejudge
 * \warning in this example an average of 5 numbers is calculated
 */

/*!
    \brief calculate average of n numbers taken from input 
    \param n amount of input numbers
    \return average of n numbers 
*/
double average(int n){
    int i, sum = 0, tmp;
    for(i = 0; i < n; i++){
        scanf("%d", &tmp);
        sum += tmp;
    }
    return (double)sum / n;
}

int main(void){
    printf("%lf", average(5));
}
