# CustomShell - EN

## Description

This project is a **mini shell** implemented in **C**, allowing users to execute custom commands and manage processes similarly to a classic terminal. The implementation **does not use the `system()` call**.

## Features

- **Manual implementation** of the `system()` mechanism
- **Login system** based on a `passwords.txt` file
- **Execution of commands without using `system()`**
- **Handling command-line arguments with `getopt`**
- **Executing commands in a separate child process**
- **Support for combining options in the command line**
- **Implementation of custom commands**:
  - `chmod` – modifies file permissions
  - `find` – supports `-name` and `-type` options
  - `tee` – supports the `-a` option
  - `watch` – supports the `-n` option

## Requirements

- **GCC** compiler
- **Linux** operating system
- Understanding of process management in C

## Installation

1. **Clone the repository:**
   ```bash
   git clone https://github.com/alexutzzro/CustomShell.git
   cd CustomShell
   ```

2. **Compile the project:**
   ```bash
   gcc -o shell Project.c
   ```

## Usage

1. **Start the custom shell:**
   ```bash
   ./shell
   ```

2. **Authentication:**
   - Upon execution, the user must authenticate using an account stored in `passwords.txt`.
   - The `passwords.txt` file format is:
     ```
     user1 password1
     user2 password2
     ```

3. **Executing commands:**
   - Commands can be entered interactively after authentication:
     ```bash
     user@shell > chmod 755 myfile.txt
     user@shell > find -name *.txt
     user@shell > tee -a log.txt
     user@shell > watch -n 5 echo message
     ```
   - If a manually implemented command is recognized, it will be executed directly.
   - If it is not a recognized command, an error message will be displayed.

## License

This project was developed as part of the **Operating Systems 1** course at the Faculty of Mathematics and Computer Science, West University of Timișoara.

---

# CustomShell - RO

## Descriere

Acest proiect reprezintă un **mini shell** realizat în **C**, care permite utilizatorilor să execute comenzi personalizate și să gestioneze procese într-un mod similar unui terminal clasic. Implementarea **nu utilizează apelul `system()`**.

## Caracteristici

- **Implementare manuală** a mecanismului `system()`
- **Mecanism de login** bazat pe un fișier `passwords.txt`
- **Execuția comenzilor fără a folosi `system()`**
- **Tratarea argumentelor liniei de comandă cu `getopt`**
- **Executarea comenzilor într-un proces copil separat**
- **Suport pentru combinarea opțiunilor în linia de comandă**
- **Implementarea comenzilor personalizate**:
  - `chmod` – modifică permisiunile unui fișier
  - `find` – permite opțiunile `-name` și `-type`
  - `tee` – permite opțiunea `-a`
  - `watch` – permite opțiunea `-n`

## Cerințe

- Compilator **GCC**
- Sistem de operare **Linux**
- Cunoștințe despre gestionarea proceselor în C

## Instalare

1. **Clonarea repository-ului:**
   ```bash
   git clone https://github.com/alexutzzro/CustomShell.git
   cd CustomShell
   ```

2. **Compilarea proiectului:**
   ```bash
   gcc -o shell Project.c
   ```

## Utilizare

1. **Pornirea shell-ului personalizat:**
   ```bash
   ./shell
   ```

2. **Autentificare:**
   - La rulare, utilizatorul trebuie să se autentifice cu un cont stocat în `passwords.txt`
   - Formatul fișierului `passwords.txt` este:
     ```
     user1 parola1
     user2 parola2
     ```

3. **Executarea comenzilor:**
   - Comenzile pot fi introduse interactiv după autentificare:
     ```bash
     user@shell > chmod 755 myfile.txt
     user@shell > find -name *.txt
     user@shell > tee -a log.txt
     user@shell > watch -n 5 echo mesaj
     ```
   - Dacă o comandă implementată manual este recunoscută, aceasta va fi executată direct.
   - Dacă nu este o comandă implementată, se va afișa un mesaj de eroare.

## Licență

Acest proiect a fost realizat în cadrul disciplinei **Sisteme de Operare 1** din cadrul Facultății de Matematică și Informatică, Universitatea de Vest din Timișoara.
