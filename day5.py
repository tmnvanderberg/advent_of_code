# example:
# page_ordering_rules = [
#     [47,53],
#     [97,13],
#     [97,61],
#     [97,47],
#     [75,29],
#     [61,13],
#     [75,53],
#     [29,13],
#     [97,29],
#     [53,29],
#     [61,53],
#     [97,53],
#     [61,29],
#     [47,13],
#     [75,47],
#     [97,75],
#     [47,61],
#     [75,61],
#     [47,29],
#     [75,13],
#     [53,13]
# ]

# page_updates = [
# [75,47,61,53,29],
# [97,61,53,29,13],
# [75,29,13],
# [75,97,47,61,53],
# [61,13,29],
# [97,13,75,29,47]
# ]

page_ordering_rules = []
with open('input.txt', 'r') as file:
    for line in file:
        page_ordering_rules.append(line.strip().split('|'))

page_updates = []
with open('updates.txt', 'r') as file:
    for line in file:
        update = line.strip().split(',')
        page_updates.append(update)

def update_in_correct_order(update):
    for left_side_index, page in enumerate(update):
        for rule in page_ordering_rules:
            if rule[0] == page and rule[1] in update: 
                right_side_idx = update.index(rule[1]);
                if right_side_idx < left_side_index:
                    return False
    return True

def get_middle_page(update):
    middle_idx = int((len(update) - 1) / 2)
    return update[middle_idx]

sum = 0
for i, update in enumerate(page_updates):
    result = update_in_correct_order(update)
    if result:
        sum += int(get_middle_page(update))

print("sum:")
print(sum)

        
