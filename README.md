# Progetto Minhash #


Nella cartella **scripts** si trovano il Makefile e lo script minhash.sh.


---


**Per installare:**
dalla cartella scripts eseguire make, verranno compilati 4 eseguibili per ogni versione dell'algoritmo:
serial, pthread, omp, pthread_prod_cons


---


**Per lanciare i test con lo script minhash.sh:**

-s   per eseguire la versione seriale

-o   per eseguire la version con OpenMP

-p   per eseguire la versione con Pthread

-c   per seguire la versione produttore-consumatore


Possono essere specificati uno o più di questi parametri in qualsiasi ordine: verranno eseguiti uno dopo l'altro i test di tutte le versioni richieste, con numero variabile di threads sulle 5 sottocartelle di default in docs.

-d <path>   per specificare un path su cui eseguire le versioni richieste. Opzionale. 
            se specificato i programmi ignorano le cartelle di default ed eseguono i test sui file del path dato


esempio:
./minhash.sh -s -o -p -c            // lancia i test su tutte le versioni sulle cartelle di default contenute in docs

./minhash.sh -o -p -d ../myfolder   // lancia i test per le versioni omp e pthread sui file al path ../myfolder


---


**Ripulire la cartella scripts:**

make clean          -> rimuove i file eseguibili.

make clean_test     -> rimuove i file txt e csv dei test.
