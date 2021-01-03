






int find_similarity(int numberOfFiles, char **files, long long unsigned **minhashDocumenti, struct doc_couple* couples ){

    for (int i = 0; i < numberOfFiles; ++i) {
        for (int j = 0; j < numberOfFiles; ++j) {
            long totalSignaturesEquals = 0;
            for (int k = 0; k < N_SIGNATURES; ++k) {
                if (minhashDocumenti[i][k] == minhashDocumenti[j][k]) {
                    totalSignaturesEquals++;
                }
            }
            double coefficient = totalSignaturesEquals / (N_SIGNATURES);
            printf("File %s e %s sono simili %s",files[i],files[j],coefficient > COEFFICIENTE_SIMILARITA ? "Si" : "No");
        }
    }
    return 0;
}



