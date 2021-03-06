#ifndef MINHASHPROJECT_GET_SIMILARITIES_H
#define MINHASHPROJECT_GET_SIMILARITIES_H
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
    long long unsigned doc_id;
    long long unsigned doc2_id;
    int shared_signatures;
};


/*
    II fase ( la I fase è il calcolo delle signatures):
        files_sketches, è un array di array di coppie {signature, doc_id}, in cui ogni array contiene le coppie relative a uno stesso doc_id, ordinate
        secondo le signatures. Adesso si copiano questi array in un solo array che conterrà quinti tutte le coppie {signature, doc_id} di tutti i documenti.
        A questo punto si fa un nuovo merdgesort ordinandoli secondo le signatures.

        Questo andrà nella funzione get_sketches ristrutturata

*/
int get_sketches(int i, struct sign_doc *files_sketches, long long unsigned *signatures);


/*
    merge sort con impostazione parallelizzabile.
*/
void mergesort_s_doc_id(struct doc_couple*  X, int l, int n);
void* mergesort_doc_id(void* args);
void merge_doc_id(struct doc_couple*  X, int index_inizio, int valore_medio, int index_fine);
void merge_signatures(struct sign_doc* X, int index_inizio, int valore_medio, int index_fine);
void mergesort_s_signatures(struct sign_doc* X, int index_inizio, int indexFine);
void* mergesort_signatures(void* args);
/*
    confronta le signatures di tutti i documenti e salva quali hanno signatures in comune e quante.
    Restituisce anche una percentuale di somiglianza tra i documenti che hanno almeno una signatures in comune.
    Dal paper: "In the final phase, we produce the complete clustering. We examine each <ID, ID, count> triplet
    and decide if the document pair exceeds our threshold for resemblance."
*/

/*
      III fase
        si espande la lista di coppie {signature, doc_id} in una lista di triple, prendendo ogni signature della lista di coppie
        e ricavandone le triple: {doc_id, doc_id, numero_di_signatures_condivise}, in cui i doc_id indicano documenti distinti.
        In teoria bisogna scorrere tutta la lista lunga n, per n volte. Ma essendo la lista ordinata, per ogni signature è sufficiente scorrere
        le successive fino a che sono uguali e fermarsi appena si incronta una signature diversa.
        Per ogni coppia di signature uguali che si incontra si crea una tripla con la coppia di doc_id relativi e un 1 che indica che questa coppia di
        documenti condive 1 signature. Inoltre essendo la lista che stiamo scorrendo ordinata per signature, e scorrendo sempre
        verso il basso, non vegnono create triple in cui, per una stessa signature, compaiono due volte la stessa coppia di doc_id (in qualsiasi ordine).

*/
int find_similarity(int numberOfFiles, char **files, long long unsigned **minhashDocumenti);
void* create_triplets(void* args);
int do_clustering(struct doc_couple* couples, int count);
void check_and_print_similarity(long long unsigned **minhashDocumenti,  struct doc_couple* couples, int index, char **files);
#endif //MINHASHPROJECT_GET_SIMILARITIES_H

