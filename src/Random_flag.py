import random

base_flag = "CTF_"
base_flag2 = "_{"

def main(level:int , start: int = 1, end:int = 9) -> str:
    number:str = ""
    
    for i in range(start, end + 1 , 1):
        number += str(random.randint(1, 9))
    
    flag:str = base_flag + str(level) + base_flag2 + number + "}"
    return flag

    
    
if __name__ == "__main__":
    print(main(6, 1, 12))
