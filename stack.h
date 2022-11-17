#ifndef STACK_FILE
#define STACK_FILE

/** Struktura stacku */
typedef struct stack
{
    int top;       ///< Vrchol stacku
    unsigned size; ///< Size stacku
    int *array;    ///< Stack
} * p_stack;

/**
 * Funkce inicializující stack.
 * @param size Maximální velikost stacku
 * @return Stack s maximání velikostí size
 */
p_stack stack_init(unsigned size);

/**
 * Pomocná funkce pro funkci Push - kontroluje přetečení stacku.
 * @param stack Kontrolovaný stack
 * @return 0 (false) při přetečení, jinak 1
 */
int checkOverflow(p_stack stack);

/**
 * Pomocná funkce pro funkci Pop - kontroluje podtečení stacku.
 * @param stack Kontrolovaný stack
 * @return 0 (false) při podtečení, jinak 1
 */
int checkUnderflow(p_stack stack);

/**
 * Funkce pro vložení do stacku. Posouvá top o +1.
 * @param stack Stack do kterého se vkládá
 * @param data Vkládaná value
 */
void push(p_stack stack, int data);

/**
 * Funkce pro odstranění ze stacku. Posouvá top o -1. (Hodnoty prvků zůstávají stejné??)
 * @param stack Stack do kterého se vkládá
 * @param data Vkládaná value
 */
void pop(p_stack stack, int data);

// TODO: Vyprázdnění stacku a alokované paměti

#endif