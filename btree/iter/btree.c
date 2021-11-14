/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree)
{
  (*tree) = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value)
{
  bool search = false;         //
  if (tree == NULL)            //pokud není nic ve stromu
  {                            //
    return search;             //nenalezeno
  }                            //
  else                         //jinak
  {                            //
    bst_node_t *where = tree;  //pomocná proměnná pro procházení
    while (where != NULL)      //zatímco je něco ve stromě
    {                          //
      if (where->key == key)   //pokud klíč v uzlu je stejný jako hledaný
      {                        //
        value = &where->value; //nahradím hodnotu
        search = true;         //nalezeno
        break;                 //
      }                        //
      if (where->key > key)    //pokud klíč v uzlu je větší než hledaný klíč
        where = where->left;   //tak jdu doleva
      else                     //jinak
        where = where->right;  //jdu doprava
    }                          //
    return search;             //nalezeno/nenalezeno
  }
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value)
{
  bst_node_t *where = (*tree);                                                     //pomocná proměnná pro procházení
  if (!(*tree))                                                                    //pokud je strom prázdný
  {                                                                                //
    bst_node_t *node = malloc(sizeof(bst_node_t));                                 //alokace paměti nového uzlu
    if (!node)                                                                     //pokud se nepovede malloc
      return;                                                                      //ukončím fci
    node->key = key;                                                               //inicializace klíče
    node->value = value;                                                           //hodnoty
    node->left = NULL;                                                             //levého potomka
    node->right = NULL;                                                            //pravého potomka
    (*tree) = node;                                                                //vložím do kořene
    return;                                                                        //ukončím fci
  }                                                                                //
  while (1)                                                                        //zatímco
  {                                                                                //
    if (where->key == key)                                                         //pokud je klič již ve stromě
    {                                                                              //
      where->value = value;                                                        //nahradím jeho hodnotu
      return;                                                                      //
    }                                                                              //
    if ((!where->left && where->key > key) || (!where->right && where->key < key)) //pokud
    {                                                                              //
      bst_node_t *node = malloc(sizeof(bst_node_t));                               //alokace paměti nového uzlu
      if (node == NULL)                                                            //pokud se nepovede malloc
        return;                                                                    //ukončím fci
      node->key = key;                                                             //inicializace klíče
      node->value = value;                                                         //hodnoty
      node->left = NULL;                                                           //levého potomka
      node->right = NULL;                                                          //pravého potomka
      if (where->key > key)                                                        //
        where->left = node;                                                        //vložím prvek
      else                                                                         //
        where->right = node;                                                       //vložím prvek
      return;                                                                      //ukončím fci
    }                                                                              //
    if (where->key > key)                                                          //pokud klíč v uzlu je větší než hledaný klíč
      where = where->left;                                                         //tak jdu doleva
    else                                                                           //jinak
      where = where->right;                                                        //jdu doprava
  }                                                                                //
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * 
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
  bst_node_t *node = (*tree);    //pomocná proměnná pro nejpravější
  while (node->right)            //zatímco vpravo ještě něco je
    node = node->right;          //přepíšu nejpravější
  target->key = node->key;       //přepíšu klíč targetu za nejpravější
  target->value = node->value;   //přepíšu hodnotu targetu za nejpravější
  bst_delete(tree, target->key); //zavolám funkci pro smazání
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key)
{
  bst_node_t *node = (*tree);                      //deklarace a inicializa uzlu
  bst_node_t *pre = (*tree);                       //předchozího uzlu
  while (node)                                     //pokud je nějaká hodnota
  {                                                //
    if (node->key > key)                           //pokud hledaný klíč je vlevo
    {                                              //
      pre = node;                                  //
      node = node->left;                           //
    }                                              //
    else if (node->key < key)                      //pokud je vpravo
    {                                              //
      pre = node;                                  //
      node = node->right;                          //
    }                                              //
    else if (!node->right && !node->left)          //pokud nemá uzel potomky
    {                                              //
      if (pre->right == node)                      //pokud je předchozí pravý roven uzlu
        pre->right = NULL;                         //nastavím ho na null
      else                                         //jinak
        pre->left = NULL;                          //nastavím vlevo
      free(node);                                  //uvolním uzel
      break;                                       //konec cyklu
    }                                              //
    else if (node->left && node->right)            //pokud uzel má oba potomky
      bst_replace_by_rightmost(node, &node->left); //nahradím nejpravějším
    else if (!node->left)                          //pokud uzel nemá levého potomka
    {                                              //
      if (pre->right == node)                      //předchozí vpravo je uzel
        pre->right = node->right;                  //předchozí pravý přepíšu uzlem pravým
      else                                         //jinak
        pre->left = node->right;                   //levý přepíšu
      free(node);                                  //uvolním uzel
      break;                                       //konec cyklu
    }                                              //
    else                                           //jinak
    {                                              //
      if (pre->right == node)                      //předchozí pravý je uzel
        pre->right = node->left;                   //předchozí pravý přepíšu uzlem levým
      else                                         //jinak
        pre->left = node->left;                    //levý přepíšu
      free(node);                                  //uvolním
      break;                                       //konec cyklu
    }
  }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree)
{
  stack_bst_t stack;                         //deklarace zásobníku
  stack_bst_init(&stack);                    //inicializace zásobníku
  if ((*tree) != NULL)                       //pokud je něco v stromu
  {                                          //
    stack_bst_push(&stack, (*tree));         //vložím do zásobníku vrchol
    bst_node_t *node = NULL;                 //pomocná proměnná pro průchod a mazání
    while (!stack_bst_empty(&stack))         //zatímco zásobník není prázdný tak
    {                                        //
      node = stack_bst_pop(&stack);          //vezmu vrchol zásobníku
      if (node->left != NULL)                //pokud má potomka vlevo
        stack_bst_push(&stack, node->left);  //vložím ho do zásobníku
      if (node->right != NULL)               //stejně pro pravou stranu
        stack_bst_push(&stack, node->right); //
      free(node);                            //uvolním uzel
    }                                        //
    (*tree) = NULL;                          //uvedu do stavu inicializace
  }
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit)
{
  while (tree != NULL)              //zatímco levá větev není null
  {                                 //
    stack_bst_push(to_visit, tree); //vložím na zásobník
    bst_print_node(tree);           //vypíšu
    tree = tree->left;              //posunu se dál
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree)
{
  stack_bst_t stack;                            //deklarace zásobníku
  stack_bst_init(&stack);                       //inicializace zásobníku
  bst_leftmost_preorder(tree, &stack);          //zavolám funkci pro nejlevější v preorder
  bst_node_t *node = tree;                      //deklarace pomocného uzlu
  while (!stack_bst_empty(&stack))              //zatímco není prázdný zásobník
  {                                             //
    node = stack_bst_pop(&stack);               //do uzlu uložím vrchol zásobníku a ten zahodím
    bst_leftmost_preorder(node->right, &stack); //nejlevější vpravo
  }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit)
{
  while (tree != NULL)              //zatímco levá větev není null
  {                                 //
    stack_bst_push(to_visit, tree); //vložím na zásobník
    tree = tree->left;              //posunu se dál
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree)
{
  stack_bst_t stack;                           //deklarace zásobníku
  stack_bst_init(&stack);                      //inicializace zásobníku
  bst_leftmost_inorder(tree, &stack);          //zavolám funkci pro nejlevější v preorder
  bst_node_t *node = tree;                     //deklarace pomocného uzlu
  while (!stack_bst_empty(&stack))             //zatímco není prázdný zásobník
  {                                            //
    node = stack_bst_pop(&stack);              //do uzlu uložím vrchol zásobníku a ten zahodím
    bst_print_node(node);                      //vypíšu
    bst_leftmost_inorder(node->right, &stack); //nejlevější vpravo
  }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit)
{
  while (tree != NULL)                  //zatímco levá větev není null
  {                                     //
    stack_bst_push(to_visit, tree);     //vložím na zásobník
    stack_bool_push(first_visit, true); //-||-
    tree = tree->left;                  //posunu se dál
  }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree)
{
  bool from_left;                                                   //deklarace log. prom.
  stack_bst_t bst_stack;                                            //deklarace zásobníku stromu
  stack_bool_t bool_stack;                                          //deklarace zásobníku bool
  stack_bst_init(&bst_stack);                                       //inicializace zásobníku
  stack_bool_init(&bool_stack);                                     //inicializace zásobníku
  bst_leftmost_postorder(tree, &bst_stack, &bool_stack);            //nejlevější
  while (!stack_bst_empty(&bst_stack))                              //zatímco zásobník není prázdný
  {                                                                 //
    tree = stack_bst_top(&bst_stack);                               //
    from_left = stack_bool_pop(&bool_stack);                        //
    if (from_left)                                                  //pokud zleva
    {                                                               //
      stack_bool_push(&bool_stack, false);                          //na log. zásobník dám false
      bst_leftmost_postorder(tree->right, &bst_stack, &bool_stack); //nejlevější vpravo
    }                                                               //
    else                                                            //jinak
    {                                                               //
      stack_bst_pop(&bst_stack);                                    //vyhodím vrchol zásobníku
      bst_print_node(tree);                                         //vypíšu
    }
  }
}
