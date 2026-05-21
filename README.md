Paralelizacija Jacobijeve iteracije z uporabo MPI

V tem projektu je implementirana paralelna različica Jacobijeve iteracije za reševanje linearnega sistema Ax = b. Jacobijeva metoda je iterativna metoda, pri kateri se vsaka komponenta novega vektorja rešitve izračuna na podlagi vrednosti iz prejšnje iteracije. Metoda je enostavna za implementacijo in dobro primerna za paralelizacijo, saj je izračun posameznih komponent med seboj neodvisen. Kljub temu pa metoda zahteva, da ima vsak proces v vsaki iteraciji dostop do celotnega vektorja iz prejšnje iteracije, kar pomeni, da je pri paralelni izvedbi potrebna komunikacija med procesi.

V tej implementaciji je uporabljena knjižnica MPI. Vsak proces izračuna svoj del vektorja x, nato pa se v vsaki iteraciji izvede globalna komunikacija z uporabo funkcije MPI_Allgather. Ta funkcija zbere delne rezultate vseh procesov in jih razpošlje nazaj vsem procesom. Ker gre za globalno komunikacijo, predstavlja ta korak glavno ozko grlo algoritma.

Program je bil preizkušen za p = 1, 2, 4 in 8 procesov. Za vsako konfiguracijo so bili izvedeni trije zagoni, izračunani pa so bili povprečni časi izvajanja. Povprečni časi so znašali približno 1.96 sekunde za en proces, 2.76 sekunde za dva procesa, 4.06 sekunde za štiri procese in 6.42 sekunde za osem procesov. Rezultati kažejo, da se čas izvajanja z večanjem števila procesov povečuje, kar pomeni, da paralelizacija v tej obliki ne izboljša zmogljivosti.

Na podlagi povprečnih časov je bil izračunan pospešek. Pri dveh procesih je znašal približno 0.71, pri štirih 0.48, pri osmih pa 0.30. To pomeni, da je program pri več procesih počasnejši kot pri sekvenčni izvedbi. Tak rezultat kaže, da komunikacijski stroški popolnoma prevladajo nad računskim delom.

Izračunana je bila tudi Karp-Flattova metrika, ki ocenjuje delež sekvenčnega oziroma neparalelizabilnega dela programa. Pri idealno paralelizabilnem programu bi bila ta metrika blizu nič. V našem primeru pa je znašala približno 1.81 pri dveh procesih, 2.42 pri štirih in 3.59 pri osmih procesih. Tako visoke vrednosti kažejo, da se z večanjem števila procesov povečuje delež časa, ki ga program porabi za komunikacijo in sinhronizacijo.

V repozitoriju sta priložena tudi grafa pospeška in Karp-Flattove metrike, ki vizualno prikazujeta rezultate. Graf pospeška prikazuje, da se program z večanjem števila procesov upočasni, graf Karp-Flattove metrike pa prikazuje naraščanje komunikacijskega deleža.

Na podlagi rezultatov lahko zaključimo, da paralelizacija Jacobijeve iteracije z uporabo MPI_Allgather ni primerna za učinkovito skaliranje na večje število procesov. Komunikacijski stroški so preveliki, da bi paralelizacija prinesla izboljšanje. Za boljše rezultate bi bila potrebna drugačna strategija komunikacije ali drugačen algoritem, ki ne zahteva globalne izmenjave podatkov v vsaki iteraciji.
