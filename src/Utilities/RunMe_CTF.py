# NEW CODE - Fixed for PyInstaller compatibility
import tkinter as tk
from tkinter import ttk
from pathlib import Path
import json, sys, hashlib

# =============================================================================
# DEBUG HEADER — printed immediately on startup
# =============================================================================
print("=" * 60)
print("[DEBUG] ===  CTF FLAG CHECKER  STARTUP  ===")
print(f"[DEBUG] Python executable : {sys.executable}")
print(f"[DEBUG] sys.argv[0]       : {sys.argv[0]}")
print(f"[DEBUG] Frozen (compiled) : {getattr(sys, 'frozen', False)}")

# When compiled by PyInstaller, extracted files live in sys._MEIPASS
# When running as plain .py, they live next to the script
if getattr(sys, "frozen", False):
    # Running as a compiled .exe
    _BASE_DIR = Path(sys._MEIPASS)
    _EXE_DIR  = Path(sys.executable).parent   # folder that contains the .exe
    print(f"[DEBUG] sys._MEIPASS      : {_BASE_DIR}")
    print(f"[DEBUG] .exe directory    : {_EXE_DIR}")
else:
    # Running as a plain .py script
    _BASE_DIR = Path(__file__).parent.resolve()
    _EXE_DIR  = _BASE_DIR
    print(f"[DEBUG] Script directory  : {_BASE_DIR}")

print("=" * 60)


# =============================================================================
# PATH HELPERS
# =============================================================================

def resource_path(relative_path: str) -> Path:
    """
    Resolve a path that was bundled INTO the PyInstaller package
    (i.e. added with --add-data).  These files are extracted to sys._MEIPASS
    at runtime.
    """
    return (_BASE_DIR / relative_path).resolve()


def external_path(relative_path: str) -> Path:
    """
    Resolve a path that lives NEXT TO the .exe (or .py) on disk —
    i.e. NOT bundled inside the package.
    flags.json should live here so users can edit it without recompiling.
    """
    return (_EXE_DIR / relative_path).resolve()


# =============================================================================
# CONFIGURATION
# =============================================================================

# flags.json is expected to sit in the SAME FOLDER as the .exe / .py
FLAGS_JSON_FILENAME = "flags.json"
ICON_FILENAME       = "ctf_icon_bmp.ico"

# Full resolved paths — printed at startup for easy debugging
FLAGS_JSON_PATH = external_path(FLAGS_JSON_FILENAME)
ICON_PATH       = resource_path(ICON_FILENAME)   # bundled via --add-data

print(f"[DEBUG] FLAGS_JSON_PATH   : {FLAGS_JSON_PATH}")
print(f"[DEBUG]   -> exists?      : {FLAGS_JSON_PATH.exists()}")
print(f"[DEBUG] ICON_PATH         : {ICON_PATH}")
print(f"[DEBUG]   -> exists?      : {ICON_PATH.exists()}")
print("=" * 60)

WINDOW_WIDTH     = 800
WINDOW_HEIGHT    = 700
WINDOW_RESIZABLE = False

COLOR_BACKGROUND = "#272b30"
COLOR_FOREGROUND = "white"
COLOR_CORRECT    = "#6bbd45"
COLOR_GROUP_HDR  = "#4a90d9"

FONT_LABEL  = ("Arial", 14, "bold")
FONT_ENTRY  = ("Arial", 14)
FONT_BUTTON = ("Arial", 12)
FONT_GROUP  = ("Arial", 13, "bold")

PADDING            = 10
ROW_PAD            = 8
LABEL_COLUMN_WIDTH = 185
ENTRY_WIDTH_PX     = 400
BUTTON_WIDTH_PX    = 80
ROW_HEIGHT         = 30

GROUPS = ["Easy", "Medium", "Hard"]


# =============================================================================
# UTILITIES & DATA LOADING
# =============================================================================

def hash_text(text: str) -> str:
    return hashlib.sha512(text.encode()).hexdigest()


def load_flags(json_path: Path) -> tuple[dict, dict, dict]:
    """
    Load flags from JSON.  Expected structure:
        {
            "flags": {
                "level_name": {
                    "flag":   "<sha512 hash>",
                    "points": 100,
                    "group":  "Easy"
                }
            }
        }
    Returns (flags, points, flag_groups).

    Search order:
        1. Bundled inside the .exe via --add-data  (sys._MEIPASS)
        2. Next to the .exe on disk                (sys.executable parent)
        3. Dev layout: two levels up from the .py  (for running from source)
    """
    print(f"[DEBUG] load_flags() called with: {json_path}")

    # --- 1. Bundled inside PyInstaller package (sys._MEIPASS) ---
    bundled = resource_path(FLAGS_JSON_FILENAME)
    print(f"[DEBUG] [1] Bundled path  : {bundled}  exists={bundled.exists()}")

    # --- 2. Next to the .exe on disk ---
    beside_exe = external_path(FLAGS_JSON_FILENAME)
    print(f"[DEBUG] [2] Beside .exe   : {beside_exe}  exists={beside_exe.exists()}")

    # --- 3. Dev layout: two levels up (running as .py from src/Utilities/) ---
    dev_path = (_EXE_DIR / ".." / ".." / FLAGS_JSON_FILENAME).resolve()
    print(f"[DEBUG] [3] Dev path      : {dev_path}  exists={dev_path.exists()}")

    if bundled.exists():
        chosen_path = bundled
        print(f"[DEBUG] Using bundled flags.json: {chosen_path}")
    elif beside_exe.exists():
        chosen_path = beside_exe
        print(f"[DEBUG] Using beside-.exe flags.json: {chosen_path}")
    elif dev_path.exists():
        chosen_path = dev_path
        print(f"[DEBUG] Using dev-layout flags.json: {chosen_path}")
    else:
        print("[ERROR] Could not find flags.json in ANY of the 3 locations above!")
        print("[ERROR] To fix: add  --add-data \"<full path to flags.json>;.\"  to your PyInstaller command.")
        return {}, {}, {}

    print(f"[DEBUG] Loading flags from: {chosen_path}")

    try:
        with open(chosen_path, "r", encoding="utf-8") as f:
            data = json.load(f)
    except Exception as e:
        print(f"[ERROR] Failed to read/parse flags.json: {e}")
        return {}, {}, {}

    flags, points, flag_groups = {}, {}, {}
    for name, info in data.get("flags", {}).items():
        if info.get("flag"):
            flags[name]       = info["flag"]
            points[name]      = info.get("points", 0)
            flag_groups[name] = info.get("group", "")
            print(f"[DEBUG]   Loaded flag: '{name}'  group='{flag_groups[name]}'  points={points[name]}")

    print(f"[DEBUG] Total flags loaded: {len(flags)}")
    print("=" * 60)
    return flags, points, flag_groups


def truncate_to_px(text: str, max_px: int, font: tuple) -> str:
    """Trim text with '…' until it fits within max_px."""
    probe   = tk.Label(font=font)
    measure = lambda t: probe.tk.call("font", "measure", probe["font"], t)
    if measure(text) <= max_px:
        probe.destroy()
        return text
    while len(text) > 1 and measure(text[:-1] + "…") > max_px:
        text = text[:-1]
    probe.destroy()
    return text[:-1] + "…"


# =============================================================================
# RENDER
# =============================================================================

class Render:
    def __init__(self, flags: dict, flag_points: dict, flag_groups: dict):
        self.flags       = flags
        self.flag_points = flag_points
        self.groups      = self._build_groups(flag_groups)

        self.earned_points = 0
        self.entry_dict    = {}
        self.check_buttons = {}
        self.label_dict    = {}
        self.group_state   = {}

        self.root         = None
        self.points_label = None
        self.result_label = None
        self._canvas      = None

    # ------------------------------------------------------------------
    def _build_groups(self, flag_groups: dict) -> dict[str, list[str]]:
        buckets: dict[str, list[str]] = {g: [] for g in GROUPS}
        other: list[str] = []
        for flag_name, group in flag_groups.items():
            if group in buckets:
                buckets[group].append(flag_name)
            else:
                other.append(flag_name)
        result = {g: v for g, v in buckets.items() if v}
        if other:
            result["Other"] = other
        return result

    # ------------------------------------------------------------------
    def run(self):
        self._build_window()
        self._canvas, main_frame = self._build_scroll_area()
        self._build_all_groups(main_frame)
        for _ in range(10):
            tk.Frame(main_frame, bg=COLOR_BACKGROUND, height=20).pack(fill="x")
        self._build_chrome()
        self._bind_scroll(self._canvas)
        self.root.after(100, self._refresh_scroll)
        self.root.mainloop()

    # ------------------------------------------------------------------
    def _build_window(self):
        self.root = tk.Tk()
        self.root.title("Flag Checker")
        self.root.configure(bg=COLOR_BACKGROUND)
        self.root.geometry(f"{WINDOW_WIDTH}x{WINDOW_HEIGHT}")
        self.root.resizable(WINDOW_RESIZABLE, WINDOW_RESIZABLE)

        print(f"[DEBUG] Attempting to load icon from: {ICON_PATH}")
        if ICON_PATH.exists():
            try:
                self.root.iconbitmap(str(ICON_PATH))
                print("[DEBUG] Icon loaded successfully.")
            except Exception as e:
                print(f"[WARN]  Icon load failed: {e}")
        else:
            print(f"[WARN]  Icon file not found at: {ICON_PATH}")

        self.points_label = tk.Label(self.root, text="Points: 0",
                                     font=FONT_ENTRY, bg=COLOR_BACKGROUND, fg=COLOR_CORRECT)
        self.points_label.place(x=10, y=10)

    def _build_chrome(self):
        frame = tk.Frame(self.root, bg=COLOR_BACKGROUND)
        frame.pack(fill="x", pady=PADDING)
        self.result_label = tk.Label(
            frame, text="Enter flags and click 'Check' to verify",
            bg=COLOR_BACKGROUND, fg=COLOR_FOREGROUND,
            font=FONT_ENTRY, wraplength=500)
        self.result_label.pack(pady=PADDING)

    # ------------------------------------------------------------------
    def _build_scroll_area(self) -> tuple[tk.Canvas, tk.Frame]:
        outer = tk.Frame(self.root, bg=COLOR_BACKGROUND)
        outer.pack(fill="both", expand=True, padx=PADDING, pady=(40, PADDING))

        canvas = tk.Canvas(outer, bg=COLOR_BACKGROUND, highlightthickness=0)
        canvas.pack(side="left", fill="both", expand=True)

        sb = ttk.Scrollbar(outer, orient="vertical", command=canvas.yview)
        sb.pack(side="right", fill="y")
        canvas.configure(yscrollcommand=sb.set)
        canvas.bind("<Configure>",
                    lambda e: canvas.configure(scrollregion=canvas.bbox("all")))

        main_frame = tk.Frame(canvas, bg=COLOR_BACKGROUND)
        canvas.create_window((0, 0), window=main_frame, anchor="nw")
        return canvas, main_frame

    def _bind_scroll(self, canvas):
        canvas.bind_all("<MouseWheel>",
                        lambda e: canvas.yview_scroll(int(-1 * (e.delta / 120)), "units"))
        canvas.bind_all("<Button-4>", lambda e: canvas.yview_scroll(-1, "units"))
        canvas.bind_all("<Button-5>", lambda e: canvas.yview_scroll(1, "units"))

    def _refresh_scroll(self):
        self.root.update_idletasks()
        self._canvas.configure(scrollregion=self._canvas.bbox("all"))
        self._canvas.yview_moveto(0.0)

    # ------------------------------------------------------------------
    def _build_all_groups(self, parent):
        if not self.groups:
            print("[WARN]  No groups to display — flags dict is empty!")
            tk.Label(parent, text="No flags loaded. Check terminal for errors.",
                     bg=COLOR_BACKGROUND, fg="red", font=FONT_LABEL).pack(pady=20)
            return
        for group_name, flag_names in self.groups.items():
            self._build_group(parent, group_name, flag_names)

    def _build_group(self, parent, group_name: str, flag_names: list[str]):
        group_container = tk.Frame(parent, bg=COLOR_BACKGROUND)
        group_container.pack(fill="x", pady=(ROW_PAD, 0))

        header = tk.Frame(group_container, bg=COLOR_BACKGROUND)
        header.pack(fill="x", padx=PADDING)

        toggle_btn = tk.Button(
            header, text="[-]", font=FONT_GROUP,
            bg=COLOR_BACKGROUND, fg=COLOR_GROUP_HDR,
            activebackground=COLOR_BACKGROUND, activeforeground=COLOR_GROUP_HDR,
            relief="flat", bd=0, cursor="hand2",
            command=lambda g=group_name: self._toggle_group(g),
        )
        toggle_btn.pack(side="left")

        tk.Label(header, text=f"  {group_name}",
                 bg=COLOR_BACKGROUND, fg=COLOR_GROUP_HDR, font=FONT_GROUP).pack(side="left")

        rows_frame = tk.Frame(group_container, bg=COLOR_BACKGROUND)
        rows_frame.pack(fill="x", padx=(PADDING * 3, 0))

        for flag_name in flag_names:
            self._build_row(rows_frame, flag_name)

        self.group_state[group_name] = {
            "open": True, "rows_frame": rows_frame, "toggle_btn": toggle_btn}

    def _toggle_group(self, group_name: str):
        state = self.group_state[group_name]
        if state["open"]:
            state["rows_frame"].pack_forget()
            state["toggle_btn"].config(text="[+]")
            state["open"] = False
        else:
            state["rows_frame"].pack(fill="x", padx=(PADDING * 3, 0))
            state["toggle_btn"].config(text="[-]")
            state["open"] = True
        self._refresh_scroll()

    # ------------------------------------------------------------------
    def _fixed_frame(self, parent, width):
        f = tk.Frame(parent, bg=COLOR_BACKGROUND, width=width, height=ROW_HEIGHT)
        f.pack_propagate(False)
        return f

    def _build_row(self, parent, flag_name: str):
        row = tk.Frame(parent, bg=COLOR_BACKGROUND)
        row.pack(fill="x", pady=ROW_PAD)

        label_text = truncate_to_px(
            f"{flag_name} ({self.flag_points.get(flag_name, 0)})",
            LABEL_COLUMN_WIDTH, FONT_LABEL)
        lf = self._fixed_frame(row, LABEL_COLUMN_WIDTH)
        lf.pack(side="left", padx=(0, PADDING))
        label = tk.Label(lf, text=label_text, anchor="w",
                         bg=COLOR_BACKGROUND, fg=COLOR_FOREGROUND, font=FONT_LABEL)
        label.pack(fill="both", expand=True)
        self.label_dict[flag_name] = label

        ef = self._fixed_frame(row, ENTRY_WIDTH_PX)
        ef.pack(side="left", padx=(0, PADDING))
        entry = tk.Entry(ef, font=FONT_ENTRY)
        entry.pack(fill="both", expand=True)
        self.entry_dict[flag_name] = entry

        bf = self._fixed_frame(row, BUTTON_WIDTH_PX)
        bf.pack(side="left")
        btn = tk.Button(bf, text="Check", font=FONT_BUTTON,
                        command=lambda n=flag_name: self._check_flag(n))
        btn.pack(fill="both", expand=True)
        self.check_buttons[flag_name] = btn

    # ------------------------------------------------------------------
    def _check_flag(self, flag_name: str):
        user_input = self.entry_dict[flag_name].get()
        print(f"[DEBUG] Checking flag '{flag_name}' — input length: {len(user_input)}")
        if hash_text(user_input) == self.flags[flag_name]:
            self.entry_dict[flag_name].config(bg=COLOR_CORRECT, state="disabled")
            self.check_buttons[flag_name].config(bg=COLOR_CORRECT, state="disabled")
            self.label_dict[flag_name].config(fg=COLOR_CORRECT)
            self.result_label.config(text=f"Correct! You entered '{user_input}' for {flag_name}.")
            self.earned_points += self.flag_points.get(flag_name, 0)
            self.points_label.config(text=f"Points: {self.earned_points}")
            print(f"[DEBUG] Correct! Points now: {self.earned_points}")
        else:
            self.result_label.config(text=f"Incorrect! Try again for {flag_name}.")
            print(f"[DEBUG] Incorrect answer for '{flag_name}'.")


# =============================================================================
# ENTRY POINT
# =============================================================================

if __name__ == "__main__":
    flags, flag_points, flag_groups = load_flags(FLAGS_JSON_PATH)
    Render(flags, flag_points, flag_groups).run()