//gamelib.h
enum Stato_Giocatore {assassinato/*0*/, defenestrato/*1*/, astronauta/*2*/, impostore/*3*/};
enum Nome_Giocatore {Rosso/*0*/, Verde/*1*/, Giallo/*2*/, Blu/*3*/, Marrone/*4*/, Viola/*5*/, Bianco/*6*/, Nero/*7*/, Arancio/*8*/, Ciano/*9*/};
enum Tipo_Stanza {
  vuoto/*30 - 0*/,
  quest_semplice/*30 - 1*/,
  quest_complessa/*15 - 2*/,
  botola/*25 - 3*/
};

struct Giocatore {
  int ID;//Identificatore per giocatore
  int player;//per riconoscere il personaggio giocante -| 1-player e 0-"npc"
  int turno;//contatore per il chara fa una mossa, non si muoverà finchè tutti avranno lo stesso valore
  int sus_level;//in "chiamata_emergenza()", indica livello di sospetto
  enum Nome_Giocatore nome;// assegnata in "imposta_gioco()-crea_giocatori(int n)"
  enum Stato_Giocatore status;//si aggiorna durante la partita
  struct Giocatore *successivo;//puntatore al prossimo pg
  struct Stanza *posizione;//si aggiorna durante la partita
};

struct Stanza {
  int n_botola; //valore ausiliario per ricerca botole
  struct Stanza *next;//per aiutare lo scorrimento delle stanze, mettendole in fila una dopo l'altra
  // indirizzi hex ciclati per ogni direzione nello scorrere lista
  struct Stanza *Avanti;
  struct Stanza *Destra;
  struct Stanza *Sinistra;
  struct Stanza *Stanza_Precedente;//Per vedere quanti chara in stanza prima quando emergenza e impostore nella stanza morto
  enum Tipo_Stanza contenuto;//contenuto Stanza
  //0-non è stata chiamata || 1- emergenza stata chiamata nella stanza,non più richiamabile
  short emergenza_chiamata;
};

//PUNZIONI
void imposta_gioco();
  //dentro imposta_gioco();
  void stampa_giocatori();
  int inizia_gioco();

void gioca();
  //inizio gioco, le funzioni richiamabili sono static dentro "gamelib.c"

void termina_gioco();
  //esce dal programma e libera la memoria occupata
  //da menù e da menù_giocatore

int cond_gioco;//per controllare se si è impostato il gioco prima
