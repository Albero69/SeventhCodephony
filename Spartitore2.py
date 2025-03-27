import serial
import tkinter as tk
from threading import Thread
import time
from tkinter import ttk

ser = None

note_positions = {
    'LA': 0, 'SI': 1, 'DO': 2, 'DO#': 3, 'RE': 4, 'RE#': 5, 'MI': 6, 'FA': 7, 'FA#': 8, 'SOL': 9, 'SOL#': 10, 'pausa': 11
}

note_heights = [145, 140, 130, 130, 120, 120, 110, 102, 102, 95, 95, 95]

def highlight_note_safe(note, duration):
    pos = note_positions.get(note)
    if pos is not None:
        if note == 'pausa':
            canvas.itemconfig(pause_symbol, fill='deepskyblue')
        else:
            canvas.itemconfig(notes_oval[pos], fill='deepskyblue')
            if '#' in note:
                canvas.itemconfig(sharp_symbols[pos], fill='deepskyblue')
        root.update()
        if note == 'pausa':
            root.after(duration, lambda: canvas.itemconfig(pause_symbol, fill='black'))
        else:
            root.after(duration, lambda: canvas.itemconfig(notes_oval[pos], fill='black'))
            if '#' in note:
                root.after(duration, lambda: canvas.itemconfig(sharp_symbols[pos], fill='black'))

def read_serial():
    while ser and ser.is_open:
        try:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8').strip()
                if line:
                    try:
                        note, duration = line.split(',')
                        duration = int(duration)
                        print(f"Ricevuto: {note}, {duration} ms")
                        root.after(0, highlight_note_safe, note, duration)
                    except ValueError:
                        print(f"Dato non valido: {line}")
        except serial.SerialException as e:
            print(f"Errore nella lettura seriale: {e}")
            break

def start_serial():
    global ser
    if ser and ser.is_open:
        ser.close()
    selected_com = com_var.get()
    try:
        ser = serial.Serial(selected_com, 9600, timeout=1)
        time.sleep(2)
        print(f"Connesso a {selected_com}")
        serial_thread = Thread(target=read_serial)
        serial_thread.daemon = True
        serial_thread.start()
        status_label.config(text=f"Connesso a {selected_com}")
    except serial.SerialException as e:
        print(f"Errore nella connessione seriale: {e}")
        status_label.config(text=f"Errore: {e}")
        ser = None

root = tk.Tk()
root.title("Spartito Musicale")

top_frame = tk.Frame(root)
top_frame.pack(fill='x', padx=5, pady=5)

com_label = tk.Label(top_frame, text="Seleziona porta COM:")
com_label.pack(side='left', padx=5)

com_var = tk.StringVar()
com_dropdown = ttk.Combobox(top_frame, textvariable=com_var)
com_dropdown['values'] = [f'COM{i}' for i in range(1, 13)]
com_dropdown.set('COM4')
com_dropdown.pack(side='left', padx=5)

run_button = tk.Button(top_frame, text="RUN", command=start_serial)
run_button.pack(side='left', padx=5)

status_label = tk.Label(top_frame, text="Non connesso")
status_label.pack(side='left', padx=5)

canvas = tk.Canvas(root, width=650, height=200)
canvas.pack()

for i in range(5):
    y = 50 + i * 15  
    canvas.create_line(0, y, 650, y, width=2)

notes_oval = []
sharp_symbols = []
notes = ['LA', 'SI', 'DO', 'DO#', 'RE', 'RE#', 'MI', 'FA', 'FA#', 'SOL', 'SOL#', 'pausa']
for i, note in enumerate(notes):
    x = 50 + i * 50
    y = note_heights[i]
    
    if note == 'pausa':
        pause_symbol = canvas.create_rectangle(x - 6, y - 10, x + 6, y, fill='black', outline='black')
        canvas.create_text(x, y + 20, text='PAUSA', font=('Arial', 10))
    else:
        oval = canvas.create_oval(x - 8, y - 8, x + 8, y + 8, fill='black', outline='black')
        notes_oval.append(oval)
        canvas.create_line(x + 8, y, x + 8, y - 30, width=2)
        if '#' in note:
            sharp = canvas.create_text(x - 15, y, text='#', font=('Arial', 12), fill='black')
            sharp_symbols.append(sharp)
        else:
            sharp_symbols.append(None)
        canvas.create_text(x, y + 20, text=note, font=('Arial', 10))

def on_closing():
    global ser
    if ser and ser.is_open:
        ser.close()
    root.destroy()

root.protocol("WM_DELETE_WINDOW", on_closing)

root.mainloop()
