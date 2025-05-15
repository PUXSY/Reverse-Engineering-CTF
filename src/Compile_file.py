from pathlib import Path
import subprocess
import os

Current_path = Path(__file__).parent.resolve()
error_list: list[str] = []

def Get_all_files_name(path: str | Path) -> list[str]:
    dir = Current_path / path.lstrip('/')
    return [str(f) for f in dir.iterdir() if f.is_file()]

def Get_files_name_to_compile(path: str | Path) -> list[str]:
    src_dir = Current_path / path.lstrip('/')
    result = subprocess.run(
        ["git", "ls-files", "--modified"],
        capture_output=True,
        text=True,
        cwd=src_dir
    )
    files = result.stdout.strip().split('\n')
    return [f for f in files if f]


def Compile_c_file(files: list[str]) -> bool:
    compilation_success = False
    
    for file in files:
        try:
            file_path = Path(file)
            
            if file_path.is_dir() or file_path.suffix != '.c':
                continue
            
            compilation_success = True
            print(f"Compiling {file_path.name}...")
            output_file = file_path.with_suffix('.exe')
            
            result = subprocess.run(
                ["gcc", "-s", str(file_path), "-o", str(output_file)],
                check=False,  
                capture_output=True,
                text=True
            )
            
            if result.returncode != 0:
                error_msg = f"Error compiling {file_path.name}:\n{result.stderr}"
                print(error_msg)
                error_list.append(error_msg)
            else:
                print(f"The file {file_path.name} Location: {file_path}")
                print(f"Compiled {file_path.name} successfully.")
            
        except Exception as e:
            print(f"An unexpected error occurred: {e}")
            error_list.append(f"An unexpected error occurred: {e}")
    
    return compilation_success

def Compile_cpp_files(files: list[str]) -> bool:
    compilation_success = False
    
    for file in files:
        try:
            file_path = Path(file)
            
            if file_path.is_dir() or file_path.suffix != '.cpp':
                continue
            
            compilation_success = True
            print(f"Compiling {file_path.name}...")
            output_file = file_path.with_suffix('.exe')
            
            result = subprocess.run(
                ["g++", "-s", str(file_path), "-o", str(output_file)],
                check=False, 
                capture_output=True,    
                text=True
            )
            
            if result.returncode != 0:
                error_msg = f"Error compiling {file_path.name}:\n{result.stderr}"
                print(error_msg)
                error_list.append(error_msg)
            else:
                print(f"The file {file_path.name} Location: {file_path}")
                print(f"Compiled {file_path.name} successfully.")
            
        except Exception as e:
            print(f"An unexpected error occurred: {e}")
            error_list.append(f"An unexpected error occurred: {e}")
    
    return compilation_success

def Compile_rust_files(files: list[str]) -> bool:
    compilation_success = False
    
    for file in files:
        try:
            file_path = Path(file)
            
            if file_path.is_dir() or file_path.suffix != '.rs':
                continue
            
            compilation_success = True
            print(f"Compiling {file_path.name}...")
            
            result = subprocess.run(
                ["rustc", str(file_path), "--out-dir", str(Current_path)],
                check=False, 
                capture_output=True,
                text=True
            )
            
            if result.returncode != 0:
                error_msg = f"Error compiling {file_path.name}:\n{result.stderr}"
                print(error_msg)
                error_list.append(error_msg)
            else:
                file_to_delete = file_path.with_suffix('.pdb')
                if file_to_delete.exists():
                    os.remove(file_to_delete)
                print(f"The file {file_path.name} Location: {file_path}")
                print(f"Compiled {file_path.name} successfully.")
            
        except Exception as e:
            print(f"An unexpected error occurred: {e}")
            error_list.append(f"An unexpected error occurred: {e}")
    
    return compilation_success

def Compile_file(files: list[str]) -> int:
    success_count = 0
    
    C_files: list[str] = [f for f in files if f.endswith('.c')]
    Cpp_files: list[str] = [f for f in files if f.endswith('.cpp')]
    Rust_files: list[str] = [f for f in files if f.endswith('.rs')]
    
    exe_files_before = len([f for f in files if f.endswith('.exe')])
    
    C_files_compiled = Compile_c_file(C_files)
    Cpp_files_compiled = Compile_cpp_files(Cpp_files)
    Rust_files_compiled = Compile_rust_files(Rust_files)
    
    new_files = Get_all_files_name("./")
    exe_files_after = len([f for f in new_files if f.endswith('.exe')])
    
    success_count = exe_files_after - exe_files_before
    
    print("\n ===----=== ===---=== ===----=== \n")
    
    if C_files_compiled:
        print(f"Compiled {len(C_files)} C files.")
    if Cpp_files_compiled:
        print(f"Compiled {len(Cpp_files)} C++ files.")
    if Rust_files_compiled:
        print(f"Compiled {len(Rust_files)} Rust files.")

    print(f"\nSuccess number is {success_count}")

    return success_count
    
def Move_file_to_dir(files: list[str], target_dir: str) -> bool:
    move_success = False
    moved_files_count = 0
    target_path = Current_path / target_dir
    
    if not target_path.exists():
        target_path.mkdir(parents=True, exist_ok=True)
    
    exe_files = [f for f in files if f.endswith('.exe')]
    
    for file in exe_files:
        try:
            file_path = Path(file)
            
            if file_path.is_dir():
                continue
            
            target_file = target_path / file_path.name
            print(f"Moving {file_path.name}...")
            
            try:
                import shutil
                shutil.copy2(file_path, target_file)
                moved_files_count += 1
                
                if file_path.exists():
                    os.remove(file_path)
                
                if target_file.exists():
                    print(f"Successfully moved {file_path.name} to {target_dir}")
                    move_success = True
                else:
                    print(f"Failed to move {file_path.name}")
                    error_list.append(f"Failed to move {file_path.name}")
                
            except FileNotFoundError:
                error_msg = f"Target directory {target_dir} does not exist."
                print(error_msg)
                error_list.append(error_msg)
            except shutil.Error as e:
                error_msg = f"Error moving {file_path.name}: {e}"
                print(error_msg)
                error_list.append(error_msg)
            
        except Exception as e:
            error_msg = f"Error moving {file_path.name}: {e}"
            print(error_msg)
            error_list.append(error_msg)
    
    return move_success and moved_files_count > 0

def Print_Summary(success_count: int, moved_files_count: int, move_success: bool) -> None:
    GREEN = "\033[92m"
    RED = "\033[91m"
    BLUE = "\033[94m"
    BOLD = "\033[1m"
    RESET = "\033[0m"
    
    print(f"\n{BOLD}{BLUE}===== COMPILATION SUMMARY ====={RESET}")
    print(f"{BOLD}Successfully compiled:{RESET} {GREEN}{success_count} files{RESET}")
    
    if error_list:
        print(f"{BOLD}Failed to compile:{RESET} {RED}{len(error_list)} files{RESET}")
        print(f"\n{BOLD}{RED}==== COMPILATION ERRORS ===={RESET}")
        for i, error in enumerate(error_list):
            print(f"{RED}Error {i + 1}:{RESET}\n{error}")
    
    print(f"\n{BOLD}{BLUE}===== FILE MOVEMENT SUMMARY ====={RESET}")
    if move_success:
        print(f"{GREEN}Successfully moved {moved_files_count} executable files{RESET}")
    else:
        print(f"{RED}Failed to move executable files{RESET}")
    
    print(f"\n{BOLD}{BLUE}===== PROCESS COMPLETE ====={RESET}")

def main() -> None:
    error_list.clear()
    files = Get_files_name_to_compile("./")
    new_move_dir = "../Executables"
    success_count = Compile_file(files)
    new_files = Get_all_files_name("./")
    exe_files = [f for f in new_files if f.endswith('.exe')]
    
    print("EXE files found:", exe_files)
    
    move_success = Move_file_to_dir(exe_files, new_move_dir)
    moved_files_count = len(exe_files)
    
    Print_Summary(success_count, moved_files_count, move_success)
        
if __name__ == "__main__":
    main()