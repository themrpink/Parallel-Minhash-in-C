#include "hash_FNV_1.h"
#include <stdlib.h>
#include <omp.h>
#include "time_test.h"
#include <stdio.h>
//region rands
unsigned long long rands[] = {  13607075548612569373LLU,
                                6724581513526549887LLU,
                                17106295739405559511LLU,
                                14787585135548641465LLU,
                                292847945840093745LLU,
                                8049763499356606027LLU,
                                15637694967600532241LLU,
                                546883269403658741LLU,
                                9547651809858159867LLU,
                                13741782622218477964LLU,
                                1281195439890782676LLU,
                                6895789442644461764LLU,
                                10135650690516633637LLU,
                                14067412151873856950LLU,
                                8257984754116390691LLU,
                                3430705361306208310LLU,
                                11216566187884449647LLU,
                                6841438022211000612LLU,
                                17135188373358694256LLU,
                                12384368699402583722LLU,
                                8753131265909837561LLU,
                                7928180098486839268LLU,
                                3317948294049201653LLU,
                                17149896817117144698LLU,
                                15532366952631652674LLU,
                                5827459046541200887LLU,
                                16527900765249747595LLU,
                                6676221153057468854LLU,
                                11078971663109956336LLU,
                                14553700831387976713LLU,
                                16154985434164788279LLU,
                                6687099180878993823LLU,
                                10543118929290118560LLU,
                                6044303804067535780LLU,
                                18051368931209684495LLU,
                                14822236361818360481LLU,
                                464356750843487092LLU,
                                17384277664958056646LLU,
                                13227070362197544836LLU,
                                12941341091256566363LLU,
                                4776066651596779628LLU,
                                4818078418607803440LLU,
                                10379017001966116427LLU,
                                8255710756562628660LLU,
                                12996123098922535529LLU,
                                10660799139924559459LLU,
                                6549486112595536064LLU,
                                7584285442778285570LLU,
                                14407340697640637034LLU,
                                11880455823546707620LLU,
                                10158649605590915634LLU,
                                17031310892897918455LLU,
                                1359456705914258913LLU,
                                1557850910689356578LLU,
                                14432935773709202344LLU,
                                6473483066673030592LLU,
                                12106314620936873090LLU,
                                6583448056779229994LLU,
                                8479753413619332985LLU,
                                14907834505421981937LLU,
                                7909609073945713871LLU,
                                15217350666928672457LLU,
                                11418418131706882065LLU,
                                1830642359891467707LLU,
                                10532601144459360175LLU,
                                178752245194694014LLU,
                                1049359246464645662LLU,
                                7778224471053680891LLU,
                                15215260858886191924LLU,
                                17406659347475386477LLU,
                                14406060784847088595LLU,
                                7397296009244981333LLU,
                                7755021588136548049LLU,
                                9621452582484857535LLU,
                                11811214309069705404LLU,
                                6214785231361814872LLU,
                                13490129081306709352LLU,
                                13103093761917352898LLU,
                                13164773927936938361LLU,
                                15955607180950688376LLU,
                                17326919350253623356LLU,
                                9602614384120864561LLU,
                                11908580967693251601LLU,
                                7626013966648637906LLU,
                                14139955287190577384LLU,
                                11404561611827368728LLU,
                                16447586033111540458LLU,
                                11410751253293825905LLU,
                                7173719193180271023LLU,
                                13283810940771574129LLU,
                                8563997062895065788LLU,
                                5110918680824755301LLU,
                                10320047841957476286LLU,
                                7040301337940045398LLU,
                                11304915738113454837LLU,
                                3760788921625533606LLU,
                                3240930222538146185LLU,
                                11558328691696563902LLU,
                                5468122936120969397LLU,
                                2774874833989784366LLU,
                                9373445716909776138LLU,
                                5137528786286706789LLU,
                                9419298703698019402LLU,
                                10820545803729088493LLU,
                                16179756200924539878LLU,
                                1771541244613947827LLU,
                                5933715785359855491LLU,
                                15607194393096034934LLU,
                                14135495408780096059LLU,
                                9315814031575003467LLU,
                                5352661913080345903LLU,
                                458839764326547558LLU,
                                9953095185359559958LLU,
                                8933312838247909554LLU,
                                4954114591959976175LLU,
                                14363575959769456526LLU,
                                5565131190638897340LLU,
                                297376938581841203LLU,
                                3055243538190604707LLU,
                                5990127727835594668LLU,
                                7442080992510488480LLU,
                                12220119297091243887LLU,
                                5589749629498347785LLU,
                                273647784084440750LLU,
                                10885833917494544680LLU,
                                7589354661681006775LLU,
                                8669129791924626242LLU,
                                13347441906607736745LLU,
                                16308205548040749367LLU,
                                16317109651005582771LLU,
                                14015980185622669193LLU,
                                3560988219446748065LLU,
                                8388761270749489121LLU,
                                12900320766095563119LLU,
                                8424121507869464614LLU,
                                1197090044535251572LLU,
                                7304255148764000654LLU,
                                16025879687559190905LLU,
                                13438728442683970267LLU,
                                3504870943865518198LLU,
                                5857349621290570491LLU,
                                17573809352767716253LLU,
                                2241407740459094696LLU,
                                10139810482826985967LLU,
                                9451604760124088547LLU,
                                5941293396739400986LLU,
                                459535475481317476LLU,
                                10379510158051150507LLU,
                                11950264440258368761LLU,
                                9995849545131003129LLU,
                                3336512161487980854LLU,
                                13461226738077569233LLU,
                                14109577940551753875LLU,
                                6056940601115943276LLU,
                                11675881951432235658LLU,
                                18039365352906842198LLU,
                                1259478543128035939LLU,
                                10059119301837424593LLU,
                                7680251611038283479LLU,
                                17637575655576333936LLU,
                                17177271429684140089LLU,
                                15238362303124959672LLU,
                                11196710064013732240LLU,
                                251973352810113506LLU,
                                10312073290506523207LLU,
                                11522003287628195589LLU,
                                18233207730152188081LLU,
                                17077628851923969272LLU,
                                6485428101908208555LLU,
                                6992706909397891290LLU,
                                9106877677712333484LLU,
                                5383626624135652339LLU,
                                1986973811820779107LLU,
                                7480798465127375142LLU,
                                14171138134388158470LLU,
                                12832654370460423822LLU,
                                7345214996418073181LLU,
                                1939088688189165076LLU,
                                10331433707254981679LLU,
                                4545489122396197894LLU,
                                7039448318078261704LLU,
                                16699023772199841418LLU,
                                15738356687283341322LLU,
                                2018154224641214941LLU,
                                13968598487745261956LLU,
                                4115034771528432391LLU,
                                7713359175027525236LLU,
                                3211235741753957023LLU,
                                11710430994315864172LLU,
                                12371922613713440189LLU,
                                2390774516272129433LLU,
                                11043387554179150496LLU,
                                9583915719729455399LLU,
                                11815756939233406799LLU,
                                3155865818981820135LLU,
                                6740592743037924425LLU,
                                9438487122589321230LLU,
                                8160521925384610550LLU,
                                13211065264639032347LLU,
};
//endregion

/**
 * versione originale della funzione di hash FNV_1a, dalla quale si ottiene la prima signature di ogni documento
 * @param shingle
 * @param hash
 * @return
 */
int hash_FNV_1a(char *shingle, long long unsigned *hash){

    long long unsigned FNV_offset_basis = FNV_OFFSET_BASIS;
    long long unsigned prime = FNV_PRIME;

    *hash = FNV_offset_basis;

    for(int i=0; i<K_SHINGLE; i++){
        (*hash) *= prime;
        (*hash) ^= shingle[i];
    }

    return 0;
}
/**
 * funzione che si limita ad eseguire lo XOR tra i valori di hash degli shingles ottenuti con hash_FNV_1a() e 199 numeri random pre-generati, uguali per tutti i
 * documenti. Di questi salva i minori, che compongono le 199+1 signatures di ogni ducumento.
 * @param shingles
 * @param tot_shingles
 * @return
 */
long long unsigned* get_signatures(char **shingles, long long tot_shingles){
    double start;
    double  end;
    start=omp_get_wtime();

    long long unsigned hash=0;
    long long unsigned minhash=MAX_LONG_LONG_U;
    long long unsigned *hashed_shingles = (long long unsigned *)malloc(tot_shingles*sizeof(long long unsigned));
    long long unsigned *signatures;

    signatures = (long long unsigned *)malloc(200*sizeof(long long unsigned));

    start=omp_get_wtime();
    for(long long j=0; j < tot_shingles; j++){
        //lancia la prima funzione di hash su ogni shingle
        hash_FNV_1a(shingles[j], &hash);
        hashed_shingles[j] = hash;

        if(hash < minhash)
            minhash = hash;
    }
    *signatures=minhash;

    //applica la funzione di hash con PRIMES_SIZE valori diversi su tutte gli hashed_shingles, e ricava i minhash
    for(int i=0; i<PRIMES_SIZE; i++){
        minhash = MAX_LONG_LONG_U;
        //lo xor viene applicato per ogni numero random [i] e tra valore di hash ottenuto degli shingle ottenuti con hash_FNV_1a
        for(long long j=0; j<tot_shingles; j++){
            hash = hashed_shingles[j] ^ rands[i];

            if(hash < minhash)
                minhash = hash;
        }
        *(signatures+i+1)=minhash;
    }
    end = omp_get_wtime();
    exectimes(end-start, GET_SIGNATURES, SET_TIME);

    free(hashed_shingles);
    return signatures;
}

