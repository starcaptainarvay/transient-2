# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.

import re
import mido
import socket
import json
import time

from _thread import start_new_thread
from triads import triads, color_mapping, random_color
# from triads import color_mapping

note_list = [
    "A0", "A#0", "B0",
    "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1",
    "A1", "A#1", "B1",
    "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2",
    "A2", "A#2", "B2",
    "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3",
    "A3", "A#3", "B3",
    "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4",
    "A4", "A#4", "B4",
    "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5",
    "A5", "A#5", "B5",
    "C6", "C#6", "D6", "D#6", "E6", "F6", "F#6", "G6", "G#6",
    "A6", "A#6", "B6",
    "C7", "C#7", "D7", "D#7", "E7", "F7", "F#7", "G7", "G#7",
    "A7", "A#7", "B7",
    "C8"
]

active_notes = {}   # Currently pressed notes
active_connections = []
notes_pressed = []  # Newly pressed notes for the next JSON dump
notes_released = [] # Newly released notes for the next JSON dump

note_lock = False

global current_pitch
current_pitch = 0

def replace_numbers(input_str):
    return re.sub(r'\d+', '', input_str)


def generalize_notes():
    return set(map(replace_numbers, list(active_notes)))


def update(note, velocity, _type):
    # print("Updating with note ", note, " vel ", velocity, "type", _type)
    # global note_lock
    if (velocity == 0 or _type == "note_off") and note in active_notes:
        # print("    note off")
        # hi Avyay I changed this line above
        # the reface sends a "note_on" with vel = 0 for note release
        # the juno-ds sends "note_off" with the release velocity
        # our code was set to detect velocity = 0 as note release, I changed it to be 0 velocity OR note_off
        notes_released.append(note)
        del active_notes[note]
        return
    # print("    note on")
    while len(notes_pressed) >= 1: # waits until next "frame" (until next packet sent, in case it's currently being sent in parallel)
        pass
    # print("    made it to next message")
    active_notes[note] = velocity
    notes_pressed.append([note, velocity])

    # ns = note[0:-1] + "-" + note[-1]


def connect(port_name):
    with mido.open_input(port_name) as port:
        print("connected")
        for message in port:
            # print(message)
            if message.type == "note_on" or message.type == "note_off":
                # print(message)
                note = note_list[message.note - 21]
                update(note, message.velocity, message.type)
            # elif message.type == "pitchwheel":
            #     global current_pitch
            #     current_pitch = message.pitch/8192
                
            #     pass
            # print(active_notes)
            # print("chord:", generalize_notes())
            # for conn in active_connections:
            #     conn.sendall(bytearray(json.dumps({
            #         ['notes']: active_notes,
            #         ['chord']: list(generalize_notes())
            #     }), 'utf-8'))

    print("port closed")


def disconnect_socket(conn: socket.socket):
    conn.close()
    active_connections.remove(conn)


def guess_chord():
    _chord = generalize_notes()
    for name, notes in triads.items():
        if _chord.issubset(notes) or notes.issubset(_chord):
            return name
    # for name, notes in thirds.items():
    #     if notes.issubset(_chord):
    #         return name
    return "  "
        

def get_color(name: str):
    if name in color_mapping:
        return color_mapping[name]
    return random_color()


def start():
    # global note_lock
    for name in mido.get_input_names():
        # print(name)
        if "reface" in name:
            start_new_thread(connect, (name,))
        if "JUNO-DS" in name and not "DAW" in name:
            start_new_thread(connect, (name,))
        if "Digital Piano" in name:
            start_new_thread(connect, (name,))
        if "Keystation 61 MK3 0" in name:
            start_new_thread(connect, (name,))
        if "CASIO" in name:
            print("connecting")
            start_new_thread(connect, (name,))
        if "Portable" in name:
            print("Transient 2 activating!!!")
            start_new_thread(connect, (name,))

    last_packet = ""

    while True:
        if len(active_connections) > 0:

            # print(f"main thread - notes pressed: {notes_pressed}, notes released: {notes_released}, active_notes: {active_notes}" )
            while len(notes_pressed) < 1 and len(notes_released) < 1:
                pass
            # note_lock = True
            # print("Made it through the while")

            packet = json.dumps({
                'notes': active_notes,
                'chord': list(generalize_notes()),
                'color': get_color(guess_chord()),
                'notes_pressed': notes_pressed,
                'notes_released': notes_released,
                'pitch': current_pitch
            }) + "<!>"

            notes_pressed.clear()
            notes_released.clear()

            # note_lock = False

            if last_packet != packet:
                # print(packet)
                for sock in active_connections:
                    try:
                        sock.sendall(bytearray(packet, 'utf-8'))
                    except ConnectionResetError as e:
                        print(e)
                        disconnect_socket(sock)
                # print("sending packet")

            last_packet = packet


def connect_to_client(conn: socket.socket):
    # conn.send(str.encode("Connected<!>"))
    active_connections.append(conn)

    while True:
        try:
            data = conn.recv(2048)
            request = data.decode("utf-8")

            if request == "close":
                print("Disconnected")
                break
            elif not data:
                print("Disconnected")
                break
            else:
                print("Received: ", request)
        except:
            break

    disconnect_socket(conn)


def start_server():
    host = "127.0.0.1"  # "10.182.114.119"
    port = 3000

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        server_socket.bind((host, port))
    except socket.error as e:
        return print(str(e))

    server_socket.listen(2)
    print("listening for connection")

    while True:
        conn, address = server_socket.accept()
        print("Accepted client", address)
        start_new_thread(connect_to_client, (conn,))
        # conn.sendall(
        #     b"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nHello world!"
        # )


# def mess_with_jess():
#     for name in mido.get_output_names():
#         if 'Digital' in name:
#             print(name)
#             with mido.open_output(name) as port:
#                 note = mido.Message('note_on')
#                 note = note.copy(note=55,time=100.0,velocity=127)
#                 print(note.bytes())
#                 port.send(note)


if __name__ == '__main__':
    start_new_thread(start_server, ())
    start()

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
