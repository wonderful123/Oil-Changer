import sys
from mido import MidiFile

def midi_note_to_name(note_number):
    notes = ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"]
    octave = (note_number // 12) - 1
    note_index = note_number % 12
    return notes[note_index] + str(octave)

def ticks_to_milliseconds(ticks, tempo, ticks_per_beat):
    microseconds_per_tick = tempo / ticks_per_beat
    return (ticks * microseconds_per_tick) / 1000

def process_midi_file(midi_path, output_path):
    default_tempo = 500000  # Default tempo (500000 microseconds per quarter note, or 120 BPM)
    midi_file = MidiFile(midi_path)

    notes = []
    note_start_times = {}
    current_time = 0

    for track in midi_file.tracks:
        for msg in track:
            current_time += msg.time  # Accumulate time

            if msg.type == 'note_on' and msg.velocity > 0:
                note_start_times[msg.note] = current_time
            elif msg.type == 'note_off' or (msg.type == 'note_on' and msg.velocity == 0):
                if msg.note in note_start_times:
                    # Calculate duration and add note to the list
                    start_time = note_start_times.pop(msg.note)
                    duration_ticks = current_time - start_time
                    duration_ms = ticks_to_milliseconds(duration_ticks, default_tempo, midi_file.ticks_per_beat)
                    note_name = midi_note_to_name(msg.note)
                    note_data = {
                        'note_name': note_name,
                        'duration_ms': duration_ms
                    }
                    notes.append(note_data)

    # Write to output file
    with open(output_path, "w") as file:
        file.write("Note wonderboyThemeNotes[] = {\n")
        for note in notes:
            file.write(f"  {{NOTE_{note['note_name']}, {int(note['duration_ms'])}, ARTICULATION_STACCATO, DYNAMICS_MEDIUM}},\n")
        file.write("};\n")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py <midi_file_path>")
        sys.exit(1)

    midi_path = sys.argv[1]
    output_path = midi_path.rsplit('.', 1)[0] + ".txt"

    process_midi_file(midi_path, output_path)
    print(f"Processed MIDI file '{midi_path}' and wrote output to '{output_path}'.")
