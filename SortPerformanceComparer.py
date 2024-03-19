import subprocess
import re
import matplotlib.pyplot as plt

def test_sort_time(algorithm, value):
    input_data = "new\n" + f"it RAND {value}\n" + f"time {algorithm}\n" + "free\nquit\n"
    command = './qtest -v 3'
    clist = command.split()
    completed_process = subprocess.run(clist, capture_output=True, text=True, input=input_data)
    output = completed_process.stdout

    elapsed_times = re.findall(r"Delta time = ([\d.]+)", output)    
    float_times = [float(time) for time in elapsed_times]

    return float_times[0]


def test_sort_average_time(algorithm, times, value):
    differences = [test_sort_time(algorithm, value) for _ in range(times)]

    average_time = sum(differences) / len(differences)    
    return average_time
    
def test_algorithm_time(algorithm):
    return [test_sort_average_time(algorithm, 30, value) for value in range(100_000, 1_100_000, 100_000)]



algorithm1_times = test_algorithm_time("sort")
print("sort time", algorithm1_times)
algorithm2_times = test_algorithm_time("list_sort")
print("list_sort time", algorithm2_times)
algorithm3_times = test_algorithm_time("tim_sort")
print("tim_sort time", algorithm3_times)

labels = ['100K', '200K', '300K', '400K', '500K', '600K', '700K', '800K', '900K', '1000K']

x_values = range(1, len(labels) + 1)

plt.figure(figsize=(10, 6))

plt.plot(x_values, algorithm1_times, marker='o', linestyle='-', color='b', label='sort')
plt.plot(x_values, algorithm2_times, marker='s', linestyle='--', color='r', label='list_sort')
plt.plot(x_values, algorithm3_times, marker='^', linestyle='-.', color='g', label='tim_sort')

plt.title('Average Time Difference for Different Algorithms')
plt.xlabel('Input Size')
plt.ylabel('Average Time Difference (seconds)')
plt.xticks(x_values, labels)  
plt.grid(True) 
plt.legend()  

plt.savefig('plot.png')
print("plot.png")