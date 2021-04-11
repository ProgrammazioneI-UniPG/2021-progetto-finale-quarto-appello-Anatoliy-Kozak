//gamelib.c
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"gamelib.h"

//INIZIALIZZAZZIONE DI FUNZIONI USATE POI
//*PRIMA OPZIONE "void imposta_gioco();" + stampa_giocatori() + inizia_gioco()*/
void nomi_random();
void crea_giocatori();
void crea_stanza_prima();

//*SECONDA OPZIONE*/
//funzioni interfaccia
static int avanza();
static void esegui_quest();
static void chiamata_emergenza();
static void uccidi_astronauta();
static int usa_botola();
static void sabotaggio();
//funzioni ausiliarie
  void muoviti(char direzione);

  void condizioni_GameOver();

  void stampa_nome_giocatore(int nome);

  void pulisciIDgiocatore();
  int giocatori_da_muovere(int t_corrente);
  struct Giocatore* trova_giocatore(int richiesta);

  int controllo_status_giocatori_stessa_stanza(int a);

  void mostra_stanza();
  void mostra_giocatori();

  void mostra_azioni_crew();
  void mostra_azioni_impostor();

  int trova_azione(char risp_azione);
  int trova_azione_NPC();

  void spedisci_a_botola(int v_rand);

//*TERZA OPZIONE "void termina_gioco()"*/
void libera_tutti();
  int elimina_giocatori();
  int elimina_stanze();

//Variabili globali
struct Giocatore *nuovo_giocatore; //Giocatore singolo \ !pNew
struct Giocatore *primo_giocatore;//Lista con tutti i giocatori \ !pFirst
struct Giocatore *lista_giocatori;// lista tutti giocatori \ !pScan

struct Giocatore *giocatore_di_turno;
unsigned short quest_da_finire = 0;
int n;//numero giocatori
int numero_botola = 0;
int player_impostore = 0;//0 crew | 1 impostor

int n_crewmate;
int n_impostor;

struct Stanza *stanza_inizio; //Prima stanza, serve per riunirsi in "chiamata_emergenza()"

struct Stanza *stanza_nuova; //Stanza appena creata \ !pNew
struct Stanza *stanza_prima; //Stanza appena creata \ !pFirst
struct Stanza *lista_stanze; //Lista delle stanze della partita \ !pScan
                            // La nuova stanza la salva direttamente nella direzione andata nello struct

//**********PRIMA OPZIONE***********/
//****DA MENU
void imposta_gioco(){
  //richiama solo funzioni e condizioni per attivarle
  int i, b=0;//numero giocatori

  puts("|*************************************|");
  puts("\n<-Benvenuto alle IMPOSTAZIONI->");
    do{
      puts("\n-> In quanti giocano? (minimo 4 - massimo 10)");
      printf("-> ");
      scanf("%d", &n);
      while(getchar() != '\n');//controllo caratteri

      if (n<4 || n>10) {
        puts("<!!!VALORE NON VALIDO, RIPROVA!!!>");
      }
    }while(n<4 || n>10);

    do{
      puts("\n-> Quante quest si desiderano far compiere agli astronauti? (minimo 1 - massimo 30)");
      printf("-> ");
      scanf("%d", &i);
      while(getchar() != '\n');//controllo caratteri

      if(i>0 && i<=30){
        quest_da_finire = i;
        b=1;
      } else {
        puts("<!!!VALORE NON VALIDO, RIPROVA!!!>");
        b=0;
      }
    }while(b == 0);

    //numero giocatori e quest preso, si vanno a creare giocatori, e messi nella prima stanza
    //AVVIENE AUTOMATICAMENTE NON APPENA SI ENTRA NEL IMPOSTA GIOCO
    if(stanza_prima != NULL){//nel caso che rimettono "imposta_gioco()" dopo esser tornati nel main menu
      //teoricamente sono solo queste a essere inizializzate all'inizio
      elimina_stanze();
    }
    //puts("<-CREATA STANZA INIZIALE->");
    crea_stanza_prima();//messo lettera per evitare di prendere una delle condizioni
//-------------------------------------------------------------------------------------
    if(primo_giocatore != NULL){
      elimina_giocatori();//toglie tutti i giocatori, reinizializza la 'stanza_inizio'
    }
    //puts("<-INIZIALIZZAZZIONE GIOCATORI->");
    crea_giocatori();
    nomi_random();//assegna nomi/colori randomi a tutti i chara

  //OPZIONI VISUALIZZABILI DI IMPOSTA GIOCO
  char risposta_imposta;
  do{
    puts("|-------------------------------------|");
    puts("\n->Scegli cosa fare:");
    puts("1. Stampa i giocatori presenti\n2. Torna al menù principale");
    printf("> ");
    risposta_imposta = getchar();
    while(getchar() != '\n');//controllo caratteri
    switch(risposta_imposta){
      case '1':
        //utilizzato per controllare che non escano caratteri come risposta
        stampa_giocatori();
        continue;
      case '2':
        //utilizzato per controllare che non escano caratteri come risposta
        if(inizia_gioco()){
          return;//ritorno al menu
        }
      default:
        puts("<!!!VALORE NON VALIDO, RIPROVA!!!>");
        continue;
    }
  }while(1);
}
//**************************************************************************************************************************/

//----DA MENU -IMPOSTAZIONI- visualizzabili e selezionabili----------------------------------------------------------------
  void stampa_giocatori(){
    int chara;
    lista_giocatori = primo_giocatore;//inizializzo la lista al primo elemento per poterla scorrere
    printf("\n");
    if(primo_giocatore == NULL){
      puts("<!!!LISTA VUOTA !ERRORE! RIPROVA INSERIMENTO!!!>");
      imposta_gioco();
    }

    do{
      chara = lista_giocatori->nome;
      switch (chara){
        case 0:
          printf("-Rosso");
          break;
        case 1:
          printf("-Verde");
          break;
        case 2:
          printf("-Giallo");
          break;
        case 3:
          printf("-Blu");
          break;
        case 4:
          printf("-Marrone");
          break;
        case 5:
          printf("-Viola");
          break;
        case 6:
          printf("-Bianco");
          break;
        case 7:
          printf("-Nero");
          break;
        case 8:
          printf("-Arancio");
          break;
        case 9:
          printf("-Ciano");
          break;
      }
      if(lista_giocatori->player == 1){
        printf("<-|TU|");
      }

      if(player_impostore == 1){
        if(lista_giocatori->status == 3){
          printf("<-|IMPOSTORE|");
        }
      }
      printf("\n");
      lista_giocatori = lista_giocatori -> successivo;
    }while(lista_giocatori != NULL);
  }

  int inizia_gioco(){
    puts("\n<--Verrai reindirizzato al menu principale-->");
    puts("|*************************************|\n");
    cond_gioco = 1;//nel main verrà riconosciuto e permettera di richiamare la funzione "gioca()"
    return 1;
  }
//----FINE OPZIONI VISUALIZZABILI------------------------------------------------------------------------------------------------------

    //SOLO DA FUNZIONI-non selezionabili, attivate automaticamente_____________________________________________________________________
    void nomi_random(){
      int array[n];
      for (int i = 0; i < n; i++) {     // riempi arrat
        array[i] = i;
      }
      for (int i = 0; i < n; i++) {    // mischia e randomizza
        int temp = array[i];
        int random = rand() % n;
        array[i]= array[random];
        array[random] = temp;
      }
      int i=0;
      lista_giocatori = primo_giocatore;
      do {
        lista_giocatori -> nome = array[i]; //assegna come nome
        i++;
        lista_giocatori = lista_giocatori->successivo;
      } while(lista_giocatori!=NULL);
    }

    void crea_giocatori(){

    //___CREA ELEMENTO GIOCATORE________________________________________
      for(int i=0; i<n; ++i){
      //...alloca memoria al giocatore
      nuovo_giocatore = (struct Giocatore*) malloc(sizeof(struct Giocatore));
      //...inizializza dati del giocatore
      nuovo_giocatore -> ID = i;//identificatore
      //il nome viene dato da una funzione subito dopo "crea_giocatori()"
      nuovo_giocatore -> player = 0;//setto tutti come NPC -> cambierà successivamente
      nuovo_giocatore -> status = 2;//astronauta -> cambierà successivamente
      nuovo_giocatore->sus_level = 0;//per chiamata emergenza, per "chiamata_emergenza()"
      nuovo_giocatore -> posizione = stanza_inizio;//creata in "crea_stanza_prima()", da dove inizia il gioco

      //distingue il primo inserito dagli altri
      if(primo_giocatore == NULL){
        //crea il primo elemento con il nuovo inserito
        primo_giocatore = nuovo_giocatore;
      } else {
        //dal secondo in poi invece...
        nuovo_giocatore -> successivo = primo_giocatore;//l'elemento successivo all'ultimo inserito è il preceente primo
        primo_giocatore = nuovo_giocatore;//l'ultimo inserito diventa il primo
      }
    }

  //_________DECIDE CHI é IL PLAYER____________________________________
    //...prende numero random, che sarà il ID del giocatore
    int player_selezionato;
    player_selezionato = (rand() %n);//seleziona numero random 0-n(numero player da tastiera)

    //...Scorro finchè non trovo elemento
    lista_giocatori = primo_giocatore;
    while(lista_giocatori!=NULL) {
      if(lista_giocatori -> ID == player_selezionato){
        lista_giocatori -> player = 1;
        //Questo diventa il player, e dopo, per riconoscerlo, basta fare condizione "player == 1"
        //lista_giocatori->status = 3;//per test
        //player_impostore = 1;//per test
        break;
      } else {
        //se non è allora scorre
        lista_giocatori = lista_giocatori->successivo;
      }
    }
    //puts("<--Gli astronauti sono stati scelti-->");

  //_______DECIDE CHI E QUANTI IMPOSTORI RANDOMICAMENTE_____________
    //da randomicamente un numero da 1 a 100
    int numero_impostori = 0;
    numero_impostori = ((rand()%100)+1);//da randomicamente un numero da 1 a 100
    //suddivisione in tre fascie di possibilita
    if(n<=6){
    //impostore: 1 probabile 80%, 2 raro 20%, 3 impossibile 0%
      if (numero_impostori <= 20){
        numero_impostori = 1;
      } else if (numero_impostori > 20 && numero_impostori <= 100){
          numero_impostori = 2;
      }
    }//opzione <=6

    else if(n<=8){
      //1 possibile 30%, 2 probabile 50%, 3 raro 20%
      if (numero_impostori <= 30){
        numero_impostori = 1;
    } else if (numero_impostori > 30 && numero_impostori <=80){
        numero_impostori = 2;
      } else if(numero_impostori > 80 && numero_impostori <=100){
          numero_impostori = 3;
        }
    }//opzione <=8

    else if(n<=10){
      //1 raro 20%, 2 probabile 40%, 3 possibile 30%
      if (numero_impostori <= 20){
        numero_impostori = 1;
      } else if (numero_impostori > 20 && numero_impostori <=70){
          numero_impostori = 2;
      } else if (numero_impostori > 70 && numero_impostori <=100){
          numero_impostori = 3;
      }
    }//opzione <=10

    //...imposto il numero dei impostori e dei astronauti
    n_impostor = numero_impostori;
    n_crewmate = n - numero_impostori;

    //...seleziono i impostori
    int nuovo;//per controllo sul numero
    do{
      do{
        nuovo = (rand() %n);
      }while(nuovo == player_selezionato);
      player_selezionato = nuovo;//OK

      lista_giocatori = primo_giocatore;//inizializzo la lista per cercare
      //...scorro finchè non trovo elemento
      while(lista_giocatori!=NULL){//scorre tutti gli elementi
        if(lista_giocatori -> ID == player_selezionato){//trova quello che mi serve
          if(lista_giocatori -> status == 2){
            if(lista_giocatori -> player){
              player_impostore = 1;
            }
            lista_giocatori -> status = 3;//diventa impostore
            break;
          } else {
            continue;
          }
        } else {
          //se non è allora scorre
          lista_giocatori = lista_giocatori->successivo;
          }
      }
      numero_impostori = numero_impostori - 1;
    }while(numero_impostori > 0);

    //puts("<--Gli impostori sono tra noi-->");
  }//FINE "creazione_giocatori()"

  //_______CREA LA STANZA INIZIALE, e si può riutilizzare_____________
    void crea_stanza_prima(){

      //...alloca memoria per la stanza
      stanza_nuova = (struct Stanza*) malloc(sizeof(struct Stanza));
      //...inizializza dati del giocatore
      stanza_nuova -> Avanti = NULL;
      stanza_nuova -> Destra = NULL;
      stanza_nuova -> Sinistra = NULL;
      stanza_nuova -> Stanza_Precedente = NULL;

      stanza_nuova -> emergenza_chiamata = 1;//poi sposta tutti (anche morti e defenestrati) nella stanza iniziale
      stanza_nuova -> contenuto = 0; //la prima stanza si preferisce farla vuota
      stanza_prima = stanza_nuova;  //crea il primo elemento con il nuovo inserito

      stanza_inizio = stanza_nuova; // così avrò da parte l'indirizzo della prima stanza per la "chiamata_emergenza()""
      stanza_inizio -> contenuto = 0; //la prima stanza si preferisce farla vuota
      stanza_inizio -> emergenza_chiamata = 1;//poi sposta tutti (anche morti e defenestrati) nella stanza inizial

      stanza_nuova -> next = stanza_prima;//puntatore indipendente dalla direzione presa
      stanza_prima = stanza_nuova;
    }//FINE "crea_stanza_prima()"
  //FINE FUNZIONI - non selezionabili, attivate automaticamente da altre funzioni_____________________________________________________________________

//**********SECONDA OPZIONE*******************************************************************************************************************************************************/
//*****DA MENU**************************************************************************************************************/
void gioca(){
  //SOLO DOPO aver chiamato "imposta_gioco()", inizia il gioco
  //Visualizzazzione dei comandi decisa da condizione del Giocatore giocante
  //foglio, forse è soliuzuibe
  int turno_corrente = 0;
  int chara_muovibili, chara_ID;//auslio per scelta prossimo giocatore
  int terminato;//controllo sul turno
  char risp_azione, controllo = 0;
  do{
    //si decide il primo a muoversi - logica identica alla scelta delle botole
    chara_muovibili = giocatori_da_muovere(turno_corrente);
    chara_ID = ((rand()%chara_muovibili)+1);//si sceglie il prossimo ID da muovere

    //comincia ricerca giocatore_di_turno
    lista_giocatori = primo_giocatore;
    do {
      //trova giocatore randomizzayo
      if(lista_giocatori->ID == chara_ID){
        giocatore_di_turno = lista_giocatori;
      }
      //cicla tutto
      lista_giocatori = lista_giocatori->successivo;
    } while(lista_giocatori!=NULL);
    //HO INDIRIZZO DEL GIOCATORE CHE SI FARà IL TURNO
    //controllo se devo fare il turno
    condizioni_GameOver();

    //---------------------------------------------------------------
    if(giocatore_di_turno->player == 1){
      //controllo se giocatore_di_turno è il giocatore giocante
      puts("<-TURNO GIOCATORE->\n");
      printf("|>Quest mandcanti: %d<|\n", quest_da_finire);
      if(giocatore_di_turno->status == 3){
        //gli impostori vedono quante vittime da uccidere
        printf("|>Vittime disponibili: %d<|\n", n_crewmate);
      }
      //mostra stanza
      puts("\n->Contenuto nella stanza");
      mostra_stanza();
      //visualizzabile da entrambi
      puts("\n\n|->AZIONI DISPONIBILI<-|\n");
      puts(">>>Vai in un'altra stanza - [v]");

      if((giocatore_di_turno->posizione) != stanza_inizio){
        //nella prima stanza si può solo muovere
        if(giocatore_di_turno->status == 2){
        //SE é GIOCANTE
          mostra_azioni_crew();
        } else if(giocatore_di_turno->status == 3){
          mostra_azioni_impostor();
        }
      }
      puts("|-------------------------------------|\n");

      controllo = 0;
      do{
        puts("->Che vuoi fare?");
        printf("-> ");
        risp_azione = getchar();
        while(getchar() != '\n');//controllo caratteri
        controllo = trova_azione(risp_azione);
      }while(controllo == 0);

    }else if(giocatore_di_turno->player == 0){
      //SE é NPC
      do{
        controllo = trova_azione_NPC();//Il ciclo e controllo è in funzione
      }while(controllo == 0);
    }
    if(giocatore_di_turno->player == 1){
      puts("\n<-TURNO TERMINATO->");
    }
    giocatore_di_turno -> turno = turno_corrente + 1;//questo turno si muoverà al prossimo giro

    //--------------------------------------------------------------------------------------------
    //controlla che tutti i giocatori abbiano mosso per questo turno
    terminato=0;
    lista_giocatori = primo_giocatore;
    do{
      if(lista_giocatori -> turno == turno_corrente) {
        terminato = terminato +  0;//se ci sono giocatori con il numero del turno corrente allora dovranno essere scelti
      } else {
          terminato = terminato + 1;//si vedrà che tutti hanno terminato se il contatore 'terminato' ha valore pari al numero di giocatori 'n'
      }
      lista_giocatori = lista_giocatori->successivo;
    }while(lista_giocatori!=NULL);
    //tutti hanno termiato il turno, si incrementa il contatore
    if(terminato == n){
      puts("\n|*************************************|\n");
      turno_corrente  = turno_corrente + 1;
      printf("\n<-|INIZIO NUOVO TURNO n.");
      printf("%d|->\n",turno_corrente+1);
    }
  }while(1);
}
//*****TERMINE GIOCA()*************************************************************************************************************/

  //------INTERFACCIA INGAME-----------------------------------------------------------------------------------------------
  //da entrambi -V<<<<<
  static int avanza(){

    int movimento_NPC;
    char movimento;
    //!!!!!se player giocante
    if(giocatore_di_turno->player == 1){
      do{
        puts("-->In che direzione?");
        puts("  a. AVANTI\n  d. DESTRA\n  s. SINISTRA");
        printf("--> ");
        movimento = getchar();

        if(movimento != 'a' && movimento != 'd' && movimento != 's'){
           puts("  <!!!VALORE NON VALIDO, si riprovi con i valori suggeriti!!!>");
           continue;
        }

        switch(movimento){
          case 'a'://avanti
          case 'A':
            while((movimento = getchar()) != '\n' && movimento != EOF);
            muoviti('a');
            return 1;
          case 'd'://fai quest
          case 'D':
            while((movimento = getchar()) != '\n' && movimento != EOF);
            muoviti('d');
            return 1;
          case 's'://chiamata emergenza
          case 'S':
            while((movimento = getchar()) != '\n' && movimento != EOF);
            muoviti('s');
            return 1;
          default:
            puts("  <!!!direzione non diponibile!!!>");
            return 0;
        }
      }while(1);

      //!!!se NPC
    } else if(giocatore_di_turno->player == 0){
        movimento_NPC = ((rand()%3)+1);//trova la mossa dell'impostore

        switch(movimento_NPC){
          case 1://avanti
            muoviti('a');
            return 1;
          case 2://fai quest
            muoviti('d');
            return 1;
          case 3://chiamata emergenza
            muoviti('s');
            return 1;
          default:
            puts("!!!ERR avanza()NPC!!!");
            return 0;
        }
      }
      //non raggiungibile
      return 0;
  }

  //solo crewmate - V
  static void esegui_quest(){
    //*solo una messa a 0 della funzione(magari riutilizza sabotaggio per cancellare la quest e risparmiare righe)
   //e sottrai il dovuto da 'quest_da_finire'*/
    if((giocatore_di_turno->posizione) -> contenuto == 1){
        (giocatore_di_turno->posizione) -> contenuto = 0;//toglie quest
        quest_da_finire = quest_da_finire - 1;//per poi togliere il dovuto
        if(giocatore_di_turno->player == 1){
          puts(" <-Quest semplice compiuta->");
        }
    }
    if((giocatore_di_turno->posizione) -> contenuto == 2){
        (giocatore_di_turno->posizione) -> contenuto = 0;//toglie quest
        quest_da_finire = quest_da_finire - 2;//per poi togliere il dovuto
        if(giocatore_di_turno->player == 1){
          puts(" <-Quest complessa compiuta->");
        }
    }
  }

  static void chiamata_emergenza(){
    //riunisci tutti nella prima stanza, se esce fuori uno da buttare fuori cambiare stato a defenestrato
    //Mostra a chiunque
    puts("  <-!!!EMERGENZA!!! - A rapporto nella stanza principale!->");
    //l'emergenza non è richiamabile nella stessa stanza
    (giocatore_di_turno->posizione)->emergenza_chiamata = 1;

    struct Giocatore *scan_giocatori;//variabile generale per scorrere la lista
    struct Giocatore *scan_sec;//variabile SECONDARIA per scorrere la lista ddentro quella generale

    //probabilita di discussione su chi esce
    int probabilita = 0;
    // le probabilita sono di base, il 30%, è vengono tolte direttamente della generazione del numero casuale
    int prob_defenestrato =((rand()%70)+1);//da randomicamente un numero da 1 a 100

    //scorre e calcola le probabilita
    scan_giocatori = primo_giocatore;
    do{
      //operazione fatta con i chara vivi
      if(scan_giocatori->status != 0 && scan_giocatori->status != 1){
        if(scan_giocatori->posizione == giocatore_di_turno->posizione){
          //calcolo probabilita, giocatore per giocatore nella stanza
          probabilita = 0;//rimetto possibilita a 0

          //SE GIOCATORE SELEZIONATO NELLA STANZA é UN ASTRONAUTA
          if(scan_giocatori->status == 2){
            scan_sec = primo_giocatore;

            for(int i=0; i<n; i++){
              // il ->sus_level cambia solo con i giocatori della stassa stanza
              if(scan_sec->status != 0 && scan_sec->status != 1){//i morti non parlano
                if(scan_sec->posizione == giocatore_di_turno->posizione){
                  if(scan_sec != scan_giocatori){
                    //in conteggio del livello di sospetto (->sus_level), cambierà come richiesto
                    if(scan_sec->status == 2){
                      //per ogni ASTRONAUTA nella stanza
                      probabilita = probabilita - 30;
                    }else if(scan_sec->status == 3){
                      //per ogni IMPOSTORE nella stanza
                      probabilita = probabilita + 20;
                    }
                  }
                }
              }
              scan_sec = scan_sec->successivo;
            }//for
            scan_giocatori->sus_level = probabilita;//Assegna al chara la probabilità calcolata
          }//conta dei astronauti nella stanza

          //SE GIOCATORE SELEZIONATO NELLA STANZA é UN IMPOSTORE
          if(scan_giocatori->status == 3){
            scan_sec = primo_giocatore;

            for(int i=0; i<n; i++){
              // il ->sus_level cambia solo con i giocatori della stassa stanza
              if(scan_sec->status != 0 && scan_sec->status != 1){//i morti non parlano
                if(scan_sec->posizione == giocatore_di_turno->posizione){
                  if(scan_sec != scan_giocatori){
                    //in conteggio del livello di sospetto (->sus_level), cambierà come richiesto
                    if(scan_sec->status == 2){
                      //per ogni ASTRONAUTA nella stanza
                      probabilita = probabilita + 20;
                    }else if(scan_sec->status == 3){
                      //per ogni IMPOSTORE nella stanza
                      probabilita = probabilita - 30;
                    }
                  }
                }
              }
              scan_sec = scan_sec->successivo;
            }//for
            scan_giocatori->sus_level = probabilita;//Assegna al chara la probabilità calcolata
          }//conta dei impostori nella stanza

        }//controllo posizione indiziato
      }//controllo sui morti

      scan_giocatori = scan_giocatori->successivo;
    }while(scan_giocatori!=NULL);

    //ADESSO DECIDE SE A VENIR BUTTATO FUORI é UN ASTRONAUTA O IMPOSTORE
    //trova il colpevole
    int sms_di_mancato_eject = 0;
    scan_giocatori = primo_giocatore;
    do{
      //vede la colpevolezza, del sospettato con livello più alto
      if(prob_defenestrato <= scan_giocatori->sus_level){
        printf("  >Analizzando la situazione, si decise che ");
        stampa_nome_giocatore(scan_giocatori->nome);
        printf("< verrà defenestrato\n");
        //stop lettura
        puts("  ->premere qualunque tasto per continuare");
        getchar();
        if(scan_giocatori->status == 3){
          n_impostor = n_impostor - 1;
        }
        //Se è il giocatore, lo termina direttamente la partita
        scan_giocatori->status = 1;
        if(scan_giocatori->player == 1){
          termina_gioco(1);
        }
        scan_giocatori->turno=1000000;//non si muoverà più
        sms_di_mancato_eject = 1;
        break;
      }else {
        sms_di_mancato_eject = 0;
        scan_giocatori = scan_giocatori->successivo;
      }
    }while(scan_giocatori!=NULL);
    if(sms_di_mancato_eject == 0){
      puts("  <-EJECT RIMANDATO->");
      //stop lettura
      puts("  ->premere qualunque tasto per continuare");
      getchar();
    }

    //I giocatori ritornato nella stanza principale(iniziale) per discutere chi buttare fuori..
    scan_giocatori = primo_giocatore;
      do {
        scan_giocatori->posizione = stanza_inizio;
        scan_giocatori->sus_level = 0;
        scan_giocatori = scan_giocatori->successivo;
      } while(scan_giocatori!=NULL);
    //elimina tutte le stanze, eccetto quella iniziale
    elimina_stanze();
    stanza_prima = stanza_inizio;
  }

  //solo impostori
  static void uccidi_astronauta(){
    struct Giocatore* scan;
    //*cambia stato giocatore ad assassinato - 0*/
    if(giocatore_di_turno->player == 1){
      puts(" |->LISTA DELLE POSSIBILI VITTIME<-|");
      //trova e stampa i giocatori eliminabili nella stanza
      scan = primo_giocatore;
      do {
        if(scan -> posizione == giocatore_di_turno -> posizione){
          if(scan -> status == 2){
            printf("    %d", scan->nome);
            stampa_nome_giocatore(scan->nome);
            printf("\n");
          }
        }
        scan = scan->successivo;
      } while(scan!=NULL);
    }
    //chiede la vittima
    int risp;
    char risposta;
    do{
      //CONTROLLA la natura del giocatore_di_turno, se è giocatore o npc
      if(giocatore_di_turno->player == 1){//interfaccia player
        //allora richiede tra i selezionati, chi uccidere
        puts("-->Chi vuoi uccidere?");
        printf("--> ");
        risposta = getchar();
        while(getchar() != '\n');//controllo caratteri
        risp = (int) risposta - 48;

      }else if(giocatore_di_turno->player == 0){//se NPC
        //allora randomizza la sua vittima
        risp = (rand()%10);//0-9
      }

      int conta = 0;
      //scorre i giocatori per trovare il giocatore selezionato
      scan = primo_giocatore;
      do{
        if(scan -> nome == risp && scan -> status != 0 && scan -> status != 1){
          if(giocatore_di_turno -> posizione == scan -> posizione){
            //trovato, esce dal ciclo
            conta = 0;
            break;
          } else if(giocatore_di_turno -> posizione != scan -> posizione){
            //non trovato, controllo negato, finchè non trova elemento per uscire dal ciclo
            conta = 1;
          }
        }else{
          //non è nemmeno l'elemento cercato e/o è assassinato/defeestrato
          conta = 1;
        }
        scan = scan->successivo;
      }while(scan != NULL);

      //controlla se NON è stato trovato l'elemento, altrimenti questo viene saltato
      if(conta == 1){
        if(giocatore_di_turno->player == 1){
          puts("  <-NON é PRESENTE NEI DINTORNI, uccidi solo quelli nella stanza->");
        }
        continue;
      }

      n_crewmate = n_crewmate - 1;

      //trova il richiesto
      switch(risp){
        case 0:
          scan=trova_giocatore(0);
          scan->status=0;
          scan->turno=10000000;//metto numero di turno teoricamente non raggiungibile
          if(giocatore_di_turno->player == 1){
            puts("  <-Giocatore ucciso->");
          }
          break;
        case 1:
          scan=trova_giocatore(1);
          scan->status=0;
          scan->turno=10000000;//metto numero di turno teoricamente non raggiungibile
          if(giocatore_di_turno->player == 1){
            puts("  <-Giocatore ucciso->");
          }
          break;
        case 2:
          scan=trova_giocatore(2);
          scan->status=0;
          scan->turno=10000000;//metto numero di turno teoricamente non raggiungibile
          if(giocatore_di_turno->player == 1){
            puts("  <-Giocatore ucciso->");
          }
          break;
        case 3:
          scan=trova_giocatore(3);
          scan->status=0;
          scan->turno=10000000;//metto numero di turno teoricamente non raggiungibile
          if(giocatore_di_turno->player == 1){
            puts("  <-Giocatore ucciso->");
          }
          break;
        case 4:
          scan=trova_giocatore(4);
          scan->status=0;
          scan->turno=10000000;//metto numero di turno teoricamente non raggiungibile
          if(giocatore_di_turno->player == 1){
            puts("  <-Giocatore ucciso->");
          }
          break;
        case 5:
          scan=trova_giocatore(5);
          scan->status=0;
          scan->turno=10000000;//metto numero di turno teoricamente non raggiungibile
          if(giocatore_di_turno->player == 1){
            puts("  <-Giocatore ucciso->");
          }
          break;
        case 6:
          scan=trova_giocatore(6);
          scan->status=0;
          scan->turno=10000000;//metto numero di turno teoricamente non raggiungibile
          if(giocatore_di_turno->player == 1){
            puts("  <-Giocatore ucciso->");
          }
          break;
        case 7:
          scan=trova_giocatore(7);
          scan->status=0;
          scan->turno=10000000;//metto numero di turno teoricamente non raggiungibile
          if(giocatore_di_turno->player == 1){
            puts("  <-Giocatore ucciso->");
          }
          break;
        case 8:
          scan=trova_giocatore(8);
          scan->status=0;
          scan->turno=10000000;//metto numero di turno teoricamente non raggiungibile
          if(giocatore_di_turno->player == 1){
            puts("  <-Giocatore ucciso->");
          }
          break;
        case 9:
          scan=trova_giocatore(9);
          scan->status=0;
          scan->turno=10000000;//metto numero di turno teoricamente non raggiungibile
          if(giocatore_di_turno->player == 1){
            puts("  <-Giocatore ucciso->");
          }
          break;
        default:
          puts("  <-VALORE NON VALIDO->");
          break;
      }
      break;
    }while(1);
    return;
  }

  static int usa_botola(){
    //""teleport"" a una zona randomica con una botola
    int l;
    if(numero_botola > 1){
      l = ((rand()%numero_botola)+1);//Random di numero stanze botole: numero stanze da 1 in poi -> Random da 1 in poi
      spedisci_a_botola(l);
      return 1;
    }else if(numero_botola < 1){
      if(giocatore_di_turno->player == 1){
        //se non ci sono botole con cui spostarsi, allora si avanza normalmente
        puts("  <-Non ci sono botole disponibili per lo spostamento, si avanzi normalmente...>");
        avanza();//se un impostore non trova la botola, si limiterà ad avanzare
        return 1;
        avanza();//se un impostore NPC non trova la botola, si limiterà ad avanzare
        return 1;
      }
    }
    return 0;
  }

  static void sabotaggio(){
  //toglie quest nella stanza di una quest, la fa diventare vuota
    lista_stanze = stanza_prima;
    //controlla in numero di botole
    if((giocatore_di_turno->posizione) -> contenuto == 1 || (giocatore_di_turno->posizione) -> contenuto == 2){
        (giocatore_di_turno->posizione) -> contenuto = 0;
    } else {
        puts(" <-Niente da sabotare->");
    }
  }
  //-----FINE - OPZIONI RICHIAMABILI DA MENU--------------------------------------------------------------------------------------------------------------

  /**/void muoviti(char direzione){

        //...alloca memoria per la stanza
        stanza_nuova = (struct Stanza*) malloc(sizeof(struct Stanza));
        //...inizializza dati del giocatore
        stanza_nuova -> Avanti = NULL;
        stanza_nuova -> Destra = NULL;
        stanza_nuova -> Sinistra = NULL;
        stanza_nuova -> Stanza_Precedente = NULL;
        stanza_nuova -> emergenza_chiamata = 0;//0 - non chiamato, quando viene chiamata emergenza passa a 1;

        //si decide il contenuto della stanza;
        int cont_scelto = ((rand()%100)+1);//da randomicamente un numero da 1 a 100

        if (cont_scelto <= 30){
          stanza_nuova -> contenuto = 0;
        } else if (cont_scelto <=60){
          stanza_nuova -> contenuto = 1;
        } else if (cont_scelto <=75){
          stanza_nuova -> contenuto = 2;
        } else if (cont_scelto <=100){
          stanza_nuova -> contenuto = 3;
        }

        //imposta variabili che facilitano l'utilizzo delle botole
        if(stanza_nuova -> contenuto == 3){
          numero_botola = numero_botola + 1;
          stanza_nuova -> n_botola = numero_botola;
        }

        switch (direzione){
          case 'a':
            if((giocatore_di_turno->posizione)->Avanti == NULL){

              (giocatore_di_turno->posizione)->Avanti = stanza_nuova;
              stanza_nuova->Stanza_Precedente = (giocatore_di_turno->posizione);
              giocatore_di_turno->posizione = stanza_nuova;

            }else if((giocatore_di_turno->posizione)->Avanti != NULL){
              giocatore_di_turno->posizione = (giocatore_di_turno->posizione)->Avanti;

            }

            //SALVA NELLA VISTA INDIPENDENTE
            stanza_nuova -> next = stanza_prima;
            stanza_prima = stanza_nuova;

            return;
            //------------------------------------------------------------
          case 'd':
            if((giocatore_di_turno->posizione)->Destra == NULL){

              (giocatore_di_turno->posizione)->Destra = stanza_nuova;
              stanza_nuova->Stanza_Precedente = (giocatore_di_turno->posizione);
              giocatore_di_turno->posizione = stanza_nuova;

            }else if((giocatore_di_turno->posizione)->Destra != NULL){
              giocatore_di_turno->posizione = (giocatore_di_turno->posizione)->Destra;
            }

            //SALVA NELLA VISTA INDIPENDENTE
            stanza_nuova -> next = stanza_prima;
            stanza_prima = stanza_nuova;

            return;
            //------------------------------------------------------------
          case 's':
            if((giocatore_di_turno->posizione)->Sinistra == NULL){

              (giocatore_di_turno->posizione)->Sinistra = stanza_nuova;
              stanza_nuova->Stanza_Precedente = (giocatore_di_turno->posizione);
              giocatore_di_turno->posizione = stanza_nuova;

            }else if((giocatore_di_turno->posizione)->Sinistra != NULL){
              giocatore_di_turno->posizione = (giocatore_di_turno->posizione)->Sinistra;
             }

             //SALVA NELLA VISTA INDIPENDENTE
             stanza_nuova -> next = stanza_prima;
             stanza_prima = stanza_nuova;

            return;
            //-------------------------------------------------------------------
          default:
            puts("<!!!ERRORE CREAZIONE STANZE!!!>");
            return;
        }
  }

  /**/void condizioni_GameOver(){
      //condizione morte giocatore_di_turno player | si "termina_gioco()" in automatico
        if(giocatore_di_turno -> player == 1){
          if(giocatore_di_turno -> status == 0){
            termina_gioco(1);
          }else if(giocatore_di_turno -> status == 1){
            termina_gioco(1);
          }
        }
      //condizione vittoria astronauti
        if(quest_da_finire <= 0){
          termina_gioco(2);
        }
      //condizione vittoria impostori
        if(n_crewmate == 0){//se non ci sono crewmate rimanenti...
          termina_gioco(3);
        }
      }

  /**/void stampa_nome_giocatore(int nome){
        switch (nome){
          case 0:
            printf(">Rosso");
            break;
          case 1:
            printf(">Verde");
            break;
          case 2:
            printf(">Giallo");
            break;
          case 3:
            printf(">Blu");
            break;
          case 4:
            printf(">Marrone");
            break;
          case 5:
            printf(">Viola");
            break;
          case 6:
            printf(">Bianco");
            break;
          case 7:
            printf(">Nero");
            break;
          case 8:
            printf(">Arancio");
            break;
          case 9:
            printf(">Ciano");
            break;
          }
      }

  /**/void pulisciIDgiocatore(){
      struct Giocatore* scan = primo_giocatore;
        do{
          scan->ID=0;//valore non assumibile normalmente da ID, e non capitabile con rand()%[],dato che utilizzato sui positivi
          scan = scan -> successivo;
        } while(scan!=NULL);
      }

      //SOLO FUNZIONI PER LA PARTITA
  /**/int giocatori_da_muovere(int t_corrente){
        //RITORNA NUMERO CHARA MOVIBILI NEL TURNO CORRENTE
        //funzionamento tipo BOTOLA, trova il numero dei pg disponibili a muoversi in questo turno
        pulisciIDgiocatore();//per ogni volta che verrà richiamata pulisce per riprendere il numero dei chara muovibili
        int cont=0;
        struct Giocatore* scan = primo_giocatore;
        do {
          //salta i chara morti
          if(scan->status != 0||scan->status != 1){
          //controlla in numero di giocatori muovibili
            if(scan -> turno == t_corrente){//se è nel turno corrente
              cont = cont + 1;//cosi si muovera anche quello con ID == 0
              scan -> ID = cont;//trovera poi con random
            }
            scan = scan->successivo;
          }
        } while(scan!=NULL);
          return cont;
      }

  /**/struct Giocatore* trova_giocatore(int richiesta){
        // per richiamare nella funzione un chara desiderato tramite nome
        //***Da accogliere in variabile "struct Giocatore*"***/
        //inizializzo al primo
        int presente = 0;
        struct Giocatore* scan = primo_giocatore;

        do {
          if(scan -> nome == richiesta){
            presente = 1;
            break;
          }
          scan = scan->successivo;
        }while(scan!=NULL);

        if(presente == 1){
          return scan;
        } else {
            return NULL;
        }
      }

  /**/void mostra_stanza(){
        int stanza;
        stanza = (giocatore_di_turno->posizione) -> contenuto;
        switch (stanza) {
          case 0:
            if(giocatore_di_turno->player == 1){
              puts(" >La stanza è vuota");
            }
            break;
          case 1:
            if(giocatore_di_turno->player == 1){
              puts(" >C'è una quest semplice");
            }
            break;
          case 2:
            if(giocatore_di_turno->player == 1){
              puts(" >C'è una quest complessa");
            }
            break;
          case 3:
          if(giocatore_di_turno->status == 3){//mostra la frase solo se sei impostore
            if(giocatore_di_turno->player == 1){
              puts(" >C'è una botola");
            }
          } else {//altrimenti per l'astronauta è solo vuota
              if(giocatore_di_turno->player == 1){
                puts(" >La stanza è vuota");
              }
            }
            break;
        }
        //cerca i giocatori nella stanza
        if(giocatore_di_turno->player == 1){
          //al NPC non gli serve vedere graficamenta quali giocatori ci sono
          mostra_giocatori();
        }
      }

  /**/void mostra_giocatori(){
        int presenza = 0;//vede se c'è almeno un chara con player
        struct Giocatore* scan = primo_giocatore;

        printf(" >Con te ");
        while(scan!=NULL) {
          if(scan -> posizione == giocatore_di_turno->posizione){
            //NON MOSTRA IL PLAYER IN SE
            if(scan->player == 1){
              scan = scan->successivo;
              continue;
            }
            //*I MORTI DEVONO ESSERE MOSTRATI, altrimenti non si potrebbe chiamare emergenza*/
            printf("\n");
            printf(" |");
            stampa_nome_giocatore(scan->nome);

            //MOSTRA STATO MOSTRABILE DEL CHARA
            //mostra gli alleati impostori se tu stesso sei impostore
            if(player_impostore == 1){
              if(scan->status == 3){
                printf("<-|ALLEATO IMPOSTORE|");
              }
            }
            //o in generale mostra se c'è un cadavere
            if(scan->status == 0){
                printf("<-|!!CADAVERE!!|");
            }
            presenza = 1;//può assumerlo solo se entra almeno una volta nel ciclo
          }
          scan = scan->successivo;
        }
        //Dato che, se non entra mai, rimarrà comunque 0, quindi sarai sempre solo
        if (presenza == 0){
          printf("non c'è nessuno, sei solo<-");
        }
      }

  /**/int controllo_status_giocatori_stessa_stanza(int a){
        int controllo_ritorno = 0;
        struct Giocatore* scan;

        scan = primo_giocatore;

        while(scan!=NULL) {
          //cicla tutti i ggiocatori
          if(scan -> status == a){
            //seleziona i astronauti
            if((giocatore_di_turno->posizione) == (scan->posizione)) {
              //se sono nella stessa posizione del impostore
              controllo_ritorno = controllo_ritorno + 1;
            } else {
              //se sono altrove
              controllo_ritorno = controllo_ritorno + 0;
            }
          }
          scan = scan->successivo;
        }

        //se c'è n'è almeno uno, quindi più di 0
        if(controllo_ritorno > 0){
          return 1;
        } else {//se non è non c'è allora
          return 0;
        }
      }

  /**/void mostra_azioni_crew(){
        //se è un'astronauta, può compiere le seguenti azioni
        //COMPIERE QUEST
        if((giocatore_di_turno->posizione)->contenuto == 1 || (giocatore_di_turno->posizione)->contenuto == 2){
          puts(">>>Completa la quest - [q]");
        }

        //CHIAMARE EMERGENZA - con funzione che controlla se nella stessa stanza hai gente mo
        if(controllo_status_giocatori_stessa_stanza(0)){//chuiede se ci sono assassinati nella stanza
          puts(">>>Chiamata emergenza - [e]");
        }
      }

  /**/void mostra_azioni_impostor(){
        //se è impostore, compirà altre operazioni

        if((giocatore_di_turno->posizione)->contenuto == 3){//se c'è botola
          puts(">>>Spostati con la botola - [b]");
        }
        if(controllo_status_giocatori_stessa_stanza(2)){//chiede se ci sono astronauti
          //vede se c'è almeno un astronauta nella stanza
          puts(">>>Uccidi - [u]");
        }
        if((giocatore_di_turno->posizione)->contenuto == 1 ||(giocatore_di_turno->posizione)->contenuto == 2){//se c'è quest
          puts(">>>Sabota la quest da fare - [s]");
        }
        return;
      }

  /**/int trova_azione(char risp_azione){
      int feed = 0;

        if(giocatore_di_turno->status == 2){
          //AZIONI ASTRONAUTI
          switch(risp_azione){
            case 'v'://avanti
            case 'V':
              avanza();
              return 1;
            case 'q'://fai quest
            case 'Q':
              if((giocatore_di_turno->posizione)->contenuto == 1 || (giocatore_di_turno->posizione)->contenuto == 2){
                esegui_quest();
                return 1;
              }else{
                if(giocatore_di_turno->player == 1){
                  puts("<!!!AZIONE NON VALIDA, non ci sono quest da fare!!!>");
                }
                return 0;
              }
            case 'e'://chiamata emergenza
            case 'E':
              if(controllo_status_giocatori_stessa_stanza(0) && (giocatore_di_turno->posizione)->emergenza_chiamata == 0){
                chiamata_emergenza();
                return 1;
              }else{
                if(giocatore_di_turno->player == 1){
                  if(controllo_status_giocatori_stessa_stanza(0) == 0){
                    puts("<!!!AZIONE NON VALIDA, emergenza non richiamabile qui!!!>");
                  }
                  if((giocatore_di_turno->posizione)->emergenza_chiamata == 1){
                    puts("<!!!AZIONE NON VALIDA, emergenza è stata già chiamata nella stanza!!!>");
                  }
                }
                return 0;
              }
            default://basic frase d'errore
              if(giocatore_di_turno->player == 1){
                puts("<!!!VALORE NON VALIDO!!!>");
              }
              return 0;
          }
        } else if(giocatore_di_turno->status == 3){
          //AZIONI IMPOSTORI
          switch(risp_azione){
            case 'v'://avanti
            case 'V':
              avanza();
              return 1;
            case 'u'://uccidi
            case 'U':
              if(controllo_status_giocatori_stessa_stanza(2) && giocatore_di_turno->posizione != stanza_inizio){
                uccidi_astronauta();
                return 1;
              } else {
                if(giocatore_di_turno->player == 1){
                  puts("<!!!AZIONE NON VALIDA, nessuno da uccidere!!!>");
                }
                return 0;
              }
            case 's'://sabota
            case 'S':
              if((giocatore_di_turno->posizione)->contenuto == 1 || (giocatore_di_turno->posizione)->contenuto == 2){
                sabotaggio();
                return 1;
              }else{
                if(giocatore_di_turno->player == 1){
                  puts("<!!!AZIONE NON VALIDA, nessuna quest nella stanza da sabotare!!!>");
                }
                return 0;
              }
            case 'b'://botola
            case 'B':
              if((giocatore_di_turno->posizione)->contenuto == 3){
                feed = usa_botola();
                return feed;
              }else{
                if(giocatore_di_turno->player == 1){
                  puts("<!!!AZIONE NON VALIDA, botola non presente nella stanza!!!>");
                }
                return 0;
              }
            default://basic frase d'errore
              if(giocatore_di_turno->player == 1){
                puts("<!!!VALORE NON VALIDO!!!>");
              }
              return 0;
          }
        } else {
          //puts("<-STATO GIOCATORE NON VIVO (trova_azione)->");
          return 0;
        }
      }

  /**/int trova_azione_NPC(){
        int uccidere, uscire = 0;
            //possibilmente manda una funzione che analizza la stanza e poi rendomizza il da farsi, se il random si può fare, allora lo ritorna, altrimenti, rirandomizza e prinde nuovo random
            if((giocatore_di_turno->posizione) == stanza_inizio){//se non si può chiamare l'emergenza allora non posso chiamare per i morti
              trova_azione('v');//VA AVANTI
            } else {
              //azioni esterne fattibilli
              if(giocatore_di_turno->status == 2){
                //SE é ASTRONAUTA
                //controlla se ci sono morti
                if(controllo_status_giocatori_stessa_stanza(0) == 1){//se c'e un assassinato
                  if((giocatore_di_turno->posizione)->emergenza_chiamata == 0){//se c'è un assassinato
                    trova_azione('e');
                  }
                } else if((giocatore_di_turno->posizione)->contenuto == 1 || (giocatore_di_turno->posizione)->contenuto == 2){//se c'è una quest
                  trova_azione('q');
                } else if ((giocatore_di_turno->posizione)->contenuto == 0){
                    uscire = trova_azione('v');//VA AVANTI
                    return uscire;
                  }
              }else if(giocatore_di_turno->status == 3){
                //SE NPC è IMPOSTORE
                //quando l'impostore si sarà spostato dalla prima stanza...deciderà a caso se uccidere o meno
                uccidere = ((rand()%3)+1);//1 possibilità su 3 di uccidere
                //controlla se c'è gente killabile
                if(controllo_status_giocatori_stessa_stanza(2)){//se c'è un astronauta
                  if(uccidere == 2){//1 possibilità su 3 di uccidere
                    trova_azione('u');
                  }
                }else if((giocatore_di_turno->posizione)->contenuto == 1 || (giocatore_di_turno->posizione)->contenuto == 2){//se c'è quest
                  trova_azione('s');//la sabota
                 }else if((giocatore_di_turno->posizione)->contenuto == 3){//se c'è una botola
                    uscire = trova_azione('b');
                    return uscire;
                  }else if((giocatore_di_turno->posizione)->contenuto == 0){//se nella stanza non c'è niente
                    trova_azione('v');//si muove
                   }
              }
             }//fine alternativa a stanza non iniziale
           return 1;
      }/**************************************************************************/

  /**/void spedisci_a_botola(int v_rand){
        //SOLO trasferire l'impostore alla prossima stanza
        int cont=0;
        struct Stanza *scan;
        scan = stanza_prima;

        do{
          //printf("cont%d\n", cont);
          //controlla che la botola di spostamento non sia quella in cui ci si trova
          if(v_rand == (giocatore_di_turno -> posizione)->n_botola){
            v_rand = ((rand()%numero_botola)+1);
            scan = stanza_prima;
          }
          //sposta il chara
          if(scan -> n_botola == v_rand){
            giocatore_di_turno -> posizione = scan;
            break;
          }
          cont = cont + 1;
          scan = scan -> next;
        }while(scan != NULL);
        return ;
      }

//_____FINE FUNZIONI AUSILIARIE_______________________________________________________________________________________________________________________

//**********TERZA OPZIONE***********/
//****DA MENU*********************************************************************************************************************************/
void termina_gioco(int riepilogo){
  //Richiama funzione libera_tutti() e chiude tutto co "exit(0)"
  //free anche sulla 'stanza_inizio'
  //riepilogo sulla partita-stampa in grosso i vincitori, stampa lista con vicino scritto stati della gente a fine partita,
  if(riepilogo == 1){//player morto
    puts("!!!<-GAME OVER - Sei morto->!!!");
    puts("\n<-Il gioco verrà terminato->");
  }
  if(riepilogo == 2){//Crew vince
    puts("<-VITTORIA DEGLI ASTRONAUTI!!!!->");
    puts("\n<-Il gioco verrà terminato->\n");

    puts("|||-Riepilogo dei giocatori-|||\n");
    puts("|-------------------------------------|\n");
    lista_giocatori = primo_giocatore;
    do {
    //controlla in numero di botole
      stampa_nome_giocatore(lista_giocatori->nome);
      if(lista_giocatori->player == 1){//TU
        printf("<-|TU|");
      }
      if(lista_giocatori->status == 0){//assassinato
        printf("<-|Ucciso|\n");
      }
      if(lista_giocatori->status == 1){//defenestrato
        printf("<-|Defenestrato|\n");
      }
      if(lista_giocatori->status == 2){//astronauta
        printf("<-|Astronauta|\n");
      }
      if(lista_giocatori->status == 3){//impostore
        printf("<-|Impostore|\n");
      }
      lista_giocatori = lista_giocatori->successivo;
    }while(lista_giocatori!=NULL);
    puts("|*************************************|\n");
  }
  if(riepilogo == 3){//impostori vincono
    puts("<-VITTORIA DEGLI IMPOSTORI!!!!->");
    puts("<-L'equipaggio è stato decimato->");
    puts("\n<-Il gioco verrà terminato->");
  }
  //quando ha finito di stampare libera tutti
  libera_tutti();
  puts("<-FINE PARTITA->");
  exit(0);
}
  //Richiamato da "termina_gioco()"
  void libera_tutti(){
    //Libera memoria co free()
    //elimina_stanze + elimina giocatori//
    int controllo_sms;

    controllo_sms = elimina_giocatori();
    if(controllo_sms == 0){
      puts("\n<-Gli astronauti sono stati congedati->");
    }

    controllo_sms = elimina_stanze();
    free(stanza_inizio);//questa è stata tenuta da parte
    if(controllo_sms == 0){
      puts("<-La nave è distrutta->\n");
    }
  }

    //SOLO DA FUNZIONI s
  int elimina_giocatori(){
      if(primo_giocatore == NULL){
        puts("<-?Non cè alcun giocatore?->");
        return 1;
      } else {
          //inizia cancellazione finche non ci sarà nessun prossimo
          struct Giocatore* scan;
          do{
            scan = primo_giocatore -> successivo;
            primo_giocatore = NULL;
            free(primo_giocatore);
            primo_giocatore = scan;
          } while(scan != NULL);
        }
      return 0;
    }

    //elimina tutto eccetto la stanza iniziale s
  int elimina_stanze(){
    //Evita di poter far scegliere una botola non consona dopo aver usato una volta "chiamata_emergenza()"
    numero_botola = 0;
      if(stanza_prima == NULL){
        puts("<-?Non cè alcuna stanza?->");
        return 1;
      } else {
          //inizia cancellazione finche non ci sarà nessun prossimo
          do{
            lista_stanze = stanza_prima -> next;
            if(lista_stanze != stanza_inizio){
              //essendo richiamata da "chiamata_emergenza()" durante la partita, non può cancellare la stanza iniziale,
              //la lista sarebbe vuota e i giocatori si ritroverebbero nel vuoto!!!
              //Si limita a uscire dal ciclo dato che teoricamente dopo questo non ci sono altre stanze salvate
              break;
            }
            stanza_prima = NULL;
            free(stanza_prima);
            stanza_prima = lista_stanze;
          }while(lista_stanze != stanza_inizio);//gira finche la stanza selezionata non è uguale alla stanza iniziale, PRIMO ELEMENTO DELLA LISTA
        }
      return 0;
    }
