from pathlib import Path
import subprocess
import os
import shutil
from colorama import Fore, Style, init

# Initialize colorama
init(autoreset=True)

# Configuration
CURRENT_PATH = Path(__file__).parent.resolve()
ERROR_LIST: list[str] = []

# Standard file types and their compilation commands
FILE_TYPES: list[ str, list[str | object] ] = [
    ('.c', ["gcc", "-s", "{file_path}", "-o", "{output_file}"]),
    ('.cpp', ["g++", "-s", "{file_path}", "-o", "{output_file}"]),
    ('.rs', ["rustc", "{file_path}", "--out-dir", "{current_path}"]),
]

# Custom compile commands for specific files
CUSTOM_COMMANDS: list[ str, str, list[str | object] ] = [
    # Format: (filename, extension, [command])
    ( "CTF_Level10.cpp", ".cpp", [ "g++", "-std=c++11", "-mwindows", "-g", "-o", "{output_file}", "{file_path}", "-IC:/vcpkg/installed/x64-mingw-static/include", "-LC:/vcpkg/installed/x64-mingw-static/lib", "-lglew32", "-lglfw3", "-lopengl32", "-lgdi32" ] )
]

def get_files(path: str = "./") -> list[str]:
    """Get all files in directory."""
    dir_path = CURRENT_PATH / path.lstrip('/')
    return [str(f) for f in dir_path.iterdir() if f.is_file()]

def get_compile_command(file_path: Path) -> list[str]:
    """Get the appropriate compile command for a file."""
    filename = file_path.name
    extension = file_path.suffix.lower()
    
    # Check for custom commands first
    for custom_name, custom_ext, custom_cmd in CUSTOM_COMMANDS:
        if filename == custom_name and extension == custom_ext:
            return custom_cmd
    
    # Fall back to standard commands
    for ext, cmd in FILE_TYPES:
        if extension == ext:
            return cmd
    
    return None

def compile_file(file_path: Path) -> bool:
    """Compile a single file."""
    command_template = get_compile_command(file_path)
    
    if not command_template:
        return False
    
    # Build command with proper substitutions
    if file_path.suffix.lower() == '.rs' and 'rustc' in command_template[0]:
        # Rust compilation
        if "{output_file}" in ' '.join(command_template):
            # Custom rust command with output file
            output_file = file_path.with_suffix('.exe')
            command = [arg.format(
                file_path=str(file_path),
                output_file=str(output_file),
                current_path=str(CURRENT_PATH)
            ) for arg in command_template]
        else:
            # Standard rust command
            command = [arg.format(
                file_path=str(file_path),
                current_path=str(CURRENT_PATH)
            ) for arg in command_template]
    else:
        # C/C++ compilation
        output_file = file_path.with_suffix('.exe')
        command = [arg.format(
            file_path=str(file_path),
            output_file=str(output_file),
            current_path=str(CURRENT_PATH)
        ) for arg in command_template]
    
    print(f"Compiling {file_path.name}...")
    print(f"Command: {' '.join(command)}")
    
    result = subprocess.run(command, capture_output=True, text=True)
    
    if result.returncode != 0:
        error_msg = f"Error compiling {file_path.name}:\n{result.stderr}"
        print(f"{Fore.RED}[FAILED] {error_msg}{Style.RESET_ALL}")
        ERROR_LIST.append(error_msg)
        return False
    
    # Cleanup for Rust files
    if file_path.suffix.lower() == '.rs':
        pdb_file = file_path.with_suffix('.pdb')
        if pdb_file.exists():
            os.remove(pdb_file)
    
    print(f"{Fore.GREEN}[OK] Compiled {file_path.name} successfully{Style.RESET_ALL}")
    return True

def compile_files(files: list[str]) -> int:
    """Compile all supported files."""
    success_count = 0
    stats = {}
    
    for file in files:
        file_path = Path(file)
        
        if file_path.is_dir():
            continue
            
        if compile_file(file_path):
            success_count += 1
            ext = file_path.suffix.lower()
            stats[ext] = stats.get(ext, 0) + 1
    
    print(f"\n{'='*50}")
    print("COMPILATION RESULTS:")
    for ext, count in stats.items():
        print(f"  {ext}: {count} files")
    print(f"Total: {success_count} files compiled successfully")
    print(f"{'='*50}")
    
    return success_count

def move_executables(target_dir: str = "../../Executables") -> tuple[bool, int]:
    """Move all executable files to target directory. NEVER delete source files."""
    target_path = CURRENT_PATH / target_dir
    target_path.mkdir(parents=True, exist_ok=True)
    
    # Look for exe files in the parent directory (src folder where files were compiled)
    exe_files = [f for f in get_files("../") if f.endswith('.exe')]
    moved_count = 0
    
    for exe_file in exe_files:
        try:
            file_path = Path(exe_file)
            target_file = target_path / file_path.name
            
            # Copy executable to target directory
            shutil.copy2(file_path, target_file)
            
            # Only remove the .exe file from current directory (NEVER source files)
            # Double check it's actually an executable before removing
            if file_path.suffix.lower() == '.exe' and file_path.exists():
                os.remove(file_path)
                print(f"{Fore.GREEN}[OK] Moved {file_path.name} to {target_dir}{Style.RESET_ALL}")
                moved_count += 1
            else:
                print(f"{Fore.RED}[X] Skipped moving {file_path.name} - not an executable{Style.RESET_ALL}")
            
        except Exception as e:
            error_msg = f"Error moving {exe_file}: {e}"
            print(f"{Fore.RED}[X] {error_msg}{Style.RESET_ALL}")
            ERROR_LIST.append(error_msg)
    
    return moved_count > 0, moved_count

def print_summary(compiled: int, moved: int, move_success: bool):
    """Print final summary with colors."""
    print(f"\n{Fore.CYAN}{Style.BRIGHT}{'='*20} SUMMARY {'='*20}{Style.RESET_ALL}")
    print(f"{Style.BRIGHT}Compiled:{Style.RESET_ALL} {Fore.GREEN}{compiled} files{Style.RESET_ALL}")
    
    if ERROR_LIST:
        print(f"{Style.BRIGHT}Errors:{Style.RESET_ALL} {Fore.RED}{len(ERROR_LIST)} files failed{Style.RESET_ALL}")
        for i, error in enumerate(ERROR_LIST, 1):
            print(f"{Fore.RED}  {i}. {error}{Style.RESET_ALL}")
    
    print(f"{Style.BRIGHT}Moved:{Style.RESET_ALL} {Fore.GREEN if move_success else Fore.RED}{moved} executables{Style.RESET_ALL}")
    print(f"{Fore.CYAN}{Style.BRIGHT}{'='*48}{Style.RESET_ALL}")

def main():
    """Main execution function."""
    ERROR_LIST.clear()
    
    files = get_files("../")
    compiled_count = compile_files(files)
    
    move_success, moved_count = move_executables()
    
    print_summary(compiled_count, moved_count, move_success)

if __name__ == "__main__":
    main()