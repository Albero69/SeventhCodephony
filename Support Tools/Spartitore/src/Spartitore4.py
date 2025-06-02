import serial
import tkinter as tk
from threading import Thread
import time
from tkinter import ttk
import pygame.mixer
import numpy as np

# Inizializza pygame mixer
pygame.mixer.init(frequency=44100, size=-16, channels=1)

ser1 = None
ser2 = None

note_positions = {
    'LA': 0, 'SI': 1, 'DO': 2, 'DOd': 3, 'RE': 4, 'REd': 5, 'MI': 6, 'FA': 7, 'FAd': 8, 'SOL': 9, 'SOLd': 10, 'pausa': 11
}

note_heights = [145, 140, 130, 130, 120, 120, 110, 102, 102, 95, 95, 95]


note_frequencies = {
    'LA': 110,    # A4
    'LAd': 116.5,  # A#4
    'SI': 123.5,    # B4
    'DO': 130.8,    # C4
    'DOd': 138.6,   # C#4
    'RE': 146.8,    # D4
    'REd': 155.6,   # D#4
    'MI': 82.4,    # E4
    'FA': 87.3,    # F4
    'FAd': 92.5,   # F#4
    'SOL': 98,   # G4
    'SOLd': 103.8,  # G#4
    'pausa': 0       # Nessun suono per la pausa
}

def generate_tone(frequency, duration_ms):
    if frequency == 0:  # Pausa
        return None
    sample_rate = 44100
    duration = duration_ms / 1000.0  # Converti in secondi
    t = np.linspace(0, duration, int(sample_rate * duration), False)
    tone = 0.5 * np.sin(2 * np.pi * frequency * t)  # Onda sinusoidale
    sound_array = (tone * 32767).astype(np.int16)  # Converti in formato 16-bit
    return pygame.mixer.Sound(buffer=sound_array)

def highlight_and_play(canvas, notes_oval, sharp_symbols, pause_symbol, note, duration):
    pos = note_positions.get(note)
    if pos is not None:
        # Evidenzia la nota
        if note == 'pausa':
            canvas.itemconfig(pause_symbol, fill='deepskyblue')
        else:
            canvas.itemconfig(notes_oval[pos], fill='deepskyblue')
            if '#' in note:
                canvas.itemconfig(sharp_symbols[pos], fill='deepskyblue')
        root.update()
        
        # Riproduci il suono
        freq = note_frequencies[note]
        sound = generate_tone(freq, duration)
        if sound:
            sound.play()
        
        # Spegni la nota dopo la durata
        if note == 'pausa':
            canvas.after(duration, lambda: canvas.itemconfig(pause_symbol, fill='black'))
        else:
            canvas.after(duration, lambda: canvas.itemconfig(notes_oval[pos], fill='black'))
            if '#' in note:
                canvas.after(duration, lambda: canvas.itemconfig(sharp_symbols[pos], fill='black'))

def read_serial(ser, canvas, notes_oval, sharp_symbols, pause_symbol):
    while ser and ser.is_open:
        try:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8').strip()
                if line:
                    try:
                        note, duration = line.split(',')
                        duration = int(duration)
                        print(f"Ricevuto: {note}, {duration} ms")
                        root.after(0, highlight_and_play, canvas, notes_oval, sharp_symbols, pause_symbol, note, duration)
                    except ValueError:
                        print(f"Dato non valido: {line}")
        except serial.SerialException as e:
            print(f"Errore nella lettura seriale: {e}")
            break

def start_serial1():
    global ser1
    if ser1 and ser1.is_open:
        ser1.close()
    selected_com = com_var1.get()
    try:
        ser1 = serial.Serial(selected_com, 9600, timeout=1)
        time.sleep(2)
        print(f"Connesso a {selected_com} (Spartito 1)")
        serial_thread = Thread(target=read_serial, args=(ser1, canvas1, notes_oval1, sharp_symbols1, pause_symbol1))
        serial_thread.daemon = True
        serial_thread.start()
        status_label1.config(text=f"Connesso a {selected_com}")
    except serial.SerialException as e:
        print(f"Errore nella connessione seriale: {e}")
        status_label1.config(text=f"Errore: {e}")
        ser1 = None

def start_serial2():
    global ser2
    if ser2 and ser2.is_open:
        ser2.close()
    selected_com = com_var2.get()
    try:
        ser2 = serial.Serial(selected_com, 9600, timeout=1)
        time.sleep(2)
        print(f"Connesso a {selected_com} (Spartito 2)")
        serial_thread = Thread(target=read_serial, args=(ser2, canvas2, notes_oval2, sharp_symbols2, pause_symbol2))
        serial_thread.daemon = True
        serial_thread.start()
        status_label2.config(text=f"Connesso a {selected_com}")
    except serial.SerialException as e:
        print(f"Errore nella connessione seriale: {e}")
        status_label2.config(text=f"Errore: {e}")
        ser2 = None

def start_both():
    start_serial1()
    start_serial2()

root = tk.Tk()
root.title("Spartito Musicale Doppio con Suono")

top_frame = tk.Frame(root)
top_frame.pack(fill='x', padx=5, pady=5)

com_label1 = tk.Label(top_frame, text="Spartito 1 - COM:")
com_label1.pack(side='left', padx=5)
com_var1 = tk.StringVar()
com_dropdown1 = ttk.Combobox(top_frame, textvariable=com_var1)
com_dropdown1['values'] = [f'COM{i}' for i in range(1, 13)]
com_dropdown1.set('COM4')
com_dropdown1.pack(side='left', padx=5)
run_button1 = tk.Button(top_frame, text="RUN 1", command=start_serial1)
run_button1.pack(side='left', padx=5)
status_label1 = tk.Label(top_frame, text="Non connesso")
status_label1.pack(side='left', padx=5)

tk.Label(top_frame, text="   ").pack(side='left')

com_label2 = tk.Label(top_frame, text="Spartito 2 - COM:")
com_label2.pack(side='left', padx=5)
com_var2 = tk.StringVar()
com_dropdown2 = ttk.Combobox(top_frame, textvariable=com_var2)
com_dropdown2['values'] = [f'COM{i}' for i in range(1, 13)]
com_dropdown2.set('COM5')
com_dropdown2.pack(side='left', padx=5)
run_button2 = tk.Button(top_frame, text="RUN 2", command=start_serial2)
run_button2.pack(side='left', padx=5)
status_label2 = tk.Label(top_frame, text="Non connesso")
status_label2.pack(side='left', padx=5)

run_both_button = tk.Button(top_frame, text="RUN ENTRAMBI", command=start_both)
run_both_button.pack(side='left', padx=10)

canvas1 = tk.Canvas(root, width=650, height=200)
canvas1.pack()
for i in range(5):
    y = 50 + i * 15  
    canvas1.create_line(0, y, 650, y, width=2)

notes_oval1 = []
sharp_symbols1 = []
notes = ['LA', 'SI', 'DO', 'DO#', 'RE', 'RE#', 'MI', 'FA', 'FA#', 'SOL', 'SOL#', 'pausa']
for i, note in enumerate(notes):
    x = 50 + i * 50
    y = note_heights[i]
    if note == 'pausa':
        pause_symbol1 = canvas1.create_rectangle(x - 6, y - 10, x + 6, y, fill='black', outline='black')
        canvas1.create_text(x, y + 20, text='PAUSA', font=('Arial', 10))
    else:
        oval = canvas1.create_oval(x - 8, y - 8, x + 8, y + 8, fill='black', outline='black')
        notes_oval1.append(oval)
        canvas1.create_line(x + 8, y, x + 8, y - 30, width=2)
        if '#' in note:
            sharp = canvas1.create_text(x - 15, y, text='#', font=('Arial', 12), fill='black')
            sharp_symbols1.append(sharp)
        else:
            sharp_symbols1.append(None)
        canvas1.create_text(x, y + 20, text=note, font=('Arial', 10))

canvas2 = tk.Canvas(root, width=650, height=200)
canvas2.pack()
for i in range(5):
    y = 50 + i * 15  
    canvas2.create_line(0, y, 650, y, width=2)

notes_oval2 = []
sharp_symbols2 = []
for i, note in enumerate(notes):
    x = 50 + i * 50
    y = note_heights[i]
    if note == 'pausa':
        pause_symbol2 = canvas2.create_rectangle(x - 6, y - 10, x + 6, y, fill='black', outline='black')
        canvas2.create_text(x, y + 20, text='PAUSA', font=('Arial', 10))
    else:
        oval = canvas2.create_oval(x - 8, y - 8, x + 8, y + 8, fill='black', outline='black')
        notes_oval2.append(oval)
        canvas2.create_line(x + 8, y, x + 8, y - 30, width=2)
        if '#' in note:
            sharp = canvas2.create_text(x - 15, y, text='#', font=('Arial', 12), fill='black')
            sharp_symbols2.append(sharp)
        else:
            sharp_symbols2.append(None)
        canvas2.create_text(x, y + 20, text=note, font=('Arial', 10))

def on_closing():
    global ser1, ser2
    if ser1 and ser1.is_open:
        ser1.close()
    if ser2 and ser2.is_open:
        ser2.close()
    pygame.mixer.quit()
    root.destroy()

root.protocol("WM_DELETE_WINDOW", on_closing)

root.mainloop()