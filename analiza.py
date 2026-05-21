import numpy as np
import matplotlib.pyplot as plt

# tvoji podatki
times = {
    1: [1.808765, 1.821287, 2.262682],
    2: [2.935688, 2.575668, 2.776210],
    4: [4.078977, 4.034724, 4.057066],
    8: [6.247386, 6.828498, 6.174974]
}

# izračun povprečij
avg = {p: np.mean(times[p]) for p in times}

# pospešek
speedup = {p: avg[1] / avg[p] for p in avg}

# Karp-Flatt
def karp_flatt(S, p):
    if p == 1:
        return 0
    return (1/S - 1/p) / (1 - 1/p)

e_metric = {p: karp_flatt(speedup[p], p) for p in speedup}

# izpis tabele
print("p\tavg_time\tS(p)\tKarp-Flatt e")
for p in avg:
    print(f"{p}\t{avg[p]:.4f}\t\t{speedup[p]:.4f}\t{e_metric[p]:.4f}")

# graf pospeška
plt.figure()
plt.plot(list(speedup.keys()), list(speedup.values()), marker='o')
plt.xlabel("Število jeder (p)")
plt.ylabel("Pospešek S(p)")
plt.title("Pospešek Jacobijeve iteracije")
plt.grid(True)
plt.savefig("speedup.png")

# graf Karp-Flatt
plt.figure()
plt.plot(list(e_metric.keys()), list(e_metric.values()), marker='o')
plt.xlabel("Število jeder (p)")
plt.ylabel("Karp-Flatt e")
plt.title("Karp-Flattova metrika")
plt.grid(True)
plt.savefig("karp_flatt.png")

print("\nGrafi shranjeni kot speedup.png in karp_flatt.png")

