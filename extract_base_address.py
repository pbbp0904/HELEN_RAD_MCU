import re

data = []
with open("../soc_system.tcl", "r") as finput, open("base_address.txt", "w+") as foutput:
    for line in finput:
        address = re.search(r"baseAddress {0[xX][0-9a-fA-F]+}", line)
        if address:
            name = re.search(r"ddc_data_[0-9]+", line)
            if name:
                data.append((name.group(0), address.group(0)[13:-1]))

    data.sort()
    for point in data:
        foutput.write(f"#define FPGA_{point[0].upper()}_BASE\t{point[1]}\n")
