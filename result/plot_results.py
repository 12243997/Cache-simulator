import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick

data = pd.read_csv("result/block_size_results.csv")

plt.plot(data["BlockSize"], data["MissRate"], marker="o")

plt.xlabel("Block Size (Bytes)")
plt.ylabel("Miss Rate")
plt.title("Block Size vs Miss Rate")

plt.gca().yaxis.set_major_formatter(mtick.PercentFormatter(1.0))

plt.xticks(data["BlockSize"]) 
#plt.savefig("result/figures/block_size_miss_rate.png")
#plt.show()

# Associativity experiment
assoc_data = pd.read_csv("result/associativity_result.csv")

plt.figure()

plt.plot(
    assoc_data["Associativity"],
    assoc_data["MissRate"],
    marker="o"
)

plt.xlabel("Associativity (Ways)")
plt.ylabel("Miss Rate")
plt.title("Associativity vs Miss Rate")

plt.xticks(assoc_data["Associativity"])

plt.gca().yaxis.set_major_formatter(
    mtick.PercentFormatter(1.0)
)

plt.savefig(
    "result/figures/associativity_miss_rate.png"
)

plt.show()