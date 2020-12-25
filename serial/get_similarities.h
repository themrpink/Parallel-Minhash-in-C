#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>

/*
    in questo file vengono salvate le coppie id_documento - nome_documento
*/
#define DOC_ID_INDEX_FILE "index_file.txt"

/*
    coppia signature - documento_id. Ogni documento ne ha 200.
*/
struct sign_doc{
    long long unsigned signature;
    long long unsigned doc_id;
};

/*
    per la coppia di documenti, la shared_signatures indica quante signatures hanno in comune. Se questa coppia esiste, il valore minimo è 1.
*/
struct doc_couple{
    long long unsigned doc1_id;
    long long unsigned doc2_id;
    int shared_signatures;
};


/*
    per ogni file, per ogni sua signature, genera una lista di coppie (signature, document_id) e le ordina in base alle signatures.
*/
int get_sketches(struct sign_doc *files_sketches, long long unsigned *signatures,char *filename);

/*
    dal nome del file, la sua dimensione in bytes e la data di modifica restituisce un id numerico univico.
*/
long long unsigned genera_doc_id(char *filename);


/*
    merge sort con impostazione parallelizzabile.
*/
void merge(struct sign_doc* X, int n, struct sign_doc* tmp);
void mergesort(struct sign_doc* X, int n, struct sign_doc* tmp);


/*
    confronta le signatures di tutti i documenti e salva quali hanno signatures in comune e quante. 
    Restituisce anche una percentuale di somiglianza tra i documenti che hanno almeno una signatures in comune.
*/
void find_similarity(int numberOfFiles, struct sign_doc **files_sketches);

