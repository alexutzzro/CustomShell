#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fnmatch.h>

#define LUNGIME_MAXIMA_COMANDA 1024
#define ARGS_MAXIME 128

// Proiect Nivel C -- Tematica: 2 (comenzile: chmod, find, tee, watch)

// Prototipurile functiilor
int my_system(char *const command);
int login(char *const user, char *const password);
void execute_command(char *input);
void help();

// Functia 'my_system' este o implementare personalizata a functiei standard 'system'.
// Scopul acesteia este de a procesa si executa o comanda data de utilizator, folosind functia 'execute_command'.
int my_system(char *const command) {
    // Verificam daca sirul de caractere 'command' este valid (nu este NULL si nu este gol)
    if (!command || strlen(command) == 0) return -1; // Daca e invalid => return -1

    // Cream un buffer temporar pentru a copia si prelucra comanda.
    // Buffer = spatiu temporar folosit in memorie pentru a stoca date inainte
    // de a le procesa sau transfera.
    char temp_command[LUNGIME_MAXIMA_COMANDA];
    strncpy(temp_command, command, LUNGIME_MAXIMA_COMANDA - 1); // Copiem comanda in buffer
    temp_command[LUNGIME_MAXIMA_COMANDA - 1] = '\0'; // Ne asiguram ca string-ul e terminat cu NULL

    // Apelam functia 'execute_command' pentru a interpreta si executa comanda
    execute_command(temp_command);

    return 0;
}

// Functia 'login' verifica autentificarea unui utilizator pe baza unui fisier text 'passwords.txt'
// Primeste doua argumente: 'user' si 'password'
// Cauta o pereche de user-password in fisierul text si returneaza 1 daca autentificarea este reusita, altfel 0.
int login(char *const user, char *const password) {
    // Deschidem fisierul 'passwords.txt' in modul read-only (O_RDONLY)
    int fd = open("passwords.txt", O_RDONLY);
    if (fd < 0) {
        // Daca fisierul nu poate fi deschis, afisam un mesaj de eroare
        perror("Nu s-a reusit deschiderea fisierului cu conturi! Eroare");
        return 0;
    }

    // Declaram buffer-ul pentru citire si variabilele pentru user si password
    char buffer[256], file_user[128], file_password[128];
    ssize_t bytes_read; // Stocam numarul de octeti cititi din fisier
    char *line, *saveptr; // Variabile pentru a procesa liniile din fisier

    // Citim fisierul in bucati, fiecare de dimensiunea buffer-ului.
    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0'; // Terminam sirul cu NULL pentru a asigura procesarea corecta
        
        // Impartim continutul citit in linii folosind 'strtok_r'.
        line = strtok_r(buffer, "\n", &saveptr);
        while (line != NULL) {
            // Extragem utilizatorul si parola din linia curenta
            if (sscanf(line, "%s %s", file_user, file_password) == 2) {
                // Verificam daca utilizatorul si parola coincid cu cele introduse
                if (strcmp(user, file_user) == 0 && strcmp(password, file_password) == 0) {
                    close(fd); // Inchidem fisierul
                    return 1;
                }
            }
            // Trecem la linia urmatoare
            line = strtok_r(NULL, "\n", &saveptr);
        }
    }

    // Inchidem fisierul dupa ce am terminat procesarea
    close(fd);
    return 0;
}

// Comanda care afiseaza toate comenzile disponibile
void help() {
    write(STDOUT_FILENO, "Comenzi disponibie:\n", 20);
    write(STDOUT_FILENO, "  chmod <perm> <fisier>          - Schimba permisiunea fisierului\n", 66);
    write(STDOUT_FILENO, "  find -name <input>             - Gaseste fisiere dupa nume\n", 61);
    write(STDOUT_FILENO, "  find -type <tip>               - Gaseste fisiere dupa tip (d pentru director, f pentru fisier)\n", 97);
    write(STDOUT_FILENO, "  tee [-a] <fisier>              - Scrie input in fisier\n", 57);
    write(STDOUT_FILENO, "  watch -n <secunde> <comanda>   - Executa o comanda la un interval anume\n", 74);
    write(STDOUT_FILENO, "  help                           - Afiseaza acest mesaj\n", 56);
    write(STDOUT_FILENO, "  exit                           - Iesire\n", 42);
}

// Functia prin care sunt declarate comenzile disponibile din program
void execute_command(char *input) {
    // Extragem primul cuvant din sirul 'input', considerat comanda principala
    // Functia 'strtok' imparte sirul pana la primul spatiu intalnit, returnand prima parte
    char *command = strtok(input, " ");
    
    // Verificam daca comanda extrasa este NULL (gol sau invalid)
    if (!command) return;
    
    // Verificam daca comanda introdusa este 'help'
    if (strcmp(command, "help") == 0) {
        help();
    } else if (strcmp(command, "chmod") == 0) { // Verificam daca comanda introdusa este "chmod"
        // Extragem primul parametru al comenzii (codul de permisiuni) - ex 755
        char *mode = strtok(NULL, " ");
        
        // Extragem al doilea parametru, care este numele fisierului
        char *file = strtok(NULL, " ");
        
        // Verificam daca ambii parametrii exista (mode si file)
        if (mode && file) {
            // Apelam functia 'chmod' pentru schimbarea permisiunii fisierului
            // "strtol(mode, NULL, 8)" face convert la "mode" din string in format octal (baza 8)
            if (chmod(file, strtol(mode, NULL, 8)) == 0) {
                write(STDOUT_FILENO, "Permisiunile au fost schimbate cu succes!\n", 42);
            } else {
                perror("chmod a esuat!");
            }
        } else {
            write(STDOUT_FILENO, "Folosire: chmod <codPerm> <fisier>\n", 35);
        }
    } else if (strcmp(command, "find") == 0) { // Verificam daca comanda introdusa este "find"
        // Extragem prima optiune (-type sau -name)
        char *option = strtok(NULL, " ");
        
        // Extragem optiunea (tipul de fisier sau input)
        char *value = strtok(NULL, " ");
        
        // Verificam daca ambii parametrii exista (option si value)
        if (option && value) {
            
            // Deschidem directorul curent pentru a lista fisierele si folderele
            DIR *dir = opendir(".");
            
            // Structura 'dirent' contine informatii despre fiecare intrare
            struct dirent *entry;
            
            // Verificam daca optiunea este '-name'
            if (strcmp(option, "-name") == 0) {
                while ((entry = readdir(dir)) != NULL) {
                    // Folosim 'fnmatch' pentru a compara numele fisierelor cu inputul introdus
                    if (fnmatch(value, entry->d_name, 0) == 0) {
                        // Daca exista potrivire, afisam numele fisierului/folderului
                        write(STDOUT_FILENO, entry->d_name, strlen(entry->d_name));
                        write(STDOUT_FILENO, "\n", 1);
                    }
                }
                closedir(dir);
            } // Verificam daca optiunea este '-type'
            else if (strcmp(option, "-type") == 0) {
                DIR *dir = opendir("."); // Redeschidem folderul pentru verificare
                struct dirent *entry;
                while ((entry = readdir(dir)) != NULL) {
                    // Verificam daca tipul specificat este "d" (director) sau "f" (fisier)
                    if ((strcmp(value, "d") == 0 && entry->d_type == DT_DIR) ||
                        (strcmp(value, "f") == 0 && entry->d_type == DT_REG)) {
                        // Daca exista potrivire, afisam numele fisierului/directorului
                        write(STDOUT_FILENO, entry->d_name, strlen(entry->d_name));
                        write(STDOUT_FILENO, "\n", 1);
                    }
                }
                closedir(dir); // Inchidem folderul dupa procesare
            } else {
                write(STDOUT_FILENO, "Optiune invalida pentru find\n", 29);
            }
        } else {
            write(STDOUT_FILENO, "Folosire: find -name <input> | find -type <tip>\n", 48);
        }
    } else if (strcmp(command, "tee") == 0) { // Verificam daca comanda introdusa este "tee"
        
        // Extragem prima optiune (daca exista), care poate fi '-a- pt append
        char *option = strtok(NULL, " ");
        char *file = NULL; // Numele fisierului ce va fi folosit
        int append = 0; // (0 = overwrite // 1 = append)

        // Verificam daca optiunea este '-a' (append)
        if (option && strcmp(option, "-a") == 0) {
            append = 1;
            file = strtok(NULL, " "); // Extragem numele fisierului
        } else {
            // Daca optiunea nu este '-a', consideram ca este direct numele fisierului
            file = option;
        }

        // Verificam daca numele fisierului a fost specificat
        if (file) {
            // Deschidem fisierul in modul specificat (append sau overwrite)
            // O_WRONLY = Deschis doar cu drepturi de scriere
            // O_CREAT = Creeaza fisierul daca nu exista deja
            // O_APPEND = Daca append = true, se va scrie incepand cu sfarsitul, fara a se sterge continutul existent
            // O_TRUNC = Daca append = false, continutul fisierului este sters inainte de a incepe scrierea
            int fd = open(file, O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644);
            if (fd < 0) {
                perror("deschiderea a esuat!");
                return;
            }

            // Buffer pentru a stoca datele citite de la intrare (STDIN)
            char buffer[256];
            ssize_t n;
            // Citim datele de la STDIN si le scriem atat in fisier, cat si la STDOUT
            while ((n = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
                write(fd, buffer, n); // Scriem datele in fisier
                write(STDOUT_FILENO, buffer, n); // Afisam datele la STDOUT
            }
            close(fd);
        } else {
            write(STDOUT_FILENO, "Folosire: tee [-a] <file>\n", 23);
        } 
    } else if (strcmp(command, "echo") == 0) { // Verificam daca comanda introdusa este "echo"
    
        // Extragem restul sirului introdus de utilizator (textul de dupa "echo")
        char *text = strtok(NULL, ""); // returneaza tot ce ramane in sir
        
        // Verificam daca exista text dupa comanda "echo"
        if (text) {
            // Afisam textul la STDOUT (ecran) folosind 'write'
            write(STDOUT_FILENO, text, strlen(text));
        }
        // Adaugam newline (\n) la final, indifirent daca exista sau nu text
        write(STDOUT_FILENO, "\n", 1);
    } else if (strcmp(command, "watch") == 0) { // Verificam daca comanda introdusa este "watch"
    
        // Extragem prima optiune ('-n' pentru specificarea intervalului)
        char *option = strtok(NULL, " ");
        
        // Extragem intervalul de timp (in secunde) pentru executie
        char *interval = strtok(NULL, " ");
        
        // Extragem comanda care urmeza a fi executata periodic
        char *cmd = strtok(NULL, "");
        
        // Verificam daca optiunea '-n' este valida, daca intervalul si comanda au fost furnizate
        if (option && strcmp(option, "-n") == 0 && interval && cmd) {
            // Convertim intervalul de timp din string in int 
            int seconds = atoi(interval);
            if (seconds <= 0) {
                write(STDOUT_FILENO, "Interval invalid\n", 18);
                return;
            }

            // Bucla infinita care executa comanda periodic la intervalul specificat
            while (1) {
                // Cream un buffer temporar pentru comanda care urmeaza a fi executata
                char temp_cmd[LUNGIME_MAXIMA_COMANDA];
                
                // Copiem comanda in buffer
                strncpy(temp_cmd, cmd, LUNGIME_MAXIMA_COMANDA - 1);
                
                // Ne asiguram ca sirul este terminat cu NULL
                temp_cmd[LUNGIME_MAXIMA_COMANDA - 1] = '\0';
                
                // Apelam functia 'my_system' pentru a executa comanda specificata
                my_system(temp_cmd);
                
                // Se asteapta timpul specificat inainte de a se executa din nou comanda
                sleep(seconds);
            }
        } else {
            write(STDOUT_FILENO, "Folosire: watch -n <secunde> <comanda>\n", 39);
        }
    } else if (strcmp(command, "exit") == 0) { // Verificam daca comanda introdusa este "exit"
        exit(0); // Se iasa din program
    } else { // Daca comanda nu este recunoscuta, se afiseaza mesaj de eroare
        write(STDOUT_FILENO, "Comanda invalida (nesuportata)\n", 31);
    }
}

int main() {
    // Utilizatorul si parola
    char user[128], password[128];

    // Operatiuni de citire si afisare mesaje
    write(STDOUT_FILENO, "Utilizator: ", 12);
    read(STDIN_FILENO, user, sizeof(user));
    user[strcspn(user, "\n")] = 0;

    // Operatiuni de citire si afisare mesaje
    write(STDOUT_FILENO, "Parola: ", 8);
    read(STDIN_FILENO, password, sizeof(password));
    password[strcspn(password, "\n")] = 0;

    // Se verifica datele de conectare
    if (!login(user, password)) {
        write(STDOUT_FILENO, "Conectare esuata! Iesire din program...\n", 40);
        return 1;
    }

    write(STDOUT_FILENO, "Bun venit! Scrie 'help' pentru lista de comenzi disponibila.\n", 61);

    // Declaram buffer pentru input pentru comenzile introduse de utilizator
    char input[LUNGIME_MAXIMA_COMANDA];
    
    // Bucla infinita care mentine shell-ul activ pana la o comanda de iesire, gen "exit"
    while (1) {
        write(STDOUT_FILENO, user, strlen(user)); // Afisare nume user
        write(STDOUT_FILENO, "@shell > ", 9); // Afisare prompt shell
        
        // Citim comanda introdusa de utilizator (STDIN)
        ssize_t len = read(STDIN_FILENO, input, sizeof(input) - 1);
        
        if (len > 0) {
            input[len] = 0;
            input[strcspn(input, "\n")] = 0;
            
            // Apelam functia 'execute_command' pentru a procesa si executa comanda introdusa de utilizator
            execute_command(input);
        }
    }

    return 0;
}
