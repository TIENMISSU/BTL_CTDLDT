<<<<<<< HEAD
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Define the Photo struct    
typedef struct SPhoto {
	int ID;
	char time[20];
	char size[20];
	char location[50];
	struct SPhoto* next;
} Photo;

// Define List as a pointer to a Photo
typedef Photo* List;  


int nextID = 1001; // Mã của ảnh tiếp theo, sau ảnh thứ 100

// List to store deleted photos
List deletedPhotos = NULL;

// Khai báo hàm DeletePermanently và MoveMultipleToDeleted
List DeletePermanently(List l, int ID);
List MoveMultipleToDeleted(List l, const char* ids);

// Initialize an empty list
List LInit() {
	return NULL;
}
// Duyệt danh sách liên kết
List Duyet_danh_sach(List l, Photo* a) {
	Photo* current = l;
	while (current->next != NULL) {
		current = current->next;
	}
	current->next = a;
}
// In danh sách các ảnh trong album
void Print(const List l) {
	for (List temp = l; temp; temp = temp->next) {
		printf("%d %s %s %s\n", temp->ID, temp->time, temp->size, temp->location);
	  }
}

// In danh sách các ảnh đã xóa (trong thùng rác)
void PrintDeleted(List l) {
	if (l == NULL) {
		printf("Khong co anh trong thung rac\n");
		return;
	}
	for (List temp = l; temp; temp = temp->next) {
		printf("%d %s %s %s\n", temp->ID, temp->time, temp->size, temp->location);
	}
	printf("\nBan co muon xoa vinh vien anh nao khong? (Yes-1/No-2): ");
	int choice;
	scanf("%d", &choice);
	if (choice == 1) {
		printf("Nhap cac ID anh can xoa vinh vien (cach nhau boi dau cach): ");
		char ids[256];
		scanf(" %[^\n]", ids);
		char id[10];
		int index = 0;
		for (int i = 0; ids[i] != '\0'; i++) {
			if (ids[i] == ' ') {
				id[index] = '\0';
				int ID = atoi(id);
				deletedPhotos = DeletePermanently(deletedPhotos, ID);
				index = 0;
			}
			else {
				id[index++] = ids[i];
			}
		}
		id[index] = '\0';  // Xử lý ID cuối cùng
		int ID = atoi(id);
		deletedPhotos = DeletePermanently(deletedPhotos, ID);
	}
}

// 43 a photo by ID and print its details
// Find a photo by ID and return a pointer to it
Photo* FindByID(List l, int ID) {
    while (l) {
        if (l->ID == ID) {
            printf("Anh voi ID %d\n", ID);
            printf("Thoi gian: %s\n", l->time);
            printf("Kich thuoc: %s\n", l->size);
            printf("Dia diem: %s\n", l->location);
            return l; // Return the pointer to the photo
        }
        l = l->next;
    }
    printf("Khong tim thay anh voi ID %d\n", ID);
    return NULL; // Return NULL if the photo is not found
}

// Find photos by date and location and print their details
// Hàm hỗ trợ để tạo chuỗi ngày từ các thành phần năm, tháng, ngày
void CreateDateString(char* date, int year, int month, int day) {
	sprintf(date, "%04d%02d%02d", year, month, day);
}

// Hàm cải thiện để tìm kiếm ảnh theo thời gian và địa điểm
void FindByDateAndLocation(List l, int year, int month, int day, int hour, const char* location) {
	bool found = false;
	char date[9];
	CreateDateString(date, year, month, day);
	while (l) {
		// Kiểm tra ngày và địa điểm
		if (strncmp(l->time, date, 8) == 0 &&
			(hour == -1 || (hour != -1 && atoi(l->time + 9) == hour)) &&
			(strcmp(location, "-1") == 0 || strcmp(l->location, location) == 0)) {
			printf("ID: %d\nThoi gian: %s\nKich thuoc: %s\nDia diem: %s\n", l->ID, l->time, l->size, l->location);
			found = true;
		}
		l = l->next;
	}
	if (!found) {
		if (strcmp(location, "-1") == 0) {
			printf("Khong tim thay anh voi ngay %s va dia diem bat ky\n", date);
		}
		else {
			printf("Khong tim thay anh voi ngay %s va dia diem %s\n", date, location);
		}
	}
}
// Hàm chèn ảnh vào cuối danh sách 
// *****TIẾN*****

List InsertAtEnd(List l, const char* time, const char* size, const char* location) {
	/* gán giá trị hiện tại của nextID cho ID, ví dụ ảnh mới đầu tiên sẽ có ID là 1001
	Sau đó giá trị nextID tiếp theo sẽ là 1002*/
	int ID = nextID++;
	/* cấp phát động một vùng nhớ để tạo một ảnh mới
	*/
	Photo* new_image = (Photo*)malloc(sizeof(Photo));
	//const char*: chuỗi bất biến, hàm chỉ đọc chuỗi, không sửa đổi
	// chỉ chuyền địa chỉ của chuỗi vào hàm -> tiết kiệm dữ liệu
	new_image->ID = ID;
	strcpy(new_image->time, time);
	strcpy(new_image->size, size);
	strcpy(new_image->location, location);
	// gán các giá trị đã nhập vào dữ liệu của new_image
	new_image->next = NULL; // phần tử ảnh mới sẽ là phần tử cuối cùng trong danh sách

	if (l == NULL) return new_image; // new_image sẽ là phần tử đầu tiên của chuỗi nếu con trỏ l bằng rỗng

	Photo* current = l; // duyệt tất cả phần tử của danh sách 
	Duyet_danh_sach(l, new_image);
	return l;
}

// Thêm ảnh vào thùng rác (Xóa ảnh)
//******TIẾN *******/

List MoveToDeleted(List l, Photo* a) {
	//Photo* a là ảnh cần xóa
	/*Nếu danh sách ảnh, hay ảnh muốn thêm mà rỗng => không làm gì cả*/
	if (l == NULL || a == NULL) return l;

	if (l == a) { // nếu phần từ cần xóa là phần tử đầu tiên
		List temp = l->next;
		a->next = NULL; // Đảm bảo phần tử xóa không trỏ lung tung.

		if (deletedPhotos == NULL) {
			// Nếu danh sách deletedPhotos rỗng, gán a làm đầu danh sách
			deletedPhotos = a;
		}
		else {
			Duyet_danh_sach(deletedPhotos, a);
		}
		return temp; // Trả về danh sách mới.
	}

	// Trường hợp phần tử cần xóa không phải là phần tử đầu
	Photo* current = l;
	while (current->next != NULL && current->next != a) {
		current = current->next; // Duyệt qua các nút
	}

	if (current->next == a) { // Nếu tìm thấy phần tử cần xóa
		current->next = a->next; // Bỏ qua nút cần xóa trong danh sách chính
		a->next = NULL; // Đảm bảo a không trỏ lung tung

		if (deletedPhotos == NULL) {
			deletedPhotos = a; // Nếu deletedPhotos rỗng, gán a làm đầu
		}
		else {
			Duyet_danh_sach(deletedPhotos, a);
		}
	}
	return l; // Trả về danh sách chính đã cập nhật.
}

// Sửa ảnh bằng ID
void Edit(Photo* photo, const char* eTime, const char* eSize, const char* eLocation) {
    if (eTime && strlen(eTime) > 0) strcpy(photo->time, eTime);
    if (eSize && strlen(eSize) > 0) strcpy(photo->size, eSize);
    if (eLocation && strlen(eLocation) > 0) strcpy(photo->location, eLocation);
    printf("Da sua thong tin anh.\n");
}


// Hàm xóa ảnh trùng lặp và di chuyển vào thùng rác
List RemoveDuplicate(List l) {
	if (!l) return l; // Nếu danh sách trống, trả về ngay

	Photo* current = l;
	while (current && current->next) {
		Photo* runner = current;
		while (runner->next) {
			if (strcmp(runner->next->time, current->time) == 0 &&
				strcmp(runner->next->size, current->size) == 0 &&
				strcmp(runner->next->location, current->location) == 0) {
				Photo* duplicate = runner->next;
				runner->next = duplicate->next;
				// Di chuyển ảnh trùng lặp vào thùng rác
				deletedPhotos = InsertAtEnd(deletedPhotos, duplicate->time, duplicate->size, duplicate->location);
				free(duplicate); // Giải phóng bộ nhớ
			}
			else {
				runner = runner->next;
			}
		}
		current = current->next;
	}
	return l;
}
// Hàm chèn ảnh theo thứ tự ID
List InsertByID(List l, Photo* e) {
	if (!l || e->ID < l->ID) {
		e->next = l;
		return e;
	}

	Photo* current = l;
	while (current->next && current->next->ID < e->ID) {
		current = current->next;
	}
	e->next = current->next;
	current->next = e;
	return l;
}

// Xóa ảnh vĩnh viễn trong thùng rác 
/******Tiến******/
List DeletePermanently(List l, int ID) {
	Photo* prev = NULL;
	Photo* current = l;
	while (current) {
		if (current->ID == ID) {
			if (prev) {
				prev->next = current->next;
			}
			else {
				l = current->next;
			}
			free(current); // Giải phóng bộ nhớ
			printf("Da xoa vinh vien anh voi ID %d\n", ID);
			return l;
		}
		prev = current;
		current = current->next;
	}
	printf("Khong tim thay anh voi ID %d trong danh sach da xoa\n", ID);
	return l;
}

// Restore a photo from the deleted list back to the main list
List Restore(List l, int ID) {
	Photo* prev = NULL;
	Photo* current = deletedPhotos;
	while (current) {
		if (current->ID == ID) {
			if (prev) {
				prev->next = current->next;
			}
			else {
				deletedPhotos = current->next;
			}
			current->next = NULL; // Detach from deleted list
			l = InsertByID(l, current); // Insert back into the main list
			printf("Da khoi phuc anh voi ID %d\n", ID);
			return l;
		}
		prev = current;
		current = current->next;
	}
	printf("Khong tim thay anh voi ID %d trong danh sach da xoa\n", ID);
	return l;
}
int main() {
	List L1 = LInit();

	// Arrays containing locations and sizes
	const char* locations[8] = { "Ha_Noi", "Bac_Ninh", "Hai_Phong", "Quang_Ninh", "Thai_Binh", "Nam_Dinh", "Bac_Giang", "Lang_Son" };
	const char* sizes[4] = { "3840x2160", "1920x1080", "2560x1440", "1280x720" };

	// Create 100 sample photos
	for (int i = 1; i <= 100; i++) {
		int hour = 12 + (i / 5) % 12;
		int minute = (i % 5) * 10;
		char time[20];
		char month_str[3], day_str[3], hour_str[3], minute_str[3];

		//giới hạn tháng nhỏ hơn bằng 12
		int month = (5 + (i + 3) / 7) % 12;
        if (month == 0) month = 12;

		//giới hạn ngày nhỏ hơn = 31
		int day = (i % 31) + 1;
		sprintf(month_str, "%02d", month);
		sprintf(day_str, "%02d", day);
		sprintf(hour_str, "%02d", hour);
		sprintf(minute_str, "%02d", minute);

		strcpy(time, "2024");
		strcat(time, month_str);
		strcat(time, day_str); // nối chu
		strcat(time, "_");
		strcat(time, hour_str);
		strcat(time, minute_str);

		L1 = InsertAtEnd(L1, time, sizes[i % 4], locations[i % 8]);
	}

	while (1) {
		printf("\n=== QUAN LY ALBUM ANH ===\n");
		printf("1. Hien thi tat ca anh\n");
		printf("2. Them anh moi\n");
		printf("3. Tim kiem anh theo ID\n");
		printf("4. Tim kiem anh theo ngay va dia diem\n");
		printf("5. Sua thong tin anh\n");
		printf("6. Xoa anh trung lap\n");
		printf("7. Cho anh vao thung rac\n");
		printf("8. Thung rac\n");
		printf("9. Khoi phuc anh\n");
		printf("0. Thoat\n");
		printf("Lua chon cua ban: ");
		int choice;
		scanf("%d", &choice);

		if (choice == 0) break;

		switch (choice) {
		case 1: {
			printf("\n--- DANH SACH ANH ---\n");
			printf("ID  | Thoi gian   | Kich thuoc | Dia diem\n");
			printf("----------------------------------------\n");
			Print(L1);
			break;
		}
		case 2: {
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

			// Tạo chuỗi thời gian
			char time[20];
			char year_str[5], month_str[3], day_str[3], hour_str[3], minute_str[3];
			sprintf(year_str, "%04d", year);
			sprintf(month_str, "%02d", month);
			sprintf(day_str, "%02d", day);
			sprintf(hour_str, "%02d", hour);
			sprintf(minute_str, "%02d", minute);

			if (hour == -1) {
				strcpy(time, year_str);
				strcat(time, month_str);
				strcat(time, day_str);
				strcat(time, "_0000"); // Mặc định là "0000" nếu không nhập giờ
			}
			else {
				strcpy(time, year_str);
				strcat(time, month_str);
				strcat(time, day_str);
				strcat(time, "_");
				strcat(time, hour_str);
				strcat(time, minute_str);
			}

			// Nhập kích thước (có thể có hoặc không)
			char inputSize[20];
			printf("Nhap kich thuoc (co the co hoac khong, nhap '-1' de bo qua, VD: 3840x2160): ");
			scanf("%s", inputSize);
			if (strcmp(inputSize, "-1") != 0) {
				strcpy(size, inputSize);
			}

			// Nhập địa điểm
			printf("Nhap dia diem (VD: Ha_Noi, nhap '-1' de bo qua): ");
			scanf("%s", location);
			if (strcmp(location, "-1") == 0) {
				strcpy(location, "Khong ro"); // Địa điểm mặc định
			}

			// Thêm ảnh vào danh sách
			L1 = InsertAtEnd(L1, time, size, location);
			printf("Da them anh thanh cong! Ma anh la: %d\n", nextID - 1);
			break;
		}
		case 3: {
			printf("\n--- TIM KIEM ANH THEO ID ---\n");
			printf("Nhap ID anh can tim (Vi du: 1001): ");
			int ID;
			scanf("%d", &ID);
			FindByID(L1, ID);
			break;
		}
		case 4: {
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
		case 5:{
            printf("\n--- SUA THONG TIN ANH ---\n");
            printf("Nhap ID anh can sua (Vi du: 1001): ");
            int ID;
            scanf("%d", &ID);

            Photo* photo = FindByID(L1, ID);
            if (photo == NULL) {
                printf("Khong tim thay ID anh.\n");
                break;
            }

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

            if (year != -1 && month != -1 && day != -1) {
                printf("Nhap gio moi (0-23, nhap -1 de bo qua): ");
                scanf("%d", &hour);
                if (hour != -1) {
                    do {
                        printf("Nhap phut moi (0-59): ");
                        scanf("%d", &minute);
                    } while (minute < 0 || minute > 59);
                }

                // Tạo chuỗi thời gian mới
                char year_str[5], month_str[3], day_str[3], hour_str[3], minute_str[3];
                sprintf(year_str, "%04d", year);
                sprintf(month_str, "%02d", month);
                sprintf(day_str, "%02d", day);
                sprintf(hour_str, "%02d", hour);
                sprintf(minute_str, "%02d", minute);

                strcpy(newTime, year_str);
                strcat(newTime, month_str);
                strcat(newTime, day_str);
                if (hour != -1) {
                    strcat(newTime, "_");
                    strcat(newTime, hour_str);
                    strcat(newTime, minute_str);
                }
            }

            // Nhập kích thước mới
            printf("Nhap kich thuoc moi (Vi du: 1234x5678, nhap -1 de bo qua): ");
            scanf("%s", newSize);
            if (strcmp(newSize, "-1") == 0) {
                newSize[0] = '\0'; // Không thay đổi kích thước
            }

            // Nhập địa điểm mới
            printf("Nhap dia diem moi (Vi du: Ha_Noi, nhap -1 de bo qua): ");
            scanf("%s", newLocation);
            if (strcmp(newLocation, "-1") == 0) {
                newLocation[0] = '\0'; // Không thay đổi địa điểm
            }

            // Gọi hàm Edit để chỉnh sửa
            Edit(photo, newTime, newSize, newLocation);
            break;
        }
		case 6: {
			printf("\n--- XOA ANH TRUNG LAP ---\n");
			L1 = RemoveDuplicate(L1);
			printf("Da xoa xong cac anh trung lap!\n");
			break;
		}
		case 7: {
			printf("\n--- XOA ANH ---\n");
			printf("Nhap cac ID anh can xoa (cach nhau boi dau cach, VD: 1001 1002 1003): ");
			char ids[256];// mảng ký tự
			//%[^\n]: Đọc tất cả các ký tự cho đến khi gặp ký tự xuống dòng (\n).
			scanf(" %[^\n]", ids);
			char id[10];
			int index = 0;

			for (int i = 0; ids[i] != '\0'; i++) {
				if (ids[i] == ' ') {
					id[index] = '\0';
					int ID = atoi(id); // chuyển string thành 1 số nguyên

					Photo* del_photo = L1;
					while (del_photo != NULL && del_photo->ID != ID) del_photo = del_photo->next;
					if (del_photo != NULL) {
						L1 = MoveToDeleted(L1, del_photo);
						printf("Da xoa anh voi ID %d\n", ID);
					}
					else {
						printf("Khong tim thay anh voi ID %d\n", ID);
					}
					index = 0;
				}
				else {
					id[index] = ids[i];
					index++;
				}
			}
			id[index] = '\0';  // Xử lý ID cuối cùng
			int ID = atoi(id);
			Photo* del_photo = L1;
			while (del_photo && del_photo->ID != ID) del_photo = del_photo->next;
			if (del_photo != NULL) {
				L1 = MoveToDeleted(L1, del_photo);
				printf("Da xoa anh voi ID %d\n", ID);
			}
			else {
				printf("Khong tim thay anh voi ID %d\n", ID);
			}
			break;
		}
		case 8: {
			printf("\n--- DANH SACH ANH DA XOA ---\n");
			printf("ID | Thoi gian | Kich thuoc | Dia diem\n");
			printf("----------------------------------------\n");
			PrintDeleted(deletedPhotos);
			break;
		}
		case 9: {
			printf("\n--- KHOI PHUC ANH ---\n");
			printf("Nhap ID anh can khoi phuc (Vi du: 1001): ");
			int ID;
			scanf("%d", &ID);
			L1 = Restore(L1, ID);
			break;
		}
		default:
			printf("Lua chon khong hop le!\n");
		}
	}

	while (L1) {
		L1 = MoveToDeleted(L1, L1);
	}
	return 0;
=======
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Define the Photo struct    
typedef struct SPhoto {
	int ID;
	char time[20];
	char size[20];
	char location[50];
	struct SPhoto* next;
} Photo;

// Define List as a pointer to a Photo
typedef Photo* List;  // Khai báo một danh sách liên kết 


int nextID = 1001; // Mã của ảnh tiếp theo, sau ảnh thứ 100

// List to store deleted photos
List deletedPhotos = NULL;

// Khai báo hàm DeletePermanently và MoveMultipleToDeleted
List DeletePermanently(List l, int ID);
List MoveMultipleToDeleted(List l, const char* ids);

// Initialize an empty list
List LInit() {
	return NULL;
}
// Duyệt danh sách liên kết
List Duyet_danh_sach(List l, Photo* a) {
	Photo* current = l;
	while (current->next != NULL) {
		current = current->next;
	}
	current->next = a;
}
// In danh sách các ảnh trong album
void Print(const List l) {
	for (List temp = l; temp; temp = temp->next) {
		printf("%d %s %s %s\n", temp->ID, temp->time, temp->size, temp->location);
	  }
}

// In danh sách các ảnh đã xóa (trong thùng rác)
void PrintDeleted(List l) {
	if (l == NULL) {
		printf("Khong co anh trong thung rac\n");
		return;
	}
	for (List temp = l; temp; temp = temp->next) {
		printf("%d %s %s %s\n", temp->ID, temp->time, temp->size, temp->location);
	}
	printf("\nBan co muon xoa vinh vien anh nao khong? (Yes-1/No-2): ");
	int choice;
	scanf("%d", &choice);
	if (choice == 1) {
		printf("Nhap cac ID anh can xoa vinh vien (cach nhau boi dau cach): ");
		char ids[256];
		scanf(" %[^\n]", ids);
		char id[10];
		int index = 0;
		for (int i = 0; ids[i] != '\0'; i++) {
			if (ids[i] == ' ') {
				id[index] = '\0';
				int ID = atoi(id);
				deletedPhotos = DeletePermanently(deletedPhotos, ID);
				index = 0;
			}
			else {
				id[index++] = ids[i];
			}
		}
		id[index] = '\0';  // Xử lý ID cuối cùng
		int ID = atoi(id);
		deletedPhotos = DeletePermanently(deletedPhotos, ID);
	}
}

// 43 a photo by ID and print its details
// Find a photo by ID and return a pointer to it
Photo* FindByID(List l, int ID) {
    while (l) {
        if (l->ID == ID) {
            printf("Anh voi ID %d\n", ID);
            printf("Thoi gian: %s\n", l->time);
            printf("Kich thuoc: %s\n", l->size);
            printf("Dia diem: %s\n", l->location);
            return l; // Return the pointer to the photo
        }
        l = l->next;
    }
    printf("Khong tim thay anh voi ID %d\n", ID);
    return NULL; // Return NULL if the photo is not found
}

// Find photos by date and location and print their details
// Hàm hỗ trợ để tạo chuỗi ngày từ các thành phần năm, tháng, ngày
void CreateDateString(char* date, int year, int month, int day) {
	sprintf(date, "%04d%02d%02d", year, month, day);
}

// Hàm cải thiện để tìm kiếm ảnh theo thời gian và địa điểm
void FindByDateAndLocation(List l, int year, int month, int day, int hour, const char* location) {
	bool found = false;
	char date[9];
	CreateDateString(date, year, month, day);
	while (l) {
		// Kiểm tra ngày và địa điểm
		if (strncmp(l->time, date, 8) == 0 &&
			(hour == -1 || (hour != -1 && atoi(l->time + 9) == hour)) &&
			(strcmp(location, "-1") == 0 || strcmp(l->location, location) == 0)) {
			printf("ID: %d\nThoi gian: %s\nKich thuoc: %s\nDia diem: %s\n", l->ID, l->time, l->size, l->location);
			found = true;
		}
		l = l->next;
	}
	if (!found) {
		if (strcmp(location, "-1") == 0) {
			printf("Khong tim thay anh voi ngay %s va dia diem bat ky\n", date);
		}
		else {
			printf("Khong tim thay anh voi ngay %s va dia diem %s\n", date, location);
		}
	}
}
// Hàm chèn ảnh vào cuối danh sách 
// *****TIẾN*****

List InsertAtEnd(List l, const char* time, const char* size, const char* location) {
	/* gán giá trị hiện tại của nextID cho ID, ví dụ ảnh mới đầu tiên sẽ có ID là 1001
	Sau đó giá trị nextID tiếp theo sẽ là 1002*/
	int ID = nextID++;
	/* cấp phát động một vùng nhớ để tạo một ảnh mới
	*/
	Photo* new_image = (Photo*)malloc(sizeof(Photo));
	//const char*: chuỗi bất biến, hàm chỉ đọc chuỗi, không sửa đổi
	// chỉ chuyền địa chỉ của chuỗi vào hàm -> tiết kiệm dữ liệu
	new_image->ID = ID;
	strcpy(new_image->time, time);
	strcpy(new_image->size, size);
	strcpy(new_image->location, location);
	// gán các giá trị đã nhập vào dữ liệu của new_image
	new_image->next = NULL; // phần tử ảnh mới sẽ là phần tử cuối cùng trong danh sách

	if (l == NULL) return new_image; // new_image sẽ là phần tử đầu tiên của chuỗi nếu con trỏ l bằng rỗng

	Photo* current = l; // duyệt tất cả phần tử của danh sách 
	Duyet_danh_sach(l, new_image);
	return l;
}

// Thêm ảnh vào thùng rác (Xóa ảnh)
//******TIẾN *******/

List MoveToDeleted(List l, Photo* a) {
	//Photo* a là ảnh cần xóa
	/*Nếu danh sách ảnh, hay ảnh muốn thêm mà rỗng => không làm gì cả*/
	if (l == NULL || a == NULL) return l;

	if (l == a) { // nếu phần từ cần xóa là phần tử đầu tiên
		List temp = l->next;
		a->next = NULL; // Đảm bảo phần tử xóa không trỏ lung tung.

		if (deletedPhotos == NULL) {
			// Nếu danh sách deletedPhotos rỗng, gán a làm đầu danh sách
			deletedPhotos = a;
		}
		else {
			Duyet_danh_sach(deletedPhotos, a);
		}
		return temp; // Trả về danh sách mới.
	}

	// Trường hợp phần tử cần xóa không phải là phần tử đầu
	Photo* current = l;
	while (current->next != NULL && current->next != a) {
		current = current->next; // Duyệt qua các nút
	}

	if (current->next == a) { // Nếu tìm thấy phần tử cần xóa
		current->next = a->next; // Bỏ qua nút cần xóa trong danh sách chính
		a->next = NULL; // Đảm bảo a không trỏ lung tung

		if (deletedPhotos == NULL) {
			deletedPhotos = a; // Nếu deletedPhotos rỗng, gán a làm đầu
		}
		else {
			Duyet_danh_sach(deletedPhotos, a);
		}
	}
	return l; // Trả về danh sách chính đã cập nhật.
}

// Sửa ảnh bằng ID
void Edit(Photo* photo, const char* eTime, const char* eSize, const char* eLocation) {
    if (eTime && strlen(eTime) > 0) strcpy(photo->time, eTime);
    if (eSize && strlen(eSize) > 0) strcpy(photo->size, eSize);
    if (eLocation && strlen(eLocation) > 0) strcpy(photo->location, eLocation);
    printf("Da sua thong tin anh.\n");
}


// Hàm xóa ảnh trùng lặp và di chuyển vào thùng rác
List RemoveDuplicate(List l) {
	if (!l) return l; // Nếu danh sách trống, trả về ngay

	Photo* current = l;
	while (current && current->next) {
		Photo* runner = current;
		while (runner->next) {
			if (strcmp(runner->next->time, current->time) == 0 &&
				strcmp(runner->next->size, current->size) == 0 &&
				strcmp(runner->next->location, current->location) == 0) {
				Photo* duplicate = runner->next;
				runner->next = duplicate->next;
				// Di chuyển ảnh trùng lặp vào thùng rác
				deletedPhotos = InsertAtEnd(deletedPhotos, duplicate->time, duplicate->size, duplicate->location);
				free(duplicate); // Giải phóng bộ nhớ
			}
			else {
				runner = runner->next;
			}
		}
		current = current->next;
	}
	return l;
}
// Hàm chèn ảnh theo thứ tự ID
List InsertByID(List l, Photo* e) {
	if (!l || e->ID < l->ID) {
		e->next = l;
		return e;
	}

	Photo* current = l;
	while (current->next && current->next->ID < e->ID) {
		current = current->next;
	}
	e->next = current->next;
	current->next = e;
	return l;
}

// Xóa ảnh vĩnh viễn trong thùng rác 
/******Tiến******/
List DeletePermanently(List l, int ID) {
	Photo* prev = NULL;
	Photo* current = l;
	while (current) {
		if (current->ID == ID) {
			if (prev) {
				prev->next = current->next;
			}
			else {
				l = current->next;
			}
			free(current); // Giải phóng bộ nhớ
			printf("Da xoa vinh vien anh voi ID %d\n", ID);
			return l;
		}
		prev = current;
		current = current->next;
	}
	printf("Khong tim thay anh voi ID %d trong danh sach da xoa\n", ID);
	return l;
}

// Restore a photo from the deleted list back to the main list
List Restore(List l, int ID) {
	Photo* prev = NULL;
	Photo* current = deletedPhotos;
	while (current) {
		if (current->ID == ID) {
			if (prev) {
				prev->next = current->next;
			}
			else {
				deletedPhotos = current->next;
			}
			current->next = NULL; // Detach from deleted list
			l = InsertByID(l, current); // Insert back into the main list
			printf("Da khoi phuc anh voi ID %d\n", ID);
			return l;
		}
		prev = current;
		current = current->next;
	}
	printf("Khong tim thay anh voi ID %d trong danh sach da xoa\n", ID);
	return l;
}
int main() {
	List L1 = LInit();

	// Arrays containing locations and sizes
	const char* locations[8] = { "Ha_Noi", "Bac_Ninh", "Hai_Phong", "Quang_Ninh", "Thai_Binh", "Nam_Dinh", "Bac_Giang", "Lang_Son" };
	const char* sizes[4] = { "3840x2160", "1920x1080", "2560x1440", "1280x720" };

	// Create 100 sample photos
	for (int i = 1; i <= 100; i++) {
		int hour = 12 + (i / 5) % 12;
		int minute = (i % 5) * 10;
		char time[20];
		char month_str[3], day_str[3], hour_str[3], minute_str[3];

		//giới hạn tháng nhỏ hơn bằng 12
		int month = (5 + (i + 3) / 7) % 12;
        if (month == 0) month = 12;

		//giới hạn ngày nhỏ hơn = 31
		int day = (i % 31) + 1;
		sprintf(month_str, "%02d", month);
		sprintf(day_str, "%02d", day);
		sprintf(hour_str, "%02d", hour);
		sprintf(minute_str, "%02d", minute);

		strcpy(time, "2024");
		strcat(time, month_str);
		strcat(time, day_str); // nối chu
		strcat(time, "_");
		strcat(time, hour_str);
		strcat(time, minute_str);

		L1 = InsertAtEnd(L1, time, sizes[i % 4], locations[i % 8]);
	}

	while (1) {
		printf("\n=== QUAN LY ALBUM ANH ===\n");
		printf("1. Hien thi tat ca anh\n");
		printf("2. Them anh moi\n");
		printf("3. Tim kiem anh theo ID\n");
		printf("4. Tim kiem anh theo ngay va dia diem\n");
		printf("5. Sua thong tin anh\n");
		printf("6. Xoa anh trung lap\n");
		printf("7. Cho anh vao thung rac\n");
		printf("8. Thung rac\n");
		printf("9. Khoi phuc anh\n");
		printf("0. Thoat\n");
		printf("Lua chon cua ban: ");
		int choice;
		scanf("%d", &choice);

		if (choice == 0) break;

		switch (choice) {
		case 1: {
			printf("\n--- DANH SACH ANH ---\n");
			printf("ID  | Thoi gian   | Kich thuoc | Dia diem\n");
			printf("----------------------------------------\n");
			Print(L1);
			break;
		}
		case 2: {
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

			// Tạo chuỗi thời gian
			char time[20];
			char year_str[5], month_str[3], day_str[3], hour_str[3], minute_str[3];
			sprintf(year_str, "%04d", year);
			sprintf(month_str, "%02d", month);
			sprintf(day_str, "%02d", day);
			sprintf(hour_str, "%02d", hour);
			sprintf(minute_str, "%02d", minute);

			if (hour == -1) {
				strcpy(time, year_str);
				strcat(time, month_str);
				strcat(time, day_str);
				strcat(time, "_0000"); // Mặc định là "0000" nếu không nhập giờ
			}
			else {
				strcpy(time, year_str);
				strcat(time, month_str);
				strcat(time, day_str);
				strcat(time, "_");
				strcat(time, hour_str);
				strcat(time, minute_str);
			}

			// Nhập kích thước (có thể có hoặc không)
			char inputSize[20];
			printf("Nhap kich thuoc (co the co hoac khong, nhap '-1' de bo qua, VD: 3840x2160): ");
			scanf("%s", inputSize);
			if (strcmp(inputSize, "-1") != 0) {
				strcpy(size, inputSize);
			}

			// Nhập địa điểm
			printf("Nhap dia diem (VD: Ha_Noi, nhap '-1' de bo qua): ");
			scanf("%s", location);
			if (strcmp(location, "-1") == 0) {
				strcpy(location, "Khong ro"); // Địa điểm mặc định
			}

			// Thêm ảnh vào danh sách
			L1 = InsertAtEnd(L1, time, size, location);
			printf("Da them anh thanh cong! Ma anh la: %d\n", nextID - 1);
			break;
		}
		case 3: {
			printf("\n--- TIM KIEM ANH THEO ID ---\n");
			printf("Nhap ID anh can tim (Vi du: 1001): ");
			int ID;
			scanf("%d", &ID);
			FindByID(L1, ID);
			break;
		}
		case 4: {
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
		case 5:{
            printf("\n--- SUA THONG TIN ANH ---\n");
            printf("Nhap ID anh can sua (Vi du: 1001): ");
            int ID;
            scanf("%d", &ID);

            Photo* photo = FindByID(L1, ID);
            if (photo == NULL) {
                printf("Khong tim thay ID anh.\n");
                break;
            }

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

            if (year != -1 && month != -1 && day != -1) {
                printf("Nhap gio moi (0-23, nhap -1 de bo qua): ");
                scanf("%d", &hour);
                if (hour != -1) {
                    do {
                        printf("Nhap phut moi (0-59): ");
                        scanf("%d", &minute);
                    } while (minute < 0 || minute > 59);
                }

                // Tạo chuỗi thời gian mới
                char year_str[5], month_str[3], day_str[3], hour_str[3], minute_str[3];
                sprintf(year_str, "%04d", year);
                sprintf(month_str, "%02d", month);
                sprintf(day_str, "%02d", day);
                sprintf(hour_str, "%02d", hour);
                sprintf(minute_str, "%02d", minute);

                strcpy(newTime, year_str);
                strcat(newTime, month_str);
                strcat(newTime, day_str);
                if (hour != -1) {
                    strcat(newTime, "_");
                    strcat(newTime, hour_str);
                    strcat(newTime, minute_str);
                }
            }

            // Nhập kích thước mới
            printf("Nhap kich thuoc moi (Vi du: 1234x5678, nhap -1 de bo qua): ");
            scanf("%s", newSize);
            if (strcmp(newSize, "-1") == 0) {
                newSize[0] = '\0'; // Không thay đổi kích thước
            }

            // Nhập địa điểm mới
            printf("Nhap dia diem moi (Vi du: Ha_Noi, nhap -1 de bo qua): ");
            scanf("%s", newLocation);
            if (strcmp(newLocation, "-1") == 0) {
                newLocation[0] = '\0'; // Không thay đổi địa điểm
            }

            // Gọi hàm Edit để chỉnh sửa
            Edit(photo, newTime, newSize, newLocation);
            break;
        }
		case 6: {
			printf("\n--- XOA ANH TRUNG LAP ---\n");
			L1 = RemoveDuplicate(L1);
			printf("Da xoa xong cac anh trung lap!\n");
			break;
		}
		case 7: {
			printf("\n--- XOA ANH ---\n");
			printf("Nhap cac ID anh can xoa (cach nhau boi dau cach, VD: 1001 1002 1003): ");
			char ids[256];// mảng ký tự
			//%[^\n]: Đọc tất cả các ký tự cho đến khi gặp ký tự xuống dòng (\n).
			scanf(" %[^\n]", ids);
			char id[10];
			int index = 0;

			for (int i = 0; ids[i] != '\0'; i++) {
				if (ids[i] == ' ') {
					id[index] = '\0';
					int ID = atoi(id); // chuyển string thành 1 số nguyên

					Photo* del_photo = L1;
					while (del_photo != NULL && del_photo->ID != ID) del_photo = del_photo->next;
					if (del_photo != NULL) {
						L1 = MoveToDeleted(L1, del_photo);
						printf("Da xoa anh voi ID %d\n", ID);
					}
					else {
						printf("Khong tim thay anh voi ID %d\n", ID);
					}
					index = 0;
				}
				else {
					id[index] = ids[i];
					index++;
				}
			}
			id[index] = '\0';  // Xử lý ID cuối cùng
			int ID = atoi(id);
			Photo* del_photo = L1;
			while (del_photo && del_photo->ID != ID) del_photo = del_photo->next;
			if (del_photo != NULL) {
				L1 = MoveToDeleted(L1, del_photo);
				printf("Da xoa anh voi ID %d\n", ID);
			}
			else {
				printf("Khong tim thay anh voi ID %d\n", ID);
			}
			break;
		}
		case 8: {
			printf("\n--- DANH SACH ANH DA XOA ---\n");
			printf("ID | Thoi gian | Kich thuoc | Dia diem\n");
			printf("----------------------------------------\n");
			PrintDeleted(deletedPhotos);
			break;
		}
		case 9: {
			printf("\n--- KHOI PHUC ANH ---\n");
			printf("Nhap ID anh can khoi phuc (Vi du: 1001): ");
			int ID;
			scanf("%d", &ID);
			L1 = Restore(L1, ID);
			break;
		}
		default:
			printf("Lua chon khong hop le!\n");
		}
	}

	while (L1) {
		L1 = MoveToDeleted(L1, L1);
	}
	return 0;
>>>>>>> 02e8a566bc433c90b2dbd56f5cfae537234cd6a4
}
