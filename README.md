# Paralelizacija Jacobijeve iteracije z uporabo MPI

Ta projekt implementira Jacobijevo iterativno metodo za reševanje linearnih sistemov Ax = b z uporabo paralelizacije preko knjižnice MPI. Implementirani sta dve različni komunikacijski strategiji, ki omogočata primerjavo učinkovitosti in skalabilnosti.

## 1. Implementirani pristopi

### 1.1 MPI_Allgather pristop (datoteka: jacobi.c)
- Vsak proces izračuna svoj del vektorja x.
- Funkcija MPI_Allgather v vsaki iteraciji zbere vse delne rezultate in jih razpošlje vsem procesom.
- Prednost: enostavna implementacija.
- Slabost: globalna komunikacija → slaba skalabilnost.

### 1.2 Scatter/Bcast/Gather pristop (datoteka: jakobieva.c)
- MPI_Scatter razdeli matriko A in vektor b med procese.
- MPI_Bcast razpošlje trenutni vektor x vsem procesom.
- MPI_Gather zbere nove delne rezultate na proces 0.
- Prednost: manj globalne sinhronizacije in boljša skalabilnost.

## 2. Struktura projekta

jacobi.c                   # Prva MPI implementacija (Allgather)  
jakobieva.c                # Druga MPI implementacija (Scatter/Bcast/Gather)  
analiza.py                 # Izračun povprečij, speedup in grafov  
speedup.png                # Graf pospeška  
karp_flatt.png             # Karp–Flatt graf  
primerjava_casov.png       # Primerjava časov obeh metod  
primerjava_speedup.png     # Primerjava pospeška obeh metod  
README.md                  # Opis projekta  

## 3. Kompilacija

mpicc jacobi.c -o jacobi  
mpicc jakobieva.c -o jakobieva  

## 4. Zagon

### 4.1 Zagon posamezne implementacije

mpirun --oversubscribe -np 4 ./jacobi  
mpirun --oversubscribe -np 4 ./jakobieva  

### 4.2 Zagon za vse p = 1, 2, 4, 8 (kot v projektu)

for p in 1 2 4 8; do  
    echo "===== np = $p ====="  
    for r in 1 2 3; do  
        echo "Run $r:"  
        mpirun --oversubscribe -np $p ./jakobieva  
    done  
done  

Enako je bilo uporabljeno tudi za jacobi.

## 5. Povzetek rezultatov

### Allgather pristop (jacobi.c)
- Čas izvajanja se povečuje z večanjem števila procesov.
- Pospešek pada.
- Karp–Flattova metrika kaže velik komunikacijski delež.

### Scatter/Bcast/Gather pristop (jakobieva.c)
- Čas izvajanja se zmanjšuje z večanjem števila procesov.
- Skalira se do 8 procesov.
- Komunikacijski overhead je bistveno manjši.

## 6. Zaključek

Druga implementacija (Scatter/Bcast/Gather) je bistveno učinkovitejša od Allgather pristopa.  
Zmanjšuje globalno komunikacijo, bolje izkorišča paralelizacijo in dosega boljše časovne rezultate.

## 7. Avtor

Marija
