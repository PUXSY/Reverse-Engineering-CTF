from pathlib import Path
import subprocess
import os

Current_path = Path(__file__).parent.resolve()


def Get_file_name(path: str) -> list[str]:
    src_dir = Current_path / path.lstrip('/')
    return [str(f) for f in src_dir.iterdir() if f.is_file()]

def Compile_file_gcc(files: list[str]) -> bool:
    compilation_success = False
    
    for file in files:
        try:
            file_path = Path(file)
            
            if file_path.is_dir() or file_path.suffix != '.c':
                continue
            
            compilation_success = True
            print(f"Compiling {file_path.name}...")
            output_file = file_path.with_suffix('.exe')
            subprocess.run(["gcc", str(file_path), "-o", str(output_file)], check=True)
            print(f"The file {file_path.name} Location: {file_path}")
            print(f"Compiled {file_path.name} successfully.")
            
        except subprocess.CalledProcessError as e:
            print(f"Error compiling {file_path.name}: {e}")
        except Exception as e:
            print(f"An unexpected error occurred: {e}")
    
    return compilation_success

def Compile_file_gplasplas(files: list[str]) -> bool:
    compilation_success = False
    
    for file in files:
        try:
            file_path = Path(file)
            
            if file_path.is_dir() or file_path.suffix != '.cpp':
                continue
            
            compilation_success = True
            print(f"Compiling {file_path.name}...")
            output_file = file_path.with_suffix('.exe')
            subprocess.run(["g++", str(file_path), "-o", str(output_file)], check=True)
            print(f"The file {file_path.name} Location: {file_path}")
            print(f"Compiled {file_path.name} successfully.")
            
        except subprocess.CalledProcessError as e:
            print(f"Error compiling {file_path.name}: {e}")
        except Exception as e:
            print(f"An unexpected error occurred: {e}")
    
    return compilation_success

def Compile_file_cargo(files: list[str]) -> bool:
    compilation_success = False
    
    for file in files:
        try:
            file_path = Path(file)
            
            if file_path.is_dir() or file_path.suffix != '.rs':
                continue
            
            compilation_success = True
            print(f"Compiling {file_path.name}...")
            subprocess.run(["rustc", str(file_path), "--out-dir", str(Current_path)], check=True)
            file_to_delete = file_path.with_suffix('.pdb')
            subprocess.run(f'del "{file_to_delete}"', shell=True)
            print(f"The file {file_path.name} Location: {file_path}")
            print(f"Compiled {file_path.name} successfully.")
            
        except subprocess.CalledProcessError as e:
            print(f"Error compiling {file_path.name}: {e}")
        except Exception as e:
            print(f"An unexpected error occurred: {e}")
    
    return compilation_success

def Compile_file(files: list[str]) -> None:
    C_files: list[str] = [f for f in files if f.endswith('.c')]
    Cpp_files: list[str] = [f for f in files if f.endswith('.cpp')]
    Rust_files: list[str] = [f for f in files if f.endswith('.rs')]
    
    C_files_compiled = Compile_file_gcc(C_files)
    Cpp_files_compiled = Compile_file_gplasplas(Cpp_files)
    Rust_files_compiled = Compile_file_cargo(Rust_files)
    
    if C_files_compiled:
        print(f"Compiled {len(C_files)} C files.")
    if Cpp_files_compiled:
        print(f"Compiled {len(Cpp_files)} C++ files.")
    if Rust_files_compiled:
        print(f"Compiled {len(Rust_files)} Rust files.")
    else:
        print(f"The Compiling was not successful.\nC File : {C_files_compiled}\nC++ File: {Cpp_files_compiled}\nRust File: {Rust_files_compiled}")
    
def Move_file_to_dir(files: list[str], target_dir: str) -> bool:
    move_success = False
    target_path = Current_path / target_dir
    
    for file in files:
        try:
            file_path = Path(file)
            
            if file_path.is_dir() or file_path.suffix != '.exe':
                continue
            
            target_file = target_path / file_path.name
            print(f"Moving {file_path.name}...")
            
            try:
                import shutil
                shutil.copy2(file_path, target_file)
            except FileNotFoundError:
                print(f"Target directory {target_dir} does not exist.")
                continue
            except shutil.Error as e:
                print(f"Error moving {file_path.name}: {e}")
                continue
            finally:
                os.remove(file_path) 
            
            if target_file.exists():
                print(f"Successfully moved {file_path.name} to {target_dir}")
                move_success = True
            else:
                print(f"Failed to move {file_path.name}")
            
        except Exception as e:
            print(f"Error moving {file_path.name}: {e}")
    
    return move_success

  
def main() -> None:
    files = Get_file_name("./")
    new_move_dir = "../Executables"
    
    Compile_file(files)
    new_files = Get_file_name("./")
    move_success = Move_file_to_dir(new_files, new_move_dir)
    if move_success:
        print(f"Moved {len(new_files)} files to {new_move_dir}.")
    else:
        print(f"Failed to move files to {new_move_dir}.")
        
        
if __name__ == "__main__":
    main()