import subprocess
import json
import os
import csv  
from flask import Flask, render_template, request, jsonify

app = Flask(__name__)

@app.route('/get_locations')
def get_locations():
    locations = {}
    try:
        
        junctions_path = os.path.join('..', 'engine', 'junctions_detailed_fixed_dehradun.csv')
        
        with open(junctions_path, mode='r', encoding='utf-8') as f:
            reader = csv.reader(f)
            next(reader) 
            
            for row in reader:
                if not row: continue 
               
                try:
                    loc_id = row[0]
                    loc_name = row[1] 
                    loc_lat = float(row[2])
                    loc_lon = float(row[3])
                    
                    locations[loc_id] = {
                        'id': loc_id,
                        'name': loc_name, 
                        'lat': loc_lat,
                        'lon': loc_lon
                    }
                except (ValueError, IndexError):
                  
                    continue
            
                    
        return jsonify(locations)
    except FileNotFoundError:
        return jsonify({"error": "junctions_detailed_fixed_dehradun.csv file not found."}), 500
    except Exception as e:
        return jsonify({"error": f"Error reading CSV: {str(e)}"}), 500


@app.route('/')
def index():
    return render_template('index.html')

@app.route('/find_route', methods=['POST'])
def find_route():
    data = request.get_json()
    start_node = data['start']
    end_node = data['end']

    try:
        # --- FIX: This path is correct ---
        # It goes from 'webapp/' up to the root, to find 'optimizer.exe'
        if os.name == 'nt':
            executable_path = '../optimizer.exe' 
        else:
            executable_path = '../optimizer' 

        command = [executable_path, start_node, end_node]
        
        process = subprocess.run(
            command,
            capture_output=True,
            text=True,
            check=True 
        )
        
        try:
            json_output = json.loads(process.stdout)
            return jsonify(json_output) 
        except json.JSONDecodeError:
            return jsonify({"error": f"Backend returned invalid data: {process.stdout}"}), 500
        
    except FileNotFoundError:
        return jsonify({"error": "optimizer.exe not found. Please compile the C code."}), 500
    except subprocess.CalledProcessError as e:
        try:
            error_json = json.loads(e.stdout)
            return jsonify(error_json), 400
        except:
            return jsonify({"error": f"C program failed: {e.stderr or e.stdout}"}), 500
    except Exception as e:
        return jsonify({"error": f"An unknown server error occurred: {str(e)}"}), 500

if __name__ == '__main__':
    app.run(debug=True)