# Assignments del Corso di Compilatori

## Requisiti di Sistema

Assicurati di avere installate le seguenti dipendenze sul tuo sistema:

- **LLVM 19+**  
  Versione 19 o superiore. Consulta la documentazione del tuo sistema operativo per le istruzioni di installazione:
  - Repository ufficiale LLVM
  - Pacchetti precompilati per la tua distribuzione

- **Clang**  
  Compilatore utilizzato per la generazione di LLVM IR da codice C/C++

- **CMake**  
  Sistema di build per la configurazione e compilazione del progetto

- **Make**


- **Strumenti LLVM**  
  - `opt`: Per l'applicazione delle ottimizzazioni LLVM
  - `llvm-dis`: Per la conversione da bitcode a IR leggibile



Repository dedicata agli assignments della seconda parte del corso di Compilatori

## Organizzazione dei File

```bash
assignmentX/
    │
    ├─── src/           # Implementazioni dei passi in C++
    ├─── test/          # File di testing (es .ll)
    ├─── script.sh      # Script per l'esecuzione automatizzata
    ├─── CMakeLists.txt # CMakeLists
    └─── readme.md      # Istruzioni per eseguire i passi
```


## Contributors
<h4 align="left">
    <ul>
        <li>Andrea Patrini <a href="https://github.com/Andreatl1" style="vertical-align: middle;"><img src="https://badgen.net/badge/github/Andreatl1/?icon=github"></a></li>
        <li>Dario D'Alterio <a href="https://github.com/DarioDalte" style="vertical-align: middle;"><img src="https://badgen.net/badge/github/DarioDalte/?icon=github"></a></li>
        <li>Arslan Shaukat <a href="https://github.com/Arslanx844" style="vertical-align: middle;"><img src="https://badgen.net/badge/github/Arslanx844/?icon=github"></a></li>
    </ul>
</h4>
