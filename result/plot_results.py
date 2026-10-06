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
plt.savefig("result/figures/block_size_miss_rate.png")
plt.show()