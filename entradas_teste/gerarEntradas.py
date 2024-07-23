import random

def generate_adjusted_inputs(group_sizes):
    inputs = []
    for size in group_sizes:
        groups = size
        candidates = size * 2
        input_str = f"{groups} {candidates}\n"
        group_coverage = {i: 0 for i in range(1, groups + 1)}
        candidate_groups = []

        for _ in range(candidates):
            group_count = random.randint(1, max(1, groups // 4))
            available_groups = [group for group in group_coverage if group_coverage[group] < 2]
            if not available_groups:
                available_groups = list(group_coverage.keys())
            group_list = random.sample(available_groups, min(group_count, len(available_groups)))
            candidate_groups.append(group_list)
            for group in group_list:
                group_coverage[group] += 1

        for group_list in candidate_groups:
            input_str += f"{len(group_list)} {' '.join(map(str, group_list))}\n"

        inputs.append(input_str.strip())
    return inputs

def generate_multiple_adjusted_inputs(group_sizes, num_cases):
    all_inputs = {size: [] for size in group_sizes}
    for _ in range(num_cases):
        new_inputs = generate_adjusted_inputs(group_sizes)
        for size, input_str in zip(group_sizes, new_inputs):
            all_inputs[size].append(input_str)
    return all_inputs

# Define the group sizes and number of additional cases
group_sizes = [10, 15, 20, 25, 30]
num_cases = 4

# Generate the inputs
additional_inputs = generate_multiple_adjusted_inputs(group_sizes, num_cases)

# Writing each input string to a separate file
for size, input_list in additional_inputs.items():
    for i, input_str in enumerate(input_list):
        file_path = f"entrada_teste_{size}-{i+1}.txt"
        with open(file_path, "w") as file:
            file.write(input_str)

