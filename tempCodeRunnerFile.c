

// Find a photo by ID and print its details
int FindByID(List l, int ID) {
    int pos = 1;
    while (l) {
        if (l->ID == ID) {
            printf("Anh voi ID %d\n", ID);
            printf("Vi tri: %d\n", pos);
            printf("Thoi gian: %s\n", l->time);
            printf("Kich thuoc: %s\n", l->size);
            printf("Dia diem: %s\n", l->location);
            return pos;
        }
        l = l->next;
        ++pos;
    }
    printf("Khong tim thay anh voi ID %d\n", ID);
    return -1;
}