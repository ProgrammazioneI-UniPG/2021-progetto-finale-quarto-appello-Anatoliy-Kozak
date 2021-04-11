//main
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"gamelib.h"

int main(){
  time_t t;
  srand ((unsigned) time(&t));

  int risposta;  /*printf("%d\n", rand() % 100); //Ritorna tra 0 e 99*/
  puts("|*************************************|");
  puts("<-BENVENUTO IN TRANNOI!!!->");
  puts("|*************************************|");
  do{
    puts("\n_-MENU PRINCIPALE-_");
    puts("1-> Imposta Gioco");
    puts("2-> Gioca");
    puts("3-> Termina Gioco");

    puts("\n>Scegli l'operazione da fare:");
    printf("> ");
    risposta = getchar();
    while(getchar() != '\n');//controllo caratteri

    switch(risposta){
      case '1':
        imposta_gioco();
        break;
      case '2':
        if(cond_gioco == 1){
          gioca();
        } else {
          puts("<!!!DEVI IMPOSTARE IL GIOCO PRIMA!!!>");
        }
        break;
      case '3':
        termina_gioco();
        break;
      default:
        puts("<!!!VALORE INSERITO NON VALIDO!!!>");
        break;
    }
  }while(cond_gioco != 3);
}
