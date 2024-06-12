import numpy as np
import matplotlib.pyplot as plt
from scapy.all import *

#read pcap file
filename=''
packets = rdpcap(filename)

flow={}
latency_flow={}
for pkt in packets:
    if TCP in pkt:
        src_ip = pkt[IP].src
        dst_ip = pkt[IP].dst
        src_port = pkt[TCP].sport
        dst_port = pkt[TCP].dport

        flow_key = src_ip + ':' + str(src_port) + '-' + dst_ip + ':' + str(dst_port)

        if flow_key in flow:
            pac_len = len(pkt)
            pac_len = pac_len * 8
            flow[flow_key].append(pac_len)
            #calculate the latency in ms
            latency = (pkt.time - flow[flow_key][-1]) * 1000
            latency_flow[flow_key].append(latency)
        else:
            flow[flow_key] = []
            flow[flow_key].append(len(pkt) * 8)
            latency_flow[flow_key] = []
            latency_flow[flow_key].append(0)

avg_tput = []
for key in flow:
    x = np.sum(flow[key])/len(flow[key])
    avg_tput.append(x)

avg_latency = []
for key in latency_flow:
    x = np.sum(latency_flow[key])/len(latency_flow[key])
    avg_latency.append(x)

#plot the average throughput of each flow
plt.figure(figsize=(10, 6))
plt.bar(range(len(avg_tput)), avg_tput)
plt.xlabel('Flow')
plt.ylabel('Average Throughput')
plt.title('Average Throughput of each Flow')
plt.show()

#plot the average latency of each flow
plt.figure(figsize=(10, 6))
plt.bar(range(len(avg_latency)), avg_latency)
plt.xlabel('Flow')
plt.ylabel('Average Latency')
plt.title('Average Latency of each Flow')
plt.show()





        

        




        


    