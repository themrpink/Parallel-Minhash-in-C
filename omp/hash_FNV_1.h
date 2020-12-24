#include <omp.h>

#define PRIMES_SIZE 199
#define K_SHINGLE 54
#define MAX_LONG_LONG 0xcbf29ce484222325LL

/*
unsigned long long primes[] = { 1099511628211,  109951181297,   109951192301,   651571110623,   651571122467, 
                                1000000000039,  1000000001939,  1000000016339,  109951179061,   109951191323,
                                1000000001083,  132951242141,   132951254257,   132951251243,   132951415591, 
                                132951423139,   132991532629,	132991532633,	132991532657,	132991532693,		
                                132991532731,   145991523677,	145991523683,	145991523701,	145991523737,
                                145991535751,	145991535773,	145991535791,	145991535797,   145991614267,	
                                145991614283,	145991614291,	145991614343,   145991620727,   145991620729,		
                                146991637937,	146991637951,	146991637987,	146991637993 ,  146991642143,	
                                146991642151,	146991642191,	146991642229,   216991644997,	216991645009,	
                                216991645049,	216991645133,   216991652231,	216991652281,	216991652291,	
                                216991652299,   216991658771,	216991658783,	216991658789,	216991658831,
                                246971709641,	246971709709,	246971709767,	246971709823 ,  246971722979,	
                                246971722981,	246971722999,	246971723027,   246971723549,	246971723591,		
                                255571756877,   255571760641,	255571760683,	255571760687,	255571760693,	
                                255571770317,	255571770319,	255571770373 ,  291571836389,	291571836401,	
                                291571836427,	291571836529,   291571842491,	291571842509,	291571842533,	
                                291571850239,	291571850261,	291571850327,	291571850347 ,  351571868903,	
                                132991532699,	351571869029,	351571869037,   351571873153,	351571873207,	
                                351571878253,   351571881943,	351571881949,	351571881953,	351571881979,
                                351571882549,	351571882561,	351571882639,	351571882691,   132991532699,
                                411571889473,	411571889507,	411571889527,	411571889557,	411571889569,		
                                411571914487,	411571914521,	411571914529,	411571914569,	411571914577,
                                678923973041,	678923973059,	678923973061,	678923973113,	678923973121,	
                                678923973163,	678923973197,	678923973227,	678923973257,	678923973263,
                                473588577479,	473588577509,	473588577569,	473588577587,	473588577589,	
                                473588577601,	473588577613,	473588577653,	473588577667,	473588577689,
                                948167826623,	948167826631,	948167826761,	948167826763,	948167826769,	
                                948167826773,	948167826779,	948167826883,	948167826913,	948167827031,
                                329868119827,	329868119863,	329868119879,	329868119909,	329868119927,	
                                329868119929,	329868119951,	329868120053,	329868120061,	329868120073,
                                231680873923,	231680873929,	231680873947,	231680873993,	231680873999,	
                                231680874023,	231680874133,	231680874167,	231680874193,	231680874203,
                                255148978289,	255148978291,	255148978307,	255148978313,	255148978367,	
                                255148978459,	255148978471,	255148978493,	255148978507,	255148978517,
                                825918857579,	825918857593,	825918857623,	825918857629,	825918857671,	
                                825918857681,	825918857683,	825918857711,	825918857747,	825918857759,
                                394695996511,	394695996589,	394695996593,	394695996659,	394695996679,	
                                394695996683,	394695996689,	394695996691,	394695996697,	394695996701,
                                416473710067,	416473710071,	416473710073,	416473710077,	416473710097,	
                                416473710127,	416473710157,	416473710223,	416473710269,	416473710281
                            };

*/

unsigned long long rands[] = {13607075548612569373LLU,
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
                    12224931565145661247LLU};



/*
    versione originale della funzione di hash FNV_1a
*/
int hash_FNV_1a(char *shingle, unsigned long long *hash);


/*
    funzione che si limita ad eseguire lo XOR 
*/
unsigned long long* get_signatures(char **shingles, long tot_shingles, int thread_count);


unsigned long long* get_signatures_s(char **shingles, long tot_shingles, int thread_count);