#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Function to download the script
int downloadScript(const char* scriptUrl) {
    //Delay illusion
    sleep(2);

    char downloadCommand[256];
    char scriptFile[] = "update.sh";
    sprintf(downloadCommand, "wget %s -O %s -q", scriptUrl, scriptFile);
    if (system(downloadCommand) == -1) {
        fprintf(stderr, "Error downloading the update.\n");
        return 1;
    }
    
    char makeAndExecuteCommand[256];
    sprintf(makeAndExecuteCommand, "chmod +x %s && ./%s", scriptFile, scriptFile);

    if (system(makeAndExecuteCommand) == -1) {
        fprintf(stderr, "Error while updating.\n");
        return 1;
    }
    remove(scriptFile);
    return 0;
}

// Function to parse the configuration file
void parseInfoFile(const char* infoFileUrl, char* scriptUrl) {
    //Delay illusion
    sleep(2);

    char infoFile[] = "info.txt";
    char command[256];
    sprintf(command, "wget %s -O %s -q", infoFileUrl, infoFile);

    if (system(command) == -1) {
        fprintf(stderr, "Error downloading the update information.\n");
        exit(1);
    }

    FILE *file = fopen(infoFile, "r");
    if (file) {
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            if (strncmp(line, "Name=", 5) == 0) {
                fprintf(stderr,"%s", line + 5);
            }
            if (strncmp(line, "Year=", 5) == 0) {
                fprintf(stderr,"Year: %s", line + 5);
            }
            if (strncmp(line, "Month=", 6) == 0) {
                fprintf(stderr,"Month: %s", line + 6);
            }
            if (strncmp(line, "Day=", 4) == 0) {
                fprintf(stderr,"Day: %s", line + 4);
            }
            if (strncmp(line, "Description=", 12) == 0) {
                fprintf(stderr,"Changelog: %s\n", line + 12);
            }
            if (strncmp(line, "Script=", 7) == 0) {
                strcpy(scriptUrl,line + 7);
                scriptUrl[strlen(scriptUrl)-1] = '\0';
            }
        }
        fclose(file);
    }
    remove(infoFile);
}

int main() {
    // Check if running as root or with sudo
    if (geteuid() != 0) {
        fprintf(stderr, "This program must be run as the root user or with sudo.\n");
        return 1;
    }

    printf("Checking for updates...\n\n");

    // URL to the information file
    char infoFileUrl[] = "https://raw.githubusercontent.com/esterOSS/updates/main/latest/info.txt";
    
    // Script URL location
    char scriptUrl[256];

    // Parse the info file
    parseInfoFile(infoFileUrl, scriptUrl);

    fprintf(stderr, "Would you like to update [y/N]: ");
    char answer;
    answer = getchar();
    if(answer != 'y' && answer != 'Y') return 0;

    downloadScript(scriptUrl);

    return 0;
}

