import tkinter as tk
from tkinter import ttk
from pathlib import Path
import json
import sys

def resource_path(relative_path: str) -> Path:
    """ Get absolute path to resource, works for dev and for PyInstaller """
    try:
        # PyInstaller sets this when bundled
        base_path = Path(sys._MEIPASS)
    except AttributeError:
        # Running in a normal Python environment
        base_path = Path(__file__).parent.resolve()

    return (base_path / relative_path).resolve()

def Get_Corect_Path(path_to_json: Path | str) -> Path:
    try:
        path = Path(path_to_json).resolve()
        print(f"Resolved path: {path}")
    except Path.exists(path_to_json):
        path = resource_path(path_to_json)
        print(f"Resolved path: {path}")
        
    return path

def Get_Flags_list(path_to_json: Path | str) -> dict[str, str]:
    flags = {}
    path = Get_Corect_Path(path_to_json)

    if not path.exists():
        print("Path does not exist")
        return flags

    with open(path, "r") as file:
        data = json.load(file)
        for level_name, level_data in data.get("flags", {}).items():
            flag = level_data.get("flag")
            if flag:
                flags[level_name] = flag

    return flags


# Load flags from JSON file
flags = Get_Flags_list("../src/flags.json")
print(flags)

# Function to check if the input matches the flag
def check_input(flag_name):
    user_input = entry_dict[flag_name].get()
    if user_input == flags[flag_name]:
        entry_dict[flag_name].config(bg="green", state="disabled", font=("Arial", 14))
        check_buttons[flag_name].config(bg="green", state="disabled")  # Disable button after correct input
        result_label.config(text=f"Correct! You entered {user_input} for {flag_name}.")
    else:
        result_label.config(text=f"Incorrect! Try again for {flag_name}.")

# Create the main window
root = tk.Tk()
root.title("Flag Checker")
root.configure(bg="#393E46")
root.geometry("600x500")  # Set a larger window size

# Create a main frame that will contain the canvas
outer_frame = tk.Frame(root, bg="#393E46")
outer_frame.pack(fill="both", expand=True, padx=10, pady=10)

# Create a canvas that will be scrollable
canvas = tk.Canvas(outer_frame, bg="#393E46", highlightthickness=0)
canvas.pack(side="left", fill="both", expand=True)

# Add a scrollbar to the canvas
scrollbar = ttk.Scrollbar(outer_frame, orient="vertical", command=canvas.yview)
scrollbar.pack(side="right", fill="y")

# Configure the canvas to use the scrollbar
canvas.configure(yscrollcommand=scrollbar.set)
canvas.bind('<Configure>', lambda e: canvas.configure(scrollregion=canvas.bbox("all")))

# Create a frame inside the canvas to hold all our content
main_frame = tk.Frame(canvas, bg="#393E46")
canvas.create_window((0, 0), window=main_frame, anchor="nw")

# Dictionaries to hold the widgets
entry_dict = {}
check_buttons = {}

# Create input boxes and checkboxes in rows
for flag_name in flags:
    row_frame = tk.Frame(main_frame, bg="#393E46")
    row_frame.pack(fill="x", pady=15, padx=20)

    section_label = tk.Label(row_frame, text=f"{flag_name}:", 
                             bg="#393E46", fg="white", font=("Arial", 14, "bold"))
    section_label.pack(side="left", padx=(0, 20))

    entry = tk.Entry(row_frame, font=("Arial", 14), width=20)
    entry.pack(side="left", padx=(0, 20))
    entry_dict[flag_name] = entry

    check_button = tk.Button(row_frame, text="Check", command=lambda name=flag_name: check_input(name),
                             font=("Arial", 12), padx=10, pady=5)
    check_button.pack(side="left")
    check_buttons[flag_name] = check_button

# Add some extra rows to demonstrate scrolling
for i in range(10):
    spacer = tk.Frame(main_frame, bg="#393E46", height=20)
    spacer.pack(fill="x")

# Label to display result (placed at the bottom of the main window)
result_frame = tk.Frame(root, bg="#393E46")
result_frame.pack(fill="x", pady=10)

result_label = tk.Label(result_frame, text="Enter flags and click 'Check' to verify", 
                       bg="#393E46", fg="white", font=("Arial", 14),
                       wraplength=500)  # Wider wraplength and bigger font
result_label.pack(pady=10)

# Add mousewheel scrolling
def _on_mousewheel(event):
    canvas.yview_scroll(int(-1*(event.delta/120)), "units")

canvas.bind_all("<MouseWheel>", _on_mousewheel)  # For Windows and MacOS
canvas.bind_all("<Button-4>", lambda e: canvas.yview_scroll(-1, "units"))  # For Linux
canvas.bind_all("<Button-5>", lambda e: canvas.yview_scroll(1, "units"))   # For Linux

# Start the Tkinter event loop
root.mainloop()
