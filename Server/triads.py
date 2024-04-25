from random import randint

triads = {
    'Ct': set({'C', 'E', 'G'}),
    'Cm': set({'C', 'D#', 'G'}),
    'C#t': set({'C#', 'F', 'G#'}),
    'C#m': set({'C#', 'E', 'G#'}),
    'Dt': set({'D', 'F#', 'A'}),
    'Dm': set({'D', 'F', 'A'}),
    'D#t': set({'D#', 'G', 'A#'}),
    'D#m': set({'D#', 'F#', 'A#'}),
    'Et': set({'E', 'G#', 'B'}),
    'Em': set({'E', 'G', 'B'}),
    'Ft': set({'F', 'A', 'C'}),
    'Fm': set({'F', 'G#', 'C'}),
    'F#t': set({'F#', 'A#', 'C#'}),
    'F#m': set({'F#', 'A', 'C#'}),
    'Gt': set({'G', 'B', 'D'}),
    'Gm': set({'G', 'A#', 'D'}),
    'G#t': set({'G#', 'C', 'D#'}),
    'G#m': set({'G#', 'B', 'D#'}),
    'At': set({'A', 'C#', 'E'}),
    'Am': set({'A', 'C', 'E'}),
    'A#t': set({'A#', 'D', 'F'}),
    'A#m': set({'A#', 'C#', 'F'}),
    'Bt': set({'B', 'D#', 'F#'}),
    'Bm': set({'B', 'D', 'F#'})
}

color_mapping = {
    'Ct': [245, 221, 66],
    'Cm': [46, 196, 41],
    'C#t': [143, 36, 14],
    'C#m': [89, 22, 22],
    'Dt': [41, 196, 139],
    'Dm': [14, 143, 36],
    'D#t': [31, 161, 35],
    'D#m': [201, 178, 24],
    'Et': [24, 201, 89],
    'Em': [169, 207, 212],
    'Ft': [164, 75, 209],
    'Fm': [66, 22, 140],
    'F#t': [143, 19, 98],
    'F#m': [37, 19, 143],
    'Gt': [6, 140, 12],
    'Gm': [237, 226, 14],
    'G#t': [0, 55, 255],
    'G#m': [13, 37, 217],
    'At': [136, 227, 129],
    'Am': [0, 217, 255],
    'A#t': [152, 11, 222],
    'A#m': [50, 11, 222],
    'Bm': [10, 103, 242]
}

# thirds = {}

# for key in triads:
#     data = list(triads[key])
#     thirds[key] = set({data[1], data[2]})

def random_color():
    return [randint(45, 180), randint(45, 180), randint(45, 180)]