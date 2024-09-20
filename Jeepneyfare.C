#include <stdio.h>


int main() {
    float kiloMeters;
    float standardFare = 13;
    float jeepneyFare;
    float amount;
    float change;
    char  discount;
    
    printf("STUDENT = S\n");
    printf("SENIOR = R\n");
    printf("PWD = P\n");
    printf("NORMAL = N\n");
    printf("Are you a: ");
    scanf(" %c", &discount); 
    
    printf("Kilometers of your destination: ");
    scanf("%f", &kiloMeters); 
    
    switch(discount) {
        case 'N':
            jeepneyFare = kiloMeters * standardFare;
            printf("Your jeepney fare is %.2f Pesos", jeepneyFare);
            printf("\nEnter your amount: ");
            scanf("%f", &amount);
            change = amount - jeepneyFare;
            printf("\nYour change is : %.2f Pesos", change);
            break;
        
        case 'S':
            jeepneyFare = kiloMeters * standardFare * 0.2;  
            printf("Your student jeepney fare is %.2f Pesos", jeepneyFare);
            printf("\nEnter your amount: ");
            scanf("%f", &amount);
            change = amount - jeepneyFare;
            printf("Your change is : %.2f Pesos", change);
            break;
        
        case 'P':
            jeepneyFare = kiloMeters * standardFare * 0.2;  
            printf("Your PWD jeepney fare is %.2f Pesos", jeepneyFare);
            printf("\nEnter your amount: ");
            scanf("%f", &amount);
            change = amount - jeepneyFare;
            printf("Your change is : %.2f Pesos", change);
            break;
        
        case 'R':
            jeepneyFare = kiloMeters * standardFare * 0.2;  
            printf("Your senior jeepney fare is %.2f pesos", jeepneyFare);
            printf("\nEnter your amount: ");
            scanf("%f", &amount);
            change = amount - jeepneyFare;
            printf("Your change is : %.2f Pesos", change);
            break;
        
        default:
            printf("Invalid discount type.\n");
            break;
    }
    
    return 0;
}
