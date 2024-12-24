List RemoveDuplicate(List l) {
    Photo* current = l;
    while (current) {
        Photo* runner = current;
        while (runner->next) {
            if (runner->next->ID == current->ID && 
                strcmp(runner->next->time, current->time) == 0 &&
                strcmp(runner->next->size, current->size) == 0 &&
                strcmp(runner->next->location, current->location) == 0) {
                runner->next = Delete(l, runner->next);
            } else {
                runner = runner->next;
            }
        }
        current = current->next;
    }
    return l;
}
