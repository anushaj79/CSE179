import numpy as np
import time

def generate_weighted_adjacency_matrix(num_nodes, probability_of_edge):
    # system clock as seed for rand num
    np.random.seed(int(time.time()))
    
    # init w/ 0s
    adj_matrix = np.zeros((num_nodes, num_nodes), dtype=int)
    
    for i in range(num_nodes):
        for j in range(i + 1, num_nodes):
            # check edge probability
            if np.random.random() < probability_of_edge:
                weight = np.random.randint(1, 101) # weights 1 to 100
                adj_matrix[i, j] = weight
                adj_matrix[j, i] = weight
    return adj_matrix

# 100 nodes, 50% edge probability
num_nodes = 100
probability = 0.5 
matrix = generate_weighted_adjacency_matrix(num_nodes, probability)

# save to file
np.savetxt("adj_matrix.txt", matrix, fmt='%d')
print("Matrix saved to adj_matrix.txt")