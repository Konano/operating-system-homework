#include <string>
#include <dirent.h>
#include <fcntl.h>

#ifdef WIN32
#include <direct.h>
#elif __linux__
#include <sys/types.h>
#include <sys/stat.h>
#endif

using namespace std;

#define PAGE_SIZE_INDEX 5
#define PAGE_SIZE 32

typedef uint8_t* BufType;

int f_memory, f_disk, pdbr, viraddr;

inline int getPage(int file, int page, int index) {
    off_t offset = page << PAGE_SIZE_INDEX;
    off_t error = lseek(file, offset, SEEK_SET);
    if (error != offset) return -1;
    BufType buf = new uint8_t[PAGE_SIZE];
    read(file, buf, 1 << PAGE_SIZE_INDEX);
    return buf[index];
}

int main() {
    f_memory = open("memory", O_RDONLY);
    f_disk = open("disk", O_RDONLY);
    puts("Please Input PDBR Content:");
    scanf("%x", &pdbr);
    int pdbr_page = pdbr >> 5;
    puts("Please Input Virtual Address:");
    while (~scanf("%x", &viraddr)) {
        printf("Virtual Address 0x%04x:\n", viraddr);

        int pde_index = (viraddr >> 10) & 31;
        int pde_contents = getPage(f_memory, pdbr_page, pde_index);
        printf("  --> PDE Index: 0x%02x, PDE Contents: 0x%02x(valid %d, pfn 0x%02x)\n", pde_index, pde_contents, pde_contents >> 7, pde_contents & 127);
        if ((pde_contents & 127) == 0x7f) {
            puts("  --> Invalid Virtual Address");
            continue;
        }

        int pte_index = (viraddr >> 5) & 31;
        int pte_contents = getPage(f_memory, pde_contents & 127, pte_index);
        printf("    --> PTE Index:0x%02x  PTE Contents: 0x%02x(valid %d, pfn 0x%02x)\n", pte_index, pte_contents, pte_contents >> 7, pte_contents & 127);
        if ((pte_contents & 127) == 0x7f) {
            puts("  --> Invalid Virtual Address");
            continue;
        }

        if (pte_contents >> 7) {
            int pm_index = viraddr & 31;
            int pm_contents = getPage(f_memory, pte_contents & 127, pm_index);
            printf("      --> To Physical Address 0x%04x --> Value: %02x\n", (((pte_contents & 127) << 5) | pm_index), pm_contents);
        } else {
            int pm_index = viraddr & 31;
            int pm_contents = getPage(f_disk, pte_contents & 127, pm_index);
            printf("      --> To Disk Sector Address 0x%04x --> Value: %02x\n", (((pte_contents & 127) << 5) | pm_index), pm_contents);
        }

        puts("Please Input Virtual Address:");
    }
    return 0;
}