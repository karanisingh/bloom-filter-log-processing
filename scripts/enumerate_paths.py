import sys
import csv
import os

pid_to_task = {}


def construct_graph(edges):
    graph = {}
    for edge in edges:
        u, v = edge
        if u not in graph:
            graph[u] = []
        graph[u].append(v)
    return graph

def find_paths(graph, node, path=None, paths=None):
    if path is None:
        path = []
    if paths is None:
        paths = []
    path.append(node)
    if node not in graph:
        paths.append(path.copy())  # Append a copy of the path
    else:
        for neighbor in graph[node]:
            find_paths(graph, neighbor, path.copy(), paths)  # Pass a copy of the path
    return paths

def parse_edges(file_path):
    global pid_to_task
    edges = []
    with open(file_path, newline='') as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
            from_node, to_node = map(int, row[:2])
            pid_to_task[to_node] = row[2]
            edges.append((from_node, to_node))
    return edges

def format_path(path):
    formatted = []
    for pid in path:
        task = pid_to_task.get(pid, "unknown")  # If task is not in the mapping, use 'unknown'
        formatted.append(f"{pid}<{task}>")
    return " --> ".join(formatted)


# Example usage
if len(sys.argv) != 2:
    print("Usage: python find_paths.py <edges_file>")
    sys.exit(1)




input_file = sys.argv[1]
output_file= f"{os.path.splitext(input_file)[0]}_PATHS.txt" 
edges = parse_edges(input_file)

root = 0  # Set the starting node
graph = construct_graph(edges)
paths = find_paths(graph, root)



with open(output_file, "w") as f:
    # Print all paths in the desired format to both the console and the file
    f.write(f"All paths from root node {root}:\n")
    print(f"All paths from root node {root}:")
    for path in paths:
        formatted_path = format_path(path)
        f.write(formatted_path + "\n")
        print(formatted_path)
