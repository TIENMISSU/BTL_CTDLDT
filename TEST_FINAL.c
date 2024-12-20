#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct SPhoto {
    int ID;
    char time[20];
    char size[20];
    char location[50];
    struct SPhoto* next;
} Photo;

typedef Photo* List;

// Global variable to keep track of the next available ID
int nextID = 1001;

List LInit() {
    return NULL;
}

void Print(const List l) {
    for (List temp = l; temp; temp = temp->next) {
        printf("%d %s %s %s\n", temp->ID, temp->time, temp->size, temp->location);
    }
}

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

void FindByDateAndLocation(List l, int year, int month, int day, int hour, const char* location) {
    int found = 0;
    char date[9];
    sprintf(date, "%04d%02d%02d", year, month, day);

    while (l) {
        // Kiểm tra ngày tháng năm và địa điểm
        if (strncmp(l->time, date, 8) == 0 &&
            (hour == -1 || (hour != -1 && strncmp(l->time + 9, "00", 2) == 0)) &&
            (strcmp(location, "-1") == 0 || strcmp(l->location, location) == 0)) {
            printf("ID: %d\nThoi gian: %s\nKich thuoc: %s\nDia diem: %s\n", l->ID, l->time, l->size, l->location);
            found = 1;
        }
        l = l->next;
    }

    if (!found) {
        printf("Khong tim thay anh voi ngay %s va dia diem %s\n", date, location);
    }
}

List Insert(List l, const char* time, const char* size, const char* location) {
    // Tạo ID mới tự động
    int ID = nextID++;
    Photo* e = (Photo*)malloc(sizeof(Photo));
    e->ID = ID;
    strcpy(e->time, time);
    strcpy(e->size, size);
    strcpy(e->location, location);
    e->next = NULL;

    // Nếu danh sách rỗng, trả về phần tử mới
    if (!l) return e;

    // Chèn phần tử mới vào vị trí thích hợp
    Photo* current = l;
    Photo* prev = NULL;
    while (current && (strcmp(current->time, time) < 0 || (strcmp(current->time, time) == 0 && strcmp(current->location, location) < 0))) {
        prev = current;
        current = current->next;
    }
    e->next = current;
    if (prev) {
        prev->next = e;
    } else {
        l = e;
    }
    return l;
}

List Delete(List l, Photo* a) {
    if (!l || !a) return l;
    if (l == a) {
        List temp = l->next;
        free(l);
        return temp;
    }

    Photo* current = l;
    while (current->next && current->next != a) {
        current = current->next;
    }
    if (current->next == a) {
        current->next = a->next;
        free(a);
    }
    return l;
}

void Edit(List l, int eID, const char* eTime, const char* eSize, const char* eLocation) {
    while (l) {
        if (l->ID == eID) {
            if (eTime && strlen(eTime) > 0) strcpy(l->time, eTime);
            if (eSize && strlen(eSize) > 0) strcpy(l->size, eSize);
            if (eLocation && strlen(eLocation) > 0) strcpy(l->location, eLocation);
            printf("Da sua thong tin anh.\n");
            return;
        }
        l = l->next;
    }
    printf("Khong tim thay ID anh.\n");
}

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

int main() {
    List L1 = LInit();

    // Mang chua dia diem va kich thuoc 
    const char* locations[8] = { "Ha_Noi", "Bac_Ninh", "Hai_Phong", "Quang_Ninh", "Thai_Binh", "Nam_Dinh", "Bac_Giang", "Lang_Son" };
    const char* sizes[4] = {"3840x2160", "1920x1080", "2560x1440", "1280x720"};

    // Tao 100 anh mau 
    for (int i = 1; i <= 100; i++) {
        int hour = 8 + (i / 5) % 12;
        int minute = (i % 5) * 10;
        char time[20];
        snprintf(time, sizeof(time), "202401%02d_%02d%02d", 1 + i/50, hour, minute);
        L1 = Insert(L1, time, sizes[i % 4], locations[i % 8]);
    }

    while (1) {
        printf("\n=== QUAN LY ALBUM ANH ===\n");
        printf("1. Them anh moi\n");
        printf("2. Tim kiem anh theo ID\n");
        printf("3. Xoa anh\n");
        printf("4. Sua thong tin anh\n");
        printf("5. Hien thi tat ca anh\n");
        printf("6. Xoa anh trung lap\n");
        printf("7. Tim kiem anh theo ngay va dia diem\n");
        printf("0. Thoat\n");
        printf("Lua chon cua ban: ");
        int choice;
        scanf("%d", &choice);

        if (choice == 0) break;

        switch (choice) {
            case 1: {
                printf("\n--- THEM ANH MOI ---\n");
                int year, month, day, hour = -1, minute = -1;
                char size[20] = "1920x1080"; // default size
                char location[50];

                // Nhập ngày tháng năm
                do {
                    printf("Nhap ngay (1-31, nhap -1 de bo qua): ");
                    scanf("%d", &day);
                } while ((day < 1 || day > 31) && day != -1);

                do {
                    printf("Nhap thang (1-12, nhap -1 de bo qua): ");
                    scanf("%d", &month);
                } while ((month < 1 || month > 12) && month != -1);

                do {
                    printf("Nhap nam (4 chu so, nhap -1 de bo qua): ");
                    scanf("%d", &year);
                } while ((year < 1000 || year > 9999) && year != -1);

                // Nhập giờ và phút (có thể có hoặc không)
                printf("Nhap gio (0-23, nhap -1 de bo qua): ");
                scanf("%d", &hour);
                if (hour != -1) {
                    do {
                        printf("Nhap phut (0-59): ");
                        scanf("%d", &minute);
                    } while (minute < 0 || minute > 59);
                }

                // Nhập địa điểm
                printf("Nhap dia diem (VD: Ha_Noi): ");
                scanf("%s", location);

                // Nhập kích thước (có thể có hoặc không)
                char inputSize[20];
                printf("Nhap kich thuoc (co the co hoac khong, nhap 'default' de bo qua, VD: 3840x2160): ");
                scanf("%s", inputSize);
                if (strcmp(inputSize, "default") != 0) {
                    strcpy(size, inputSize);
                }

                // Tạo chuỗi thời gian
                char time[20];
                if (hour == -1) {
                    snprintf(time, sizeof(time), "%04d%02d%02d", year, month, day);
                } else {
                    snprintf(time, sizeof(time), "%04d%02d%02d_%02d%02d", year, month, day, hour, minute);
                }

                // Thêm ảnh vào danh sách
                L1 = Insert(L1, time, size, location);
                printf("Da them anh thanh cong! Ma anh la: %d\n", nextID - 1);
                break;
            }
            case 2: {
                printf("\n--- TIM KIEM ANH THEO ID ---\n");
                printf("Nhap ID anh can tim (Vi du: 1001): ");
                int ID;
                scanf("%d", &ID);
                FindByID(L1, ID);
                break;
            }
            case 3: {
                printf("\n--- XOA ANH ---\n");
                printf("Nhap ID anh can xoa (Vi du: 1001): ");
                int ID;
                scanf("%d", &ID);
                Photo* del = L1;
                while (del && del->ID != ID) del = del->next;
                if (del) {
                    L1 = Delete(L1, del);
                    printf("Da xoa anh voi ID %d\n", ID);
                } else {
                    printf("Khong tim thay anh!\n");
                }
                break;
            }
            case 4: {
                printf("\n--- SUA THONG TIN ANH ---\n");
                printf("Nhap ID anh can sua (Vi du: 1001): ");
                int ID;
                scanf("%d", &ID);

                int year = -1, month = -1, day = -1, hour = -1, minute = -1;
                char newTime[20] = "", newSize[20] = "", newLocation[50] = "";

                // Nhập thời gian mới
                do {
                    printf("Nhap ngay moi (1-31, nhap -1 de bo qua): ");
                    scanf("%d", &day);
                } while ((day < 1 || day > 31) && day != -1);

                do {
                    printf("Nhap thang moi (1-12, nhap -1 de bo qua): ");
                    scanf("%d", &month);
                } while ((month < 1 || month > 12) && month != -1);

                do {
                    printf("Nhap nam moi (4 chu so, nhap -1 de bo qua): ");
                    scanf("%d", &year);
                } while ((year < 1000 || year > 9999) && year != -1);

                printf("Nhap gio moi (0-23, nhap -1 de bo qua): ");
                scanf("%d", &hour);
                if (hour != -1) {
                    do {
                        printf("Nhap phut moi (0-59): ");
                        scanf("%d", &minute);
                    } while (minute < 0 || minute > 59);
                }

                // Tạo chuỗi thời gian mới
                if (year != -1 && month != -1 && day != -1) {
                    if (hour == -1) {
                        sprintf(newTime, "%04d%02d%02d", year, month, day);
                    } else {
                        sprintf(newTime, "%04d%02d%02d_%02d%02d", year, month, day, hour, minute);
                    }
                }

                // Nhập kích thước mới
                printf("Nhap kich thuoc moi (bo qua neu khong sua): ");
                scanf("%s", newSize);
                if (strcmp(newSize, "-1") == 0) {
                    newSize[0] = '\0'; // Không thay đổi kích thước
                }

                // Nhập địa điểm mới
                printf("Nhap dia diem moi (bo qua neu khong sua): ");
                scanf("%s", newLocation);
                if (strcmp(newLocation, "-1") == 0) {
                    newLocation[0] = '\0'; // Không thay đổi địa điểm
                }

                Edit(L1, ID, newTime, newSize, newLocation);
                break;
            }
            case 5: {
                printf("\n--- DANH SACH ANH ---\n");
                printf("ID | Thoi gian | Kich thuoc | Dia diem\n");
                printf("----------------------------------------\n");
                Print(L1);
                break;
            }
            case 6: {
                printf("\n--- XOA ANH TRUNG LAP ---\n");
                L1 = RemoveDuplicate(L1);
                printf("Da xoa xong cac anh trung lap!\n");
                break;
            }
            case 7: {
                printf("\n--- TIM KIEM ANH THEO NGAY VA DIA DIEM ---\n");
                int year, month, day, hour;
                char location[50];

                do {
                    printf("Nhap ngay (1-31): ");
                    scanf("%d", &day);
                } while (day < 1 || day > 31);

                do {
                    printf("Nhap thang (1-12): ");
                    scanf("%d", &month);
                } while (month < 1 || month > 12);

                do {
                    printf("Nhap nam (4 chu so): ");
                    scanf("%d", &year);
                } while (year < 1000 || year > 9999);

                printf("Nhap gio (0-23, nhap -1 de bo qua): ");
                scanf("%d", &hour);

                printf("Nhap dia diem (VD: Ha_Noi, nhap -1 de bo qua): ");
                scanf("%s", location);

                // Tìm kiếm ảnh theo ngày và địa điểm
                printf("\n--- TIM KIEM ANH ---\n");
                FindByDateAndLocation(L1, year, month, day, hour, location);
                break;
            }
            default:
                printf("Lua chon khong hop le!\n");
        }
    }

    while (L1) {
        L1 = Delete(L1, L1);
    }

    return 0;
}