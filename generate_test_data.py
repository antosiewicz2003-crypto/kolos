#!/usr/bin/env python3
"""Generate binary PGM test data files for the DANTE unit tests."""
import struct
import os

def write_pgm(filename, width, height, max_val, pixels):
    """Write a binary PGM file in the custom format described in the assignment.
    
    Format: "P2" (2 bytes) + width (4 bytes LE) + height (4 bytes LE) + max_val (1 byte) + pixel data
    """
    with open(filename, 'wb') as f:
        f.write(b'P2')
        f.write(struct.pack('<i', width))
        f.write(struct.pack('<i', height))
        f.write(struct.pack('B', max_val))
        for p in pixels:
            f.write(struct.pack('B', p))


def generate_invent():
    pixels = [58, 103, 167, 141, 202, 235, 237, 240, 214, 86, 158, 38, 105, 70, 205, 45]
    write_pgm('invent.bin', 1, 16, 255, pixels)


def generate_plain():
    pixels = [244, 246, 173, 4, 126, 175, 231, 33, 63, 201]
    write_pgm('plain.bin', 10, 1, 255, pixels)


def generate_hill():
    data = [
        [231, 233, 0, 102, 68, 53, 83, 110, 126, 77, 220, 178, 67, 210, 101, 235, 2, 118, 181],
        [127, 179, 51, 166, 171, 149, 39, 186, 104, 25, 163, 144, 191, 137, 49, 224, 160, 212, 127],
        [220, 140, 184, 186, 130, 253, 61, 148, 127, 109, 191, 7, 199, 50, 194, 250, 223, 225, 207],
        [6, 208, 133, 252, 162, 209, 1, 81, 39, 61, 21, 92, 235, 222, 251, 135, 9, 234, 207],
        [187, 102, 146, 55, 184, 252, 187, 40, 45, 143, 140, 29, 147, 107, 33, 105, 86, 222, 137],
        [137, 104, 204, 181, 174, 227, 197, 145, 211, 213, 145, 51, 205, 32, 140, 126, 192, 28, 73],
        [20, 175, 141, 53, 244, 250, 205, 57, 72, 136, 146, 51, 254, 61, 122, 29, 168, 88, 89],
        [38, 165, 17, 240, 95, 238, 133, 177, 180, 243, 225, 162, 60, 148, 158, 23, 214, 55, 62],
        [198, 53, 13, 70, 80, 215, 167, 193, 202, 79, 172, 43, 121, 158, 123, 212, 135, 225, 118],
        [53, 1, 75, 201, 167, 81, 113, 247, 27, 47, 101, 104, 167, 112, 223, 17, 31, 153, 21],
        [73, 101, 190, 171, 131, 65, 215, 37, 241, 244, 97, 92, 123, 14, 129, 62, 219, 171, 0],
    ]
    pixels = []
    for row in data:
        pixels.extend(row)
    write_pgm('hill.bin', 19, 11, 255, pixels)


def generate_seed():
    threshold_result = [
        [0, 255, 255, 0, 255, 0, 255, 255, 255, 255, 0, 0, 0],
        [255, 0, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255],
        [255, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 255],
        [255, 255, 0, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255],
        [0, 255, 0, 0, 255, 255, 0, 255, 0, 0, 255, 0, 255],
        [0, 0, 0, 255, 255, 0, 255, 0, 255, 0, 255, 255, 255],
        [0, 255, 0, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0],
        [0, 0, 0, 0, 0, 255, 255, 255, 0, 255, 0, 255, 0],
        [255, 0, 255, 255, 255, 0, 0, 255, 0, 0, 255, 255, 0],
        [255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 255, 255],
        [255, 0, 0, 0, 0, 0, 255, 0, 255, 0, 255, 255, 255],
        [255, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 0],
        [0, 255, 255, 0, 255, 255, 0, 255, 255, 0, 0, 0, 0],
        [0, 255, 0, 255, 255, 0, 255, 0, 0, 255, 0, 255, 0],
        [0, 255, 0, 255, 255, 0, 0, 255, 255, 0, 0, 255, 255],
    ]

    high_count = sum(1 for row in threshold_result for v in row if v == 255)
    low_count = 15 * 13 - high_count

    high_val = 200
    low_val = 50
    total = high_count * high_val + low_count * low_val
    mean = total // (15 * 13)

    assert high_val > mean and low_val <= mean, f"mean={mean}, need high>{mean} and low<={mean}"

    pixels = []
    for row in threshold_result:
        for v in row:
            pixels.append(high_val if v == 255 else low_val)

    write_pgm('seed.bin', 13, 15, 255, pixels)


def generate_foot():
    width = 10
    height = 29
    
    area_data = [
        (0, 0, 1), (0, 1, 1), (0, 2, 1), (0, 3, 1), (0, 4, 1), (0, 5, 1), (0, 6, 1), (0, 7, 1), (0, 8, 1), (0, 9, 1),
        (1, 0, 1), (1, 1, 1), (1, 2, 1), (1, 3, 1), (1, 4, 1), (1, 5, 1), (1, 6, 1), (1, 7, 1), (1, 8, 1), (1, 9, 1),
        (2, 0, 1), (2, 1, 1), (2, 2, 1), (2, 3, 1), (2, 4, 1), (2, 5, 1), (2, 6, 1), (2, 7, 1), (2, 8, 1), (2, 9, 1),
        (3, 0, 1), (3, 1, 1), (3, 2, 1), (3, 3, 1), (3, 4, 1), (3, 5, 1), (3, 6, 1), (3, 7, 1), (3, 8, 1), (3, 9, 1),
        (4, 0, 1), (4, 1, 1), (4, 2, 1), (4, 3, 1), (4, 4, 1), (4, 5, 1), (4, 6, 1), (4, 7, 1), (4, 8, 1), (4, 9, 1),
        (5, 0, 1), (5, 1, 1), (5, 2, 1), (5, 3, 1), (5, 4, 1), (5, 5, 1), (5, 6, 1), (5, 7, 1), (5, 8, 1), (5, 9, 1),
        (6, 0, 1), (6, 1, 1), (6, 2, 1), (6, 3, 1), (6, 4, 1), (6, 5, 1), (6, 6, 1), (6, 7, 1), (6, 8, 1), (6, 9, 1),
        (7, 0, 1), (7, 1, 1), (7, 2, 1), (7, 3, 1), (7, 4, 1), (7, 5, 1), (7, 6, 1), (7, 7, 1), (7, 8, 1), (7, 9, 1),
        (8, 0, 1), (8, 1, 1), (8, 2, 1), (8, 3, 1), (8, 4, 1), (8, 5, 1), (8, 6, 1), (8, 7, 1), (8, 8, 1), (8, 9, 1),
        (9, 0, 1), (9, 1, 1), (9, 2, 1), (9, 3, 1), (9, 4, 1), (9, 5, 1), (9, 6, 1), (9, 7, 1), (9, 8, 1), (9, 9, 1),
        (10, 0, 1), (10, 1, 1), (10, 2, 1), (10, 3, 1), (10, 4, 1), (10, 5, 1), (10, 6, 1), (10, 7, 1), (10, 8, 1), (10, 9, 1),
        (11, 0, 1), (11, 1, 1), (11, 2, 1), (11, 3, 1), (11, 4, 1), (11, 5, 1), (11, 6, 1), (11, 7, 1), (11, 8, 1), (11, 9, 1),
        (12, 0, 1), (12, 1, 1), (12, 2, 1), (12, 3, 1), (12, 4, 1), (12, 5, 1), (12, 6, 1), (12, 7, 1), (12, 8, 1), (12, 9, 1),
        (13, 0, 1), (13, 1, 1), (13, 2, 1), (13, 3, 1), (13, 4, 1), (13, 5, 1), (13, 6, 1), (13, 7, 1), (13, 8, 1), (13, 9, 1),
        (14, 0, 1), (14, 1, 1), (14, 2, 1), (14, 3, 1), (14, 4, 1), (14, 5, 1), (14, 6, 1), (14, 7, 1), (14, 8, 1), (14, 9, 1),
        (15, 0, 1), (15, 1, 1), (15, 2, 1), (15, 3, 1), (15, 4, 1), (15, 5, 1), (15, 6, 1), (15, 7, 1), (15, 8, 1), (15, 9, 1),
        (16, 0, 1), (16, 1, 1), (16, 2, 1), (16, 3, 1), (16, 4, 1), (16, 5, 1), (16, 6, 1), (16, 7, 1), (16, 8, 1), (16, 9, 1),
        (17, 0, 1), (17, 1, 1), (17, 2, 1), (17, 3, 1), (17, 4, 2), (17, 5, 1), (17, 6, 1), (17, 7, 1), (17, 8, 1), (17, 9, 1),
        (18, 0, 1), (18, 1, 1), (18, 2, 1), (18, 3, 1), (18, 4, 1), (18, 6, 1), (18, 7, 1), (18, 8, 1), (18, 9, 1),
        (19, 0, 1), (19, 1, 1), (19, 2, 1), (19, 3, 1), (19, 4, 1), (19, 5, 1), (19, 6, 1), (19, 7, 1), (19, 8, 1), (19, 9, 1),
        (20, 0, 1), (20, 1, 1), (20, 2, 1), (20, 3, 1), (20, 4, 1), (20, 5, 1), (20, 6, 1), (20, 7, 1), (20, 8, 1), (20, 9, 1),
        (21, 0, 1), (21, 1, 1), (21, 2, 1), (21, 3, 1), (21, 4, 1), (21, 5, 1), (21, 6, 1), (21, 7, 1), (21, 8, 1), (21, 9, 1),
        (22, 0, 1), (22, 1, 1), (22, 2, 1), (22, 3, 1), (22, 4, 1), (22, 5, 1), (22, 6, 1), (22, 7, 1), (22, 8, 1), (22, 9, 1),
        (23, 0, 1), (23, 1, 1), (23, 2, 1), (23, 3, 1), (23, 4, 1), (23, 5, 1), (23, 6, 1), (23, 7, 1), (23, 8, 1), (23, 9, 1),
        (24, 0, 1), (24, 1, 1), (24, 2, 1), (24, 3, 1), (24, 4, 1), (24, 5, 1), (24, 6, 1), (24, 7, 1), (24, 8, 1), (24, 9, 1),
        (25, 0, 1), (25, 1, 1), (25, 2, 1), (25, 3, 1), (25, 4, 1), (25, 5, 1), (25, 6, 1), (25, 7, 1), (25, 8, 1), (25, 9, 1),
        (26, 0, 1), (26, 1, 1), (26, 2, 1), (26, 3, 1), (26, 4, 1), (26, 5, 1), (26, 6, 1), (26, 7, 1), (26, 8, 1), (26, 9, 1),
        (27, 0, 1), (27, 1, 1), (27, 2, 1), (27, 3, 1), (27, 4, 1), (27, 5, 1), (27, 6, 1), (27, 7, 1), (27, 8, 1), (27, 9, 1),
        (28, 0, 1), (28, 1, 1), (28, 2, 1), (28, 3, 1), (28, 4, 1), (28, 5, 1), (28, 6, 1), (28, 7, 1), (28, 8, 1), (28, 9, 1),
    ]
    
    img = [[0] * width for _ in range(height)]
    
    covered = set()
    for (top_y, left_x, size) in area_data:
        covered.add((top_y, left_x))
    
    missing = set()
    for y in range(height):
        for x in range(width):
            if (y, x) not in covered:
                missing.add((y, x))
    
    val_counter = 0
    used_vals = {}
    
    for y in range(height):
        for x in range(width):
            if (y, x) in missing:
                continue
            
            area_entry = None
            for (ty, lx, sz) in area_data:
                if ty == y and lx == x:
                    area_entry = (ty, lx, sz)
                    break
            
            if area_entry and area_entry[2] == 2:
                pass
            else:
                while True:
                    candidate = val_counter % 256
                    val_counter += 1
                    ok = True
                    for dy in range(-1, 2):
                        for dx in range(-1, 2):
                            if dy == 0 and dx == 0:
                                continue
                            ny, nx = y + dy, x + dx
                            if 0 <= ny < height and 0 <= nx < width:
                                if img[ny][nx] == candidate and (ny, nx) in used_vals:
                                    ok = False
                                    break
                        if not ok:
                            break
                    if ok:
                        img[y][x] = candidate
                        used_vals[(y, x)] = candidate
                        break
    
    merged_val = None
    for (ty, lx, sz) in area_data:
        if sz == 2:
            while True:
                candidate = val_counter % 256
                val_counter += 1
                ok = True
                for dy in range(-1, 2):
                    for dx in range(-1, 2):
                        if dy == 0 and dx == 0:
                            continue
                        ny, nx = ty + dy, lx + dx
                        if 0 <= ny < height and 0 <= nx < width:
                            if (ny, nx) in used_vals and used_vals[(ny, nx)] == candidate:
                                ok = False
                                break
                    if not ok:
                        break
                if ok:
                    merged_val = candidate
                    img[ty][lx] = candidate
                    used_vals[(ty, lx)] = candidate
                    break
            break
    
    for (y, x) in missing:
        img[y][x] = merged_val
        used_vals[(y, x)] = merged_val
    
    for y in range(height):
        for x in range(width):
            if (y, x) in missing:
                for dy in range(-1, 2):
                    for dx in range(-1, 2):
                        if dy == 0 and dx == 0:
                            continue
                        ny, nx = y + dy, x + dx
                        if 0 <= ny < height and 0 <= nx < width:
                            if (ny, nx) != (17, 4) and used_vals.get((ny, nx)) == merged_val:
                                pass
    
    pixels = []
    for row in img:
        pixels.extend(row)
    write_pgm('foot.bin', width, height, 255, pixels)


def generate_corrupted_files():
    with open('leg.bin', 'wb') as f:
        f.write(b'P3')
        f.write(struct.pack('<i', 6))
        f.write(struct.pack('<i', 9))
        f.write(struct.pack('B', 255))
        f.write(bytes([0] * 54))

    with open('neighbor.bin', 'wb') as f:
        f.write(b'P2')
        f.write(struct.pack('<i', 0))
        f.write(struct.pack('<i', 9))
        f.write(struct.pack('B', 255))

    with open('soil.bin', 'wb') as f:
        f.write(b'P2')
        f.write(struct.pack('<i', 6))
        f.write(struct.pack('<i', 0))
        f.write(struct.pack('B', 255))

    with open('property.bin', 'wb') as f:
        f.write(b'P2')
        f.write(struct.pack('<i', 6))
        f.write(struct.pack('<i', 9))
        f.write(struct.pack('B', 10))
        f.write(bytes([0] * 54))

    with open('spot.bin', 'wb') as f:
        f.write(b'P2')
        f.write(struct.pack('<i', 6))
        f.write(struct.pack('<i', 9))
        f.write(struct.pack('B', 255))
        f.write(bytes([0] * 10))


if __name__ == '__main__':
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    
    generate_invent()
    print("Generated invent.bin")
    
    generate_plain()
    print("Generated plain.bin")
    
    generate_hill()
    print("Generated hill.bin")
    
    generate_seed()
    print("Generated seed.bin")
    
    generate_foot()
    print("Generated foot.bin")
    
    generate_corrupted_files()
    print("Generated corrupted test files (leg.bin, neighbor.bin, soil.bin, property.bin, spot.bin)")
    
    print("\nAll test data files generated successfully!")
