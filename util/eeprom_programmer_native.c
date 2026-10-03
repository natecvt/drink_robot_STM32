#include "eeprom_programmer_native.h"

HANDLE hComm;
FILE *file;
IAccount ia = {0};
Ingredient ig;
RAccount ra = {0};
Recipe rp;

int open_com() 
{
    
    DCB dcbSerialParams = { 0 };
    COMMTIMEOUTS timeouts = { 0 };

    hComm = CreateFileA(COM, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hComm == INVALID_HANDLE_VALUE) {
        printf("Error opening COM port\n");
        return -1;
    }

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hComm, &dcbSerialParams)) {
        printf("Error getting COM state\n");
        CloseHandle(hComm);
        return -1;
    }
    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    
    if (!SetCommState(hComm, &dcbSerialParams)) {
        printf("Error setting COM state\n");
        CloseHandle(hComm);
        return -1;
    }

    timeouts.ReadIntervalTimeout = 500;
    timeouts.ReadTotalTimeoutConstant = 500;
    timeouts.ReadTotalTimeoutMultiplier = 1;
    timeouts.WriteTotalTimeoutConstant = 500;
    timeouts.WriteTotalTimeoutMultiplier = 1;
    SetCommTimeouts(hComm, &timeouts);

    DWORD evnt_msk = EV_RXFLAG | EV_RXCHAR;
    if (!SetCommMask(hComm, evnt_msk)) {
        printf("Error setting COM mask\n");
        CloseHandle(hComm);
        return -1;
    }

    //Clear both the TX and RX buffer
    PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR); //purge both tx and rx buffer

    printf("COM set up!\n");

    return 0;
}

boolean write_to_com(char *data)
{
    DWORD bytesWritten;
    WriteFile(hComm, data, strlen(data), &bytesWritten, NULL);

    return bytesWritten != strlen(data);
}

int __event_listener_com()
{
    DWORD bytes_read = 0, total_read = 0, errors;
    char rx_data[RCPT_SIZ];

    COMSTAT status;

    if (!ClearCommError(hComm, &errors, &status)) {
        printf("ClearCommError failed: %lu\n", GetLastError());
    }

    while (total_read < RCPT_SIZ - 1)
    {
        DWORD bytes_read = 0;

        BOOL success = ReadFile(
            hComm,
            &rx_data[total_read],
            1,
            &bytes_read,
            NULL
        );

        if (!success)
        {
            printf("ReadFile failed\n");
            return -1;
        }

        if (bytes_read == 0)
        {
            printf("Read timeout\n");
            return -1;
        }

        if (rx_data[total_read] == '\n')
        {
            total_read++;
            break;
        }

        total_read++;
    }

    printf("%d Received: %s\n", total_read, rx_data);

    rx_data[total_read] = '\0';
    if (total_read != RCPT_SIZ-1) {
        printf("Bad receipt message\n");
        return -1;
    }

    return 0;
}

int __transmit_and_response_listen(boolean is_ig)
{
    /** #TODO: add account management here */
    boolean erc = 0;
    char rx_data[RCPT_SIZ];
    DWORD *bytes_read;

    if (is_ig) erc = write_to_com((char *)(&ig));
    else erc = write_to_com((char *)(&rp));

    if (erc) return -1;

    /** carriage return for complete transmission signal */
    write_to_com("\n");

    /** #TODO: expand on the checksum */
    return __event_listener_com(rx_data, bytes_read);
}

int close_com()
{
    if (CloseHandle(hComm)) return -1;
    return 0;
}

int open_file()
{
    /** open only for reading */
    file = fopen(DATA_FILE, "r");

    if (file == NULL) {
        printf("Error opening file\n");
        return -1;
    }

    printf("File set up for reading!\n");
}

int __read_file_line(char *data)
{
    if (fgets(data, MAX_LINE_LEN, file) == NULL) {
        printf("EOF hit\n");
        return -1;
    }

    /** replace EOL with null-terminator */
    data[strcspn(data, "\r\n")] = '\0';

    printf("Line data: %s\n", data);

    return 0;
}

uint8_t __char_to_num(char c)
{
    if (c < '0' || c > '9') return 10;
    return c - '0';
}

uint8_t __char_to_num_multi(char *s)
{
    uint8_t l = strlen(s);

    if (l > 3) return 255;
    uint8_t res = 0, 
        oom = 1 * (10 * (l > 1) + (l <= 1)) * (10 * (l > 2) + (l <= 2));

    for (uint8_t i = 0; i < l; i++) {
        res += oom * __char_to_num(s[i]);
        oom /= 10;
    }

    return res;
}

int __get_ingred_from_line(char *data)
{
    char *token = strtok(data, ",");

    if (strlen(token) != 0) {
        if (strcpy(ig.name, token) != ig.name) return -1;
    }

    printf("Successful ingredient transcription\n");

    if (strtok(NULL, ",") == NULL) return 0;
}

int __get_recipe_from_line(char *data)
{
    void * attributes[4] = {
        &rp.ningred,
        &rp.ingred_amounts,
        &rp.ingred_indexes,
        &rp.name
    };

    uint8_t i = 0, j = 0;

    /** take out spaces and commas */
    char *token = strtok(data, ",");

    while (token != NULL) {
    
        if (strlen(token) != 0) {

            /** numeric elements cannot contain bad characters */
            /** the ningred element */
            if (i == 0) {
                *(uint8_t *)attributes[i] = __char_to_num(token[0]);
                i++;
            }
            /** the ingred amounts and indexes */
            else if (i == 1 || i == 2)
            {
                *((uint8_t*)attributes[i]+j) = __char_to_num_multi(token);
                j++;
            }
            /** the name attribute */
            else {
                strcpy(attributes[i], token);
                break;
            }
            /** transition between ingred amounts and ingred indexes */
            if (j == rp.ningred) {
                i++;
                j = 0;
            }
        }

        token = strtok(NULL, ",");
    }

    printf("Successful recipe transcription\n");
    return 0;
}

boolean __decide_ingred_or_recipe(char *data) {
    /** has to be at least 2 commas for it to be a recipe, if at all */
    return (data - strchr(data, ',') == data - strrchr(data, ','));
}

int __line_interpret_switch(char *data, boolean is_ig)
{
    if (is_ig) return __get_ingred_from_line(data);
    else return __get_recipe_from_line(data);
}

int process_all_file_lines()
{
    char data[MAX_LINE_LEN];
    while (__read_file_line(data) != -1) {
        boolean is_ig = __decide_ingred_or_recipe(data);

        if (__line_interpret_switch(data, is_ig)) {
            printf("Line interpret unsuccessful\n");
            return -1;
        }

        if (__transmit_and_response_listen(is_ig)) {
            printf("Object transmission unsuccessful\n");
            return -2;
        }
    }

    printf("Full file consumed");
    return 0;
}

int close_file()
{
    if (fclose(file)) return -1;
    return 0;
}

int main()
{
    open_file();
    open_com();

    // write_to_com("Hello\n");
    int ret = process_all_file_lines();

    close_com();
    close_file();
    return ret;
}