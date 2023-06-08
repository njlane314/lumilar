import itertools
################################################
# CONFIGURATION
case_name = "dune-hd"
output_file = "dune_hd_placements.txt"
################################################
detector_configs = {
    "dune-hd": {
        "detector_width": 3500,
        "detector_height": 12000,
        "detector_depth": 57500,
        "cell_width": 1150,
        "cell_height": 600,
        "sensor_shape": "rectangle",
        "sensor_width": 1000,
        "sensor_height": 120,
        "plane_orientation": "+x"
    },
    "dune-vd": {},
    "solar": {},
}
################################################
detector_config = detector_configs[case_name]

detector_width = detector_config["detector_width"]
detector_height = detector_config["detector_height"]
detector_depth = detector_config["detector_depth"]
cell_width = detector_config["cell_width"]
cell_height = detector_config["cell_height"]
sensor_shape = detector_config["sensor_shape"]
sensor_width = detector_config["sensor_width"]
sensor_height = detector_config["sensor_height"]
plane_orientation = detector_config["plane_orientation"]
################################################
sensor_positions = []
if plane_orientation == "+x":
    plane_width = detector_depth
    plane_height = detector_height
    sensor_x = detector_width / 2

    if plane_width % cell_width != 0 & plane_height % cell_height != 0:
        print("Error: Detector dimensions must be divisible by the cell dimensions")
        exit(1)

    num_cells_width = plane_width // cell_width
    num_cells_height = plane_height // cell_height

    for z in range(num_cells_width):
        for y in range(num_cells_height):
            sensor_z = (z * cell_width) + (cell_width / 2) - (plane_width / 2)
            sensor_y = (y * cell_height) + (cell_height / 2) - (plane_height / 2)

            sensor_positions.append((sensor_x, sensor_y, sensor_z, sensor_width, sensor_height, plane_orientation))
elif plane_orientation == "-x":
    plane_width = detector_depth
    plane_height = detector_height
    sensor_x = -detector_width / 2
    
    if plane_width % cell_width != 0 & plane_height % cell_height != 0:
        print("Error: Detector dimensions must be divisible by the cell dimensions")
        exit(1)

    num_cells_width = plane_width // cell_width
    num_cells_height = plane_height // cell_height

    for z in range(num_cells_width):
        for y in range(num_cells_height):
            sensor_z = (z * cell_width) + (cell_width / 2) - (plane_width / 2)
            sensor_y = (y * cell_height) + (cell_height / 2) - (plane_height / 2)

            sensor_positions.append((sensor_x, sensor_y, sensor_z, sensor_width, sensor_height, plane_orientation))
elif plane_orientation == "+y":
    plane_width = detector_width
    plane_height = detector_depth
    sensor_y = detector_height / 2

    if plane_width % cell_width != 0 & plane_height % cell_height != 0:
        print("Error: Detector dimensions must be divisible by the cell dimensions")
        exit(1)

    num_cells_width = plane_width // cell_width
    num_cells_height = plane_height // cell_height

    for x in range(num_cells_width):
        for z in range(num_cells_height):
            sensor_x = (x * cell_width) + (cell_width / 2) - (plane_width / 2)
            sensor_z = (z * cell_height) + (cell_height / 2) - (plane_height / 2)

            sensor_positions.append((sensor_x, sensor_y, sensor_z, sensor_width, sensor_height, plane_orientation))
elif plane_orientation == "-y":
    plane_width = detector_width
    plane_height = detector_depth
    sensor_y = -detector_height / 2

    if plane_width % cell_width != 0 & plane_height % cell_height != 0:
        print("Error: Detector dimensions must be divisible by the cell dimensions")
        exit(1)

    num_cells_width = plane_width // cell_width
    num_cells_height = plane_height // cell_height

    for x in range(num_cells_width):
        for z in range(num_cells_height):
            sensor_x = (x * cell_width) + (cell_width / 2) - (plane_width / 2)
            sensor_z = (z * cell_height) + (cell_height / 2) - (plane_height / 2)

            sensor_positions.append((sensor_x, sensor_y, sensor_z, sensor_width, sensor_height, plane_orientation))
elif plane_orientation == "+z":
    plane_width = detector_width
    plane_height = detector_height
    sensor_z = detector_depth / 2

    if plane_width % cell_width != 0 & plane_height % cell_height != 0:
        print("Error: Detector dimensions must be divisible by the cell dimensions")
        exit(1)

    num_cells_width = plane_width // cell_width
    num_cells_height = plane_height // cell_height

    for x in range(num_cells_width):
        for y in range(num_cells_height):
            sensor_x = (x * cell_width) + (cell_width / 2) - (plane_width / 2)
            sensor_y = (y * cell_height) + (cell_height / 2) - (plane_height / 2)

            sensor_positions.append((sensor_x, sensor_y, sensor_z, sensor_width, sensor_height, plane_orientation))
elif plane_orientation == "-z":
    plane_width = detector_width
    plane_height = detector_height
    sensor_z = -detector_depth / 2

    if plane_width % cell_width != 0 & plane_height % cell_height != 0:
        print("Error: Detector dimensions must be divisible by the cell dimensions")
        exit(1)

    num_cells_width = plane_width // cell_width
    num_cells_height = plane_height // cell_height

    for x in range(num_cells_width):
        for y in range(num_cells_height):
            sensor_x = (x * cell_width) + (cell_width / 2) - (plane_width / 2)
            sensor_y = (y * cell_height) + (cell_height / 2) - (plane_height / 2)

            sensor_positions.append((sensor_x, sensor_y, sensor_z, sensor_width, sensor_height, plane_orientation))
else:
    print("Error: Invalid plane")
    exit(1)
################################################
with open(output_file, "w") as file:
    file.write(f"{detector_width} {detector_height} {detector_depth}\n")
    
    for position in sensor_positions:
        x, y, z, width, height, plane_orientation = position
        file.write(f"{sensor_shape} {plane_orientation} {x} {y} {z} {width} {height}\n")

print(f"Sensor placements saved to {output_file}")