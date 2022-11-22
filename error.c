#include "error.h"

/** Seznam chyb definovaný jako globální proměnná pro odstranění nutnosti jej předávat
 *  do každé funkce, která by mohla chybovat.
 *  Není určen k přímému přístupu - využijte přiložených funkcí error_list_init, error a error_eval.
*/
p_error_list errors;

void error_list_init()
{
    errors = (p_error_list)malloc(sizeof(struct error_list));
    if(errors == NULL)
    {
        return;
    }
    errors->data = NULL;
    errors->next = NULL;
}

void error(int line, char* message, error_code error_code)
{
    if (errors == NULL)
    {
        fprintf(stderr, "Interní chyba: Před voláním funkce error je třeba zavolat error_list_init v těle funkce main!\n");
        return;
    }

    p_error data = (p_error)malloc(sizeof(struct error));
    if(data == NULL)
    {
        return;
    }
    data->line = line;
    data->message = message;
    data->error_code = error_code;

    if (errors->data == NULL)
    {
        errors->data = data;
        return;
    }

    p_error_list new_node = (p_error_list)malloc(sizeof(struct error_list));
    if(new_node == NULL)
    {
        return;
    }
    new_node->data = data;
    
    if(errors->next == NULL)
    {
        errors->next = new_node;
        return;
    }


    p_error_list err_node = errors;

    while (err_node->next != NULL)
    {
        err_node = err_node->next;

        if (err_node->next == NULL)
        {
            err_node->next = new_node;
            return;
        }
    }
    
}

int error_eval()
{
    // Žádná chyba -> vracíme 0 (úspěch)
    if (errors->data == NULL)
    {
        return 0;
    }

    int return_code = errors->data->error_code;
    
    fprintf(stderr, "Překladač se ukončil s chybovou návratovou hodnotou %d\n\n", return_code);

    error_out(errors->data);

    while (errors->next != NULL)
    {
        p_error_list deleted = errors;
        errors = errors->next;
        free(deleted->data);
        free(deleted);
        error_out(errors->data);
    }

    free(errors->data);
    free(errors);
    
    return return_code;
}

void error_out(p_error error)
{
    // LEX_ERROR = 1,
    // SYNTAX_ERROR = 2,
    // SEM_UNDEFINED_FUNC_ERROR = 3,
    // SEM_INVALID_CALL_ERROR = 4,
    // SEM_UNDEFINED_VAR_ERROR = 5,
    // SEM_INVALID_RETURN_ERROR = 6,
    // SEM_INVALID_TYPE_ERROR = 7,
    // SEM_CRITICAL_ERROR = 8,
    // SEM_INTERNAL_ERROR = 99,
    char * error_code_desc;
    
    switch (error->error_code)
    {
    case LEX_ERROR:
        error_code_desc = "Chyba lexikální analýzy";
        break;
    case SYNTAX_ERROR:
        error_code_desc = "Chyba syntaktické analýzy";
        break;
    case SEM_UNDEFINED_FUNC_ERROR:
        error_code_desc = "Sémantická chyba - nedefinovaná funkce nebo pokus o redefinici";
        break;
    case SEM_INVALID_CALL_ERROR:
        error_code_desc = "Sémantická běhová chyba - špatný počet/typ parametrů u volání funkce či typ návratové hodnoty z funkce.";
        break;
    case SEM_UNDEFINED_VAR_ERROR:
        error_code_desc = "Sémantická chyba v programu – použití nedefinované proměnné.";
        break;
    case SEM_INVALID_RETURN_ERROR:
        error_code_desc = "Sémantická/běhová chyba v programu – chybějící/přebývající výraz v příkazu návratu z funkce.";
        break;
    case SEM_INVALID_TYPE_ERROR:
        error_code_desc = "Sémantická/běhová chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech.";
        break;
    case SEM_OTHER_ERROR:
        error_code_desc = "Sémantická chyba";
        break;
    case SEM_INTERNAL_ERROR:
        error_code_desc = "Interní chyba překladače";
        break;
    
    default:
        break;
    }

    
    fprintf(stderr, "[Řádek %d] %s\n\t%s\n", error->line, error_code_desc, error->message);
}