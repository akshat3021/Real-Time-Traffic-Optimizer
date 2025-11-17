import csv
import json

INPUT_CSV = "engine/junctions_detailed_fixed_dehradun.csv"
OUTPUT_JS = "webapp/static/locations.js"   # <-- Auto-generated file

def main():
    locations = {}

    with open(INPUT_CSV, newline='', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        for row in reader:
            j_id = row.get("junction_id")
            if not j_id:
                continue

            # read coordinates (csv uses latitude/longitude)
            lat = float(row.get("latitude") or 0)
            lon = float(row.get("longitude") or 0)

            name = row.get("place_name", f"node_{j_id}")

            locations[int(j_id)] = {
                "lat": lat,
                "lon": lon,
                "name": name,
                "type": "junction"
            }

    # Write JS file
    with open(OUTPUT_JS, "w", encoding="utf-8") as f:
        f.write("const locations = ")
        json.dump(locations, f, indent=4)
        f.write(";\n")

    print(f"Generated: {OUTPUT_JS}")
    print(f"Total nodes: {len(locations)}")

if __name__ == "__main__":
    main()
