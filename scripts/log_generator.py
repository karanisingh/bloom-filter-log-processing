import random
import sys

def generate_logs(output_file, num_entries, num_tasks=65):




    pid_pool = set()
    pid_task_map = {}
    
    # root pid and task
    pid_pool.add(0)
    pid_task_map[0] = 0
    
    logs=[]


    for _ in range(num_entries):
        # Parent pid existing pid
        parent_pid = random.choice(list(pid_pool))

        # Unique child PID
        child_pid = random.randint(1, 1000)
        while child_pid in pid_pool:
            child_pid = random.randint(1, 1000)

        # Task ID 
        task_id = random.randint(1, num_tasks)

        pid_pool.add(child_pid)
        pid_task_map[child_pid] = task_id
        logs.append((parent_pid, child_pid, task_id))

    with open(output_file, 'w', newline='') as file:
        for log in logs:
            file.write(f"{log[0]},{log[1]},{log[2]}\n")
            print(log)



generate_logs(output_file=sys.argv[1], num_entries=sys.argv[2], num_tasks=sys.argv[3])
