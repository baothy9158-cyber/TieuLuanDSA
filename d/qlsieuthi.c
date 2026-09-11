#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
struct product
{
    char ten[20];
    int gia;
    int soLuong;
    int ID;
};
struct product *products = NULL;
int count = 0;
int capacity = 0;

int ensureCapacity(struct product **products, int *capacity, int count)
{
    if (count < *capacity)
        return 1;

    int newCapacity = (*capacity == 0) ? 2 : *capacity * 2;
    struct product *temporary = realloc(
        *products, newCapacity * sizeof(struct product));

    if (temporary == NULL)
        return 0;

    *products = temporary;
    *capacity = newCapacity;
    return 1;
}

int idExists(const struct product products[], int count, int id)
{
    for (int i = 0; i < count; i++)
    {
        if (products[i].ID == id)
            return 1;
    }

    return 0;
}
void saveHistoryadded(const char *filename)
{
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        printf("Khong the mo file de ghi lich su\n");
        return;
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(file,"-------------------------------\n");
    fprintf(file, "Lich su them san pham:[%02d:%02d]Added\n",
            t->tm_hour, t->tm_min);

    fprintf(file, "%d %s %d %d\n",
            products[count - 1].ID,
            products[count - 1].ten,
            products[count - 1].gia,
            products[count - 1].soLuong);

    fclose(file);
}
void saveHistorydeleted(const char *filename, struct product deletedProduct)
{
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        printf("Khong the mo file de ghi lich su\n");
        return;
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(file,"-------------------------------\n");
    fprintf(file, "Lich su xoa san pham:[%02d:%02d]Deleted\n",
            t->tm_hour, t->tm_min);

    fprintf(file, "%d %s %d %d\n",
            deletedProduct.ID,
            deletedProduct.ten,
            deletedProduct.gia,
            deletedProduct.soLuong);

    fclose(file);
}
void saveHistoryEdit(const char *filename, struct product editedProduct)
{
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        printf("Khong the mo file de ghi lich su\n");
        return;
    }
    
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(file,"-------------------------------\n");
    fprintf(file, "Lich su sua san pham:[%02d:%02d]Edited\n",
            t->tm_hour, t->tm_min);
    fprintf(file, "%d %s %d %d\n",
            editedProduct.ID,
            editedProduct.ten,
            editedProduct.gia,
            editedProduct.soLuong);
    fclose(file);
}
int saveProducts(const char *filename)
{
    FILE *file = fopen(filename, "w");

    if (file == NULL)
    {
        printf("Khong the mo file de ghi\n");
        return 0;
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%d %s %d %d\n",
                products[i].ID,
                products[i].ten,
                products[i].gia,
                products[i].soLuong);
    }

    fclose(file);
    return 1;
}
int nhapProduct(struct product **products, int *count, int *capacity)
{
    int newID;

    if (!ensureCapacity(products, capacity, *count))
    {
        printf("Khong the cap phat bo nho\n");
        return 0;
    }

    printf("Nhap ID san pham: ");
    if (scanf("%d", &newID) != 1)
    {
        printf("ID khong hop le\n");
        return 0;
    }

    if (newID < 0 || newID > 1000)
    {
        printf("ID khong hop le\n");
        return 0;
    }
    if (idExists(*products, *count, newID))
    {
        printf("ID da ton tai\n");
        return 0;
    }

    (*products)[*count].ID = newID;
    printf("Nhap ten san pham: ");
    scanf("%19s", (*products)[*count].ten);
    printf("Nhap gia san pham: ");
    scanf("%d", &(*products)[*count].gia);
    printf("Nhap so luong san pham: ");
    scanf("%d", &(*products)[*count].soLuong);
    (*count)++;
    if (saveProducts("products.txt"))
    {
        printf("Da luu san pham vao file\n");
    }
    else
    {
        printf("Khong the luu san pham vao file\n");
    }   
    return 1;
}
void xuatProduct(const struct product *products, int count)
{
    if (count == 0)
    {
        printf("Danh sach trong!\n");
        return;
    }

    printf("\n+------+----------------------+----------+----------+\n");
    printf("| %-4s | %-20s | %-8s | %-8s |\n", "ID", "Ten san pham", "Gia", "So luong");
    printf("+------+----------------------+----------+----------+\n");

    for (int i = 0; i < count; i++)
    {
        printf("| %-4d | %-20s | %-8d | %-8d |\n",
               products[i].ID,
               products[i].ten,
               products[i].gia,
               products[i].soLuong);
    }
    printf("+------+----------------------+----------+----------+\n");
}

int loadProducts(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("Khong the mo file de doc\n");
        return 0;
    }
    count = 0;
    while (fscanf(file, "%d %19s %d %d",
                  &products[count].ID,
                  products[count].ten,
                  &products[count].gia,
                  &products[count].soLuong) == 4)
    {
        printf("Da doc san pham: ID=%d, Ten=%s, Gia=%d, So luong=%d\n",
               products[count].ID,
               products[count].ten,
               products[count].gia,
               products[count].soLuong);
        count++;
        if (!ensureCapacity(&products, &capacity, count))
        {
            printf("Khong the cap phat bo nho\n");
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    return 1;
}
void xoaproduct(char *keyword)//chức năng xóa sản phẩm theo ID va ten
{
    int id = atoi(keyword);
    int index = -1;
    for (int i = 0; i < count; i++)
    {
        if (products[i].ID == id || strcmp(products[i].ten, keyword) == 0)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("San pham voi ID %d khong ton tai\n", id);
        return;
    }
    struct product deletedProduct = products[index];
    for (int i = index; i < count - 1; i++)
    {
        products[i] = products[i + 1];
    }
    count--;
    saveProducts("products.txt");
    saveHistorydeleted("history.txt", deletedProduct);
    printf("Da xoa san pham voi ID %d\n", id);
}
void searchProduct(char *keyword)
{
    int gia = atoi(keyword);
    int id = atoi(keyword);
    int found = 0;
    for (int i = 0; i < count; i++)
    {
        if (products[i].ID == id || strcmp(products[i].ten, keyword) == 0 || products[i].gia == gia)
        {
            printf("San pham tim thay: ID=%d, Ten=%s, Gia=%d, So luong=%d\n",products[i].ID,products[i].ten,products[i].gia,products[i].soLuong);
            found = 1; 
        }
    }

    if (!found)
    {
        printf("Khong tim thay san pham voi ID %d hoac ten %s\n", id, keyword);
    }
}
void editProduct(char *keyword)
{
    int id = atoi(keyword);
    int index = -1;
    for (int i = 0; i < count; i++)
    {
        if (products[i].ID == id || strcmp(products[i].ten, keyword) == 0)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("San pham voi ID %d khong ton tai\n", id);
        return;
    }

    printf("Nhap ten san pham moi: ");
    scanf("%19s", products[index].ten);
    printf("Nhap gia san pham moi: ");
    scanf("%d", &products[index].gia);
    printf("Nhap so luong san pham moi: ");
    scanf("%d", &products[index].soLuong);
    saveProducts("products.txt");
    saveHistoryEdit("history.txt", products[index]);
    printf("Da cap nhat thong tin san pham %s\n", products[index].ten);
}
void insertionSortByIDtangdan(struct product arr[], int count) {
    for (int i = 1; i < count; i++) {
        struct product key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].ID > key.ID) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
    printf("Da sap xep danh sach theo ID tang dan.\n");
}
void insertionSortByIDgiamdan(struct product arr[], int count) {
    for (int i = 1; i < count; i++) {
        struct product key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].ID < key.ID) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
    printf("Da sap xep danh sach theo ID giam dan.\n");
}
void insertionSortByTenAtoZ(struct product arr[], int count) {
    for (int i = 1; i < count; i++) {
        struct product key = arr[i];
        int j = i - 1;
        while (j >= 0 && strcmp(arr[j].ten, key.ten) > 0) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
    printf("Da sap xep danh sach theo ten tu A den Z.\n");
}
void insertionSortByTenZtoA(struct product arr[], int count) {
    for (int i = 1; i < count; i++) {
        struct product key = arr[i];
        int j = i - 1;
        while (j >= 0 && strcmp(arr[j].ten, key.ten) < 0) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
    printf("Da sap xep danh sach theo ten tu Z den A.\n");
}
void insertionSortByGiaTangDan(struct product arr[], int count) {
    for (int i = 1; i < count; i++) {
        struct product key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].gia > key.gia) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
    printf("Da sap xep danh sach theo gia tang dan.\n");
}
void insertionSortByGiaGiamDan(struct product arr[], int count) {
    for (int i = 1; i < count; i++) {
        struct product key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].gia < key.gia) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
    printf("Da sap xep danh sach theo gia giam dan.\n");
}
void statistics(struct product arr[],int count){
    int totalcost=0;
    for(int i=0;i<count;i++){
        totalcost+=arr[i].gia*arr[i].soLuong;
    }
    printf("Tong gia tri cua tat ca san pham: %d\n", totalcost);
    
}

int main(void)
{
    int select;
    int selectSort;
    char keyword[20];
    do
    {
        printf("\nChon chuc nang:\n");
        printf("1. Nhap san pham\n");
        printf("2. Xuat san pham\n");
        printf("3. Xoa san pham\n");
        printf("4. Doc san pham tu file\n");
        printf("5. Tim kiem san pham\n");
        printf("6. Chinh sua san pham\n");
        printf("7. Sap xep san pham\n");
        printf("0. Thoat\n");
        scanf("%d", &select);

        switch (select)
        {
        case 1:
            nhapProduct(&products, &count, &capacity);
            break;
        case 2:
            xuatProduct(products, count);
            break;
        case 3:
            printf("Nhap ID hoac ten san pham can xoa: ");
            scanf("%19s", keyword);
            xoaproduct(keyword);
            break;
        case 4:
            loadProducts("products.txt");
            break;
        case 5:
            printf("Nhap ID hoac ten san pham can tim: ");
            scanf("%19s", keyword);
            searchProduct(keyword);
            break;
        case 6:
            printf("Nhap ID hoac ten san pham can chinh sua: ");
            scanf("%19s", keyword);
            editProduct(keyword);
            break;
        case 7:
            printf("Chon chuc nang sap xep:\n");
            printf("1. Sap xep theo ID tang dan\n");
            printf("2. Sap xep theo ID giam dan\n");
            printf("3. Sap xep theo ten tu A den Z\n");
            printf("4. Sap xep theo ten tu Z den A\n");
            printf("5. Sap xep theo gia tang dan\n");
            printf("6. Sap xep theo gia giam dan\n");
            scanf("%d", &selectSort);
            switch (selectSort)
            {
            case 1:
                insertionSortByIDtangdan(products, count);
                break;
            case 2:
                insertionSortByIDgiamdan(products, count);
                break;
            case 3:
                insertionSortByTenAtoZ(products, count);
                break;
            case 4:
                insertionSortByTenZtoA(products, count);
                break;
            case 5:
                insertionSortByGiaTangDan(products, count);
                break;
            case 6:
                insertionSortByGiaGiamDan(products, count);
                break;
            default:
                printf("Chuc nang sap xep khong hop le\n");
            }
            xuatProduct(products, count);
            saveProducts("products.txt");
        break;
        case 0:
            break;
        default:
            printf("Chuc nang khong hop le\n");
            break;
        }
    } while (select != 0);

    free(products);
    return 0;
}



