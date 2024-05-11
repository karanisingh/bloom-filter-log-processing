import sys
import os

def extract_chain(path, start_task, end_task):
    """Extracts the chain between start_task and end_task within a given path."""
    try:
        start_idx = next(i for i, node in enumerate(path) if f"<{start_task}>" in node)
        end_idx = next(i for i, node in enumerate(path) if f"<{end_task}>" in node and i > start_idx)
        return " --> ".join(path[start_idx:end_idx + 1])
    except StopIteration:
        return None

def main(input_file, start_task, end_task):



    # Parse the paths file
    paths = []
    with open(input_file, 'r') as file:
        for line in file:
            if line.startswith("All paths from root node"):
                continue
            paths.append(line.strip().split(" --> "))

    # Extract unique chains
    output_file=f"{os.path.splitext(input_file)[0][:-6]}_SOLUTION.txt"
    with open(output_file, "w") as f:
        chains = set()
        for path in paths:
            chain = extract_chain(path, start_task, end_task)
            if chain:
                chains.add(chain)

        if chains:
            print(f"Rule violation from task {start_task} to {end_task} found in the following unique chains:")
            f.write(f"Rule violation from task {start_task} to {end_task} found in the following unique chains:\n")
            for chain in sorted(chains):
                print(chain)
                f.write(chain + "\n")
        else:
            f.write(f"No chains found from task {start_task} to {end_task}.\n")
            print(f"No chains found from task {start_task} to {end_task}.")

if __name__ == '__main__':
    if len(sys.argv) != 4:
        print("Usage: python find_rule_violations.py <paths_file> <start_task> <end_task>")
        sys.exit(1)

    input_file = sys.argv[1]
    start_task = sys.argv[2]
    end_task = sys.argv[3]
    main(input_file, start_task, end_task)
