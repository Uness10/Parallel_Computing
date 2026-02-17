import matplotlib.pyplot as plt

p = [1,2,4,8,16]
T = [0.01275, 0.008505, 0.006411, 0.06256, 0.6855]   # fill from output

S = [T[0]/t for t in T]
E = [S[i]/p[i] for i in range(len(p))]

plt.figure(); plt.plot(p,S,'o-'); plt.xlabel('Threads'); plt.ylabel('Speedup');plt.savefig('speedup.png'); plt.show()
plt.figure(); plt.plot(p,E,'o-'); plt.xlabel('Threads'); plt.ylabel('Efficiency'); plt.savefig('efficiency.png'); plt.show()