/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
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
  *tree = NULL; //inicializace kořene stromu na NULL
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value)
{
  if (tree == NULL)                            //pokud není nic v kořenu
    return false;                              //ukončím s tím, že jsem nenašel
                                               //
  if (tree->key == key)                        //pokud je klíč v listu stejný jako hledaný klíč
  {                                            //
    value = &tree->value;                      //uložím hodnotu do value
    return true;                               //a ukončím s tím, že jsem našel
  }                                            //
                                               //
  if (tree->key > key)                         //pokud je klíč v listu větší jako klíč
    return bst_search(tree->left, key, value); //zavolám rekurzivně hledání vlevo s novým kořenem
  return bst_search(tree->right, key, value);  //zavolám rekurzivně hledání vpravo s novým kořenem
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value)
{

  if ((*tree) == NULL)                             //pokud ve stromu ještě není nic
  {                                                //
    bst_node_t *node = malloc(sizeof(bst_node_t)); //alokuji si nový list
    if (node == NULL)                              //pokud se nepovede malloc
      return;                                      //ukončím fci
    node->key = key;                               //inicializuji klíč v novém listu
    node->value = value;                           //-||- hodnotu
    node->left = NULL;                             //-||- levého potomka
    node->right = NULL;                            //-||- pravého potomka
    (*tree) = node;                                //vložím do kořene nový list
    return;                                        //ukončím fci
  }                                                //
  if ((*tree)->key == key)                         //pokud klíč v listu je stejný jako klíč
  {                                                //
    (*tree)->value = value;                        //nahradím hodnotu
    return;                                        //ukončím fci
  }                                                //
  if ((*tree)->key > key)                          //pokud je klíč v listu větší než klíč
  {                                                //
    bst_insert(&(*tree)->left, key, value);        //rekurzivní volání vložení pro levého potomka
    return;                                        //ukončení fce
  }                                                //
  bst_insert(&(*tree)->right, key, value);         //rekurzivní volání vložení pro pravého potomka
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
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
  if ((*tree) != NULL && target != NULL)                 //pokud nejsou NULL
  {                                                      //
    if ((*tree)->right == NULL)                          //pokud vpravo ještě něco je
    {                                                    //
      target->key = (*tree)->key;                        //nahradím klíč
      target->value = (*tree)->value;                    //a hodnotu
      bst_delete(tree, target->key);                     //uvolním nejpravější
    }                                                    //
    else                                                 //jinak
      bst_replace_by_rightmost(target, &(*tree)->right); //jdu doprava
  }
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
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key)
{
  if ((*tree) != NULL)                                            //pokud ve stromu něco je
  {                                                               //
    if ((*tree)->key > key)                                       //rušený klíč je vlevo
    {                                                             //
      bst_delete(&(*tree)->left, key);                            //jdu doleva
    }                                                             //
    else if ((*tree)->key < key)                                  //rušený klíč je vpravo
    {                                                             //
      bst_delete(&(*tree)->right, key);                           //jdu doprava
    }                                                             //
    else if ((*tree)->left == NULL && (*tree)->right == NULL)     //pokud rušený klíč nemá potomka
    {                                                             //
      free((*tree));                                              //uvolním
      (*tree) = NULL;                                             //
    }                                                             //
    else if ((*tree)->left != NULL && (*tree)->right != NULL)     //pokud má oba pomotky
    {                                                             //
      if ((*tree)->left->right != NULL)                           //pokud vlevo pravý není null
        bst_replace_by_rightmost((*tree), &(*tree)->left->right); //nejpravější uzel levého podstromu
      else                                                        //jinak
        bst_replace_by_rightmost((*tree), &(*tree)->left);        //levý prvek
    }                                                             //
    else if ((*tree)->left == NULL)                               //pokud nemá levého potomka
    {                                                             //
      bst_node_t *node = (*tree);                                 //
      (*tree) = (*tree)->right;                                   //
      free(node);                                                 //
    }                                                             //
    else                                                          //pokud nemá pravého potmka
    {                                                             //
      bst_node_t *node = (*tree);                                 //pomocná proměnná
      (*tree) = (*tree)->left;                                    //nastavím následující
      free(node);                                                 //uvolním
    }                                                             //
  }                                                               //
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree)
{
  if ((*tree) != NULL)
  {
    bst_dispose(&(*tree)->left);
    bst_dispose(&(*tree)->right);
    free((*tree));
    (*tree) = NULL;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree)
{
  if (tree != NULL)
  {
    bst_print_node(tree);
    bst_preorder(tree->left);
    bst_preorder(tree->right);
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree)
{
  if (tree != NULL)
  {
    bst_preorder(tree->left);
    bst_print_node(tree);
    bst_preorder(tree->right);
  }
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree)
{
  if (tree != NULL)
  {
    bst_preorder(tree->left);
    bst_preorder(tree->right);
    bst_print_node(tree);
  }
}
