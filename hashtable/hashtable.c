/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key)
{
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++)
  {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table)
{
  for (size_t i = 0; i < HT_SIZE; i++) //inicializace přes všechny prvky
  {                                    //v tabulce
    (*table)[i] = NULL;                //na NULL
  }                                    //
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key)
{
  int h = get_hash(key);             //získám hash z klíče
  ht_item_t *item = (*table)[h];     //pomocná proměnná pro daný řádek v tabulce s indexem h
  while (item != NULL)               //zatímco je nějaký prvek
  {                                  //
    if (strcmp(item->key, key) == 0) //pokud klíč prvku je hledaný klíč
      return item;                   //vrácení hodnoty po vyhledání
    item = item->next;               //jinak se posunu v tabulce dál
  }                                  //
  return NULL;                       //když je tabulka prázdná tak se vrátí NULL
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value)
{
  ht_item_t *item = ht_search(table, key);         //hledám jestli neexistuje daný klíč
  if (item != NULL)                                //pokud se nalezne
  {                                                //
    item->value = value;                           //změní se hodnota
    return;                                        //ukončí se fce
  }                                                //
  ht_item_t *new_item = malloc(sizeof(ht_item_t)); //alokace nového prvku
  if (new_item == NULL)                            //pokud se nepovede alokace
    return;                                        //konec fce
  new_item->key = key;                             //do alok. prvku vložím klíč
  new_item->value = value;                         //hodnotu
  new_item->next = NULL;                           //a ukazatel na další
  int h = get_hash(key);                           //získám z klíče hash
  if ((item = (*table)[h]))                        //pokud v tabulce na indexu něco je
    new_item->next = item;                         //do nového prvku nastavím next jako původní první
  (*table)[h] = new_item;                          //na začátek na indexu h nastavím nový prvek
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key)
{
  ht_item_t *item = ht_search(table, key); //zkusím vyhledat prvek
  if (item != NULL)                        //pokud byl nalezen
    return &(item->value);                 //vrátím ukazatel na hodnotu
  return NULL;                             //jinak vracím NULL
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key)
{
  int h = get_hash(key);
  ht_item_t *item = (*table)[h], *next_item = NULL, *prev_item = NULL;
  while (item != NULL)
  {
    next_item = item->next;
    if (strcmp(item->key, key) == 0)
    {
      free(item);
      if (!prev_item)
      {
        (*table)[h] = next_item;
        return;
      }
      prev_item->next = next_item;
      return;
    }
    item = item->next;
  }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table)
{
  ht_item_t *item, *item_to_delete;    //pomocné proměnné
  for (size_t i = 0; i < HT_SIZE; i++) //pro všechny indexy v tabulce
  {                                    //
    item = (*table)[i];                //nastavím první položku na indexu i do itemu
    while (item != NULL)               //zatímco item není NULL
    {                                  //
      item_to_delete = item;           //nastavím si první položku do item_to_delete
      item = item->next;               //nastavím následující prvek jako nový první prvek
      free(item_to_delete);            //uvolním prvek item_to_delete
    }                                  //
    (*table)[i] = NULL;                //inicializuje
  }                                    //
}
