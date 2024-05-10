import sys
import re

def parse_paths(file_path):
    """Parse paths from a file into a list of strings."""
    paths = []
    with open(file_path, 'r') as f:
        for line in f:
            if line.startswith('0<'):
                paths.append(line.strip())
    return paths

def extract_task_ids(path):
    """Extract the task IDs from a path in the form pid<task>."""
    return list(map(int, re.findall(r'<(\d+)>', path)))

def find_rule_violations(paths, pattern):
    """Find all paths containing the specified pattern of task IDs."""
    violations = []
    for path in paths:
        task_ids = extract_task_ids(path)
        pattern_idx = 0
        for task_id in task_ids:
            if task_id == pattern[pattern_idx]:
                pattern_idx += 1
            if pattern_idx == len(pattern):
                violations.append(path)
                break
    return violations

def main(input_file, ancestor, descendant):
    paths = parse_paths(input_file)
    pattern = (ancestor, descendant)
    violations = find_rule_violations(paths, pattern)

    if violations:
        print(f"Rule violation pattern {pattern} found in the following paths:")
        for violation in violations:
            print(violation)
    else:
        print(f"No instances of the rule violation pattern {pattern} found.")

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python find_rule_violations.py <paths_file> <ancestor> <descendant>")
        print("Example pattern: 27 29")
        sys.exit(1)

    input_file = sys.argv[1]
    ancestor = sys.argv[2]
    descendant = sys.argv[3]

    main(input_file, ancestor, descendant)
