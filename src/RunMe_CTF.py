import tkinter as tk
from tkinter import ttk
from pathlib import Path
import json
import sys
import hashlib

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

def Get_Flags_list(path_to_json: Path | str) -> tuple[dict[str, str], dict[str, int]]:
    flags = {}
    points = {}
    path = Get_Corect_Path(path_to_json)

    if not path.exists():
        print("Path does not exist")
        path = resource_path("./flags.json")
        print("Using default path:", path)
    if not path.exists():
        print("Default path does not exist")
        print("Using empty flags list")
        return flags, points

    with open(path, "r") as file:
        data = json.load(file)
        for level_name, level_data in data.get("flags", {}).items():
            flag = level_data.get("flag")
            point = level_data.get("points", 0)
            if flag:
                flags[level_name] = flag
                points[level_name] = point

    return flags, points


flags, flag_points = Get_Flags_list("../src/flags.json")

print(flags)

earned_points = 0

def update_points_display():
    points_label.config(text=f"Points: {earned_points}")

def check_input(flag_name):
    global earned_points
    user_input = entry_dict[flag_name].get()
    if hashlib.sha512(user_input.encode()).hexdigest() == flags[flag_name]:
        entry_dict[flag_name].config(bg="green", state="disabled", font=("Arial", 14))
        check_buttons[flag_name].config(bg="green", state="disabled")
        result_label.config(text=f"Correct! You entered {user_input} for {flag_name}.")
        earned_points += flag_points[flag_name]
        update_points_display()
    else:
        result_label.config(text=f"Incorrect! Try again for {flag_name}.")

# Create the main window
root = tk.Tk()
root.title("Flag Checker")
root.configure(bg="#393E46")
root.geometry("650x600")
root.resizable(False, False)


points_label = tk.Label(root, text="Points: 0", font=("Arial", 14, "bold"),
                        bg="#393E46", fg="white")
points_label.place(x=10, y=10)

# Main outer frame (below points label)
outer_frame = tk.Frame(root, bg="#393E46")
outer_frame.pack(fill="both", expand=True, padx=10, pady=(40,10))  # Push down for points label

# Scrollable canvas
canvas = tk.Canvas(outer_frame, bg="#393E46", highlightthickness=0)
canvas.pack(side="left", fill="both", expand=True)

scrollbar = ttk.Scrollbar(outer_frame, orient="vertical", command=canvas.yview)
scrollbar.pack(side="right", fill="y")

canvas.configure(yscrollcommand=scrollbar.set)
canvas.bind('<Configure>', lambda e: canvas.configure(scrollregion=canvas.bbox("all")))

main_frame = tk.Frame(canvas, bg="#393E46")
canvas.create_window((0, 0), window=main_frame, anchor="nw")

entry_dict = {}
check_buttons = {}

# Input sections for each flag
for flag_name in flags:
    row_frame = tk.Frame(main_frame, bg="#393E46")
    row_frame.pack(fill="x", pady=15, padx=10)

    section_label = tk.Label(row_frame, text=f"{flag_name}:", 
                             bg="#393E46", fg="white", font=("Arial", 14, "bold"))
    section_label.pack(side="left", padx=(0, 20))

    entry = tk.Entry(row_frame, font=("Arial", 14), width=35) 
    entry.pack(side="left", padx=(0, 20))
    entry_dict[flag_name] = entry

    check_button = tk.Button(row_frame, text="Check", command=lambda name=flag_name: check_input(name),
                             font=("Arial", 12), padx=10, pady=5)
    check_button.pack(side="left")
    check_buttons[flag_name] = check_button

# Extra spacing for scroll
for i in range(10):
    spacer = tk.Frame(main_frame, bg="#393E46", height=20)
    spacer.pack(fill="x")

# Result label
result_frame = tk.Frame(root, bg="#393E46")
result_frame.pack(fill="x", pady=10)

result_label = tk.Label(result_frame, text="Enter flags and click 'Check' to verify", 
                       bg="#393E46", fg="white", font=("Arial", 14),
                       wraplength=500)
result_label.pack(pady=10)

# Scroll binding
def _on_mousewheel(event):
    canvas.yview_scroll(int(-1*(event.delta/120)), "units")

canvas.bind_all("<MouseWheel>", _on_mousewheel)
canvas.bind_all("<Button-4>", lambda e: canvas.yview_scroll(-1, "units"))
canvas.bind_all("<Button-5>", lambda e: canvas.yview_scroll(1, "units"))

root.mainloop()
