import random
import hashlib
from pathlib import Path
import json
import sys

import random

class Create_Flag:
    def __init__(self, FlagSentences: list[str]):
        self.flag_sentences = FlagSentences
        self.rm = random.Random()
        
    def Create_Flag(self, secret_mesege: str) -> str:
        chars = list(secret_mesege)

        # Replace spaces with random separators and capitalize next char if removed
        i = 0
        while i < len(chars):
            if chars[i] == " ":
                c = self.rm.randint(1, 2)
                if c == 1:
                    chars[i] = "_"
                else:
                    chars[i] = "-"
            i += 1

        secret_mesege = "".join(chars)

        LookupTable = {
            "i": ("1", "!", "|"),
            "l": ("1", "|", "!"),
            "o": ("0", "@"),
            "a": ("^", ),
            "g": ("9",),
            "x": ("%", "*"),
            "s": ("5", "$"),
            "t": ("7", "+"),
            "b": ("8",),
            "z": ("2",),
            "c": ("(", "<"),
            "m": ("nn",),
            "w": ("vv",),
        }

        chars = list(secret_mesege)

        for i, char in enumerate(chars):
            lower = char.lower()
            if lower in LookupTable:
                choice = self.rm.choices([1, 2, 3], weights=[0.10, 0.10, 0.80], k=1)[0]
                if choice == 1:
                    chars[i] = self.rm.choice(LookupTable[lower])
                elif choice == 2:
                    chars[i] = char.upper()
                else:
                    chars[i] = char

        for i, char in enumerate(chars):
            if char.isalpha() and self.rm.random() < 0.1:
                chars[i] = char.upper()

        return "".join(chars)
                    
    def Create_Flags(self) -> list[str]:
        return ["CTF{" + self.Create_Flag(secret_message) + "}" for secret_message in self.flag_sentences]

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

def resource_path(relative_path: str) -> Path:
    try:
        base_path = Path(sys._MEIPASS)
    except AttributeError:
        base_path = Path(__file__).parent.resolve()
    return (base_path / relative_path).resolve()

def Get_Corect_Path(path_to_json: Path | str) -> Path:
    try:
        path = Path(path_to_json).resolve()
    except Path.exists(path_to_json):
        path = resource_path(path_to_json)
    return path

def Get_Flags_list(path_to_json: Path | str) -> list[dict[str, str | int]]:
    flags = []
    path = Get_Corect_Path(path_to_json)

    if not path.exists():
        print("Path does not exist")
        path = resource_path("./flags.json")
        print("Using default path:", path)
    if not path.exists():
        print("Default path does not exist")
        print("Using empty flags list")
        return flags

    with open(path, "r") as file:
        data = json.load(file)
        for level_name, level_data in data.get("flags", {}).items():
            flag = level_data.get("flag")
            points = level_data.get("points")
            group = level_data.get("group")
            
            flags.append({
                "name": level_name,
                "flag": flag,
                "points": points,
                "group": group
            })

    return flags

def flags_sha512(flag:  str) -> str:
    return hashlib.sha512(flag.encode()).hexdigest()

def cut_string_middle(s: str, cuts: int) -> list[str]:
    if cuts <= 0:
        return [s]
    
    length = len(s)
    part_length = length // (cuts)
    result = []
    
    for i in range(cuts):
        start = i * part_length
        end = (i + 1) * part_length
        result.append(s[start:end])
    
    result.append(s[(cuts * part_length):])
    return result
        
def main() -> None:
    fg = Create_Flag(
        ["Congratulations U solved the first level!",           #Level 1
         "It's still easy?",                                    #Level 2
         "You understand how this works, right?",               #Level 3
         "This is getting difficult.",                          #Level 4
         "ASCII is more like BINASCII!!!",                      #Level 5
         "You should be able to solve this one...",             #Level 6
         "When creating the flags ti's wasn't finish yet..",    #Level 7
         "To be honest, I don't understand what I did here.",   #Level 8
         "If it doesn't work, blame ChatGPT!",                  #Level 9
         "This is the first level using class.",                #Level 10
         "This is my first time using OpenGL.",                 #Level 11
         ]
        )

    while True: 
        flags = fg.Create_Flags()
        for i, flag in enumerate(flags):
            print(f"Flag {i + 1}: {flag}")
            
        if input("Generate new flags? (y/n): ").lower() == "y":
            continue
        else:
            for i, flag in enumerate(flags):
                print(f"Flag {i + 1}: { flags_sha512(flag)}")
                
            break

if __name__ == "__main__": 
    flags = Get_Flags_list("./flags.json")
    c = 0
    for flag in flags:
        c += flag.get("points")
        
    print("Total points:", c)
