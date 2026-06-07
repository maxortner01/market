import struct

record_count = 4
data = struct.pack("<I", record_count)
symbol_ids = [1000, 1001, 1002, 1003]
prices = [150.25, 200.50, 75.10, 300.00]
volumes = [1000, 2000, 3000, 4000]
timestamps = [1700000000000000000 + i for i in range(4)]

for symbol_id in symbol_ids:
    data += struct.pack("<I", symbol_id)
for price in prices:
    data += struct.pack("<d", price)
for volume in volumes:
    data += struct.pack("<Q", volume)
for timestamp in timestamps:
    data += struct.pack("<Q", timestamp)

with open("testing/fixtures/raw_market_data.bin", "wb") as handle:
    handle.write(data)

print(f"wrote {len(data)} bytes")
