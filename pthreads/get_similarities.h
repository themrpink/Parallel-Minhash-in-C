#ifndef MINHASHPROJECT_GET_SIMILARITIES_H
#define MINHASHPROJECT_GET_SIMILARITIES_H

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
        files_sketches, è un array di coppie {signature, doc_id}. Le coppie sono relative a uno stesso doc_id, ordinate
        secondo le signatures. Adesso si copiano questi array in un solo array che conterrà quindi tutte le coppie {signature, doc_id} di tutti i documenti.
        A questo punto si fa un nuovo mergesort ordinandoli secondo le signatures.

        Nella versione seriale questo é un array di array, qua é stato fatto l'unrolling e distribuite tutte le coppie su un unico vettore unidimensionale.
        Crea le coppie doc_id - signatures (200 coppie per ogni doc con tutte le sue signatures)

*/
int get_sketches(int i, struct sign_doc *files_sketches, long long unsigned *signatures,char *filename);


/*
    merge sort con impostazione parallelizzabile.
*/
void mergesort_s_doc_id(struct doc_couple*  X, int l, int n);
void merge_signatures(struct sign_doc* X, int l, int m, int r);
void merge_doc_id(struct doc_couple*  X, int l, int m, int r);
void mergesort_s_signatures(struct sign_doc* X, int l, int n);


/*
      III fase
        si espande la lista di coppie {signature, doc_id} in una lista di triple, prendendo ogni signature della lista di coppie
        e ricavandone le triple: {doc_id, doc_id, numero_di_signatures_condivise}, in cui i doc_id indicano solo documenti distinti.
        In teoria bisogna scorrere tutta la lista lunga n, per n volte. Ma essendo stata la lista ordinata con il mergesort, per ogni signature è sufficiente scorrere
        le successive fino a che sono uguali e fermarsi appena si incronta una signature diversa.
*/
int find_similarity(int numberOfFiles, char **files, long long unsigned **minhashDocumenti);

/*
        Per ogni coppia di signature uguali che si incontra si crea una tripla con la coppia di doc_id relativi e un 1 che indica che questa coppia di
        documenti condive 1 signature. Inoltre essendo la lista che stiamo scorrendo ordinata per signature, e scorrendo sempre
        verso il basso, non vegnono create triple in cui, per una stessa signature, compaiono due volte la stessa coppia di doc_id (in qualsiasi ordine).
*/
int create_triplets(struct sign_doc* files_sketches, int numberOfFiles, struct doc_couple* couples);

/*
    raccoglie le coppie di documenti che hanno almeno una signature in comune in strutture del tipo:
    doc_id1
    doc_id2
    numero di signatures condivise
*/
int do_clustering(struct doc_couple* couples, int count);

/*
    stampa a schermo le coppie di file che condividono signatures insieme al grado di somiglianza (numero di signatures in comune / numero tot di signatures)

    confronta le signatures di tutti i documenti e salva quali hanno signatures in comune e quante.
    Restituisce anche una percentuale di somiglianza tra i documenti che hanno almeno una signatures in comune.
    Dal paper: "In the final phase, we produce the complete clustering. We examine each <ID, ID, count> triplet
    and decide if the document pair exceeds our threshold for resemblance."
*/
void check_and_print_similarity(long long unsigned **minhashDocumenti,  struct doc_couple* couples, int index, char **files);
#endif //MINHASHPROJECT_GET_SIMILARITIES_H

