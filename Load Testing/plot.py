#!/usr/bin/env python3

data_file = open("data", 'r')
users = []
throughput = []
response_time = []

for data in data_file:
    lst = data.split()
    users.append(int(lst[0]))
    throughput.append(int(float(lst[1])))
    response_time.append(float(lst[2]))

data_file.close()

def graph_response_time():
    import matplotlib.pyplot as plt
    plt.figure(figsize=(10, 8))
    plt.plot(users, response_time, label='Response Time', ls='solid', color='red',
             marker='.', markersize=9, mew=2, linewidth=2)

    plt.xlabel("Concurrent Users")
    plt.ylabel("Avg Response Time (ms)")
    plt.grid(visible=True)
    plt.legend(loc=0)
    plt.savefig("response.png", bbox_inches='tight')


def graph_throughput():
    import matplotlib.pyplot as plt
    plt.figure(figsize=(10, 8))
    plt.plot(users, throughput, label='Throughput', ls='solid', color='blue',
             marker='.', markersize=9, mew=2, linewidth=2)

    plt.xlabel("Concurrent Users")
    plt.ylabel("Throughput (req/s)")
    plt.grid(visible=True)
    plt.legend(loc=0)
    plt.savefig("throughput.png", bbox_inches='tight')


#graph_response_time()
graph_throughput()
