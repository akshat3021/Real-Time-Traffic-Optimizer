# engine/snap_junctions_to_roads.py
import csv, time, requests, sys
from urllib.parse import quote_plus

OSRM_NEAREST = "https://router.project-osrm.org/nearest/v1/driving/"

INPUT = "engine/junctions_detailed_fixed_dehradun.csv"
OUTPUT = "engine/junctions_detailed_fixed_dehradun_snapped.csv"

def snap_point(lat, lon, max_retries=3):
    # OSRM expects lon,lat
    url = f"{OSRM_NEAREST}{lon},{lat}?number=1"
    for attempt in range(max_retries):
        try:
            r = requests.get(url, timeout=10)
            if r.status_code == 200:
                j = r.json()
                if j.get("code") == "Ok" and j.get("waypoints"):
                    wp = j["waypoints"][0]
                    snapped = wp.get("location")  # [lon, lat]
                    if snapped and len(snapped) == 2:
                        return float(snapped[1]), float(snapped[0])
                return None
            else:
                print(f"OSRM returned {r.status_code} for {lat},{lon}; retrying...", file=sys.stderr)
        except Exception as e:
            print("OSRM request error:", e, file=sys.stderr)
        time.sleep(0.5 + attempt*0.5)
    return None

def main():
    rows = []
    with open(INPUT, newline='', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        fieldnames = reader.fieldnames
        # Expect fieldnames like: junction_id,place_name,latitude,longitude
        for r in reader:
            rows.append(r)

    out_rows = []
    total = len(rows)
    print(f"Snapping {total} junctions to nearest road...")
    for i, r in enumerate(rows, start=1):
        try:
            lat = float(r.get('latitude') or r.get('lat') or 0)
            lon = float(r.get('longitude') or r.get('lon') or 0)
        except:
            lat = lon = 0
        if lat == 0 and lon == 0:
            print(f"[{i}/{total}] skipping {r.get('junction_id')} - no coords", file=sys.stderr)
            out_rows.append(r)
            continue
        snapped = snap_point(lat, lon)
        if snapped:
            s_lat, s_lon = snapped
            r['latitude'] = f"{s_lat:.6f}"
            r['longitude'] = f"{s_lon:.6f}"
            print(f"[{i}/{total}] snapped {r.get('junction_id')} -> {s_lat:.6f},{s_lon:.6f}")
        else:
            print(f"[{i}/{total}] failed to snap {r.get('junction_id')}; keeping original", file=sys.stderr)
        out_rows.append(r)
        # polite pause to avoid hammering public OSRM
        time.sleep(0.15)

    # write CSV
    with open(OUTPUT, 'w', newline='', encoding='utf-8') as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        for r in out_rows:
            writer.writerow(r)

    print("Saved snapped CSV to:", OUTPUT)

if __name__ == "__main__":
    main()
