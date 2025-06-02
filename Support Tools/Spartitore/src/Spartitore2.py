import serial
import tkinter as tk
from threading import Thread
import time

# Serial Port conf
try:
    ser = serial.Serial('COM4', 9600, timeout=1) 
    time.sleep(2)
except serial.SerialException as e:
    print(f"Errore nella connessione seriale: {e}")
    ser = None

# MAP
note_positions = {
    'LA': 0, 'SI': 1, 'DO': 2, 'DO#': 3, 'RE': 4, 'RE#': 5, 'MI': 6, 'FA': 7, 'FA#': 8, 'SOL': 9, 'SOL#': 10
}

# Altezze note px
note_heights = [145, 140, 130, 130, 120, 120, 110, 102, 102, 95, 95]

# Funzione per accendere e spegnere una nota
def highlight_note_safe(note, duration):
    pos = note_positions.get(note)
    if pos is not None:
        # Accendi 
        canvas.itemconfig(notes_oval[pos], fill='deepskyblue')
        if '#' in note:
            canvas.itemconfig(sharp_symbols[pos], fill='deepskyblue')
        root.update()
        # Spegni 
        root.after(duration, lambda: canvas.itemconfig(notes_oval[pos], fill='black'))
        if '#' in note:
            root.after(duration, lambda: canvas.itemconfig(sharp_symbols[pos], fill='black'))

# Serial Read
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
                        # Update
                        root.after(0, highlight_note_safe, note, duration)
                    except ValueError:
                        print(f"Dato non valido: {line}")
        except serial.SerialException as e:
            print(f"Errore nella lettura seriale: {e}")
            break

# Configurazione della GUI
root = tk.Tk()
root.title("Spartito Musicale")

# Canvas
canvas = tk.Canvas(root, width=600, height=200)
canvas.pack()

# Disegna le linee del pentagramma
for i in range(5):
    y = 50 + i * 15  
    canvas.create_line(0, y, 600, y, width=2)  # Linee estese e più spesse

# Disegna le note
notes_oval = []
sharp_symbols = []
notes = ['LA', 'SI', 'DO', 'DO#', 'RE', 'RE#', 'MI', 'FA', 'FA#', 'SOL', 'SOL#'] #ordine
for i, note in enumerate(notes):
    x = 50 + i * 50  # x
    y = note_heights[i]  # y
    
    oval = canvas.create_oval(x - 8, y - 8, x + 8, y + 8, fill='black', outline='black')
    notes_oval.append(oval)
    # gambo
    canvas.create_line(x + 8, y, x + 8, y - 30, width=2)
    # #
    if '#' in note:
        sharp = canvas.create_text(x - 15, y, text='#', font=('Arial', 12), fill='black')
        sharp_symbols.append(sharp)
    else:
        sharp_symbols.append(None) 
    # Aggiungi etichetta sotto la nota
    canvas.create_text(x, y + 20, text=note, font=('Arial', 10))

# Avvia il thread per leggere dalla seriale
if ser:
    serial_thread = Thread(target=read_serial)
    serial_thread.daemon = True  # Il thread si chiude quando la GUI viene chiusa
    serial_thread.start()

# Chiudi la porta seriale al termine
def on_closing():
    if ser and ser.is_open:
        ser.close()
    root.destroy()

root.protocol("WM_DELETE_WINDOW", on_closing)

# Avvia la GUI
root.mainloop()