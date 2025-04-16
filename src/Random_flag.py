import random

def Generate_flag(level: int, start: int = 1, end: int = 9, end_rdd: int = 9) -> str:
    """
    Generate a CTF flag with random numbers
    
    Args:
        level: The difficulty level of the flag
        start: Starting position for random number generation
        end: Ending position for random number generation
        end_rdd: Upper limit for random numbers
    
    Returns:
        A CTF flag string in the format CTF_level_{random numbers}
    """
    base_flag = "CTF_"
    base_flag2 = "_{"
    number = ""
    
    for i in range(start, end + 1, 1):
        number += str(random.randint(1, end_rdd))
    
    flag = base_flag + str(level) + base_flag2 + number + "}"
    return flag

def septit(s: str, sBy: int) -> None:
    if len(s) % sBy != 0:
        print("Error: String length is not divisible by", sBy)
        return

    part_len = len(s) // sBy
    for i in range(sBy):
        part = s[i * part_len : (i + 1) * part_len]
        print(f"number {i + 1}\n{part}")

def ascii(word):
    new_word = ""
    max_len = 9
    for c in word:
        print(ord(c))
        new_word += str(ord(c))

    print("set_" + new_word[:max_len])


def main() -> None:
    level_start = 6
    level_end = 6
    added_by = 4
    base_random = 24
    
    for flag_level in range(level_start, level_end + 1, 1):
        if (flag_level % 2) == 0:
            base_random += added_by
        print(Generate_flag(flag_level, level_start, base_random, 9))

if __name__ == "__main__":
    ascii("corrupted")