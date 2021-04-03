#ifndef MINHASHPROJECT_GET_SIMILARITIES_H
#define MINHASHPROJECT_GET_SIMILARITIES_H


/*
    coppia signature - documento_id. Ogni documento ne ha 200.
*/
struct sign_doc{
    long long unsigned signature;
    int doc_id;
};




/*
    II fase ( la I fase è il calcolo delle signatures):
        files_sketches, è un array di coppie {signature, doc_id}. Le coppie sono relative a uno stesso doc_id, ordinate
        secondo le signatures. Adesso si copiano questi array in un solo array che conterrà quindi tutte le coppie {signature, doc_id} di tutti i documenti.
        A questo punto si fa un nuovo mergesort ordinandoli secondo le signatures.

        Nella versione seriale questo é un array di array, qua é stato fatto l'unrolling e distribuite tutte le coppie su un unico vettore unidimensionale.
        Crea le coppie doc_id - signatures (200 coppie per ogni doc con tutte le sue signatures)

*/
int get_sketches(int i, struct sign_doc *files_sketches, long long unsigned *signatures);


/*
    merge sort parallelo, per ordinare secondo le signatures.
*/

void mergesort_s_signatures(struct sign_doc* X, int l, int n);
void merge_signatures(struct sign_doc* X, int l, int m, int r);

/*
    merge sort serial, usato dopo la prima chiamata ricorsiva parallela per evitare l'incremento esponenziale dei thread.
*/
void mergesort_s_signatures_Serial(struct sign_doc* X, int l, int n);

/*
      III fase
        ordina la lista di coppie docID signatures e cerca le signatures condivise, inserendole in una matrice in cui ogni elemento corrisponde al 
        numero di signature condivise da una coppia di documenti, ordinati secondo il loro ID.
*/
int find_similarity(int numberOfFiles, char **files, long long unsigned **minhashDocumenti);

/*
        Per ogni coppia di signature uguali che si incontra incrementa di uno il valore delle signatures condivise della corrispondente coppia di documenti
*/
void create_signatures_matrix(struct sign_doc* files_sketches, int numberOfFiles, int **matrix_signatures_couples);

/*
    Dalla matrice delle signatures condivise ricava i valori di somiglianza tra i documenti
*/
void print_similarities(int **matrix_signatures_couples, int numberOfFiles, char **files, float *some_results);



#endif //MINHASHPROJECT_GET_SIMILARITIES_H

