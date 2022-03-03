#include <iostream>
#include <windows.h>
using namespace std;

int main() {

    setlocale(LC_ALL, "ru");

    char A = 65; //алфавит в аски
    bool checkT = false; //переменная проверки на существование
    char diskName;
    char diskNameArray[4]; //массив с директорией диска

    cout << "Введи имя диска: ";
    cin >> diskName;

    diskNameArray[0] = diskName;
    diskNameArray[1] = ':';
    diskNameArray[2] = '\\';
    diskNameArray[3] = '\0';

    DWORD checkDiskMask = GetLogicalDrives(); //битовая маска всех дисков

    for (int i = 0; i < 26; i++) { //проверка на существование 
        if (((checkDiskMask >> i) & 1) && (A == diskName)) {
            checkT = true;
        }
        A++;
    }

    if (checkT == false) {
        cout << "Диска с именем: " << diskName << " не сущуствует!" << endl;
    }
    else {

        cout << "\n\nИнформация по диску " << diskName << ":" << endl;

        char fullNameDisk[20];
        char fileSystemName[10];
        unsigned long serialNumberDisk;

        UINT diskType = GetDriveTypeA(LPCSTR(diskNameArray)); //вывод типа диска
        cout << "\nТип диска: ";

        switch (diskType)
        {
        case DRIVE_UNKNOWN:
            cout << "Неизвестный (UNKNOWN)" << endl;
            break;
        case DRIVE_NO_ROOT_DIR:
            cout << "Неправильный путь (NO_ROOT_DIR)" << endl;
            break;
        case DRIVE_REMOVABLE:
            cout << "Съемный (REMOVABLE)" << endl;
            break;
        case DRIVE_FIXED:
            cout << "Фиксированный (FIXED)" << endl;
            break;
        case DRIVE_REMOTE:
            cout << "Удаленный или сетевой (REMOTE)" << endl;
            break;
        case DRIVE_CDROM:
            cout << "CDROM" << endl;
            break;
        case DRIVE_RAMDISK:
            cout << "В оперативной памяти (RAMDISK)" << endl;
            break;
        }

        BOOL infoFlag = GetVolumeInformationA(diskNameArray, fullNameDisk, 100, &serialNumberDisk, NULL, NULL, fileSystemName, 100);
        //флаг на получение инфы о файловой системе и диске
        //имя\путь, название, длинна буфера названия, серийный номер диска, макс длина файла, опции файловой системы, имя файл системы, для буф имя ф системы

        if (infoFlag != 0) {
            cout << "Название диска: " << fullNameDisk << endl;
            cout << "Серийный номер диска: " << serialNumberDisk << endl;
            cout << "Файловая система: " << fileSystemName << endl;
        }
        else {
            cout << "Информации нет" << endl;
        }

        //Далее изучаем свободное место на диске 
        ULARGE_INTEGER freeBytes;
        ULARGE_INTEGER allBytes;
        ULARGE_INTEGER allFreeBytes;
        WCHAR diskNameArraySecond[4] = L"C:\\";
        diskNameArraySecond[0] = diskNameArray[0];

        BOOL spaceFlag = GetDiskFreeSpaceEx(diskNameArraySecond, (PULARGE_INTEGER)&freeBytes, (PULARGE_INTEGER)&allBytes, (PULARGE_INTEGER)&allFreeBytes);
        //флаг на получение инфы о состоянии памяти
        //имя\путь, доступные для исп байты, макс объем, свободно всего

        if (spaceFlag != 0) {
            cout << "Полный объем памяти диска: " << ((allBytes.QuadPart) >> 30) << " Гб" << endl;
            cout << "Количество незанятого пространства: " << ((allFreeBytes.QuadPart) >> 30) << " Гб" << endl;
        }
        else {
            cout << "Информация отсутсвует" << endl;
        }

    }
    system("pause");
    return 0;
}